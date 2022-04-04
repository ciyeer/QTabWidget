#include "qt_ext_table_widget.h"

#include <qpainter.h>

QtExtTabWidget::QtExtTabWidget(QWidget *parent)
{
    setTabBar(new QtExtTabBar(this));
}

void QtExtTabWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    // painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    painter.drawRect(this->rect());
    // MyTabBar *tabar = (MyTabBar *)tabBar();
    // painter.drawRect(QRect(0,0, this->width(), tabar->tabSize().height()+1));

    QTabWidget::paintEvent(ev);
}
