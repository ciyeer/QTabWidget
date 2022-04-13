#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qt_ext_table_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    ui->tabWidget->addTab2(new QWidget(), tr("this is first tab"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    connect(ui->tabWidget, SIGNAL(TabInserted(int)), this, SLOT(OnTabInserted(int)));
    connect(ui->tabWidget, SIGNAL(AddBtnClicked()), this, SLOT(OnAddBtnClicked()));
    connect(ui->tabWidget, SIGNAL(TabClosed(int)), this, SLOT(OnCloseTab(int)));
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
