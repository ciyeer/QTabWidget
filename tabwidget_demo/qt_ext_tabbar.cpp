#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <QStyleOption>
#include <QLabel>

#include "draw_helper.h"
#include "qt_ext_tabbar.h"
#include "round_shadow_helper.h"
#include "common_ui_define.h"

// #define DEV_DEBUG

void QExtTabBarStyle::drawPrimitive(PrimitiveElement pe, 
                const QStyleOption *option, 
                QPainter *painter,
                const QWidget *widget) const
{
    if (pe == QStyle::PE_IndicatorArrowLeft) {
        drawArrow(pe, option, painter, widget);
    } else if (pe == QStyle::PE_IndicatorArrowRight) {
        drawArrow(pe, option, painter, widget);
    } else{
        QProxyStyle::drawPrimitive(pe, option, painter, widget);
    }
}

int QExtTabBarStyle::pixelMetric(PixelMetric metric, 
            const QStyleOption *option, 
            const QWidget *widget) const
{
    if (PM_TabBarScrollButtonWidth == metric) {
        return 30;
    } else {
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

QRect QExtTabBarStyle::subElementRect(SubElement element, 
                                        const QStyleOption *option, 
                                        const QWidget *widget) const
{
    if (SE_TabBarTabLeftButton == element) {
        return calcIconRect(true, option);
    } else if (SE_TabBarTabRightButton == element) {
        return calcIconRect(false, option);
    } else {
        return QProxyStyle::subElementRect(element, option, widget);
    }
}

QRect QExtTabBarStyle::calcIconRect(bool left, const QStyleOption *option) const
{
    const QStyleOptionTab *tab_option = qstyleoption_cast<const QStyleOptionTab *>(option);
    QSize icon_size = tab_option->iconSize;
    const QRect tab_rect = tab_option->rect;
    QPoint center_pos;
    QRect button_rect;
    const int icon_padding = 8;
    if (left) {
        center_pos = QPoint(icon_padding+icon_size.width()/2+tab_rect.x(), 
                            tab_rect.y()+(tab_rect.height()-icon_size.height())/2+icon_size.height()/2);
    } else {
        center_pos = QPoint(tab_rect.x()+tab_rect.width()-icon_padding-icon_size.width()/2, 
                                tab_rect.y()+(tab_rect.height()-icon_size.height())/2+icon_size.height()/2);
    }
    button_rect = QRect(QPoint(0, 0), icon_size);
    button_rect.moveCenter(center_pos);
    return button_rect;
}

void QExtTabBarStyle::drawArrow(PrimitiveElement pe, const QStyleOption *option, 
                                QPainter *painter,
                                const QWidget *widget) const
{
    const QToolButton *tool_btn = static_cast<const QToolButton *>(widget);
    if (nullptr != tool_btn && !tool_btn->isVisible()) 
        return;
    painter->save();
    const QStyleOptionTab *tab_option = qstyleoption_cast<const QStyleOptionTab *>(option);
    QBrush rect_brush;
    if (!tool_btn->isEnabled()) {
        rect_brush = Qt::transparent;
    } else if (tool_btn->underMouse()) {
        rect_brush = QColor(214, 214, 214);
    } else {
        rect_brush = Qt::transparent;
    }
    QRect draw_rect = QRect(0, 0, 16, 16);
    draw_rect.moveCenter(option->rect.center());
    RoundShadowHelper round_helper;
    round_helper.FillRoundShadow(painter, option->rect,rect_brush.color(), 4);
    painter->restore();
    QProxyStyle::drawPrimitive(pe, option, painter, widget);
}

QtExtTabBar::QtExtTabBar(QWidget *parent, TAB_ADD_BUTTON tab_add_button) : QTabBar(parent)
{
    this->setStyle(new QExtTabBarStyle(this));
    if (tab_add_button.draw_plus_btn_) 
        addTab(0);
}

void QtExtTabBar::setTabButton2(int index, QTabBar::ButtonPosition btn_pos, QPushButton *btn)
{
    btn->setFixedSize(this->iconSize());
    if (QTabBar::RightSide == btn_pos) 
        connect(btn, SIGNAL(clicked()), this, SLOT(OnRightWidgetClicked()));
    this->setTabButton(index, btn_pos, btn);
}

QSize QtExtTabBar::tabSizeHint(int index) const
{
    if (index == count()-1 && tab_add_button_.draw_plus_btn_)
        return tab_add_button_.last_tab_size_;
    return tab_size_;
}

void QtExtTabBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawTab(&painter);
    if (tab_add_button_.draw_plus_btn_)
        drawPlusBtn(&painter);
}

void QtExtTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    int i = pointInTabRectIndex(event->pos());
    QTabBar::mouseReleaseEvent(event);
}

void QtExtTabBar::mousePressEvent(QMouseEvent *event)
{
    int index = pointInTabRectIndex(event->pos());
    if (index == count()-1 && tab_add_button_.draw_plus_btn_) {
        emit AddBtnClicked();
        return;
    }

    QTabBar::mousePressEvent(event);
}

void QtExtTabBar::mouseMoveEvent(QMouseEvent *event)
{
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

void QtExtTabBar::setupUI()
{
}

void QtExtTabBar::updateUI()
{
}

int QtExtTabBar::pointInTabRectIndex(const QPoint &point)
{
    for (int i = 0 ; i < count(); ++i) {
        QRect rect = tabRect(i);
        if (rect.contains(point)) {
            return i;
        }
    }
    return -1;
}

void QtExtTabBar::drawTab(QPainter *painter)
{
    RoundShadowHelper helper(6,4);
    int border = helper.GetShadowWidth()/2.0;
    painter->save();
    int tab_count = tab_add_button_.draw_plus_btn_ ? count()-1 : count();
    QStyleOptionTabV3 option;
    for (int index = 0; index < tab_count; index++) {
        QRect rect = tabRect(index);
        initStyleOption(&option, index);
        // draw background 
        QRect draw_rect = QRect(QPoint(rect.x()+border, rect.y() + border), QSize(rect.width()-border*2, rect.height()-border*2));
        drawTabBg(painter, helper, option, draw_rect, rect);
        drawTabText(painter, draw_rect, option);
    }
    painter->restore();
}

void QtExtTabBar::drawTabText(QPainter *painter, const QRect &draw_rect, const QStyleOptionTabV3 &option)
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

void QtExtTabBar::drawTabBg(QPainter *painter, RoundShadowHelper &helper, 
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

void QtExtTabBar::drawPlusBtn(QPainter *painter)
{
    painter->save();
    int last_index = count()-1;
    QStyleOptionTabV3 option;
    initStyleOption(&option, last_index);
    QRect draw_rect = QRect(QPoint(0, 0), tab_add_button_.tab_add_btn_size_);
    draw_rect.moveCenter(tabRect(last_index).center());
    DrawCircle::Draw(painter, draw_rect, tab_btn_add_color_);
    DrawCharacter::DrawPlus(painter, draw_rect);
    painter->restore();
}

void QtExtTabBar::OnRightWidgetClicked()
{
    QObject *obj = (QObject *)(sender());
    for (int i = 0 ; i < count(); ++i) {
       QWidget *widget = tabButton(i, QTabBar::RightSide); 
       if (widget != obj)
           continue;
       emit CloseTab(i);
       break;
    }
}
