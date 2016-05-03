#include "GCMPUI/gcmpframelesswidget.h"
#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <qmath.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QCursor>
#include <assert.h>
#include "GCMPUI/GCMPUiCommon.h"

const int g_nXOffset = 3;
const int g_nYOffset = 3;
const int g_nXRadius = 3;
const int g_nYRadius = 3;
const int g_nPadding = 3;

GCMPFramelessWidget::GCMPFramelessWidget(const QString strTitle, GCMPFrameTitleStyle oStyle, QWidget* pCenterWidget, QWidget *parent)
    : QWidget(parent), m_strTitle(strTitle), m_bMaxed(false), m_bPressed(false), m_bBeginResizing(false), m_pCenterWidget(pCenterWidget)
    , m_oStyle(oStyle)
{
    assert(m_pCenterWidget != nullptr);
    if (m_pCenterWidget)
    {
        m_pCenterWidget->setParent(this);
    }
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //addShadowEffect();
    adaptToDesktop();
    createAndInitContent();
    this->setMouseTracking(true);
    this->installEventFilter(this);
}

GCMPFramelessWidget::~GCMPFramelessWidget()
{
   
}

void GCMPFramelessWidget::addShadowEffect()
{
    QGraphicsDropShadowEffect *pShadowEff = new QGraphicsDropShadowEffect(this);
    pShadowEff->setOffset(-5, 5);
    pShadowEff->setColor(Qt::gray);
    pShadowEff->setBlurRadius(2);
    this->setGraphicsEffect(pShadowEff);
}

void GCMPFramelessWidget::adaptToDesktop()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //得到客户区矩形
    QRect clientRect = desktopWidget->availableGeometry();
    clientRect.setX(clientRect.x() + 10);
    clientRect.setY(clientRect.y() + 10);
    clientRect.setWidth(clientRect.width() - 20);
    clientRect.setHeight(clientRect.height() - 20);
    this->setGeometry(clientRect);
}

void GCMPFramelessWidget::paintEvent(QPaintEvent * pEvent)
{
    //强行绘制阴影
    paintShadowFrame();
}

void GCMPFramelessWidget::paintShadowFrame()
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(g_nXOffset, g_nYOffset, 
        this->width() - 2 * g_nXOffset, this->height() - 2 * g_nYOffset, g_nXRadius, g_nYRadius);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));
    painter.setPen(QColor(0, 0, 0, 255));
    painter.drawPath(path);
    QColor color(0, 0, 0, 0);
    int nAlphaBase = 50;
    for (int i = 0; i < 3; i++)
    {
        QPainterPath shadowPath;
        shadowPath.setFillRule(Qt::WindingFill);
        shadowPath.addRoundedRect(g_nXOffset - i, g_nYOffset - i, this->width() - (g_nXOffset - i) * 2,
            this->height() - (g_nYOffset - i) * 2, g_nXRadius, g_nYRadius);
        color.setAlpha(nAlphaBase + i * 10);
        painter.setPen(color);
        painter.drawPath(shadowPath);
    }
}

void GCMPFramelessWidget::createAndInitContent()
{
    m_pTitleWidet = createTitleWidget();
    m_pTitleWidet->setMouseTracking(true);
    m_pTitleWidet->installEventFilter(this);
    if (m_pCenterWidget == nullptr)
    {
        m_pCenterWidget = new QWidget(this);
    }
    m_pCenterWidget->setMouseTracking(true);
    m_pCenterWidget->installEventFilter(this);
    QVBoxLayout* pFrameLayout = new QVBoxLayout(this);
    pFrameLayout->setMargin(0);
    pFrameLayout->setSpacing(0);
    pFrameLayout->setContentsMargins(0, 0, 0, 0);

    pFrameLayout->addWidget(m_pTitleWidet);
    pFrameLayout->addWidget(m_pCenterWidget);
    pFrameLayout->setStretchFactor(m_pTitleWidet, 1);
    pFrameLayout->setStretchFactor(m_pCenterWidget, 19);
    this->setLayout(pFrameLayout);
    this->setMinimumSize(g_MinmumFrameSize);
}

