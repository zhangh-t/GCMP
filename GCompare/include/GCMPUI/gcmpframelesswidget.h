#ifndef GCMPFRAMELESSWIDGET_H
#define GCMPFRAMELESSWIDGET_H
//无边框带阴影圆角矩形窗体
#include <QWidget>
#include <QLabel>
#include "GCMPUI/GCMPButton.h"

enum GCMPUI_EXPORT GCMPFrameTitleStyle
{
    gtsNone = -1,
    gtsLeft = 0,
    gtsMiddle,
};

class GCMPUI_EXPORT GCMPFramelessWidget : public QWidget
{
    Q_OBJECT

public:
    GCMPFramelessWidget(const QString strTitle, GCMPFrameTitleStyle oStyle, QWidget* pCenterWidget, QWidget *parent = 0);
    ~GCMPFramelessWidget();

public:
    void setTitle(QString strTitle) { m_strTitle = strTitle; }
    GCMPFrameTitleStyle getTitleStyle() const { return m_oStyle; }
    QWidget* centerWidget() const { return m_pCenterWidget; }
    QWidget* titleWidget() const { return m_pTitleWidet; }
    QLabel* titleLabel() const { return m_pTitleLabel; }
protected:
    virtual void paintEvent(QPaintEvent * pEvent);

protected:
    virtual void addShadowEffect(); //不适合主窗体
    virtual void adaptToDesktop();
    virtual bool eventFilter(QObject *pObj, QEvent *pEvnt);

private slots:
    void onMinimize() { this->showMinimized(); }
    void onMaximize();

private:
    enum CursorPosType
    {
        cptNone = -1,       //没有在边栏上
        cptLeftTop = 0,     //左上角
        cptLeftBottom,      //左下角
        cptRightTop,        //右上角
        cptRightBottom,     //右下角
        cptLeft,            //左边框
        cptRight,           //右边框
        cptTop,             //上边框
        cptBottom,          //下边框
    };

    enum MouseEventType
    {
        metNone = -1,
        metMouseMove = 0,
        metLButtomPress,
        metLButtomRelease,
        metElse,
    };

    void paintShadowFrame();
    void createAndInitContent();
    QWidget* createTitleWidget();
    MouseEventType processMouseEvent(QEvent* pEvent);
    void resizeWindow(QPoint oCurPoint);

private:
    //改变鼠标样式
    CursorPosType getCursorPosType(QPoint& oCursorPos);

private:
    QWidget* m_pTitleWidet;
    QLabel* m_pTitleLabel;
    QString m_strTitle;
    bool m_bMaxed;
private:
    bool m_bPressed;
    QPoint m_oPressPoint;
    QPoint m_oRefPos;
private:
    bool m_bBeginResizing;
    QPoint m_oStartPnt;
    CursorPosType m_enCursorType;
    QWidget* m_pCenterWidget;
    GCMPFrameTitleStyle m_oStyle;
};

#endif // GCMPFRAMELESSWIDGET_H
