#include "GDBCompare/Ui/GCMPButton.h"


void GCMPButton::loadQSS()
{
    QString strBKColor = QString("rgba(%1, %2, %3, %4)").arg(m_bkColor.red()).arg(m_bkColor.green()).arg(m_bkColor.blue()).arg(m_bkColor.alpha());
    QString strTextColor = QString("rgba(%1, %2, %3, %4)").arg(m_textColor.red()).arg(m_textColor.green()).arg(m_textColor.blue()).arg(m_textColor.alpha());
    QString strBorderColor = QString("rgba(%1, %2, %3, %4)").arg(m_borderColor.red()).arg(m_borderColor.green()).arg(m_borderColor.blue()).arg(m_borderColor.alpha());
    QString strHoverBKColor = QString("rgba(%1, %2, %3, %4)").arg(m_hoverColor.red()).arg(m_hoverColor.green()).arg(m_hoverColor.blue()).arg(m_hoverColor.alpha());
    QString strPressedColor = QString("rgba(%1, %2, %3, %4)").arg(m_pressedColor.red()).arg(m_pressedColor.green()).arg(m_pressedColor.blue()).arg(m_pressedColor.alpha());
    QString strBorder = m_bEnableBorder ? "border : none" : "border : none";

    QString strBtnQss = QString("QPushButton{background-color: %1 ; color : %2; %3; min-height : %4px; min-width : %5px}"
        "QPushButton:hover {background-color:%6; color: %7; %8; min-height : %9px; min-width : %10px}"
        "QPushButton:pressed{background-color:%11; color : %12; %13; min-height : %14px; min-width : %15px}")
        .arg(strBKColor)
        .arg(strTextColor)
        .arg(strBorder)
        .arg(m_oMinSize.height())
        .arg(m_oMinSize.width())
        .arg(strHoverBKColor)
        .arg(strTextColor)
        .arg(strBorder)
        .arg(m_oMinSize.height())
        .arg(m_oMinSize.width())
        .arg(strPressedColor)
        .arg(strTextColor)
        .arg(strBorder)
        .arg(m_oMinSize.height())
        .arg(m_oMinSize.width());

    this->setStyleSheet(strBtnQss);
}

GCMPButton::GCMPButton(QString strTitle, QColor hoverColor, QColor pressedColor, QSize minSize, QWidget* pParent, bool bEnableBorder)
    :m_strTitle(strTitle), m_hoverColor(hoverColor), QPushButton(strTitle, pParent), m_pressedColor(pressedColor),
    m_oMinSize(minSize), m_bkColor(Qt::white), m_borderColor(Qt::white), m_textColor(Qt::black), m_bEnableBorder(bEnableBorder)
{
    loadQSS();
}

GCMPButton::GCMPButton(QIcon& oIcon, QColor hoverColor, QColor pressedColor, QSize minSize, QWidget* pParent, bool bEnableBorder)
    : m_oImage(oIcon), m_hoverColor(hoverColor), QPushButton(pParent), m_pressedColor(pressedColor), m_oMinSize(minSize)
    , m_bkColor(Qt::white), m_borderColor(Qt::white), m_textColor(Qt::black), m_bEnableBorder(bEnableBorder)
{
    this->setIcon(m_oImage);
    loadQSSForIcon();
}

void GCMPButton::loadQSSForIcon()
{
    loadQSS();
}
