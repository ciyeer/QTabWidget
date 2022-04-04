#ifndef QTEXTTABWIDGET_H
#define QTEXTTABWIDGET_H

#include "qt_ext_tabbar.h"

#include <QPaintEvent>
#include <QTabWidget>

class QtExtTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit QtExtTabWidget(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *ev) override;

};

#endif // QtExtTabWidget_H
