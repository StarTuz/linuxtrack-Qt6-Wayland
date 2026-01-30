/********************************************************************************
** Form generated from reading UI file 'cap_tweaking.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAP_TWEAKING_H
#define UI_CAP_TWEAKING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CapTweakingForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QSlider *CapHy;
    QLabel *label_6;
    QLabel *label_2;
    QSlider *CapHz;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *CapTweakingForm)
    {
        if (CapTweakingForm->objectName().isEmpty())
            CapTweakingForm->setObjectName("CapTweakingForm");
        CapTweakingForm->resize(407, 167);
        verticalLayout_2 = new QVBoxLayout(CapTweakingForm);
        verticalLayout_2->setObjectName("verticalLayout_2");
        frame = new QFrame(CapTweakingForm);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(frame);
        label->setObjectName("label");
        QFont font;
        font.setBold(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        CapHy = new QSlider(frame);
        CapHy->setObjectName("CapHy");
        CapHy->setMinimum(1);
        CapHy->setMaximum(299);
        CapHy->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(CapHy, 1, 0, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        CapHz = new QSlider(frame);
        CapHz->setObjectName("CapHz");
        CapHz->setMinimum(1);
        CapHz->setMaximum(299);
        CapHz->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(CapHz, 3, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);


        verticalLayout_2->addWidget(frame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(CapTweakingForm);

        QMetaObject::connectSlotsByName(CapTweakingForm);
    } // setupUi

    void retranslateUi(QWidget *CapTweakingForm)
    {
        CapTweakingForm->setWindowTitle(QCoreApplication::translate("CapTweakingForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("CapTweakingForm", "Three point Cap finetuning", nullptr));
        label_6->setText(QCoreApplication::translate("CapTweakingForm", "Minimize left/right monvement while panning left/right", nullptr));
        label_2->setText(QCoreApplication::translate("CapTweakingForm", "Minimize back/forth movement while panning up/down", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CapTweakingForm: public Ui_CapTweakingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAP_TWEAKING_H
