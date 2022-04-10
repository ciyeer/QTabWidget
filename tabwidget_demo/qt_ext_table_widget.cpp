#include "qt_ext_table_widget.h"

#include <qpainter.h>
class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 100; // 设置每个tabBar中item的大小
            s.rheight() = 68;
        }
        return s;
    }
    virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
        const QWidget *w = Q_NULLPTR) const
    {
        if (pe == QStyle::PE_IndicatorTabTear) {
            int i = 0;
        }
        QProxyStyle::drawPrimitive(pe, opt, p, w);
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
        }

        if (element == CE_TabBarTab)
        {
            QProxyStyle::drawControl(element, option, painter, widget);
        }

    }
};


QtExtTabWidget::QtExtTabWidget(QWidget *parent)
{
    QtExtTabBar *tab_bar = new QtExtTabBar(this);
    setTabBar(tab_bar);
    connect(tab_bar, SIGNAL(AddBtnClicked()), this, SLOT(OnAddButon()));
    connect(tab_bar, SIGNAL(AddBtnClicked()), this, SIGNAL(AddBtnClicked()));
    connect(tab_bar, SIGNAL(CloseTab(int)), this, SLOT(OnCloseTab(int)));
}

void QtExtTabWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawRect(this->rect());

    QTabWidget::paintEvent(ev);
}

void QtExtTabWidget::OnAddButon()
{
    this->insertTab(this->tabBar()->count()-1, new QWidget(), "test");
}

void QtExtTabWidget::OnCloseTab(int index)
{
    this->removeTab(index); 
}
