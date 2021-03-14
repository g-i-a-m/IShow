/********************************************************************************
** Form generated from reading UI file 'QLayerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLAYERWIDGET_H
#define UI_QLAYERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QLayerWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QFormLayout *formLayout;
    QLabel *labelImage;
    QLabel *labelMail;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_4;
    QLabel *tips;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *QLayerWidget)
    {
        if (QLayerWidget->objectName().isEmpty())
            QLayerWidget->setObjectName(QStringLiteral("QLayerWidget"));
        QLayerWidget->setEnabled(true);
        QLayerWidget->resize(400, 300);
        QLayerWidget->setAutoFillBackground(true);
        gridLayout = new QGridLayout(QLayerWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(125, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        labelImage = new QLabel(QLayerWidget);
        labelImage->setObjectName(QStringLiteral("labelImage"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelImage);

        labelMail = new QLabel(QLayerWidget);
        labelMail->setObjectName(QStringLiteral("labelMail"));

        formLayout->setWidget(0, QFormLayout::FieldRole, labelMail);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(1, QFormLayout::LabelRole, horizontalSpacer_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        tips = new QLabel(QLayerWidget);
        tips->setObjectName(QStringLiteral("tips"));

        horizontalLayout->addWidget(tips);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout);


        gridLayout->addLayout(formLayout, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(125, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 2, 1, 1, 1);


        retranslateUi(QLayerWidget);

        QMetaObject::connectSlotsByName(QLayerWidget);
    } // setupUi

    void retranslateUi(QWidget *QLayerWidget)
    {
        QLayerWidget->setWindowTitle(QApplication::translate("QLayerWidget", "Form", Q_NULLPTR));
        labelImage->setText(QApplication::translate("QLayerWidget", "ico", Q_NULLPTR));
        labelMail->setText(QApplication::translate("QLayerWidget", "garfiel.c@gmail.com", Q_NULLPTR));
        tips->setText(QApplication::translate("QLayerWidget", "tips", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QLayerWidget: public Ui_QLayerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLAYERWIDGET_H
