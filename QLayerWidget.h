#ifndef _LAYERWIDGET_H
#define _LAYERWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QImage>
#include "ui_QLayerWidget.h"
#include "qmymovie.h"

enum LayerWidgetType
{
	ContactAuthor = 0,
	Feedback = 1,
	Details = 2
};

class QLayerWidget : public QWidget
{
	Q_OBJECT;
public:
	QLayerWidget(QWidget *parent = 0, LayerWidgetType type = ContactAuthor, QString details = "");
	~QLayerWidget();

protected:
	void mouseDoubleClickEvent(QMouseEvent* event);

private:
	QMyMovie m_movie;                       //¶¯Ì¬Í¼Æ¬Àà
	QImage m_image;							//¾²Ì¬Í¼Æ¬Àà
	QLabel* m_labelImageShow;				//Í¼Æ¬ÈÝÆ÷
	QLabel* m_labelMail;					//Í¼Æ¬ÈÝÆ÷
	Ui::QLayerWidget ui;                    //´°¿ÚUI

	LayerWidgetType m_type;
};



#endif //_LAYERWIDGET_H
