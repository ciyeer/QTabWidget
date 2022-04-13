#include "qt_ext_table_widget.h"

#include <QPainter>

QtExtTabWidget::QtExtTabWidget(QWidget *parent)
{
    tab_bar_ = new QtExtTabBar(this);
    setTabBar(tab_bar_);
    connect(tab_bar_, SIGNAL(AddBtnClicked()), this, SIGNAL(AddBtnClicked()));
    connect(tab_bar_, SIGNAL(CloseTab(int)), this, SLOT(OnCloseTab(int)));
    connect(tab_bar_, SIGNAL(CloseTab(int)), this, SLOT(TabClosed(int)));
}

void QtExtTabWidget::addTab2(QWidget *widget, const QString &label)
{
    insertTab2(-1, widget, label);
}

void QtExtTabWidget::insertTab2(int index, QWidget *widget, const QString &label)
{
    int tab_index = this->insertTab(index, widget, label);
    this->setCurrentIndex(tab_index);
}

void QtExtTabWidget::setTabButton2(int index, QTabBar::ButtonPosition btn_pos, QPushButton *btn)
{
    tab_bar_->setTabButton2(index, btn_pos, btn);
}

void QtExtTabWidget::paintEvent(QPaintEvent *ev)
{
    QTabWidget::paintEvent(ev);
}

void QtExtTabWidget::tabInserted(int index) 
{
    emit TabInserted(index);
}

void QtExtTabWidget::OnCloseTab(int index)
{
    this->removeTab(index); 
    emit TabClosed(index);
    if (this->currentIndex() == index && index-1>0)
        this->setCurrentIndex(index-1);
}
