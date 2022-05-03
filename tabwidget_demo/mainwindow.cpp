#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qt_ext_table_widget.h"
#include "NcFrameLessHelper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();

    QWidget *widget = new QWidget();
    widget->setStyleSheet("background-color:#FF00FF00");
    ui->tabWidget->addTab2(widget, tr("this is first tab"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete frame_less_helper_;
}

void MainWindow::setupUI()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    frame_less_helper_ = new NcFramelessHelper();
    frame_less_helper_->activeOnWithChildWidget(this, ui->tabWidget->tabBar());

    connect(ui->tabWidget, SIGNAL(TabInserted(int)), this, SLOT(OnTabInserted(int)));
    connect(ui->tabWidget, SIGNAL(AddBtnClicked()), this, SLOT(OnAddBtnClicked()));
    connect(ui->tabWidget, SIGNAL(TabClosed(int)), this, SLOT(OnCloseTab(int)));
    connect(ui->tabWidget, SIGNAL(closeWnd()), this, SLOT(OnCloseWnd()));
    connect(ui->tabWidget, SIGNAL(minWnd()), this, SLOT(OnMinWnd()));
    connect(ui->tabWidget, SIGNAL(maxWnd()), this, SLOT(OnMaxWnd()));
    connect(ui->tabWidget, SIGNAL(restoreWnd()), this, SLOT(OnRestoreWnd()));
    ui->tabWidget->setTabBKImage(":/images/default_100_precent/tab.png",
                                ":/images/default_100_precent/tab_hover.png", 
                                ":/images/default_100_precent/tab_check.png");
    ui->tabWidget->setTabBarBKImage(":/images/default_100_precent/caption_bg.png");
    ui->tabWidget->setTabTextColor(Qt::white, Qt::white, Qt::white);
}

void MainWindow::OnTabInserted(int index)
{
    QPushButton *button = new QPushButton();
    button->setFixedSize(this->iconSize());
    button->setStyleSheet("border-image: url(:/images/x-capture-options.png);");
    ui->tabWidget->setTabButton2(index, QTabBar::LeftSide, button);

    button = new QPushButton();
    button->setStyleSheet("QPushButton{border-image: url(:/images/close.png)}"
                          "QPushButton:hover{border-image: url(:/images/close_hover.png)}");
    ui->tabWidget->setTabButton2(index, QTabBar::RightSide, button);
}

void MainWindow::OnTabClosed(int index)
{
    //todo something
}

void MainWindow::OnAddBtnClicked()
{
    ui->tabWidget->addTab2(new QWidget(), tr("this is first tab"));
}

void MainWindow::OnCloseWnd()
{
    this->close();
}

void MainWindow::OnMinWnd()
{
    this->showMinimized();
}

void MainWindow::OnMaxWnd()
{
    this->showMaximized();
    ui->tabWidget->setMaxRestoreBtnVisible(false, true);
    ui->tabWidget->currentWidget()->setFocus();
}

void MainWindow::OnRestoreWnd()
{
    this->showNormal();
    ui->tabWidget->setMaxRestoreBtnVisible(true, false);
}
