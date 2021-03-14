/*
*�ļ�����:����QMovie��,��QMovie����һ����QLabel,������ʾ�ı���Ϣ(����,���Űٷֱȵ�)
*��Ҫ����:
*
*��������:2013/09/27
*��    ��:���ڷ�
*/
#ifndef QMYMOVIE_H
#define QMYMOVIE_H

#include <QMovie>
#include <QLabel>
#include <QTimer>

#define TIMER_INTERVAL          1000*2     //timer���ʱ��
#define LABEL_ALIGNMENT         5          //�ı���ϢLabel�������ϱ߾�(��λ:����)
#define IN
#define OUT
class QMyMovie : public QMovie
{
	Q_OBJECT;
public:
    QMyMovie(QObject *parent = 0);
    ~QMyMovie();
    void setLabelText(IN QString text, IN int msec = 5000); //��ʾ�ı���Ϣ msecΪ��ʾʱ��,��λ����
    void setShowLabel();                                    //��ʾ�ı���ϢLabel
    void setHideLabel();                                    //�����ı���ϢLabel
    void setLabelParent(QWidget* parent);                   //������ʾ��ϢLabel�ĸ�����

private slots:
    void showTextTimeOut();                                 //timer�ۺ���

private:
    QLabel* m_pLabelText;                                   //������ʾ�ı���Ϣ
    QTimer* m_pTimer;                                       //�����Զ�����
};

#endif // QMYMOVIE_H
