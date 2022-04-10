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

int QExtTabBarStyle::pixelMetric(PixelMetric metric, 
            const QStyleOption *option, 
            const QWidget *widget) const
{
    if (PM_TabBarScrollButtonWidth == metric) {
        return 40;
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

QtExtTabBar::QtExtTabBar(QWidget *parent, TAB_ADD_BUTTON tab_add_button) : QTabBar(parent)
{
    this->setStyle(new QExtTabBarStyle(this));
    if (tab_add_button.draw_plus_btn_) 
        addTab(0);
}

void QtExtTabBar::UpdateTab(int index)
{
    if (index == count()-1 && tab_add_button_.draw_plus_btn_)
        return;
    QPushButton *button = new QPushButton();
    button->setFixedSize(this->iconSize());
    button->setStyleSheet("border-image: url(:/images/x-capture-options.png);");
    this->setTabButton(index, QTabBar::LeftSide, button);
    button = new QPushButton();
    button->setFixedSize(this->iconSize());

    connect(button, SIGNAL(clicked()), this, SLOT(OnRightWidgetClicked()));
    button->setStyleSheet("QPushButton{border-image: url(:/images/close.png)}"
                          "QPushButton:hover{border-image: url(:/images/close_hover.png)}");
    this->setTabButton(index, QTabBar::RightSide, button);
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
    DrawTab(&painter);
    if (tab_add_button_.draw_plus_btn_)
        DrawPlusBtn(&painter);
}

void QtExtTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    int i = PointInTabRectIndex(event->pos());
    QTabBar::mouseReleaseEvent(event);
}

void QtExtTabBar::mousePressEvent(QMouseEvent *event)
{
    int index = PointInTabRectIndex(event->pos());
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

    //TODO ���������tab button �����ԣ�ʵ������֮�١��ҴӾ�̬�ĽǶȿ�Qt��Դ��
    // ����һ�����⣺��������Բ���tab add rect��Χ֮�ڣ����Ǵ�initStyleOption���ص�
    // option �ṹ����state����ΪQStyle::State_Selected
    // �������µķ���workaround
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
    int tab_count = tab_add_button_.draw_plus_btn_ ? count()-1 : count();
    QStyleOptionTabV3 option;
    for (int index = 0; index < tab_count; index++) {
        QRect rect = tabRect(index);
        initStyleOption(&option, index);
        // draw background 
        QRect draw_rect = QRect(QPoint(rect.x()+border, rect.y() + border), QSize(rect.width()-border*2, rect.height()-border*2));
        _drawTabBg(painter, helper, option, draw_rect, rect);
        _drawTabText(painter, draw_rect, option);
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
