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
    void setTabButton2(int index, QTabBar::ButtonPosition, QPushButton *btn);

protected:
    virtual void paintEvent(QPaintEvent *ev) override;
    virtual void tabInserted(int index) override;

signals:
    void AddBtnClicked();
    void TabInserted(int index);
    void TabClosed(int index);

protected slots:
    void OnCloseTab(int index);

private:
    QtExtTabBar *tab_bar_;

};

#endif // QtExtTabWidget_H
