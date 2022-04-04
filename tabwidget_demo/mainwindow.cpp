#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qt_ext_table_widget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
    ui->tabWidget->setStyleSheet("QTabWidget::pane{border:none;background:none;}\
                                 QTabWidget::pane:top{top:0px;}\
                                 QTabWidget::pane:bottom{bottom:-1px;}\
                                 QTabWidget::pane:left{right:-1px;}\
                                 QTabWidget::pane:right{left:-1px;}");
    ui->tab1->setStyleSheet(QString("QWidget{background:%1;}").arg(COLOR_SELECT_0));
    ui->tab2->setStyleSheet(QString("QWidget{background:%1;}").arg(COLOR_SELECT_1));
    ui->tabWidget->tabBar()->setStyleSheet("border:1px solid green");
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
