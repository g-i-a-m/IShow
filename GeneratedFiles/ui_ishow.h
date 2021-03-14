/********************************************************************************
** Form generated from reading UI file 'ishow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ISHOW_H
#define UI_ISHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "qmylabel.h"

QT_BEGIN_NAMESPACE

class Ui_IShowClass
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QMyLabel *m_labelImageShow;

    void setupUi(QWidget *IShowClass)
    {
        if (IShowClass->objectName().isEmpty())
            IShowClass->setObjectName(QStringLiteral("IShowClass"));
        IShowClass->resize(226, 157);
        gridLayout = new QGridLayout(IShowClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        m_labelImageShow = new QMyLabel(IShowClass);
        m_labelImageShow->setObjectName(QStringLiteral("m_labelImageShow"));

        gridLayout->addWidget(m_labelImageShow, 0, 0, 1, 1);


        retranslateUi(IShowClass);

        QMetaObject::connectSlotsByName(IShowClass);
    } // setupUi

    void retranslateUi(QWidget *IShowClass)
    {
        IShowClass->setWindowTitle(QApplication::translate("IShowClass", "IShow", Q_NULLPTR));
        m_labelImageShow->setText(QApplication::translate("IShowClass", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class IShowClass: public Ui_IShowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISHOW_H
