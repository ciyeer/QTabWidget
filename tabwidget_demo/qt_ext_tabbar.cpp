#include <QPainter>

#include "qt_ext_tabbar.h"
#include "round_shadow_helper.h"

QtExtTabBar::QtExtTabBar(QWidget *parent) : QTabBar(parent)
{
}

QSize QtExtTabBar::tabSize() const
{
    return tabSizeHint(0);
}

QSize QtExtTabBar::tabSizeHint(int index) const
{
    return QSize(215,40);
}

void QtExtTabBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    DrawTabBg(&painter);
    DrawTabText(&painter);
    for(int index = 0 ; index < count(); index++)
    {
        QRect rc = tabRect(index);
//        rc = rc.marginsAdded(QMargins(-10, -10, -10, -10));
//
//        QStyleOptionTabV3 option;
//        initStyleOption(&option, i);
//
//        //tab背景
//        painter.setPen(Qt::NoPen);
//        if(QStyle::State_MouseOver & option.state)
//        {
//            painter.setBrush(QColor(COLOR_HOVER));
//        }
//        else if(QStyle::State_Selected & option.state)
//        {
//            if(i == 0)
//            {
//                painter.setBrush(QColor(COLOR_SELECT_0));
//            }
//            else if(i == 1)
//            {
//                painter.setBrush(QColor(COLOR_SELECT_1));
//            }
//        }
//        else
//        {
//            if(i == 0)
//            {
//                painter.setBrush(QColor(COLOR_NORMAL_0));
//            }
//            else if(i == 1)
//            {
//                painter.setBrush(QColor(COLOR_NORMAL_1));
//            }
//        }
//        painter.drawRect(rc);
//
//        //tab文字
//        painter.setPen(Qt::black);
//        painter.setBrush(Qt::NoBrush);
//        painter.drawText(rc,Qt::AlignCenter,tabText(i));
//
//        //红色角标
//        if(i == 1)
//        {
//            QRect rect(0,0,30,30);
//            rect.moveTopRight(rc.topRight());
//
//            painter.setPen(Qt::NoPen);
//            painter.setBrush(QColor(COLOR_MARK));
//            painter.drawEllipse(rect);
//
//            painter.setPen(QColor(Qt::white));
//            painter.drawText(rect, Qt::AlignCenter, "99");
//        }
    }
}

void QtExtTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    int i = PointInTabRectIndex(event->pos());
    QTabBar::mouseReleaseEvent(event);
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
    for (int index = 0; index < count(); index++) {
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
