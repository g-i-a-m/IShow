#include "qmylabel.h"

QMyLabel::QMyLabel(QWidget *parent)
    : QLabel(parent),
    m_pTimer(NULL)
{
    m_pLabelText = new(std::nothrow) QLabel(this);
    if (m_pLabelText)
    {
        m_pLabelText->setWordWrap(true);
        QPalette pe;
        pe.setColor(QPalette::WindowText,QColor(0,130,154));
        m_pLabelText->setPalette(pe);
    }

    m_pTimer = new(std::nothrow) QTimer(this);
    if (m_pTimer)
    {
        m_pTimer->setInterval(TIMER_INTERVAL);
        connect(m_pTimer,SIGNAL(timeout()),this,SLOT(showTextTimeOut()));
    }
}

QMyLabel::~QMyLabel()
{
    if (m_pLabelText)
    {
        delete m_pLabelText;
        m_pLabelText = NULL;
    }

    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

void QMyLabel::setLabelText(IN QString text, IN int msec)
{
    if (m_pLabelText)
    {
        setShowLabel();
        QRect rect = this->rect();
        int x(0),y(0);
        if (rect.width() > LABEL_ALIGNMENT*2)
        {
            x = LABEL_ALIGNMENT;
        }
        if (rect.height() > LABEL_ALIGNMENT*2)
        {
            y = LABEL_ALIGNMENT;
        }
        
        int iMaxWidth = rect.width() - LABEL_ALIGNMENT*2;
        if (iMaxWidth > 0)
        {
            m_pLabelText->setMaximumWidth(iMaxWidth*2);
        }
        m_pLabelText->move(x,y);

        m_pLabelText->setText(text);
        m_pLabelText->adjustSize();

        if (m_pTimer && msec > 1000)
        {
            m_pTimer->start(msec);
        }
    }
}

void QMyLabel::setShowLabel()
{
    if (m_pLabelText)
    {
        m_pLabelText->show();
    }
}

void QMyLabel::setHideLabel()
{
    if (m_pLabelText)
    {
        m_pLabelText->hide();
    }
}

void QMyLabel::showTextTimeOut()
{
    if (m_pTimer)
    {
        m_pTimer->stop();
    }
    setHideLabel();
}
