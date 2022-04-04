#ifndef QTEXTTABBAR_H
#define QTEXTTABBAR_H

#include <QtWidgets>

#define COLOR_HOVER         "#D6D6D6"
#define COLOR_SELECT_0      "#4F94CD"
#define COLOR_SELECT_1      "#EEB422"
#define COLOR_NORMAL_0      "#63B8FF"
#define COLOR_NORMAL_1      "#FFFF00"
#define COLOR_MARK          "#FF3333"

//1.文字颜色可设置
//2.文字大小和字体设置
//3.tab button大小和颜色可设置

class QtExtTabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit QtExtTabBar(QWidget *parent = 0);

    QSize tabSize() const { return tab_size_; }
    void setTabSize(const QSize &tabSize) { tab_size_ = tabSize; }
    QSize tabBtnSize() const { return tab_add_btn_size_; }
    void setTabBtnSize(const QSize &tabBtnSize) { tab_add_btn_size_ = tabBtnSize; }
    bool drawPlusBtn() const { return draw_plus_btn_; }
    void setPlusBtn(bool isDraw) { draw_plus_btn_ = isDraw; }

protected:
    QSize tabSizeHint(int index) const;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    int PointInTabRectIndex(const QPoint &point);
    void DrawTabBg(QPainter *painter);
    void DrawTabText(QPainter *painter);
    void DrawTabButton();
    void DrawPlusBtn(QPainter *painter);
    void _drawTabBg(QPainter *painter, int index);

Q_SIGNALS:
    void BtnClicked();

private:
    QMargins margins_ = {-20, 0, -20, 0};
    QSize tab_size_ = QSize(230, 40);
    QSize tab_add_btn_size_ = QSize(20, 20);
    bool draw_plus_btn_ = true;
    
};

#endif // QTEXTTABBAR_H
