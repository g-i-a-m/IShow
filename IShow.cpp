#include "ishow.h"
#include <QDir>
#include <QtGlobal>
#include <Windows.h>
#include <string>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPainter>
#include <QDesktopWidget>
#include "./MediaInfo/MediaInfoDLL.h"
using namespace std;
using namespace MediaInfoDLL;

IShow::IShow(QWidget *parent)
    : QWidget(parent),m_curMovieSpeed(2),m_curAngle(0),m_bPause(true),m_pCtrlPause(NULL),
	m_pDetailsWidget(NULL), m_pFeedbackWidget(NULL), m_pContactAuthorWidget(NULL)
{
    ui.setupUi(this);
    m_movie.setLabelParent(ui.m_labelImageShow);

    setAutoFillBackground(false);//关闭自动填充背景
    setWindowFlags(Qt::FramelessWindowHint);//设置无边框
    setAttribute(Qt::WA_TranslucentBackground, true);//设置透明背景
    m_frameRate[0]=FRAMERATE_25;
    m_frameRate[1]=FRAMERATE_50;
    m_frameRate[2]=FRAMERATE_100;
    m_frameRate[3]=FRAMERATE_200;
    m_frameRate[4]=FRAMERATE_300;
    m_frameRate[5]=FRAMERATE_400;
    m_frameRateInfo[0]="1/4X";
    m_frameRateInfo[1]="1/2X";
    m_frameRateInfo[2]="1X";
    m_frameRateInfo[3]="2X";
    m_frameRateInfo[4]="3X";
    m_frameRateInfo[5]="4X";
}

IShow::~IShow()
{
    
}

