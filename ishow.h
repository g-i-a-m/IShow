/*
*文件名称:图片显示窗口类头文件
*简要描述:
*
*创建日期:2013/09/16
*作    者:陈腾飞
*/
#ifndef ISHOW_H
#define ISHOW_H

#include <QtGlobal>
#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QMovie>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QToolTip>
#include <QClipboard>
#include <QProcess>
#include <QPrinter>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QEvent>
#include <QDebug>
#include <QSystemTrayIcon>
#include "ui_ishow.h"
#include "qmymovie.h"
#include "QLayerWidget.h"

#define IN
#define OUT

enum FrameRate
{
    FRAMERATE_25  = 25,
    FRAMERATE_50  = 50,
    FRAMERATE_100 = 100,
    FRAMERATE_200 = 200,
    FRAMERATE_300 = 300,
    FRAMERATE_400 = 400
};

enum ImageType
{
    IMAGE_TYPE_STATIC=0,
    IMAGE_TYPE_GIF
};

class IShow : public QWidget
{
	Q_OBJECT;
public:
    IShow(QWidget *parent = 0);
    ~IShow();
    void OpenImage(IN QString &path);       //打开图片并显示
	void InitMenu(IN int& type);            //初始化菜单项

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void LastPicture();                     //切换到上一张图片
    void NextPicture();                     //切换到下一张图片
    void EditPicture();                     //用画图工具打开
    void DrawPrintImage(IN QPrinter* printer); //打印机打印
    void PrintImage();                      //菜单打印
    void MenuMagnify();                     //菜单放大
    void MenuShrink();                      //菜单缩小
    void MenuSrcSize();                     //菜单原始大小
    void MenuQuickPlay();                   //菜单快放
    void MenuSlowPlay();                    //菜单慢放
    void MenuClockwise();                   //菜单顺时针旋转
    void MenuAnticlockwise();               //菜单逆时针旋转
    void MenuCtrlclockwise1();              //菜单顺时针旋转1度
    void MenuAntictrlclockwise1();          //菜单逆时针旋转1读

	void ShowMediaDetails();				//显示文件媒体信息
    void ShowFilePath();                    //显示当前打开图片路径
    void OpenFilePos();                     //打开当前图片位置
    void CopyFilePath();                    //赋值图片路径
    void CopyImage();                       //复制图像
    void Close();                           //关闭
	void MenuTricksOfUsing();				//使用技巧
	void MenuFeedback();					//反馈意见
	void MenuContactAuthor();				//联系作者
    void MoviePause();                      //Movie暂停
	void CloseFeedbackWidget();             //Timer槽关闭反馈窗口
	void CloseContactAuthorWidget();		//Timer槽关闭联系作者窗口

private:
	QString GetFileFormat(QString &path);	//获取文件格式
	void AdaptiveMagnifySize();				//放大菜单项槽函数
    void Magnify(IN int num,IN int iFrist = 0);//放大缩小
    void Clockwise(IN int num);             //图片旋转
    void MovieQucikPlay(IN int num);        //Movie快放慢放
	void MovieMagnify(IN int num);			//Movie放大缩小
    void MovieSetFrame(IN int num);         //Movie帧跳转

private:
    QPoint windowPos;                       //窗口移动参数
    QPoint mousePos;                        //窗口移动参数
    QPoint dPos;                            //窗口移动参数
    QPoint mouseDown;                       //鼠标按下位置(用于显示图片路径的提示框)
    QMyMovie m_movie;                       //动态图片类
    bool m_bPause;                          //动态图片是否暂停
    QImage m_src_image;                     //静态图片类
    QImage m_modify_image;                  //改变过大小的的图片(修改大小要在原图上做,图片翻转要在此对象上做)
    QString m_ImagePath;                    //图片路径
	QString m_ImageDetails;					//图片详细信息

	QLayerWidget* m_pDetailsWidget;			//详细信息窗口
	QLayerWidget* m_pFeedbackWidget;		//建议反馈窗口
	QLayerWidget* m_pContactAuthorWidget;	//联系作者窗口

    Ui::IShowClass ui;                      //窗口UI
    QMenu* m_pMenu;                         //右键菜单
    QMenu* m_pSubMenuCtrl;                  //控制子菜单
	QMenu* m_pSubMenuMore;                  //更多信息子菜单
    QAction *m_pMediaDetails,*m_pShowPath,*m_pOpenFilePos,*m_pCopyPath,*m_pCopyImage,*m_pPrintImage,*m_pClose,*m_pMore;//菜单项
    QAction *m_pLastPic,*m_pNextPic,*m_pCtrlMagnify,*m_pCtrlShrink,*m_pCtrlSrcSize,*m_pEditPic,\
        *m_pCtrlclockwise,*m_pCtrlAnticlockwise,*m_pCtrlPause,*m_pCtrlLastFrame,*m_pCtrlNextFrame,\
        *m_pCtrlclockwise_1,*m_pCtrlAnticlockwise_1;//控制子菜单项

	QAction *m_pMenuMoreTricks, *m_pMenuMoreFeedback, *m_pMenuMoreContactAuthor;//更多信息子菜单项

    int m_curShowType;                      //当前显示图片格式
    int m_lastNumerator;                    //上次图片放大比率
    int m_curAngle;                         //当前的旋转角度

    int m_frameRate[6];                     //动态图片支持 1/4X~4X 的速率变化
    QString m_frameRateInfo[6];             //1/4X~4X 的字符
    int m_curMovieSpeed;                    //QMovie当前播放速度
    QSize m_movieSize;                      //QMovie原始尺寸
};

#endif // ISHOW_H
