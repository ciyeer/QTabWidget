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
    ~QtExtTabWidget() = default;

protected:
    virtual void paintEvent(QPaintEvent *ev) override;

signals:
    void AddButtonClick();

protected slots:
    void OnAddButon();
    void OnCloseTab(int index);

};

#endif // QtExtTabWidget_H