void IShow::OpenImage(IN QString &path)
{
    m_lastNumerator = 10;
	qDebug() << path;
	QString strFormat = GetFileFormat(path);
	if (strFormat.isEmpty())
	{
		QFileInfo info(path);
		strFormat = info.suffix();
	}
    if ( QString::localeAwareCompare(strFormat,QString("jpg"))==0 || QString::localeAwareCompare(strFormat,QString("JPG"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("png"))==0 || QString::localeAwareCompare(strFormat,QString("PNG"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("jpeg"))==0 || QString::localeAwareCompare(strFormat,QString("JPEG"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("Bitmap"))==0 || QString::localeAwareCompare(strFormat,QString("BMP"))==0 || \
		 QString::localeAwareCompare(strFormat, QString("bmp")) == 0 || \
		 QString::localeAwareCompare(strFormat,QString("ico"))==0 || QString::localeAwareCompare(strFormat,QString("ICO"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("pbm"))==0 || QString::localeAwareCompare(strFormat,QString("PBM"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("pgm"))==0 || QString::localeAwareCompare(strFormat,QString("PGM"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("ppm"))==0 || QString::localeAwareCompare(strFormat,QString("PPM"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("xbm"))==0 || QString::localeAwareCompare(strFormat,QString("XBM"))==0 || \
		 QString::localeAwareCompare(strFormat,QString("xpm"))==0 || QString::localeAwareCompare(strFormat,QString("XPM"))==0 || \
		 QString::localeAwareCompare(strFormat, QString("webp")) == 0 || QString::localeAwareCompare(strFormat, QString("WEBP")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("WebP")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("tga")) == 0 || QString::localeAwareCompare(strFormat, QString("TGA")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("icns")) == 0 || QString::localeAwareCompare(strFormat, QString("ICNS")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("jp2")) == 0 || QString::localeAwareCompare(strFormat, QString("JP2")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("mng")) == 0 || QString::localeAwareCompare(strFormat, QString("MNG")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("tiff")) == 0 || QString::localeAwareCompare(strFormat, QString("TIFF")) == 0 ||\
		 QString::localeAwareCompare(strFormat, QString("tif")) == 0 ||	QString::localeAwareCompare(strFormat, QString("TIF")) == 0 || \
		 QString::localeAwareCompare(strFormat, QString("wbmp")) == 0 || QString::localeAwareCompare(strFormat, QString("WBMP")) == 0)
	{
		m_src_image.load(path, strFormat.toStdString().c_str());
		if (!m_src_image.isNull())
		{
			QSize size = m_src_image.size();
			ui.m_labelImageShow->resize(size);
			this->setFixedSize(size);
			ui.m_labelImageShow->setPixmap(QPixmap::fromImage(m_src_image));
			m_curShowType = IMAGE_TYPE_STATIC;
			m_curAngle = 0;
			qDebug() << "IShow open static picture\n";
		}
	}
    else if ( QString::localeAwareCompare(strFormat,QString("gif"))==0 || QString::localeAwareCompare(strFormat,QString("GIF"))==0 )
    {
        qDebug() << "IShow open gif picture\n";
        m_movie.setFileName(path);
        ui.m_labelImageShow->setMovie(&m_movie);
        m_movie.start();
        QRect rect = m_movie.frameRect();
        m_movieSize.setWidth(rect.right());
        m_movieSize.setHeight(rect.bottom());
        ui.m_labelImageShow->resize(m_movieSize);
        this->setFixedSize(m_movieSize);
        m_curShowType = IMAGE_TYPE_GIF;
        m_curMovieSpeed = 2;//速率数组下标
    }
    else
    {
        // do nothing
    }
    InitMenu(m_curShowType);
	AdaptiveMagnifySize();
    m_ImagePath = path;//保存当前打开的图片路径
}

void IShow::InitMenu(IN int& type)
{
    m_pSubMenuCtrl = new QMenu();
    m_pSubMenuCtrl->setTitle(tr("Control"));
    m_pSubMenuCtrl->setIcon(QIcon(QString(":/IShow/res/control.ico")));
    m_pCtrlMagnify = new QAction(tr("Magnify"),this);
    m_pCtrlShrink = new QAction(tr("Shrink"),this);
    m_pCtrlSrcSize = new QAction(tr("Source size"),this);
    m_pSubMenuCtrl->addAction(m_pCtrlMagnify);
    connect(m_pCtrlMagnify,SIGNAL(triggered()),this,SLOT(MenuMagnify()));
    m_pSubMenuCtrl->addAction(m_pCtrlShrink);
    connect(m_pCtrlShrink,SIGNAL(triggered()),this,SLOT(MenuShrink()));
    m_pSubMenuCtrl->addAction(m_pCtrlSrcSize);
    connect(m_pCtrlSrcSize,SIGNAL(triggered()),this,SLOT(MenuSrcSize()));
    if (type==IMAGE_TYPE_GIF)
    {
        m_pCtrlclockwise = new QAction(tr("Quick Play"),this);
        m_pCtrlAnticlockwise = new QAction(tr("Slow Play"),this);
        m_pCtrlPause = new QAction(tr("Pause/Restore"),this);
        m_pCtrlNextFrame = new QAction(tr("Next Frame"),this);
        m_pSubMenuCtrl->addAction(m_pCtrlclockwise);
        m_pSubMenuCtrl->insertSeparator(m_pCtrlclockwise);
        connect(m_pCtrlclockwise,SIGNAL(triggered()),this,SLOT(MenuQuickPlay()));
        m_pSubMenuCtrl->addAction(m_pCtrlAnticlockwise);
        connect(m_pCtrlAnticlockwise,SIGNAL(triggered()),this,SLOT(MenuSlowPlay()));
        m_pSubMenuCtrl->addAction(m_pCtrlPause);
        m_pSubMenuCtrl->insertSeparator(m_pCtrlPause);
        connect(m_pCtrlPause,SIGNAL(triggered()),this,SLOT(MoviePause()));
        m_pSubMenuCtrl->addAction(m_pCtrlNextFrame);
        connect(m_pCtrlNextFrame,SIGNAL(triggered()),&m_movie,SLOT(jumpToNextFrame()));
    }
    else
    {
        m_pCtrlclockwise = new QAction(tr("Clockwise 90"),this);
        m_pCtrlAnticlockwise = new QAction(tr("Antic clockwise 90"),this);
        m_pCtrlclockwise_1 = new QAction(tr("Clockwise 1"),this);
        m_pCtrlAnticlockwise_1 = new QAction(tr("Antic clockwise 1"),this);
        m_pSubMenuCtrl->addAction(m_pCtrlclockwise);
        m_pSubMenuCtrl->insertSeparator(m_pCtrlclockwise);
        connect(m_pCtrlclockwise,SIGNAL(triggered()),this,SLOT(MenuClockwise()));

        m_pSubMenuCtrl->addAction(m_pCtrlAnticlockwise);
        connect(m_pCtrlAnticlockwise,SIGNAL(triggered()),this,SLOT(MenuAnticlockwise()));

	    m_pSubMenuCtrl->addAction(m_pCtrlclockwise_1);
        connect(m_pCtrlclockwise_1,SIGNAL(triggered()),this,SLOT(MenuCtrlclockwise1()));

	    m_pSubMenuCtrl->addAction(m_pCtrlAnticlockwise_1);
        connect(m_pCtrlAnticlockwise_1,SIGNAL(triggered()),this,SLOT(MenuAntictrlclockwise1()));
    }

	m_pSubMenuMore = new QMenu();
	m_pSubMenuMore->setTitle(tr("More..."));
	m_pSubMenuMore->setIcon(QIcon(QString(":/IShow/res/more.ico")));
	m_pMenuMoreTricks = new QAction(tr("Tricks"), this);
	m_pMenuMoreFeedback = new QAction(tr("Feed back"), this);
	m_pMenuMoreContactAuthor = new QAction(tr("Contact Author"), this);
	m_pSubMenuMore->addAction(m_pMenuMoreTricks);
	connect(m_pMenuMoreTricks, SIGNAL(triggered()), this, SLOT(MenuTricksOfUsing()));
	m_pSubMenuMore->addAction(m_pMenuMoreFeedback);
	connect(m_pMenuMoreFeedback, SIGNAL(triggered()), this, SLOT(MenuFeedback()));
	m_pSubMenuMore->addAction(m_pMenuMoreContactAuthor);
	connect(m_pMenuMoreContactAuthor, SIGNAL(triggered()), this, SLOT(MenuContactAuthor()));

    m_pMenu = new QMenu();
    m_pEditPic     = new QAction(tr("Edit"),this);
    m_pEditPic->setIcon(QIcon(QString(":/IShow/res/edit.ico")));
    m_pLastPic     = new QAction(tr("Last Picture"),this);
    m_pLastPic->setIcon(QIcon(QString(":/IShow/res/last.ico")));
    m_pNextPic     = new QAction(tr("Next Picture"),this);
    m_pNextPic->setIcon(QIcon(QString(":/IShow/res/next.ico")));
	m_pMediaDetails= new QAction(tr("Details"), this);
    m_pShowPath    = new QAction(tr("Show file path"),this);
    m_pOpenFilePos = new QAction(tr("Open file path"),this);
    m_pCopyPath    = new QAction(tr("Copy path"),this);
    m_pCopyPath->setIcon(QIcon(QString(":/IShow/res/copy.ico")));
    m_pCopyImage   = new QAction(tr("Copy image"),this);
    m_pCopyImage->setIcon(QIcon(QString(":/IShow/res/copyimage.ico")));
    m_pPrintImage  = new QAction(tr("Print"),this);
    m_pPrintImage->setIcon(QIcon(QString(":/IShow/res/print.ico")));
    m_pClose       = new QAction(tr("Close"),this);
    m_pClose->setIcon(QIcon(QString(":/IShow/res/close.ico")));
    m_pMenu->addMenu(m_pSubMenuCtrl);
    if (type!=IMAGE_TYPE_GIF)
    {
        m_pMenu->insertSeparator(m_pEditPic);
        m_pMenu->addAction(m_pEditPic);
        connect(m_pEditPic,SIGNAL(triggered()),this,SLOT(EditPicture()));
    }
    m_pMenu->insertSeparator(m_pLastPic);
    m_pMenu->addAction(m_pLastPic);
    connect(m_pLastPic,SIGNAL(triggered()),this,SLOT(LastPicture()));
    m_pMenu->addAction(m_pNextPic);
    connect(m_pNextPic,SIGNAL(triggered()),this,SLOT(NextPicture()));
    m_pMenu->insertSeparator(m_pMediaDetails);
	m_pMenu->addAction(m_pMediaDetails);
	connect(m_pMediaDetails, SIGNAL(triggered()), this, SLOT(ShowMediaDetails()));
	m_pMenu->addAction(m_pShowPath);
    connect(m_pShowPath,SIGNAL(triggered()),this,SLOT(ShowFilePath()));
    m_pMenu->addAction(m_pOpenFilePos);
    connect(m_pOpenFilePos,SIGNAL(triggered()),this,SLOT(OpenFilePos()));
    m_pMenu->addAction(m_pCopyPath);
    m_pMenu->insertSeparator(m_pCopyPath);
    connect(m_pCopyPath,SIGNAL(triggered()),this,SLOT(CopyFilePath()));
    m_pMenu->addAction(m_pCopyImage);
    connect(m_pCopyImage,SIGNAL(triggered()),this,SLOT(CopyImage()));
    if (type!=IMAGE_TYPE_GIF)
    {
        m_pMenu->insertSeparator(m_pPrintImage);
        m_pMenu->addAction(m_pPrintImage);
        connect(m_pPrintImage,SIGNAL(triggered()),this,SLOT(PrintImage()));
    }
    m_pMenu->addAction(m_pClose);
    connect(m_pClose,SIGNAL(triggered()),this,SLOT(Close()));
	m_pMenu->insertSeparator(m_pClose);
	m_pMenu->addMenu(m_pSubMenuMore);
	m_pMenu->insertSeparator(m_pClose);
}

void IShow::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)//鼠标左键按下 : 实现拖动位置
    {
        this->windowPos = this->pos();
        this->mousePos = event->globalPos();
        this->dPos = mousePos - windowPos;
    }
    else if (event->button()==Qt::RightButton)//鼠标右键按下
    {
        dPos.setX(-1);
        dPos.setY(-1);
    }
    else if (event->button()==Qt::MidButton)//鼠标滚轮按下
    {
        //鼠标滚轮按下
    }
}

void IShow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::RightButton)//鼠标右键 : 弹出功能菜单
    {
        mouseDown = event->globalPos();
        m_pMenu->popup(event->globalPos());
    }
}

void IShow::mouseMoveEvent(QMouseEvent *event)
{
    if (dPos.x()!=-1)
    {
        this->move(event->globalPos() - this->dPos);
    }
}

void IShow::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button()==Qt::LeftButton)//鼠标左键双击 : 实现关闭
    {
        this->close();
    }
}

void IShow::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    if (event->orientation() == Qt::Vertical)//鼠标滚轮滚动
    {
        if (m_curShowType==IMAGE_TYPE_GIF)//GIF
        {
            //改变播放速率
            if ( (QApplication::keyboardModifiers()==Qt::ControlModifier) || (QApplication::keyboardModifiers()==Qt::ShiftModifier) )
            {
                if (numSteps>0)
                {
                    MovieQucikPlay(1);
                }
                else
                {
                    MovieQucikPlay(-1);
                }
            }
            else//改变大小
            {
                if (numSteps>0)
                {
                    MovieMagnify(1);
                }
                else
                {
                    MovieMagnify(-1);
                }
            }
        }
        else//静态图片
        {
            //改变静态图片角度
            if ((QApplication::keyboardModifiers()==Qt::ControlModifier) || (QApplication::keyboardModifiers()==Qt::ShiftModifier) )
            {
                if (numSteps>0)
                {
                    Clockwise(1);
                }
                else
                {
                    Clockwise(-1);
                }
                
            }
            else//改变大小
            {
                if (numSteps>0)
                {
                    Magnify(1);
                }
                else
                {
                    Magnify(-1);
                }
            }
        }
    }
    else
    {
        //do nothing
    }
}

