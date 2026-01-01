/********************************************************************************
** Form generated from reading UI file 'profile_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILE_SETUP_H
#define UI_PROFILE_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProfileSetupForm
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *gridFrame;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSlider *YawSens;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSlider *RollSens;
    QLabel *label_11;
    QLabel *label_9;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *PitchSens;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_6;
    QSlider *TySens;
    QLabel *label_13;
    QCheckBox *PitchEnable;
    QCheckBox *YawEnable;
    QCheckBox *RollEnable;
    QCheckBox *TxEnable;
    QCheckBox *TyEnable;
    QCheckBox *TzEnable;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QSlider *TxSens;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QSlider *TzSens;
    QLabel *label_14;
    QLabel *label_18;
    QCheckBox *PitchInvert;
    QCheckBox *YawInvert;
    QCheckBox *RollInvert;
    QCheckBox *TxInvert;
    QCheckBox *TyInvert;
    QCheckBox *TzInvert;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_17;
    QSpacerItem *horizontalSpacer;
    QLabel *label_15;
    QSlider *Smoothing;
    QLabel *label_16;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *DetailedAxisSetup;

    void setupUi(QWidget *ProfileSetupForm)
    {
        if (ProfileSetupForm->objectName().isEmpty())
            ProfileSetupForm->setObjectName("ProfileSetupForm");
        ProfileSetupForm->resize(555, 345);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProfileSetupForm->sizePolicy().hasHeightForWidth());
        ProfileSetupForm->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(ProfileSetupForm);
        verticalLayout->setObjectName("verticalLayout");
        gridFrame = new QFrame(ProfileSetupForm);
        gridFrame->setObjectName("gridFrame");
        gridFrame->setFrameShape(QFrame::StyledPanel);
        gridFrame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(gridFrame);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_3 = new QLabel(gridFrame);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        YawSens = new QSlider(gridFrame);
        YawSens->setObjectName("YawSens");
        YawSens->setOrientation(Qt::Horizontal);
        YawSens->setTickPosition(QSlider::TicksBelow);
        YawSens->setTickInterval(20);

        horizontalLayout_2->addWidget(YawSens);

        label_10 = new QLabel(gridFrame);
        label_10->setObjectName("label_10");

        horizontalLayout_2->addWidget(label_10);


        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_4 = new QLabel(gridFrame);
        label_4->setObjectName("label_4");

        horizontalLayout_3->addWidget(label_4);

        RollSens = new QSlider(gridFrame);
        RollSens->setObjectName("RollSens");
        RollSens->setOrientation(Qt::Horizontal);
        RollSens->setTickPosition(QSlider::TicksBelow);
        RollSens->setTickInterval(20);

        horizontalLayout_3->addWidget(RollSens);

        label_11 = new QLabel(gridFrame);
        label_11->setObjectName("label_11");

        horizontalLayout_3->addWidget(label_11);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        label_9 = new QLabel(gridFrame);
        label_9->setObjectName("label_9");
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 0, 0, 1, 1);

        label_2 = new QLabel(gridFrame);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(gridFrame);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        PitchSens = new QSlider(gridFrame);
        PitchSens->setObjectName("PitchSens");
        PitchSens->setOrientation(Qt::Horizontal);
        PitchSens->setTickPosition(QSlider::TicksBelow);
        PitchSens->setTickInterval(20);

        horizontalLayout->addWidget(PitchSens);

        label_8 = new QLabel(gridFrame);
        label_8->setObjectName("label_8");

        horizontalLayout->addWidget(label_8);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label_6 = new QLabel(gridFrame);
        label_6->setObjectName("label_6");

        horizontalLayout_5->addWidget(label_6);

        TySens = new QSlider(gridFrame);
        TySens->setObjectName("TySens");
        TySens->setOrientation(Qt::Horizontal);
        TySens->setTickPosition(QSlider::TicksBelow);
        TySens->setTickInterval(20);

        horizontalLayout_5->addWidget(TySens);

        label_13 = new QLabel(gridFrame);
        label_13->setObjectName("label_13");

        horizontalLayout_5->addWidget(label_13);


        gridLayout->addLayout(horizontalLayout_5, 5, 1, 1, 1);

        PitchEnable = new QCheckBox(gridFrame);
        PitchEnable->setObjectName("PitchEnable");
        PitchEnable->setLayoutDirection(Qt::LeftToRight);
        PitchEnable->setChecked(false);

        gridLayout->addWidget(PitchEnable, 1, 0, 1, 1);

        YawEnable = new QCheckBox(gridFrame);
        YawEnable->setObjectName("YawEnable");

        gridLayout->addWidget(YawEnable, 2, 0, 1, 1);

        RollEnable = new QCheckBox(gridFrame);
        RollEnable->setObjectName("RollEnable");

        gridLayout->addWidget(RollEnable, 3, 0, 1, 1);

        TxEnable = new QCheckBox(gridFrame);
        TxEnable->setObjectName("TxEnable");

        gridLayout->addWidget(TxEnable, 4, 0, 1, 1);

        TyEnable = new QCheckBox(gridFrame);
        TyEnable->setObjectName("TyEnable");

        gridLayout->addWidget(TyEnable, 5, 0, 1, 1);

        TzEnable = new QCheckBox(gridFrame);
        TzEnable->setObjectName("TzEnable");

        gridLayout->addWidget(TzEnable, 6, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_5 = new QLabel(gridFrame);
        label_5->setObjectName("label_5");

        horizontalLayout_4->addWidget(label_5);

        TxSens = new QSlider(gridFrame);
        TxSens->setObjectName("TxSens");
        TxSens->setOrientation(Qt::Horizontal);
        TxSens->setTickPosition(QSlider::TicksBelow);
        TxSens->setTickInterval(20);

        horizontalLayout_4->addWidget(TxSens);

        label_12 = new QLabel(gridFrame);
        label_12->setObjectName("label_12");

        horizontalLayout_4->addWidget(label_12);


        gridLayout->addLayout(horizontalLayout_4, 4, 1, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_7 = new QLabel(gridFrame);
        label_7->setObjectName("label_7");

        horizontalLayout_6->addWidget(label_7);

        TzSens = new QSlider(gridFrame);
        TzSens->setObjectName("TzSens");
        TzSens->setOrientation(Qt::Horizontal);
        TzSens->setTickPosition(QSlider::TicksBelow);
        TzSens->setTickInterval(20);

        horizontalLayout_6->addWidget(TzSens);

        label_14 = new QLabel(gridFrame);
        label_14->setObjectName("label_14");

        horizontalLayout_6->addWidget(label_14);


        gridLayout->addLayout(horizontalLayout_6, 6, 1, 1, 1);

        label_18 = new QLabel(gridFrame);
        label_18->setObjectName("label_18");

        gridLayout->addWidget(label_18, 0, 2, 1, 1);

        PitchInvert = new QCheckBox(gridFrame);
        PitchInvert->setObjectName("PitchInvert");

        gridLayout->addWidget(PitchInvert, 1, 2, 1, 1);

        YawInvert = new QCheckBox(gridFrame);
        YawInvert->setObjectName("YawInvert");

        gridLayout->addWidget(YawInvert, 2, 2, 1, 1);

        RollInvert = new QCheckBox(gridFrame);
        RollInvert->setObjectName("RollInvert");

        gridLayout->addWidget(RollInvert, 3, 2, 1, 1);

        TxInvert = new QCheckBox(gridFrame);
        TxInvert->setObjectName("TxInvert");

        gridLayout->addWidget(TxInvert, 4, 2, 1, 1);

        TyInvert = new QCheckBox(gridFrame);
        TyInvert->setObjectName("TyInvert");

        gridLayout->addWidget(TyInvert, 5, 2, 1, 1);

        TzInvert = new QCheckBox(gridFrame);
        TzInvert->setObjectName("TzInvert");

        gridLayout->addWidget(TzInvert, 6, 2, 1, 1);


        verticalLayout->addWidget(gridFrame);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_17 = new QLabel(ProfileSetupForm);
        label_17->setObjectName("label_17");

        horizontalLayout_7->addWidget(label_17);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        label_15 = new QLabel(ProfileSetupForm);
        label_15->setObjectName("label_15");

        horizontalLayout_7->addWidget(label_15);

        Smoothing = new QSlider(ProfileSetupForm);
        Smoothing->setObjectName("Smoothing");
        Smoothing->setOrientation(Qt::Horizontal);

        horizontalLayout_7->addWidget(Smoothing);

        label_16 = new QLabel(ProfileSetupForm);
        label_16->setObjectName("label_16");

        horizontalLayout_7->addWidget(label_16);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        DetailedAxisSetup = new QPushButton(ProfileSetupForm);
        DetailedAxisSetup->setObjectName("DetailedAxisSetup");

        horizontalLayout_8->addWidget(DetailedAxisSetup);


        verticalLayout->addLayout(horizontalLayout_8);

        QWidget::setTabOrder(PitchEnable, PitchSens);
        QWidget::setTabOrder(PitchSens, YawEnable);
        QWidget::setTabOrder(YawEnable, YawSens);
        QWidget::setTabOrder(YawSens, RollEnable);
        QWidget::setTabOrder(RollEnable, RollSens);
        QWidget::setTabOrder(RollSens, TxEnable);
        QWidget::setTabOrder(TxEnable, TxSens);
        QWidget::setTabOrder(TxSens, TyEnable);
        QWidget::setTabOrder(TyEnable, TySens);
        QWidget::setTabOrder(TySens, TzEnable);
        QWidget::setTabOrder(TzEnable, TzSens);
        QWidget::setTabOrder(TzSens, Smoothing);
        QWidget::setTabOrder(Smoothing, DetailedAxisSetup);

        retranslateUi(ProfileSetupForm);

        QMetaObject::connectSlotsByName(ProfileSetupForm);
    } // setupUi

    void retranslateUi(QWidget *ProfileSetupForm)
    {
        ProfileSetupForm->setWindowTitle(QCoreApplication::translate("ProfileSetupForm", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("ProfileSetupForm", "Low", nullptr));
        label_10->setText(QCoreApplication::translate("ProfileSetupForm", "High", nullptr));
        label_4->setText(QCoreApplication::translate("ProfileSetupForm", "Low", nullptr));
        label_11->setText(QCoreApplication::translate("ProfileSetupForm", "High", nullptr));
        label_9->setText(QCoreApplication::translate("ProfileSetupForm", "Axis (enable)", nullptr));
        label_2->setText(QCoreApplication::translate("ProfileSetupForm", "Sensitivity", nullptr));
        label->setText(QCoreApplication::translate("ProfileSetupForm", "Low", nullptr));
        label_8->setText(QCoreApplication::translate("ProfileSetupForm", "High", nullptr));
        label_6->setText(QCoreApplication::translate("ProfileSetupForm", "Low", nullptr));
        label_13->setText(QCoreApplication::translate("ProfileSetupForm", "High", nullptr));
#if QT_CONFIG(tooltip)
        PitchEnable->setToolTip(QCoreApplication::translate("ProfileSetupForm", "Enable Pitch Axis", nullptr));
#endif // QT_CONFIG(tooltip)
        PitchEnable->setText(QCoreApplication::translate("ProfileSetupForm", "Pitch", nullptr));
#if QT_CONFIG(tooltip)
        YawEnable->setToolTip(QCoreApplication::translate("ProfileSetupForm", "Enable Yaw Axis", nullptr));
#endif // QT_CONFIG(tooltip)
        YawEnable->setText(QCoreApplication::translate("ProfileSetupForm", "Yaw", nullptr));
#if QT_CONFIG(tooltip)
        RollEnable->setToolTip(QCoreApplication::translate("ProfileSetupForm", "Enable Yaw Axis", nullptr));
#endif // QT_CONFIG(tooltip)
        RollEnable->setText(QCoreApplication::translate("ProfileSetupForm", "Roll", nullptr));
#if QT_CONFIG(tooltip)
        TxEnable->setToolTip(QCoreApplication::translate("ProfileSetupForm", "Enable X Axis", nullptr));
#endif // QT_CONFIG(tooltip)
        TxEnable->setText(QCoreApplication::translate("ProfileSetupForm", "Move left/right", nullptr));
#if QT_CONFIG(tooltip)
        TyEnable->setToolTip(QCoreApplication::translate("ProfileSetupForm", "Enable Y Axis", nullptr));
#endif // QT_CONFIG(tooltip)
        TyEnable->setText(QCoreApplication::translate("ProfileSetupForm", "Move up/down", nullptr));
#if QT_CONFIG(tooltip)
        TzEnable->setToolTip(QCoreApplication::translate("ProfileSetupForm", "Enable Z Axis", nullptr));
#endif // QT_CONFIG(tooltip)
        TzEnable->setText(QCoreApplication::translate("ProfileSetupForm", "Move back/forth", nullptr));
        label_5->setText(QCoreApplication::translate("ProfileSetupForm", "Low", nullptr));
        label_12->setText(QCoreApplication::translate("ProfileSetupForm", "High", nullptr));
        label_7->setText(QCoreApplication::translate("ProfileSetupForm", "Low", nullptr));
        label_14->setText(QCoreApplication::translate("ProfileSetupForm", "High", nullptr));
        label_18->setText(QCoreApplication::translate("ProfileSetupForm", "Invert", nullptr));
        PitchInvert->setText(QString());
        YawInvert->setText(QString());
        RollInvert->setText(QString());
        TxInvert->setText(QString());
        TyInvert->setText(QString());
        TzInvert->setText(QString());
        label_17->setText(QCoreApplication::translate("ProfileSetupForm", "Common Smoothing:", nullptr));
        label_15->setText(QCoreApplication::translate("ProfileSetupForm", "None", nullptr));
        label_16->setText(QCoreApplication::translate("ProfileSetupForm", "Max", nullptr));
        DetailedAxisSetup->setText(QCoreApplication::translate("ProfileSetupForm", "Detailed Axes Setup", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProfileSetupForm: public Ui_ProfileSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILE_SETUP_H
