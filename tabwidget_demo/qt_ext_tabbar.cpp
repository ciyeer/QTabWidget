#include <QPainter>
#include <QDebug>
#include <QDateTime>

#include "draw_helper.h"
#include "qt_ext_tabbar.h"
#include "round_shadow_helper.h"
#include "common_ui_define.h"

// #define DEV_DEBUG

class QExtTabBarStyle: public QProxyStyle
{
public:
    virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption *option, QPainter *painter,
        const QWidget *widget = Q_NULLPTR) const override
    {
        if (pe == QStyle::PE_IndicatorArrowLeft) {
            painter->save();
            painter->setPen(Qt::red);
            painter->drawRect(option->rect);
            painter->restore();
            QProxyStyle::drawPrimitive(pe, option, painter, widget);
        } else if (pe == QStyle::PE_IndicatorArrowRight) {
            QProxyStyle::drawPrimitive(pe, option, painter, widget);
        } else{
            QProxyStyle::drawPrimitive(pe, option, painter, widget);
        }
    }

    virtual int 
    pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override
    {
        if (PM_TabBarScrollButtonWidth == metric) {
            return 40;
        } else {
            return QProxyStyle::pixelMetric(metric, option, widget);
        }
    }
};

QtExtTabBar::QtExtTabBar(QWidget *parent) : QTabBar(parent)
{
    this->setStyle(new QExtTabBarStyle());
}

QSize QtExtTabBar::tabSizeHint(int index) const
{
    if (index == count()-1 && draw_plus_btn_)
        return last_tab_size_;
    return tab_size_;
}

void QtExtTabBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    DrawTab(&painter);
    if (draw_plus_btn_)
        DrawPlusBtn(&painter);
    for(int index = 0 ; index < count(); index++)
    {
    }
}

void QtExtTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    int i = PointInTabRectIndex(event->pos());
    QTabBar::mouseReleaseEvent(event);
}

void QtExtTabBar::mousePressEvent(QMouseEvent *event)
{
    int index = PointInTabRectIndex(event->pos());
    if (index == count()-1 && draw_plus_btn_) {
        emit BtnClicked();
        return;
    }

    QTabBar::mousePressEvent(event);
}

void QtExtTabBar::mouseMoveEvent(QMouseEvent *event)
{
    if (icon_left_rect_.contains(event->pos())) {
        int i = 0;
    }
    QTabBar::mouseMoveEvent(event);
}

bool QtExtTabBar::event(QEvent *ev) {

    //TODO 在这里更新tab button 的属性，实属无奈之举。我从静态的角度看Qt的源码
    // 发现一个问题：当鼠标明显不在tab add rect范围之内，但是从initStyleOption返回的
    // option 结构体中state还是为QStyle::State_Selected
    // 采用如下的方法workaround
    if (ev->type() == QEvent::HoverMove
        || ev->type() == QEvent::HoverEnter) {
        QHoverEvent *he = static_cast<QHoverEvent *>(ev);
        QRect tab_add_rect = tabRect(count()-1);
        if (tab_add_rect.contains(he->pos())) {
            tab_btn_add_color_ = CIRCLE_BG_COLOR;
            update(tab_add_rect);
        } else {
            tab_btn_add_color_ = NORMAL_TAB_COLOR;
            update(tab_add_rect);
        }
    } else if (ev->type() == QEvent::HoverLeave) {
        QRect tab_add_rect = tabRect(count()-1);
        tab_btn_add_color_ = NORMAL_TAB_COLOR;
        update(tab_add_rect);
    }

    QTabBar::event(ev);
    return true;
}

int QtExtTabBar::PointInTabRectIndex(const QPoint &point)
{
    for (int i = 0 ; i < count(); ++i) {
        QRect rect = tabRect(i);
        if (rect.contains(point)) {
            return i;
        }
    }
    return -1;
}

void QtExtTabBar::DrawTab(QPainter *painter)
{
    RoundShadowHelper helper(6,4);
    int border = helper.GetShadowWidth()/2.0;
    painter->save();
    int tab_count = draw_plus_btn_ ? count()-1 : count();
    QStyleOptionTabV3 option;
    for (int index = 0; index < tab_count; index++) {
        QRect rect = tabRect(index);
        initStyleOption(&option, index);
        // draw background 
        QRect draw_rect = QRect(QPoint(rect.x()+border, rect.y() + border), QSize(rect.width()-border*2, rect.height()-border*2));
        _drawTabBg(painter, helper, option, draw_rect, rect);
        _drawTabText(painter, draw_rect, option);
        _drawLeftIcon(painter, option);
        _drawRightIcon(painter, option);
    }
    painter->restore();
}