void IShow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers()==Qt::NoModifier)
    {
        if (event->key()==Qt::Key_Up)//上
        {
            qDebug() << "No modifier key is pressed (up)!\n";
            LastPicture();
        }
        else if (event->key()==Qt::Key_Down)//下
        {
            qDebug() << "No modifier key is pressed (down)!\n";
            NextPicture();
        }
        else if (event->key()==Qt::Key_Space && m_curShowType==IMAGE_TYPE_GIF)
        {
            MoviePause();
        }
        else if (event->key()==Qt::Key_Left && m_curShowType== IMAGE_TYPE_STATIC)//左
        {
            qDebug() << "No modifier key is pressed (left)!\n";
            if (!m_src_image.isNull())
            {
                Clockwise(-90);
            }
        }
        else if (event->key()==Qt::Key_Right && m_curShowType== IMAGE_TYPE_STATIC)//右
        {
            qDebug() << "No modifier key is pressed (right)!\n";
            if (!m_src_image.isNull())
            {
                Clockwise(90);
            }
        }
        else if (event->key()==Qt::Key_Escape)
        {
            this->close();
        }
        else
        {
            //do nothing
        }
    }
    else if (event->modifiers()==Qt::ShiftModifier)//A Shift key on the keyboard is pressed.
    {
        qDebug() << "Shift key is pressed!\n";
    }
    else if (event->modifiers()==Qt::ControlModifier)//A Ctrl key on the keyboard is pressed.
    {
        qDebug() << "Ctrl key is pressed!\n";
    }
    else if (event->modifiers()==Qt::AltModifier)//An Alt key on the keyboard is pressed.
    {
        qDebug() << "Alt key is pressed!\n";
    }
    else
    {
        //do nothing
    }
}

