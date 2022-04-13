#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

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
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
