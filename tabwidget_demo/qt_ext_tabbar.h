#ifndef QTEXTTABBAR_H
#define QTEXTTABBAR_H

#include <QtWidgets>

#define COLOR_HOVER         "#D6D6D6"
#define COLOR_SELECT_0      "#4F94CD"
#define COLOR_SELECT_1      "#EEB422"
#define COLOR_NORMAL_0      "#63B8FF"
#define COLOR_NORMAL_1      "#FFFF00"
#define COLOR_MARK          "#FF3333"

class RoundShadowHelper;

//- 1.������ɫ������
//2.���ִ�С����������
//- 3.tab button��С����ɫ������
//4. ��껬��tabĳЩָ������ʱ��������Ӧ

class QtExtTabBar : public QTabBar
{
    Q_OBJECT

public:
    struct TB_TEXT_COLOR {
        QColor Normal_ = Qt::black;
        QColor Hover_ = Qt::yellow;
        QColor Selected_ = Qt::red;
        // no disabled property
        // QColor Disabled_ = Qt::gray;
    };

    struct TB_BG_COLOR {
        QColor Normal_ = Qt::transparent;
        QColor Hover_ = QColor(214, 214, 214);
        QColor Selected_ = Qt::white;
        // no disabled property
        // QColor Disabled_ = Qt::gray;
    };

public:
    explicit QtExtTabBar(QWidget *parent = 0);

    QSize tabSize() const { return tab_size_; }
    void setTabSize(const QSize &tab_size) { tab_size_ = tab_size; }

    QSize tabBtnSize() const { return tab_add_btn_size_; }
    void setTabBtnSize(const QSize &tab_btn_size) { tab_add_btn_size_ = tab_btn_size; }

    bool drawPlusBtn() const { return draw_plus_btn_; }
    void setPlusBtn(bool is_draw) { draw_plus_btn_ = is_draw; update(); }

    TB_TEXT_COLOR TBTextColor() const { return tb_text_color_;}
    void setTBTextColor(TB_TEXT_COLOR text_color) { tb_text_color_ = text_color; update();}

protected:
    QSize tabSizeHint(int index) const;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool event(QEvent *ev) override;


private:
    int PointInTabRectIndex(const QPoint &point);
    void DrawTab(QPainter *painter);
    void DrawTabButton();
    void DrawPlusBtn(QPainter *painter);
    void _drawTabBg(QPainter *painter, RoundShadowHelper &helper, 
                    const QStyleOptionTabV3 &option, 
                    QRect draw_rect, QRect real_rect);
    void _drawTabText(QPainter *painter, 
                        const QRect &draw_rect, 
                        const QStyleOptionTabV3 &option);
    void _drawLeftIcon(QPainter *painter, const QStyleOptionTabV3 &option);
    void _drawRightIcon(QPainter *painter, const QStyleOptionTabV3 &option);
    bool _isDrawLeftIcon() const;
    bool _isDrawRightIcon() const;

Q_SIGNALS:
    void BtnClicked();

private:
    static const int icon_padding_ = 8;

private:
    QMargins margins_ = {-28, 0, -20, 0};
    QSize tab_size_ = QSize(230, 40);
    QSize last_tab_size_ = QSize(30, 40);
    QSize tab_add_btn_size_ = QSize(20, 20);
    bool draw_plus_btn_ = true;
    TB_TEXT_COLOR  tb_text_color_;
    TB_BG_COLOR tb_bg_color_;
    QRect icon_left_rect_ = {0, 0, 0, 0};
    QSize icon_left_size_ = QSize(16, 16);
    QPixmap icon_left_pixmap_ = QPixmap(":/images/x-capture-options.png");
    QRect icon_right_rect_ = {0, 0, 0, 0};
    QSize icon_right_size_ = QSize(16, 16);
    QPixmap icon_right_pixmap_ = QPixmap(":/images/x-capture-options.png");
    QColor tab_btn_add_color_  = Qt::transparent;
};

#endif // QTEXTTABBAR_H