void IShow::MenuMagnify()
{
    if (m_curShowType==IMAGE_TYPE_GIF)
    {
        MovieMagnify(1);
    }
    else
    {
        Magnify(1);
    }
}

void IShow::MenuShrink()
{
    if (m_curShowType==IMAGE_TYPE_GIF)
    {
        MovieMagnify(-1);
    }
    else
    {
        Magnify(-1);
    }
}

void IShow::MenuSrcSize()
{
    if (m_curShowType==IMAGE_TYPE_GIF)
    {
        m_movie.stop();
        m_movie.setScaledSize(m_movieSize);
        this->setFixedSize(m_movieSize);
        m_movie.start();
    }
    else
    {
        m_modify_image = m_src_image;
        QMatrix matrix;
        matrix.rotate(m_curAngle);
        m_modify_image = m_modify_image.transformed(matrix,Qt::FastTransformation);//旋转角度
        this->setFixedSize(m_modify_image.size());
        ui.m_labelImageShow->clear();
        ui.m_labelImageShow->setPixmap(QPixmap::fromImage(m_modify_image));
    }
    m_lastNumerator = 10;//重置
}

void IShow::MenuQuickPlay()
{
    if (m_curShowType==IMAGE_TYPE_GIF)
    {
        MovieQucikPlay(1);
    }
}

