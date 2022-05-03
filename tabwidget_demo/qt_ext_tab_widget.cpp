#include "qt_ext_tab_widget.h"
#include "button_widget.h"

#include <QPainter>

const int kBUTTON_CONTAINER_WIDTH = 100;

QtExtTabWidget::QtExtTabWidget(QWidget *parent)
{
    setupUI();
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

void QtExtTabWidget::setTabBKImage(const QString &normal_image, 
                                    const QString &hover_image, 
                                    const QString &selected_image)
{
    tab_bar_->setTBBGImage(QtExtTabBar::TB_BK_IMAGE(true, normal_image, hover_image, selected_image));
}

void QtExtTabWidget::setTabBarBKImage(const QString &image_path) 
{
    tab_bar_->setTabBarBKImage(image_path);
}

void QtExtTabWidget::setTabTextColor(const QColor &normal, 
                                    const QColor &hover, 
                                    const QColor &select)
{
    tab_bar_->setTBTextColor(QtExtTabBar::TB_TEXT_COLOR(normal, hover, select));
}

void QtExtTabWidget::setMaxRestoreBtnVisible(bool max_visible, bool restore_visible)
{
    button_widget_->setMaxRestoreBtnVisible(max_visible, restore_visible);
}

void QtExtTabWidget::paintEvent(QPaintEvent *ev)
{
    QTabWidget::paintEvent(ev);
}

void QtExtTabWidget::tabInserted(int index) 
{
    emit TabInserted(index);
}

void QtExtTabWidget::resizeEvent(QResizeEvent *ev)
{
    button_widget_->setFixedSize(QSize(kBUTTON_CONTAINER_WIDTH, tab_bar_->tabSize().height()));
    int tab_bar_width = this->width()-button_widget_->width();
    button_widget_->move(QPoint(tab_bar_width, 0));
    tab_bar_->setFixedWidth(tab_bar_width);
    QTabWidget::resizeEvent(ev);
}

void QtExtTabWidget::OnCloseTab(int index)
{
    this->removeTab(index); 
    emit TabClosed(index);
    if (this->currentIndex() == index && index-1>0)
        this->setCurrentIndex(index-1);
}

void QtExtTabWidget::OnTabBarDoubleClicked(int index)
{
    if (-1 != index) {
        OnCloseTab(index);
    } else {
        emit tabBarDoubleClicked();
    }
}

void QtExtTabWidget::setupUI()
{
    tab_bar_ = new QtExtTabBar(this);
    setTabBar(tab_bar_);
    connect(tab_bar_, SIGNAL(AddBtnClicked()), this, SIGNAL(AddBtnClicked()));
    connect(tab_bar_, SIGNAL(CloseTab(int)), this, SLOT(OnCloseTab(int)));
    connect(tab_bar_, SIGNAL(CloseTab(int)), this, SLOT(TabClosed(int)));

    button_widget_ = new ButtonWidget(this);
    connect(button_widget_, SIGNAL(closeClicked()), this, SIGNAL(closeWnd()));
    connect(button_widget_, SIGNAL(minClicked()), this, SIGNAL(minWnd()));
    connect(button_widget_, SIGNAL(maxClicked()), this, SIGNAL(maxWnd()));
    connect(button_widget_, SIGNAL(restoreClicked()), this, SIGNAL(restoreWnd()));

    connect(this, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(OnTabBarDoubleClicked(int)));
}