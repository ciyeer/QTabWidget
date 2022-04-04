#include <QPainter>

#include "draw_helper.h"
#include "qt_ext_tabbar.h"
#include "round_shadow_helper.h"


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
            return 30;
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
        return tab_add_btn_size_;
    return tab_size_;
}

void QtExtTabBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    DrawTabBg(&painter);
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

void QtExtTabBar::DrawTabBg(QPainter *painter)
{
    RoundShadowHelper helper(6,4);
    int border = helper.GetShadowWidth()/2.0;
    painter->save();
    int tab_count = draw_plus_btn_ ? count()-1 : count();
    for (int index = 0; index < tab_count; index++) {
        QRect rect = tabRect(index);
        QStyleOptionTabV3 option;
        initStyleOption(&option, index);

        // draw background 
        QRect draw_rect = QRect(QPoint(rect.x()+border, rect.y() + border), QSize(rect.width()-border*2, rect.height()-border*2));
        painter->setPen(Qt::NoPen);
        if(QStyle::State_Selected & option.state) {
            helper.RoundShadow(painter, rect);
            helper.FillRoundShadow(painter, draw_rect, Qt::white, 4);
        }
        else if(QStyle::State_MouseOver & option.state) {
            helper.FillRoundShadow(painter, draw_rect, QColor(214, 214, 214), 4);
        }

        // draw text
        QRect text_rect = draw_rect.marginsAdded(margins_);
        painter->setPen(Qt::black);
        QString text = fontMetrics().elidedText(option.text, Qt::ElideRight, text_rect.width(), 0); 
        painter->drawText(text_rect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }
    painter->restore();
}

void QtExtTabBar::DrawTabText(QPainter *painter)
{
    
}

void QtExtTabBar::_drawTabBg(QPainter *painter, int index)
{
    RoundShadowHelper helper(6,4);
    int border = helper.GetShadowWidth()/2.0;

    QRect rect = tabRect(index);
    QStyleOptionTabV3 option;
    initStyleOption(&option, index);

    QRect draw_rect = QRect(QPoint(rect.x()+border, rect.y() + border), QSize(rect.width()-border*2, rect.height()-border*2));
    painter->setPen(Qt::NoPen);
    if(QStyle::State_Selected & option.state) {
        helper.RoundShadow(painter, rect);
        helper.FillRoundShadow(painter, draw_rect, Qt::white, 4);
    }
    else if(QStyle::State_MouseOver & option.state) {
        helper.FillRoundShadow(painter, draw_rect, QColor(214, 214, 214), 4);
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
    DrawCircle::Draw(painter, draw_rect, color);
    DrawCharacter::DrawPlus(painter, draw_rect);
    painter->restore();
}
