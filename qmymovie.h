/*
*文件名称:派生QMovie类,对QMovie加入一个子QLabel,用于显示文本信息(速率,缩放百分比等)
*简要描述:
*
*创建日期:2013/09/27
*作    者:陈腾飞
*/
#ifndef QMYMOVIE_H
#define QMYMOVIE_H

#include <QMovie>
#include <QLabel>
#include <QTimer>

#define TIMER_INTERVAL          1000*2     //timer间隔时间
#define LABEL_ALIGNMENT         5          //文本信息Label距离左上边距(单位:像素)
#define IN
#define OUT
class QMyMovie : public QMovie
{
	Q_OBJECT;
public:
    QMyMovie(QObject *parent = 0);
    ~QMyMovie();
    void setLabelText(IN QString text, IN int msec = 5000); //显示文本信息 msec为显示时长,单位毫秒
    void setShowLabel();                                    //显示文本信息Label
    void setHideLabel();                                    //隐藏文本信息Label
    void setLabelParent(QWidget* parent);                   //设置显示信息Label的父窗口

private slots:
    void showTextTimeOut();                                 //timer槽函数

private:
    QLabel* m_pLabelText;                                   //用于显示文本信息
    QTimer* m_pTimer;                                       //用于自动隐藏
};

#endif // QMYMOVIE_H