QWidget* GCMPFramelessWidget::createTitleWidget()
{
    QColor oGray = QColor(231, 231, 231, 255);
    QColor oBlue = QColor(78, 166, 234, 255);
    QColor oRed = QColor(255, 0, 0, 200);
    QColor oDarkRed = QColor(230, 0, 0, 200);

    QWidget* pTitleWidget = new QWidget(this);
    m_pTitleLabel = new QLabel(pTitleWidget);
    m_pTitleLabel->setText(m_strTitle);
    m_pTitleLabel->setAlignment(m_oStyle == gtsLeft ? Qt::AlignRight : Qt::AlignCenter);
    m_pTitleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    GCMPButton* pMinimizeBtn = new GCMPButton(QIcon("://GDBCompare//minimize"), oGray, oBlue, QSize(35, 30), pTitleWidget);
    pMinimizeBtn->setMinimumSize(QSize(35, 30));
    pMinimizeBtn->setFixedSize(QSize(35, 30));
    pMinimizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    GCMPButton* pMaxmizeBtn = new GCMPButton(QIcon("://GDBCompare//maxmize"), oGray, oBlue, QSize(35, 30), pTitleWidget);
    pMaxmizeBtn->setMinimumSize(QSize(35, 30));
    pMaxmizeBtn->setFixedSize(QSize(35, 30));
    pMaxmizeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    GCMPButton* pCloseBtn = new GCMPButton(QIcon("://GDBCompare//close"), oRed, oDarkRed, QSize(35, 30), pTitleWidget);
    pCloseBtn->setMinimumSize(QSize(35, 30));
    pCloseBtn->setFixedSize(QSize(35, 30));
    pCloseBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    //signal - slot
    connect(pMinimizeBtn, SIGNAL(clicked()), this, SLOT(onMinimize()));
    this->connect(pMaxmizeBtn, SIGNAL(clicked()), this, SLOT(onMaximize()));
    this->connect(pCloseBtn, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout* pHLayout = new QHBoxLayout(this);
    pHLayout->setContentsMargins(g_nXOffset + 15, g_nYOffset + 1, 0, 0);
    pHLayout->setMargin(4);
    QSpacerItem* pSpaceItem = new QSpacerItem(500, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if (m_oStyle == gtsLeft)
    {
        pHLayout->addWidget(m_pTitleLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
        pHLayout->addSpacerItem(pSpaceItem);
        pHLayout->setSpacing(0);
    }
    else
    {
        pHLayout->addSpacerItem(pSpaceItem);
        pHLayout->addWidget(m_pTitleLabel, 0, Qt::AlignHCenter | Qt::AlignVCenter);
        pHLayout->setSpacing(0);
    }
    pHLayout->addWidget(pMinimizeBtn, 0, Qt::AlignRight | Qt::AlignTop);
    pHLayout->addWidget(pMaxmizeBtn, 0, Qt::AlignRight | Qt::AlignTop);
    pHLayout->addWidget(pCloseBtn, 0, Qt::AlignRight | Qt::AlignTop);
    pTitleWidget->setLayout(pHLayout);
    return pTitleWidget;
}

void GCMPFramelessWidget::onMaximize()
{
    if (m_bMaxed)
    {
        this->showNormal();
    }
    else
    {
        this->showMaximized();
    }
    m_bMaxed = !m_bMaxed;
}

bool GCMPFramelessWidget::eventFilter(QObject *pObj, QEvent *pEvnt)
{
    MouseEventType oType = processMouseEvent(pEvnt);
    QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvnt);
    QPoint oCurPoint = QPoint(0, 0);
    if (pMouseEvent != nullptr)
    {
        oCurPoint = pMouseEvent->globalPos();
    }
    if (pEvnt->type() == QEvent::MouseButtonPress)
    {
        //在客户区里点了一下
        if (m_enCursorType == cptNone)
        {
            if (pObj == m_pTitleWidet)
            {
                m_bPressed = true;
                m_oPressPoint = oCurPoint;
                m_oRefPos = this->pos();
                return true;
            }
            if (pObj == this)
            {
                mousePressEvent(pMouseEvent);
            }
        }
        else
        {
            //想resize了
            if (oType == metLButtomPress)
            {
                m_bBeginResizing = true;
                m_oStartPnt = oCurPoint;
                this->mouseGrabber();
            }
        }
    }

    else if (pEvnt->type() == QEvent::MouseButtonRelease)
    {
        //在客户区里松开了
        if (m_enCursorType == cptNone)
        {
            if (pObj == m_pTitleWidet)
            {
                m_bPressed = false;
                m_oPressPoint = QPoint(0, 0);
                return true;
            }
            if (pObj == this)
            {
                QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvnt);
                mousePressEvent(pMouseEvent);
            }
        }
        else
        {
            //想resize了
            if (oType == metLButtomRelease)
            {
                m_bBeginResizing = false;
                this->releaseMouse();
            }
        }
    }

    if (pEvnt->type() == QEvent::MouseMove)
    {
        //在客户区里移动
        if (m_enCursorType == cptNone)
        {
            if (pObj == m_pTitleWidet)
            {
                if (m_bPressed)
                {
                    QPoint oTarget = oCurPoint;
                    this->move(m_oRefPos + (oTarget - m_oPressPoint));
                    return true;
                }
            }
        }
        else
        {
            if (oType == metMouseMove)
            {
                resizeWindow(oCurPoint);
            }
        }
    }

    return QWidget::eventFilter(pObj, pEvnt);
}

GCMPFramelessWidget::CursorPosType GCMPFramelessWidget::getCursorPosType(QPoint& oCursorPos)
{
    GCMPFramelessWidget::CursorPosType oRet = m_enCursorType;

    QRect rc = this->rect();
    QPoint lt = mapToGlobal(rc.topLeft()) - QPoint(g_nXOffset, g_nYOffset);
    QPoint rb = mapToGlobal(rc.bottomRight()) + QPoint(g_nXOffset, g_nYOffset);
    QPoint lb = QPoint(lt.x(), rb.y());
    QPoint rt = QPoint(rb.x(), lt.y());

    //左上角点区域
    QRect ltPointRc = QRect(lt, lt + QPoint(2 * g_nXOffset, 2 * g_nYOffset));
    //左下角点区域
    QRect lbPointRc = QRect(lb, lb + QPoint(2 * g_nXOffset, -2 * g_nYOffset));
    //右上角点区域
    QRect rtPointRc = QRect(rt, rt + QPoint(-2 * g_nXOffset, 2 * g_nYOffset));
    //右下交点区域
    QRect rbPointRc = QRect(rb, rb + QPoint(-2 * g_nXOffset, -2 * g_nYOffset));

    //左边栏区域
    QRect lRc = QRect(QPoint(ltPointRc.x(), ltPointRc.y() + ltPointRc.height()), QPoint(lbPointRc.x() + lbPointRc.width(), lbPointRc.y()));
    //右边栏区域
    QRect rRC = QRect(QPoint(rtPointRc.x(), rtPointRc.y() + rtPointRc.height()), QPoint(rbPointRc.x() + rbPointRc.width(), rbPointRc.y()));
    //上边栏区域
    QRect tRC = QRect(QPoint(ltPointRc.x() + ltPointRc.width(), ltPointRc.y()), QPoint(rtPointRc.x(), rtPointRc.y() + rtPointRc.height()));
    //下边栏区域
    QRect bRC = QRect(QPoint(lbPointRc.x() + lbPointRc.width(), lbPointRc.y()), QPoint(rbPointRc.x(), rbPointRc.y() + rbPointRc.height()));

    if (ltPointRc.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
        oRet = cptLeftTop;
    }

    else if (lbPointRc.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
        oRet = cptLeftBottom;
    }

    else if (rtPointRc.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
        oRet = cptRightTop;
    }

    else if (rbPointRc.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
        oRet = cptRightBottom;
    }

    else if (lRc.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeHorCursor));
        oRet = cptLeft;
    }

    else if (rRC.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeHorCursor));
        oRet = cptRight;
    }

    else if (tRC.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeVerCursor));
        oRet = cptTop;
    }

    else if (bRC.contains(oCursorPos))
    {
        this->setCursor(QCursor(Qt::SizeVerCursor));
        oRet = cptBottom;
    }

    else if (!m_bBeginResizing)
    {
        this->setCursor(QCursor(Qt::ArrowCursor));
        oRet = cptNone;
    }
    return oRet;
}

