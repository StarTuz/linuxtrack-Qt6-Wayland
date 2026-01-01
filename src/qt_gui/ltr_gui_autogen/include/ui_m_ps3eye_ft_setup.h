/********************************************************************************
** Form generated from reading UI file 'm_ps3eye_ft_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_M_PS3EYE_FT_SETUP_H
#define UI_M_PS3EYE_FT_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MacPS3FtSetupForm
{
public:
    QVBoxLayout *verticalLayout_5;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QGridLayout *gridLayout;
    QLabel *label_15;
    QComboBox *WebcamResolutionsMac;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_11;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_45;
    QLineEdit *CascadePathMac;
    QPushButton *FindCascadeMac;
    QGridLayout *gridLayout_2;
    QLabel *label_48;
    QSlider *OptimLevelMac;
    QLabel *label_67;
    QLabel *label_63;
    QLabel *label_12;
    QLabel *label;
    QSlider *ExpFilterFactorMac;
    QLabel *label_2;
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

    void setupUi(QWidget *MacPS3FtSetupForm)
    {
        if (MacPS3FtSetupForm->objectName().isEmpty())
            MacPS3FtSetupForm->setObjectName("MacPS3FtSetupForm");
        MacPS3FtSetupForm->resize(467, 425);
        verticalLayout_5 = new QVBoxLayout(MacPS3FtSetupForm);
        verticalLayout_5->setObjectName("verticalLayout_5");
        frame = new QFrame(MacPS3FtSetupForm);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, 4, -1, 4);
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
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(WebcamResolutionsMac->sizePolicy().hasHeightForWidth());
        WebcamResolutionsMac->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(WebcamResolutionsMac, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_5->addWidget(frame);

        frame_2 = new QFrame(MacPS3FtSetupForm);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_2);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(-1, 4, -1, 4);
        label_11 = new QLabel(frame_2);
        label_11->setObjectName("label_11");
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_4->addWidget(label_11);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_45 = new QLabel(frame_2);
        label_45->setObjectName("label_45");

        horizontalLayout_2->addWidget(label_45);

        CascadePathMac = new QLineEdit(frame_2);
        CascadePathMac->setObjectName("CascadePathMac");

        horizontalLayout_2->addWidget(CascadePathMac);

        FindCascadeMac = new QPushButton(frame_2);
        FindCascadeMac->setObjectName("FindCascadeMac");

        horizontalLayout_2->addWidget(FindCascadeMac);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        label_48 = new QLabel(frame_2);
        label_48->setObjectName("label_48");

        gridLayout_2->addWidget(label_48, 0, 0, 1, 1);

        OptimLevelMac = new QSlider(frame_2);
        OptimLevelMac->setObjectName("OptimLevelMac");
        OptimLevelMac->setMaximum(3);
        OptimLevelMac->setPageStep(1);
        OptimLevelMac->setValue(0);
        OptimLevelMac->setOrientation(Qt::Horizontal);
        OptimLevelMac->setTickPosition(QSlider::TicksBelow);

        gridLayout_2->addWidget(OptimLevelMac, 0, 2, 1, 1);

        label_67 = new QLabel(frame_2);
        label_67->setObjectName("label_67");

        gridLayout_2->addWidget(label_67, 0, 3, 1, 1);

        label_63 = new QLabel(frame_2);
        label_63->setObjectName("label_63");

        gridLayout_2->addWidget(label_63, 0, 1, 1, 1);

        label_12 = new QLabel(frame_2);
        label_12->setObjectName("label_12");

        gridLayout_2->addWidget(label_12, 1, 0, 1, 1);

        label = new QLabel(frame_2);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label, 1, 1, 1, 1);

        ExpFilterFactorMac = new QSlider(frame_2);
        ExpFilterFactorMac->setObjectName("ExpFilterFactorMac");
        ExpFilterFactorMac->setOrientation(Qt::Horizontal);
        ExpFilterFactorMac->setTickPosition(QSlider::TicksBelow);
        ExpFilterFactorMac->setTickInterval(20);

        gridLayout_2->addWidget(ExpFilterFactorMac, 1, 2, 1, 1);

        label_2 = new QLabel(frame_2);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 1, 3, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);


        verticalLayout_5->addWidget(frame_2);

        frame_3 = new QFrame(MacPS3FtSetupForm);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(-1, 4, -1, 4);
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


        retranslateUi(MacPS3FtSetupForm);

        QMetaObject::connectSlotsByName(MacPS3FtSetupForm);
    } // setupUi

    void retranslateUi(QWidget *MacPS3FtSetupForm)
    {
        MacPS3FtSetupForm->setProperty("WindowTitle", QVariant(QCoreApplication::translate("MacPS3FtSetupForm", "WebcamSetupForm", nullptr)));
        label_3->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Webcam Setup", nullptr));
        label_15->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Resolution", nullptr));
        WebcamResolutionsMac->setItemText(0, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 187 FPS", nullptr));
        WebcamResolutionsMac->setItemText(1, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 150 FPS", nullptr));
        WebcamResolutionsMac->setItemText(2, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 137 FPS", nullptr));
        WebcamResolutionsMac->setItemText(3, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 120 FPS", nullptr));
        WebcamResolutionsMac->setItemText(4, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 100 FPS", nullptr));
        WebcamResolutionsMac->setItemText(5, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 75 FPS", nullptr));
        WebcamResolutionsMac->setItemText(6, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 60 FPS", nullptr));
        WebcamResolutionsMac->setItemText(7, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 50 FPS", nullptr));
        WebcamResolutionsMac->setItemText(8, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 37 FPS", nullptr));
        WebcamResolutionsMac->setItemText(9, QCoreApplication::translate("MacPS3FtSetupForm", "320x240 @ 30 FPS", nullptr));
        WebcamResolutionsMac->setItemText(10, QCoreApplication::translate("MacPS3FtSetupForm", "640x480 @ 60 FPS", nullptr));
        WebcamResolutionsMac->setItemText(11, QCoreApplication::translate("MacPS3FtSetupForm", "640x480 @ 50 FPS", nullptr));
        WebcamResolutionsMac->setItemText(12, QCoreApplication::translate("MacPS3FtSetupForm", "640x480 @ 40 FPS", nullptr));
        WebcamResolutionsMac->setItemText(13, QCoreApplication::translate("MacPS3FtSetupForm", "640x480 @ 30 FPS", nullptr));
        WebcamResolutionsMac->setItemText(14, QCoreApplication::translate("MacPS3FtSetupForm", "640x480 @ 15 FPS", nullptr));

        label_11->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Facetracker Setup", nullptr));
        label_45->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Cascade", nullptr));
        FindCascadeMac->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Browse", nullptr));
        label_48->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Optimize for:", nullptr));
        label_67->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Speed", nullptr));
        label_63->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Precision", nullptr));
        label_12->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Smoothing:", nullptr));
        label->setText(QCoreApplication::translate("MacPS3FtSetupForm", "None", nullptr));
        label_2->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Max", nullptr));
        label_9->setText(QCoreApplication::translate("MacPS3FtSetupForm", "<html><head/><body><p><span style=\" font-weight:600;\">Picture Controls</span></p></body></html>", nullptr));
        AGC->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Auto Gain", nullptr));
        AEX->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Auto Exposure", nullptr));
        AWB->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Auto White Ballance", nullptr));
        PLF50->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Power Line Frequency 50Hz", nullptr));
        label_5->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Exposure", nullptr));
        label_6->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Gain", nullptr));
        label_7->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Brightness", nullptr));
        label_8->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Contrast", nullptr));
        label_10->setText(QCoreApplication::translate("MacPS3FtSetupForm", "Sharpness", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MacPS3FtSetupForm: public Ui_MacPS3FtSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_M_PS3EYE_FT_SETUP_H