void IShow::MenuSlowPlay()
{
    if (m_curShowType==IMAGE_TYPE_GIF)
    {
        MovieQucikPlay(-1);
    }
}

void IShow::MenuClockwise()
{
    if (m_curShowType!=IMAGE_TYPE_GIF)
    {
        Clockwise(90);
    }
}

void IShow::MenuAnticlockwise()
{
    if (m_curShowType!=IMAGE_TYPE_GIF)
    {
        Clockwise(-90);
    }
}

void IShow::MenuCtrlclockwise1()
{
    if (m_curShowType!=IMAGE_TYPE_GIF)
    {
        Clockwise(1);
    }
}

void IShow::MenuAntictrlclockwise1()
{
    if (m_curShowType!=IMAGE_TYPE_GIF)
    {
        Clockwise(-1);
    }
}

QString IShow::GetFileFormat(QString &path)
{
	//检测文件格式
	MediaInfo MI;
	MI.Open(path.toStdWString());

	MI.Option(__T("Complete"));
	m_ImageDetails = QString::fromWCharArray(MI.Inform().c_str());

	MI.Option(__T("Inform"), __T("General;%Format%"));
	QString strFormat = QString::fromWCharArray(MI.Inform().c_str());
	MI.Close();
	return strFormat;
}

void IShow::ShowMediaDetails()
{
	if (m_pDetailsWidget == NULL)
	{
		m_pDetailsWidget = new QLayerWidget(NULL, Details, m_ImageDetails);
	}
	m_pDetailsWidget->show();
}

void IShow::ShowFilePath()//显示文件路径槽函数
{
    ui.m_labelImageShow->setLabelText(m_ImagePath);
}

