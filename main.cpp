/*
*�ļ�����:ͼƬ�鿴
*��������:�ҵ���ʾ��̫С,windows�Դ���ͼƬ����Լ�������������һЩͼƬ�
		  �����ߺ��ѳ�����������޵���Ļ,�ټ��ϴ󲿷�ͼƬ���߲��������ͼ,
		  �ټ����ڼ��еĵ������Ǻ���û���ҵ�������
*��Ҫ����:�ޱ߿���������Ļ,����ʽ�Ŵ���С,��ת,��ʼ����Ӧ��Ļ�ߴ�,��ͼ��
		  ͼһ�����߸㶨�ȵ�
*��������:2013/09/16
*��    ��:���ڷ�
*/
#include "ishow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QIcon>
#include <QDebug>
#include <QTextCodec>
#include <QFileDialog>
#include <QTranslator>
#include <QLocale>
#include <QMessageBox>

//#define _USE_LIMIT
#ifdef _USE_LIMIT
#include <QTime>
#endif

#define OPEN_MAX_COUNT  50     //����һ�δ򿪵�ͼƬ����

int main(int argc, char *argv[])
{
    QStringList lstStyle = QStyleFactory::keys();
    QStyle* style = QStyleFactory::create(lstStyle.at(3));
    if (style)
    {
        QApplication::setStyle(style);
    }
    
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QString(":/IShow/res/IShow_1.ico")));

#ifdef _USE_LIMIT
	QDateTime limitTime;
	limitTime.setDate(QDate(2020, 8, 20));
	limitTime.setTime(QTime(20, 50));
	qint64 currTime = QDateTime::currentSecsSinceEpoch();
	QString l = limitTime.toString();
	QString c = QDateTime::currentDateTime().toString();
	if (currTime - limitTime.toSecsSinceEpoch() >= 0)
	{
		QMessageBox::critical(NULL, "Error", "Unknow error happened, please try it again...", QMessageBox::Yes, QMessageBox::Yes);
		return 0;
	}
#endif //_USE_LIMIT

    //��ȡ������ϵͳ����
    QTextCodec *codec = QTextCodec::codecForName("system");
    QTextCodec::setCodecForLocale(codec);

    QTranslator qtTranslator;
    QLocale locale/*(QLocale::English)*/;//French  German  Chinese   English
	//QLocale locale(QLocale::English);//French German Chinese English
    switch (locale.language())
    {
    case QLocale::Chinese:
        qtTranslator.load("ishow_zh.qm",":/Language/");
    	break;
    case QLocale::English:
        qtTranslator.load("ishow_en.qm",":/Language/");
        break;
    default:
        qtTranslator.load("ishow_en.qm",":/Language/");
        break;
    }
    app.installTranslator(&qtTranslator);

    if (argc>1)
    {
        for (int i=1; i<argc && i<=OPEN_MAX_COUNT; ++i)//������50��ͼƬ,//argv[0]��Ӧ�ó���·��
        {
            IShow* showWidget = new IShow;
			showWidget->OpenImage(codec->toUnicode(argv[i]).replace("\\","/",Qt::CaseInsensitive));
			showWidget->show();
        }
        return app.exec();
    }
    else
    {
        QStringList list = QFileDialog::getOpenFileNames(NULL,"Select files to open","/",\
			"Images (*.gif *.bmp *.ico *.pgm *.pbm *.ppm *.xbm *.xpm *.png *.webp *.tga \
			*.icns *.jp2 *.mng *.tif *.tiff *.wbmp *.xpm *.jpg *.jpeg)");
        QStringList::Iterator it = list.begin();
        if (it == list.end())
        {
            return 0;
        }

        int iCount = 0;
        while(it != list.end() && iCount<OPEN_MAX_COUNT)
        {
            IShow* showWidget = new IShow;
            showWidget->OpenImage(it->replace("\\","/",Qt::CaseInsensitive));
            showWidget->show();
            ++it;
			++iCount;
        }
        int iRet = app.exec();
        return iRet;
    }
}
