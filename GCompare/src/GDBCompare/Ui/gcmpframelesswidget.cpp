#include "GDBCompare/Ui/gcmpframelesswidget.h"
#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <qmath.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>


const int g_nXOffset = 3;
const int g_nYOffset = 3;
const int g_nXRadius = 3;
const int g_nYRadius = 3;

GCMPFramelessWidget::GCMPFramelessWidget(const QString strTitle, QWidget *parent)
    : QWidget(parent), m_strTitle(strTitle), m_bMaxed(false), m_bPressed(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //addShadowEffect();
    adaptToDesktop();
    createAndInitContent();
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
    m_pTitleWidet->installEventFilter(this);
    m_pCentralWidget = new QWidget(this);

    QVBoxLayout* pFrameLayout = new QVBoxLayout(this);
    pFrameLayout->setMargin(0);
    pFrameLayout->setSpacing(0);
    pFrameLayout->setContentsMargins(0, 0, 0, 0);

    pFrameLayout->addWidget(m_pTitleWidet);
    pFrameLayout->addWidget(m_pCentralWidget);
    pFrameLayout->setStretchFactor(m_pTitleWidet, 1);
    pFrameLayout->setStretchFactor(m_pCentralWidget, 19);
    this->setLayout(pFrameLayout);
}

QWidget* GCMPFramelessWidget::createTitleWidget()
{
    QColor oGray = QColor(231, 231, 231, 255);
    QColor oBlue = QColor(78, 166, 234, 255);
    QColor oRed = QColor(255, 0, 0, 200);
    QColor oDarkRed = QColor(230, 0, 0, 200);

    QWidget* pTitleWidget = new QWidget(this);
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
    QSpacerItem* pSpaceItem = new QSpacerItem(900, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    pHLayout->addSpacerItem(pSpaceItem);
    pHLayout->setSpacing(0);
    pHLayout->setContentsMargins(0, 0, 0, 0);
    pHLayout->setMargin(4);
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
    if (pEvnt->type() == QEvent::MouseButtonPress && pObj == m_pTitleWidet)
    {
        m_bPressed = true;
        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvnt);
        m_oPressPoint = pMouseEvent->globalPos();
        m_oRefPos = this->pos();
        return true;
    }

    else if (pEvnt->type() == QEvent::MouseButtonRelease && pObj == m_pTitleWidet)
    {
        m_bPressed = false;
        m_oPressPoint = QPoint(0, 0);
        return true;
    }

    if (pEvnt->type() == QEvent::MouseMove && pObj == m_pTitleWidet)
    {
        if (m_bPressed)
        {
            QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvnt);
            QPoint oTarget = pMouseEvent->globalPos();
            this->move(m_oRefPos + (oTarget - m_oPressPoint));
            return true;
        }
    }

    return QWidget::eventFilter(pObj, pEvnt);
}