void IShow::OpenFilePos()//打开文件位置槽函数
{
    wchar_t path[256]={0};
    QString canPath = QFileInfo(m_ImagePath).canonicalPath().replace("/","\\",Qt::CaseInsensitive);
    wcsncat(path,L"explorer ",wcslen(L"explorer "));
    wcsncat(path,canPath.toStdWString().c_str(),canPath.toStdWString().size());
    QProcess::startDetached(QString::fromWCharArray(path));
}

void IShow::CopyFilePath()//复制图片路径槽函数
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(m_ImagePath);
}

void IShow::CopyImage()//复制图片槽函数
{
    if (m_curShowType!=IMAGE_TYPE_GIF)
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setImage(m_src_image);
    }
}

void IShow::Close()//关闭菜单项槽函数
{
    this->close();
}

void IShow::MenuTricksOfUsing()
{
	QString strPath = QCoreApplication::applicationDirPath() + QString::fromWCharArray(L"/ReadMe.txt");
	ShellExecute(NULL, L"open", L"C:/Windows/System32/notepad.exe", strPath.toStdWString().c_str(), NULL, SW_SHOW);
}

void IShow::MenuFeedback()
{
	if (m_pFeedbackWidget == NULL)
	{
		QDesktopWidget *desktop = QApplication::desktop();
		QWidget *parent = desktop->screen(1);
		m_pFeedbackWidget = new QLayerWidget(parent, Feedback);
	}

	m_pFeedbackWidget->show();
	QTimer::singleShot(3500, this, SLOT(CloseFeedbackWidget()));//一次性Timer
}

void IShow::MenuContactAuthor()
{
	if (m_pContactAuthorWidget == NULL)
	{
		QDesktopWidget *desktop = QApplication::desktop();
		QWidget *parent = desktop->screen(1);
		m_pContactAuthorWidget = new QLayerWidget(parent);
	}

	m_pContactAuthorWidget->show();
}

void IShow::AdaptiveMagnifySize()//刚打开自动调整大小
{
	int iMagnify = 0;
	QDesktopWidget* pDesktop = QApplication::desktop();
	if (pDesktop != NULL)
	{
		QSize desktopSize = pDesktop->availableGeometry().size();
		QSize imageSize = m_src_image.size();
		if (imageSize.width() > desktopSize.width() || imageSize.height() > desktopSize.height())
		{
			float iWidthPercent = (float)desktopSize.width() / (float)imageSize.width();
			float iheightPercent = (float)desktopSize.height() / (float)imageSize.height();
			if (iWidthPercent < iheightPercent)
			{
				iMagnify = (1 - iWidthPercent) * -10 -1;
			}
			else
			{
				iMagnify = (1 - iheightPercent) * -10 -1;
			}
			Magnify(iMagnify,1);
		}
	}
}

void IShow::Magnify(IN int num, IN int iFrist)//放大菜单项槽函数
{
	float lastRate = (float)m_lastNumerator / 10.0;
	m_lastNumerator += num;
	float rate = (float)m_lastNumerator / 10.0;

	//每次放大缩小均以原图为准,防止多次放大缩小及旋转带来失真
	QSize imgSize = m_src_image.size();
	QSize shrinkSize(qRound((float)imgSize.width()*rate), qRound((float)imgSize.height()*rate));
	m_modify_image = m_src_image.scaled(shrinkSize);//缩放

	//获取当前窗口大小及位置和当前鼠标位置
	/*note:之所以找当前鼠标在图片中的坐标相当于在原图中的坐标,原因:
	图片的每次缩放都是在原图的基础上重新计算的(1.1 1.2 1.3倍来放大的),故获不能简单的乘以1.1来计算新位置坐标(1*1.2和1*1.1*1.1是不相等的)*/
	int newX(0), newY(0);
	QPoint desktopMousePos = cursor().pos();
	QPoint DialogMousePoint = this->mapFromGlobal(desktopMousePos);//鼠标坐标,转换成窗口坐标系坐标
	QPoint srcDialogMousePoint(DialogMousePoint.x() / lastRate, DialogMousePoint.y() / lastRate);//相当于在原始未缩放时鼠标在图片中的坐标
	if (iFrist == 0)
	{
		//计算以鼠标为中心放大图片后图片的位置
		newX = desktopMousePos.x() - ((float)srcDialogMousePoint.x()*rate);
		newY = desktopMousePos.y() - ((float)srcDialogMousePoint.y()*rate);
	}
	else
	{
		//首次打开图片自动调整大小时居中显示
		QDesktopWidget* pDesktop = QApplication::desktop();
		if (pDesktop != NULL)
		{
			QSize desktopSize = pDesktop->availableGeometry().size();
			newX = (desktopSize.width() - shrinkSize.width()) / 2;
			newY = (desktopSize.height() - shrinkSize.height()) / 2;
		}
	}

    //保持旋转角度
	QMatrix matrix;
	matrix.rotate(m_curAngle);
	m_modify_image = m_modify_image.transformed(matrix, Qt::FastTransformation);//旋转角度
	this->setFixedSize(m_modify_image.size());
	QRect dstRect = this->rect();
	ui.m_labelImageShow->clear();
	ui.m_labelImageShow->setPixmap(QPixmap::fromImage(m_modify_image));
	ui.m_labelImageShow->setLabelText(tr("Magnification:") + QString::number(m_lastNumerator) + "0%");
    
	//设置窗口位置坐标
	this->move(newX, newY);
}