GCMPFramelessWidget::MouseEventType GCMPFramelessWidget::processMouseEvent(QEvent* pEvent)
{
    QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
    if (pMouseEvent != nullptr)
    {
        QPoint oCursorGloable = pMouseEvent->globalPos();
        m_enCursorType = getCursorPosType(oCursorGloable);
        if (pMouseEvent->type() == QEvent::MouseButtonPress && pMouseEvent->button() == Qt::LeftButton)
        {
            return metLButtomPress;
        }

        else if (pMouseEvent->type() == QEvent::MouseMove)
        {
            return metMouseMove;
        }

        else if (pMouseEvent->type() == QEvent::MouseButtonRelease && pMouseEvent->button() == Qt::LeftButton)
        {
            return metLButtomRelease;
        }

        else
        {
            return metElse;
        }
    }
    else
    {
        return metNone;
    }
}

void GCMPFramelessWidget::resizeWindow(QPoint oCurPoint)
{
    if (m_enCursorType != cptNone && m_bBeginResizing)
    {
        QRect oGeometry = this->rect();
        QPoint lt = mapToGlobal(oGeometry.topLeft());
        QPoint rb = mapToGlobal(oGeometry.bottomRight());
        QPoint lb = QPoint(lt.x(), rb.y());
        QPoint rt = QPoint(rb.x(), lt.y());
        QPoint vec = oCurPoint - m_oStartPnt;
        QRect rcResult = QRect(lt, rb);
        switch (m_enCursorType)
        {
        case GCMPFramelessWidget::cptNone:
            break;

        case GCMPFramelessWidget::cptLeftTop:
        {
            rcResult.setTopLeft(oCurPoint);
            break;
        }

        case GCMPFramelessWidget::cptLeftBottom:
        {
            rcResult.setBottomLeft(oCurPoint );
            break;
        }
        case GCMPFramelessWidget::cptRightTop:
        {
            rcResult.setTopRight(oCurPoint);
            break;
        }
        case GCMPFramelessWidget::cptRightBottom:
        {
            rcResult.setBottomRight(oCurPoint);
            break;
        }
        case GCMPFramelessWidget::cptLeft:
        {
            rcResult.setLeft(oCurPoint.x());
            break;
        }
        case GCMPFramelessWidget::cptRight:
        {
            rcResult.setRight(oCurPoint.x());
            break;
        }
        case GCMPFramelessWidget::cptTop:
        {
            rcResult.setTop(oCurPoint.y());
            break;
        }
        case GCMPFramelessWidget::cptBottom:
        {
            rcResult.setBottom(oCurPoint.y());
            break;
        }
        default:
            break;
        }

        QSize oMinimize = this->minimumSize();
        QSize curSize = QSize(rcResult.width(), rcResult.height());
        if (curSize.width() >= oMinimize.width() && curSize.height() >= oMinimize.height())
        {
            this->setGeometry(rcResult);
        }
    }
}



