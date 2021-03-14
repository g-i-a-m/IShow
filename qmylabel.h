/*
*�ļ�����:����QLabel��,����ʾͼƬ��QLabel����һ����QLabel,������ʾ�ı���Ϣ(���Űٷֱ�,��̬ͼƬ��֡�ʵ�)
*��Ҫ����:
*
*��������:2013/09/27
*��    ��:���ڷ�
*/
#ifndef QMYLABEL_H
#define QMYLABEL_H

#include <QLabel>
#include <QTimer>

#define TIMER_INTERVAL          1000*2     //timer���ʱ��
#define LABEL_ALIGNMENT         5          //�ı���ϢLabel�������ϱ߾�(��λ:����)
#define IN
#define OUT
//�Զ���Label�ؼ�
class QMyLabel : public QLabel
{
	Q_OBJECT;
public:
    QMyLabel(QWidget *parent);
    ~QMyLabel();
    void setLabelText(IN QString text,IN int msec = 5000);  //��ʾ�ı���Ϣ msecΪ��ʾʱ��,��λ����
    void setShowLabel();                                    //��ʾ�ı���ϢLabel
    void setHideLabel();                                    //�����ı���ϢLabel

private slots:
    void showTextTimeOut();                                 //timer�ۺ���

private:
    QLabel* m_pLabelText;                                   //������ʾ�ı���Ϣ
    QTimer* m_pTimer;                                       //�����Զ�����
};

#endif // QMYLABEL_H