void QtExtTabBar::_drawTabText(QPainter *painter, const QRect &draw_rect, const QStyleOptionTabV3 &option)
{
    // draw text
    QColor text_color = tb_text_color_.Normal_;
    QRect text_rect = draw_rect.marginsAdded(margins_);
    if (QStyle::State_Selected & option.state) {
        text_color = tb_text_color_.Selected_;
    } else if (QStyle::State_MouseOver & option.state) {
        text_color = tb_text_color_.Hover_;
    }
    painter->setPen(text_color);
    QString text = fontMetrics().elidedText(option.text, Qt::ElideRight, text_rect.width(), 0); 
    painter->drawText(text_rect, Qt::AlignLeft | Qt::AlignVCenter, text);
}

void QtExtTabBar::_drawTabBg(QPainter *painter, RoundShadowHelper &helper, 
                            const QStyleOptionTabV3 &option, QRect draw_rect, QRect real_rect)
{
    painter->setPen(Qt::NoPen);
    if(QStyle::State_Selected & option.state) {
        helper.RoundShadow(painter, real_rect);
        helper.FillRoundShadow(painter, draw_rect, tb_bg_color_.Selected_, helper.GetRadius());
    } else if(QStyle::State_MouseOver & option.state) {
        helper.FillRoundShadow(painter, draw_rect, tb_bg_color_.Hover_, helper.GetRadius());
    }
}

void QtExtTabBar::DrawPlusBtn(QPainter *painter)
{
    painter->save();
    int last_index = count()-1;
    QStyleOptionTabV3 option;
    initStyleOption(&option, last_index);
    QRect draw_rect = QRect(QPoint(0, 0), tab_add_btn_size_);
    draw_rect.moveCenter(tabRect(last_index).center());
    QColor color = QStyle::State_MouseOver & option.state ? CIRCLE_BG_COLOR : NORMAL_TAB_COLOR;
    if (QStyle::State_Active & option.state) {
        int i = 0;
    } else if (QStyle::State_MouseOver & option.state ) {
        int i = 0;
    }
    DrawCircle::Draw(painter, draw_rect, tab_btn_add_color_);
    DrawCharacter::DrawPlus(painter, draw_rect);
    painter->restore();
}

void QtExtTabBar::_drawLeftIcon(QPainter *painter, const QStyleOptionTabV3 &option)
{
    Q_ASSERT(nullptr != painter); 
#ifndef DEV_DEBUG
    if (!_isDrawLeftIcon())
        return;
#endif //
    painter->save();
    QPoint center_pos = QPoint(icon_padding_+icon_left_size_.width()/2+option.rect.x(), 
                                option.rect.y()+(option.rect.height()-icon_left_size_.height())/2+icon_left_size_.height()/2);
    icon_left_rect_ = QRect(QPoint(0, 0), icon_left_size_);
    icon_left_rect_.moveCenter(center_pos);
#ifdef DEV_DEBUG
    painter->setPen(Qt::red);
    painter->drawRect(draw_rect);
#endif // DEV_DEBUG

    QPoint pos = QCursor::pos();

    painter->drawPixmap(icon_left_rect_, icon_left_pixmap_);

    painter->restore();
}

void QtExtTabBar::_drawRightIcon(QPainter *painter, const QStyleOptionTabV3 &option)
{
    Q_ASSERT(nullptr != painter); 
#ifndef DEV_DEBUG
    if (!_isDrawRightIcon())
        return;
#endif //
    painter->save();
    QPoint center_pos = QPoint(option.rect.x()+option.rect.width()-icon_padding_-icon_right_size_.width()/2, 
                                option.rect.y()+(option.rect.height()-icon_right_size_.height())/2+icon_right_size_.height()/2);
    QRect draw_rect = QRect(QPoint(0, 0), icon_right_size_);
    draw_rect.moveCenter(center_pos);
#ifdef DEV_DEBUG
    painter->setPen(Qt::red);
    painter->drawRect(draw_rect);
#endif // DEBUG
    painter->drawPixmap(draw_rect, icon_left_pixmap_);
    painter->restore();
}

bool QtExtTabBar::_isDrawLeftIcon() const 
{
    return !icon_left_pixmap_.isNull() && icon_left_size_.isValid();
}

bool QtExtTabBar::_isDrawRightIcon() const
{
    return !icon_right_pixmap_.isNull() && icon_right_size_.isValid();
}
