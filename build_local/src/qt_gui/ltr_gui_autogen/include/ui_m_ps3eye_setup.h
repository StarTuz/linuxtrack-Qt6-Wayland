/********************************************************************************
** Form generated from reading UI file 'm_ps3eye_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_M_PS3EYE_SETUP_H
#define UI_M_PS3EYE_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MacPS3SetupForm
{
public:
    QVBoxLayout *verticalLayout_5;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QGridLayout *gridLayout;
    QLabel *label_15;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_32;
    QSlider *WebcamThresholdMac;
    QLabel *label;
    QComboBox *WebcamResolutionsMac;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_42;
    QSpinBox *WebcamMinBlobMac;
    QLabel *label_40;
    QSpinBox *WebcamMaxBlobMac;
    QLabel *label_41;
    QSpacerItem *horizontalSpacer;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *AGC;
    QCheckBox *AEX;
    QCheckBox *AWB;
    QCheckBox *PLF50;
    QFormLayout *formLayout;
    QLabel *label_5;
    QSlider *EXPOSURE;
    QLabel *label_6;
    QSlider *GAIN;
    QLabel *label_7;
    QSlider *BRIGHTNESS;
    QLabel *label_8;
    QSlider *CONTRAST;
    QLabel *label_10;
    QSlider *SHARPNESS;

    void setupUi(QWidget *MacPS3SetupForm)
    {
        if (MacPS3SetupForm->objectName().isEmpty())
            MacPS3SetupForm->setObjectName("MacPS3SetupForm");
        MacPS3SetupForm->resize(430, 420);
        verticalLayout_5 = new QVBoxLayout(MacPS3SetupForm);
        verticalLayout_5->setObjectName("verticalLayout_5");
        frame = new QFrame(MacPS3SetupForm);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        QFont font;
        font.setBold(true);
        label_3->setFont(font);

        verticalLayout->addWidget(label_3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_15 = new QLabel(frame);
        label_15->setObjectName("label_15");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_15, 0, 0, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_32 = new QLabel(frame);
        label_32->setObjectName("label_32");
        sizePolicy.setHeightForWidth(label_32->sizePolicy().hasHeightForWidth());
        label_32->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_32);

        WebcamThresholdMac = new QSlider(frame);
        WebcamThresholdMac->setObjectName("WebcamThresholdMac");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(WebcamThresholdMac->sizePolicy().hasHeightForWidth());
        WebcamThresholdMac->setSizePolicy(sizePolicy1);
        WebcamThresholdMac->setMaximum(255);
        WebcamThresholdMac->setValue(150);
        WebcamThresholdMac->setOrientation(Qt::Horizontal);
        WebcamThresholdMac->setTickPosition(QSlider::TicksBelow);
        WebcamThresholdMac->setTickInterval(32);

        horizontalLayout->addWidget(WebcamThresholdMac);

        label = new QLabel(frame);
        label->setObjectName("label");
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        WebcamResolutionsMac = new QComboBox(frame);
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->addItem(QString());
        WebcamResolutionsMac->setObjectName("WebcamResolutionsMac");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(WebcamResolutionsMac->sizePolicy().hasHeightForWidth());
        WebcamResolutionsMac->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(WebcamResolutionsMac, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_5->addWidget(frame);

        frame_2 = new QFrame(MacPS3SetupForm);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_4 = new QLabel(frame_2);
        label_4->setObjectName("label_4");
        label_4->setFont(font);

        verticalLayout_2->addWidget(label_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_42 = new QLabel(frame_2);
        label_42->setObjectName("label_42");
        sizePolicy.setHeightForWidth(label_42->sizePolicy().hasHeightForWidth());
        label_42->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_42);

        WebcamMinBlobMac = new QSpinBox(frame_2);
        WebcamMinBlobMac->setObjectName("WebcamMinBlobMac");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(WebcamMinBlobMac->sizePolicy().hasHeightForWidth());
        WebcamMinBlobMac->setSizePolicy(sizePolicy3);
        WebcamMinBlobMac->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WebcamMinBlobMac->setMinimum(1);
        WebcamMinBlobMac->setMaximum(10000);
        WebcamMinBlobMac->setValue(4);

        horizontalLayout_2->addWidget(WebcamMinBlobMac);

        label_40 = new QLabel(frame_2);
        label_40->setObjectName("label_40");
        sizePolicy.setHeightForWidth(label_40->sizePolicy().hasHeightForWidth());
        label_40->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_40);

        WebcamMaxBlobMac = new QSpinBox(frame_2);
        WebcamMaxBlobMac->setObjectName("WebcamMaxBlobMac");
        sizePolicy3.setHeightForWidth(WebcamMaxBlobMac->sizePolicy().hasHeightForWidth());
        WebcamMaxBlobMac->setSizePolicy(sizePolicy3);
        WebcamMaxBlobMac->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WebcamMaxBlobMac->setMinimum(1);
        WebcamMaxBlobMac->setMaximum(10000);
        WebcamMaxBlobMac->setValue(2500);

        horizontalLayout_2->addWidget(WebcamMaxBlobMac);

        label_41 = new QLabel(frame_2);
        label_41->setObjectName("label_41");
        sizePolicy.setHeightForWidth(label_41->sizePolicy().hasHeightForWidth());
        label_41->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_41);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_5->addWidget(frame_2);

        frame_3 = new QFrame(MacPS3SetupForm);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_9 = new QLabel(frame_3);
        label_9->setObjectName("label_9");
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_3->addWidget(label_9);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        AGC = new QCheckBox(frame_3);
        AGC->setObjectName("AGC");

        horizontalLayout_4->addWidget(AGC);

        AEX = new QCheckBox(frame_3);
        AEX->setObjectName("AEX");

        horizontalLayout_4->addWidget(AEX);

        AWB = new QCheckBox(frame_3);
        AWB->setObjectName("AWB");

        horizontalLayout_4->addWidget(AWB);


        verticalLayout_3->addLayout(horizontalLayout_4);

        PLF50 = new QCheckBox(frame_3);
        PLF50->setObjectName("PLF50");

        verticalLayout_3->addWidget(PLF50);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_5 = new QLabel(frame_3);
        label_5->setObjectName("label_5");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_5);

        EXPOSURE = new QSlider(frame_3);
        EXPOSURE->setObjectName("EXPOSURE");
        EXPOSURE->setMaximum(255);
        EXPOSURE->setOrientation(Qt::Horizontal);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, EXPOSURE);

        label_6 = new QLabel(frame_3);
        label_6->setObjectName("label_6");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_6);

        GAIN = new QSlider(frame_3);
        GAIN->setObjectName("GAIN");
        GAIN->setMaximum(63);
        GAIN->setOrientation(Qt::Horizontal);
        GAIN->setTickPosition(QSlider::NoTicks);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, GAIN);

        label_7 = new QLabel(frame_3);
        label_7->setObjectName("label_7");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_7);

        BRIGHTNESS = new QSlider(frame_3);
        BRIGHTNESS->setObjectName("BRIGHTNESS");
        BRIGHTNESS->setMaximum(255);
        BRIGHTNESS->setOrientation(Qt::Horizontal);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, BRIGHTNESS);

        label_8 = new QLabel(frame_3);
        label_8->setObjectName("label_8");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_8);

        CONTRAST = new QSlider(frame_3);
        CONTRAST->setObjectName("CONTRAST");
        CONTRAST->setMaximum(255);
        CONTRAST->setOrientation(Qt::Horizontal);

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, CONTRAST);

        label_10 = new QLabel(frame_3);
        label_10->setObjectName("label_10");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, label_10);

        SHARPNESS = new QSlider(frame_3);
        SHARPNESS->setObjectName("SHARPNESS");
        SHARPNESS->setMaximum(63);
        SHARPNESS->setOrientation(Qt::Horizontal);

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, SHARPNESS);


        verticalLayout_3->addLayout(formLayout);


        verticalLayout_5->addWidget(frame_3);


        retranslateUi(MacPS3SetupForm);

        QMetaObject::connectSlotsByName(MacPS3SetupForm);
    } // setupUi

    void retranslateUi(QWidget *MacPS3SetupForm)
    {
        MacPS3SetupForm->setProperty("WindowTitle", QVariant(QCoreApplication::translate("MacPS3SetupForm", "WebcamSetupForm", nullptr)));
        label_3->setText(QCoreApplication::translate("MacPS3SetupForm", "Webcam Setup", nullptr));
        label_15->setText(QCoreApplication::translate("MacPS3SetupForm", "Resolution", nullptr));
        label_2->setText(QCoreApplication::translate("MacPS3SetupForm", "Threshold", nullptr));
        label_32->setText(QCoreApplication::translate("MacPS3SetupForm", "0", nullptr));
        label->setText(QCoreApplication::translate("MacPS3SetupForm", "255", nullptr));
        WebcamResolutionsMac->setItemText(0, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 187 FPS", nullptr));
        WebcamResolutionsMac->setItemText(1, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 150 FPS", nullptr));
        WebcamResolutionsMac->setItemText(2, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 137 FPS", nullptr));
        WebcamResolutionsMac->setItemText(3, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 120 FPS", nullptr));
        WebcamResolutionsMac->setItemText(4, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 100 FPS", nullptr));
        WebcamResolutionsMac->setItemText(5, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 75 FPS", nullptr));
        WebcamResolutionsMac->setItemText(6, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 60 FPS", nullptr));
        WebcamResolutionsMac->setItemText(7, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 50 FPS", nullptr));
        WebcamResolutionsMac->setItemText(8, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 37 FPS", nullptr));
        WebcamResolutionsMac->setItemText(9, QCoreApplication::translate("MacPS3SetupForm", "320x240 @ 30 FPS", nullptr));
        WebcamResolutionsMac->setItemText(10, QCoreApplication::translate("MacPS3SetupForm", "640x480 @ 60 FPS", nullptr));
        WebcamResolutionsMac->setItemText(11, QCoreApplication::translate("MacPS3SetupForm", "640x480 @ 50 FPS", nullptr));
        WebcamResolutionsMac->setItemText(12, QCoreApplication::translate("MacPS3SetupForm", "640x480 @ 40 FPS", nullptr));
        WebcamResolutionsMac->setItemText(13, QCoreApplication::translate("MacPS3SetupForm", "640x480 @ 30 FPS", nullptr));
        WebcamResolutionsMac->setItemText(14, QCoreApplication::translate("MacPS3SetupForm", "640x480 @ 15 FPS", nullptr));

        label_4->setText(QCoreApplication::translate("MacPS3SetupForm", "Troubleshooting", nullptr));
        label_42->setText(QCoreApplication::translate("MacPS3SetupForm", "Valid Blob Size:", nullptr));
        label_40->setText(QCoreApplication::translate("MacPS3SetupForm", "to", nullptr));
        label_41->setText(QCoreApplication::translate("MacPS3SetupForm", "pixels", nullptr));
        label_9->setText(QCoreApplication::translate("MacPS3SetupForm", "<html><head/><body><p><span style=\" font-weight:600;\">Picture Controls</span></p></body></html>", nullptr));
        AGC->setText(QCoreApplication::translate("MacPS3SetupForm", "Auto Gain", nullptr));
        AEX->setText(QCoreApplication::translate("MacPS3SetupForm", "Auto Exposure", nullptr));
        AWB->setText(QCoreApplication::translate("MacPS3SetupForm", "Auto White Ballance", nullptr));
        PLF50->setText(QCoreApplication::translate("MacPS3SetupForm", "Power Line Frequency 50Hz", nullptr));
        label_5->setText(QCoreApplication::translate("MacPS3SetupForm", "Exposure", nullptr));
        label_6->setText(QCoreApplication::translate("MacPS3SetupForm", "Gain", nullptr));
        label_7->setText(QCoreApplication::translate("MacPS3SetupForm", "Brightness", nullptr));
        label_8->setText(QCoreApplication::translate("MacPS3SetupForm", "Contrast", nullptr));
        label_10->setText(QCoreApplication::translate("MacPS3SetupForm", "Sharpness", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MacPS3SetupForm: public Ui_MacPS3SetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_M_PS3EYE_SETUP_H
