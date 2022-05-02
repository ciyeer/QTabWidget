#include "button_widget.h"
#include "ui_button_widget.h"

#include <QApplication>

ButtonWidget::ButtonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonWidget)
{
    ui->setupUi(this);
    setupUI();
}

ButtonWidget::~ButtonWidget()
{
    delete ui;
}

void ButtonWidget::setMaxRestoreBtnVisible(bool max_visible, bool restore_visible)
{
    ui->btn_max->setVisible(max_visible);
    ui->btn_restore->setVisible(restore_visible);
}

void ButtonWidget::setupUI()
{
    ui->btn_restore->setVisible(false);
}

void ButtonWidget::on_btn_close_clicked()
{
    emit closeClicked();
}

void ButtonWidget::on_btn_min_clicked()
{
    emit minClicked();
}

void ButtonWidget::on_btn_max_clicked()
{
    emit maxClicked();
//    ui->btn_max->setVisible(false);
//    ui->btn_restore->setVisible(true);
}

void ButtonWidget::on_btn_restore_clicked()
{
    emit restoreClicked();
//    ui->btn_restore->setVisible(false);
//    ui->btn_max->setVisible(true);
}
