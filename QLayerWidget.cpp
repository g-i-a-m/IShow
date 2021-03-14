#include "QLayerWidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QClipboard>

QLayerWidget::QLayerWidget(QWidget *parent, LayerWidgetType type, QString details) :
	QWidget(parent),
	m_type(type)
{
	ui.setupUi(this);
	setAutoFillBackground(true);//关闭自动填充背景
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);//设置无边框置顶隐藏任务栏图标
	setWindowOpacity(0.6);

	QDesktopWidget* pDesktop = QApplication::desktop();
	if (pDesktop != NULL)
	{
		QSize desktopSize = pDesktop->availableGeometry().size();
		setGeometry(0, 0, desktopSize.width(), desktopSize.height());
	}

	//获取图片文件后缀名
	/*QString path = "C:\\Users\\Garfiel\\Downloads\\Email_72.png";
	QFileInfo info(path);
	QString Extensions = info.suffix();*/
	if (m_type == ContactAuthor)
	{
		m_image.load(":/picture/res/Email_72.png");
		if (!m_image.isNull())
		{
			QSize imageSize = m_image.size();
			ui.labelImage->resize(imageSize);
			ui.labelImage->setPixmap(QPixmap::fromImage(m_image));
		}

		//邮箱
		QPalette label_pe;
		QFont font;
		font.setPointSize(30);
		label_pe.setColor(QPalette::WindowText, QColor(0, 130, 154));
		ui.labelMail->setWordWrap(true);
		ui.labelMail->setPalette(label_pe);
		ui.labelMail->setFont(font);
		ui.labelMail->setText("garfiel.c@gmail.com");

		ui.tips->setPalette(label_pe);
		ui.tips->setText(tr("Double click to close and copy to clipboard"));
	}
	else if (m_type == Feedback)
	{
		m_movie.setFileName(":/picture/res/bluff.gif");
		ui.labelImage->setMovie(&m_movie);
		QRect rect = m_movie.frameRect();
		ui.labelImage->resize(rect.size());
		m_movie.start();
		ui.labelMail->hide();
		ui.tips->hide();
	}
	else if (m_type == Details)
	{
		ui.labelImage->hide();
		ui.labelMail->hide();

		QPalette label_pe;
		QFont font;
		font.setPointSize(30);
		label_pe.setColor(QPalette::WindowText, QColor(0, 130, 154));

		ui.tips->setPalette(label_pe);
		ui.tips->setText(details);
		ui.labelImage->resize(400,300);
		ui.tips->show();
	}
}


QLayerWidget::~QLayerWidget()
{
	QClipboard *board = QApplication::clipboard();
	board->setText("garfiel.c@gmail.com");
}

void QLayerWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)//鼠标左键双击 : 实现关闭
	{
		QClipboard *board = QApplication::clipboard();
		board->setText("garfiel.c@gmail.com");
		this->close();
	}
}
