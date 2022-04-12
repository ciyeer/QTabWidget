#include "qt_ext_table_widget.h"

#include <QPainter>

QtExtTabWidget::QtExtTabWidget(QWidget *parent)
{
    QtExtTabBar *tab_bar = new QtExtTabBar(this);
    setTabBar(tab_bar);
    connect(tab_bar, SIGNAL(AddBtnClicked()), this, SLOT(OnAddButon()));
    connect(tab_bar, SIGNAL(AddBtnClicked()), this, SIGNAL(AddBtnClicked()));
    connect(tab_bar, SIGNAL(CloseTab(int)), this, SLOT(OnCloseTab(int)));
}

void QtExtTabWidget::addTab2(QWidget *widget, const QString &label)
{
    insertTab2(-1, widget, label);
}

void QtExtTabWidget::insertTab2(int index, QWidget *widget, const QString &label)
{
    int tab_index = this->insertTab(index, widget, label);
    static_cast<QtExtTabBar *>(this->tabBar())->UpdateTabButton(tab_index);
    this->setCurrentIndex(tab_index);
}

void QtExtTabWidget::paintEvent(QPaintEvent *ev)
{
//    QPainter painter(this);
//    painter.setPen(Qt::red);
//    painter.drawRect(this->rect());
//
    QTabWidget::paintEvent(ev);
}

void QtExtTabWidget::tabInserted(int index) 
{
    emit TabInserted(index);
}

void QtExtTabWidget::OnAddButon()
{
    this->insertTab2(this->tabBar()->count()-1, new QWidget(), "test");
}

void QtExtTabWidget::OnCloseTab(int index)
{
    this->removeTab(index); 
}
