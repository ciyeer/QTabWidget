#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qt_ext_table_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->addTab2(new QWidget(), tr("this is first tab"));

    QPushButton *button = new QPushButton();
    button->setFixedSize(this->iconSize());
    button->setStyleSheet("border-image: url(:/images/x-capture-options.png);");
    ui->tabWidget->setTabButton2(0, QTabBar::LeftSide, button);

    button = new QPushButton();
    button->setStyleSheet("QPushButton{border-image: url(:/images/close.png)}"
                          "QPushButton:hover{border-image: url(:/images/close_hover.png)}");
    ui->tabWidget->setTabButton2(0, QTabBar::RightSide, button);
}

MainWindow::~MainWindow()
{
    delete ui;
}
