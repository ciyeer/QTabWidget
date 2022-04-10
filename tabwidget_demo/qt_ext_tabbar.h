#ifndef QTEXTTABBAR_H
#define QTEXTTABBAR_H

#include <QtWidgets>

class RoundShadowHelper;

//- 1.文字颜色可设置
//2.文字大小和字体设置
//- 3.tab button大小和颜色可设置
//- 4. 鼠标滑过tab某些指定区域时，做出反应
//- 5. 采用style提供的借口绘制icon和button

class QtExtTabBar;

class QExtTabBarStyle: public QProxyStyle
{
public:
    QExtTabBarStyle(QtExtTabBar *tab_bar, QStyle *parent = Q_NULLPTR) 
        : QProxyStyle(parent) { tab_bar_ = tab_bar; }
    ~QExtTabBarStyle() = default;

    void drawPrimitive(PrimitiveElement pe, 
                    const QStyleOption *option, 
                    QPainter *painter,
                    const QWidget *widget = Q_NULLPTR) const Q_DECL_OVERRIDE;
    int pixelMetric(PixelMetric metric, 
                const QStyleOption *option, 
                const QWidget *widget) const Q_DECL_OVERRIDE;
    
    virtual QRect 
    subElementRect(SubElement element, 
                    const QStyleOption *option, 
                    const QWidget *widget) const Q_DECL_OVERRIDE;
    // 1.定制通用的接口

private:
    QRect calcIconRect(bool left, const QStyleOption *option) const;

private:
    QtExtTabBar *tab_bar_;
};

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

    struct TAB_ADD_BUTTON {
        bool draw_plus_btn_ = true;
        QSize last_tab_size_ = QSize(30, 40);
        QSize tab_add_btn_size_ = QSize(20, 20);
    };

public:
    explicit QtExtTabBar(QWidget *parent = 0, TAB_ADD_BUTTON tab_add_button = TAB_ADD_BUTTON());

    QSize tabSize() const { return tab_size_; }
    void setTabSize(const QSize &tab_size) { tab_size_ = tab_size; }

    TAB_ADD_BUTTON tabAddButton(TAB_ADD_BUTTON tab_add_button) { tab_add_button_ = tab_add_button; }
    void setTabAddButton(const TAB_ADD_BUTTON &tab_add_button) { 
        tab_add_button_ = tab_add_button; 
        if (tab_add_button_.draw_plus_btn_) 
            update();
    }

    TB_TEXT_COLOR TBTextColor() const { return tb_text_color_;}
    void setTBTextColor(TB_TEXT_COLOR text_color) { tb_text_color_ = text_color; update();}
    
    TB_BG_COLOR TBBGColor() const { return tb_bg_color_;}
    void setTBBGColor(TB_BG_COLOR bg_color) { tb_bg_color_ = bg_color; update();}

    void UpdateTab(int index);

protected:
    QSize tabSizeHint(int index) const;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual bool event(QEvent *ev) override;
    virtual void tabInserted(int index) override;

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

Q_SIGNALS:
    void AddBtnClicked();
    void CloseTab(int index);

protected slots:
    void OnRightWidgetClicked();

private:
    QMargins margins_ = {-28, 0, -20, 0};
    QSize tab_size_ = QSize(230, 40);
    TB_TEXT_COLOR  tb_text_color_;
    TB_BG_COLOR tb_bg_color_;
    QColor tab_btn_add_color_  = Qt::transparent;
    TAB_ADD_BUTTON tab_add_button_;
};

#endif // QTEXTTABBAR_H
