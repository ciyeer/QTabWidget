#ifndef QTEXTTABWIDGET_H
#define QTEXTTABWIDGET_H

#include "qt_ext_tabbar.h"

#include <QWidget>
#include <QTabWidget>

class ButtonWidget;

class QtExtTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit QtExtTabWidget(QWidget *parent = 0);
    ~QtExtTabWidget() = default;
    void addTab2(QWidget *widget, const QString &label);
    void insertTab2(int index, QWidget *widget, const QString &label);
    void setTabButton2(int index, QTabBar::ButtonPosition, QPushButton *btn);
    void setTabBKImage(const QString &normal_image, 
                        const QString &hover_image, 
                        const QString &selected_image);
    void setTabBarBKImage(const QString &image_path);
    void setTabTextColor(const QColor &normal, 
                        const QColor &hover, 
                        const QColor &select);
    void setMaxRestoreBtnVisible(bool max_visible, bool restore_visible);

protected:
    virtual void paintEvent(QPaintEvent *ev) override;
    virtual void tabInserted(int index) override;
    virtual void resizeEvent(QResizeEvent *ev) override;

signals:
    void AddBtnClicked();
    void TabInserted(int index);
    void TabClosed(int index);
    void closeWnd();
    void minWnd();
    void maxWnd();
    void restoreWnd();
    void tabBarDoubleClicked();

protected slots:
    void OnCloseTab(int index);
    void OnTabBarDoubleClicked(int index);


private:
    void setupUI();

private:
    QtExtTabBar *tab_bar_;
    ButtonWidget *button_widget_;

};

#endif // QtExtTabWidget_H
