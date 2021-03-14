/*
*文件名称:派生QLabel类,对显示图片的QLabel加入一个子QLabel,用于显示文本信息(缩放百分比,动态图片的帧率等)
*简要描述:
*
*创建日期:2013/09/27
*作    者:陈腾飞
*/
#ifndef QMYLABEL_H
#define QMYLABEL_H

#include <QLabel>
#include <QTimer>

#define TIMER_INTERVAL          1000*2     //timer间隔时间
#define LABEL_ALIGNMENT         5          //文本信息Label距离左上边距(单位:像素)
#define IN
#define OUT
//自定义Label控件
class QMyLabel : public QLabel
{
	Q_OBJECT;
public:
    QMyLabel(QWidget *parent);
    ~QMyLabel();
    void setLabelText(IN QString text,IN int msec = 5000);  //显示文本信息 msec为显示时长,单位毫秒
    void setShowLabel();                                    //显示文本信息Label
    void setHideLabel();                                    //隐藏文本信息Label

private slots:
    void showTextTimeOut();                                 //timer槽函数

private:
    QLabel* m_pLabelText;                                   //用于显示文本信息
    QTimer* m_pTimer;                                       //用于自动隐藏
};

#endif // QMYLABEL_H
