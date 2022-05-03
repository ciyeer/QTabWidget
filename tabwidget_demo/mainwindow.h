#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class NcFramelessHelper;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setupUI();

protected slots:
    void OnTabInserted(int index);
    void OnTabClosed(int index);
    void OnAddBtnClicked();
    void OnCloseWnd();
    void OnMinWnd();
    void OnMaxWnd();
    void OnRestoreWnd();
    void OnMaxOrRestore();

private:
    Ui::MainWindow *ui;
    NcFramelessHelper *frame_less_helper_;
};

#endif // MAINWINDOW_H