void IShow::Clockwise(IN int num)//旋转槽函数  负数为顺时针  正数为逆时针
{
    //每次旋转均以原图为准,防止多次放大缩小及带来失真
    QSize imgSize = m_src_image.size();
    QSize shrinkSize(imgSize.width()*m_lastNumerator/10,imgSize.height()*m_lastNumerator/10);
    m_modify_image = m_src_image.scaled(shrinkSize);//缩放

    QMatrix matrix;
    m_curAngle+=num;
    if(m_curAngle>=360)
        m_curAngle-=360;
    if(m_curAngle<=-360)
        m_curAngle+=360;
    matrix.rotate(m_curAngle);
    m_modify_image = m_modify_image.transformed(matrix,Qt::FastTransformation);//旋转角度
    this->setFixedSize(m_modify_image.size());
    ui.m_labelImageShow->clear();
    ui.m_labelImageShow->setPixmap(QPixmap::fromImage(m_modify_image));
    ui.m_labelImageShow->setLabelText((m_curAngle>0?tr("Clockwise:"):tr("Antic clockwise:"))+ QString::number(qAbs(m_curAngle))+tr("angle"));
}

void IShow::MovieQucikPlay(IN int num)
{
    m_curMovieSpeed+=num;
    if (m_curMovieSpeed>5)
    {
        m_curMovieSpeed=5;
        return;
    }
    if (m_curMovieSpeed<0)
    {
        m_curMovieSpeed=0;
        return;
    }
    m_movie.setSpeed(m_frameRate[m_curMovieSpeed]);
    m_movie.setLabelText(m_frameRateInfo[m_curMovieSpeed]);
}

void IShow::MovieMagnify(IN int num)
{
	float lastRate = (float)m_lastNumerator / 10.0;
	m_lastNumerator += num;
	float rate = (float)m_lastNumerator / 10.0;

	//每次放大缩小均以原图为准,防止多次放大缩小及旋转带来失真
	QSize shrinkSize(qRound((float)m_movieSize.width()*rate), qRound((float)m_movieSize.height()*rate));

	//获取当前窗口大小及位置和当前鼠标位置
	/*note:之所以找当前鼠标在图片中的坐标相当于在原图中的坐标,原因:
	图片的每次缩放都是在原图的基础上重新计算的(1.1 1.2 1.3倍来放大的),故获不能简单的乘以1.1来计算新位置坐标(1*1.2和1*1.1*1.1是不相等的)*/
	QPoint desktopMousePos = cursor().pos();
	QPoint DialogMousePoint = this->mapFromGlobal(desktopMousePos);//鼠标坐标,转换成窗口坐标系坐标
	QPoint srcDialogMousePoint(DialogMousePoint.x() / lastRate, DialogMousePoint.y() / lastRate);//相当于在原始未缩放时鼠标在图片中的坐标

	//计算以鼠标为中心放大图片后图片的位置
	int newX = desktopMousePos.x() - ((float)srcDialogMousePoint.x()*rate);
	int newY = desktopMousePos.y() - ((float)srcDialogMousePoint.y()*rate);

    m_movie.stop();
    m_movie.setScaledSize(shrinkSize);
    m_movie.setLabelText(QString::number(m_lastNumerator)+"0%");
    this->setFixedSize(shrinkSize);
	this->move(newX, newY);
    m_movie.start();
}

