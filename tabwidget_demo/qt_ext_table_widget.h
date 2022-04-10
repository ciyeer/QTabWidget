#ifndef QTEXTTABWIDGET_H
#define QTEXTTABWIDGET_H

#include "qt_ext_tabbar.h"

#include <QTabWidget>

class QtExtTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit QtExtTabWidget(QWidget *parent = 0);
    ~QtExtTabWidget() = default;
    void addTab2(QWidget *widget, const QString &label);
    void insertTab2(int index, QWidget *widget, const QString &label);

protected:
    virtual void paintEvent(QPaintEvent *ev) override;

signals:
    void AddButtonClick();

protected slots:
    void OnAddButon();
    void OnCloseTab(int index);

};

#endif // QtExtTabWidget_H