void IShow::MovieSetFrame(IN int num)
{
    int iCurFrameNum = m_movie.currentFrameNumber();
    iCurFrameNum+=num;
    m_movie.jumpToFrame(iCurFrameNum);
}

void IShow::MoviePause()
{
    m_movie.setPaused(m_bPause);
    m_bPause = !m_bPause;
}

void IShow ::CloseFeedbackWidget()
{
	if (m_pFeedbackWidget != NULL)
		m_pFeedbackWidget->hide();
	this->close();
}

void IShow::CloseContactAuthorWidget()
{
	if (m_pContactAuthorWidget != NULL)
		m_pContactAuthorWidget->hide();
}

void IShow::LastPicture()
{
    QDir dir = QFileInfo(m_ImagePath).dir();
    if (dir.exists())
    {
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();
        if (list.count()>0)
        {
            QString strLastFilePath("");
            for(int i=0;i<list.count();++i)
            {
                QFileInfo file_info = list.at(i);
                QString suffix = file_info.suffix();
                if(QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("jpeg"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("bmp"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("ico"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("pbm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("pgm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("ppm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("xbm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("xpm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("gif"), Qt::CaseInsensitive) == 0)
                {
                    QString strCurrFilePath = file_info.absoluteFilePath();
                    if (QString::compare(m_ImagePath,strCurrFilePath,Qt::CaseInsensitive)==0)
                    {
                        if (!strLastFilePath.isEmpty())
                        {
                            if (IMAGE_TYPE_GIF == m_curShowType)
                            {
                                m_movie.stop();
                            }
                            OpenImage(strLastFilePath);
                        }
                        break;
                    }
                    else
                    {
                        strLastFilePath = strCurrFilePath;
                    }
                }
            }
        }
    }
}

void IShow::NextPicture()
{
    QDir dir = QFileInfo(m_ImagePath).dir();
    if (dir.exists())
    {
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();
        if (list.count()>0)
        {
            bool bFindCurrImg = false;
            for(int i=0;i<list.count();++i)
            {
                QFileInfo file_info = list.at(i);
                QString suffix = file_info.suffix();
                if(QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("jpeg"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("bmp"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("ico"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("pbm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("pgm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("ppm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("xbm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("xpm"), Qt::CaseInsensitive) == 0 || \
                    QString::compare(suffix, QString("gif"), Qt::CaseInsensitive) == 0)
                {
                    QString strCurrFilePath = file_info.absoluteFilePath();
                    if (bFindCurrImg)
                    {
                        if (!strCurrFilePath.isEmpty())
                        {
                            if (IMAGE_TYPE_GIF == m_curShowType)
                            {
                                m_movie.stop();
                            }
                            OpenImage(strCurrFilePath);
                        }
                        break;
                    }
                    else
                    {
                        if (QString::compare(m_ImagePath,strCurrFilePath,Qt::CaseInsensitive)==0)
                        {
                            bFindCurrImg = true;
                        }
                    }
                }
            }
        }
    }
}

void IShow::EditPicture()
{
    QString imagePath = "\"" + m_ImagePath + "\"";
    wstring p = imagePath.toStdWString();
    const wchar_t *pp = p.c_str();
    ShellExecute(NULL,L"open",L"C:/Windows/System32/mspaint.exe",pp,NULL,SW_SHOW);
}

void IShow::DrawPrintImage(IN QPrinter* printer)
{
    QPainter paint(printer);
    QRect rect = paint.viewport();
    QSize size = m_src_image.size();
    size.scale(rect.size(),Qt::KeepAspectRatio);
    paint.setViewport(0,0,size.width(),size.height());
    paint.setWindow(m_src_image.rect());
    paint.drawImage(0,0,m_src_image);
}

void IShow::PrintImage()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preDialog(&printer,this);
    connect(&preDialog,SIGNAL(paintRequested(QPrinter* )),this,SLOT(DrawPrintImage(QPrinter* )));
    preDialog.exec();
}
