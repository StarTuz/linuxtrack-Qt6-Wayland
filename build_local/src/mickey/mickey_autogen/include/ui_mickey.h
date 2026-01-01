/********************************************************************************
** Form generated from reading UI file 'mickey.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MICKEY_H
#define UI_MICKEY_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Mickey
{
public:
    QVBoxLayout *verticalLayout_4;
    QTabWidget *MickeyTabs;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *PrefLayout1;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *RelativeCB;
    QRadioButton *AbsoluteCB;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QSlider *SensSlider;
    QLabel *label_4;
    QLabel *label_5;
    QSlider *CurveSlider;
    QSlider *DZSlider;
    QLabel *label_7;
    QCheckBox *StepOnly;
    QLabel *label_13;
    QSlider *SmoothingSlider;
    QPushButton *ApplyButton;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_3;
    QPushButton *CalibrateButton;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_8;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *PrefLayout;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_7;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_6;
    QGridLayout *HotkeyStack;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_10;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_3;
    QSpinBox *CenterTimeout;
    QLabel *label_2;
    QSpinBox *CalibrationTimeout;
    QLabel *label_11;
    QLabel *label_12;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *HelpButton;
    QPushButton *RecenterButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *QuitButton;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLabel *StatusLabel;

    void setupUi(QWidget *Mickey)
    {
        if (Mickey->objectName().isEmpty())
            Mickey->setObjectName("Mickey");
        Mickey->resize(569, 415);
        Mickey->setMinimumSize(QSize(470, 415));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ltr/linuxtrack.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Mickey->setWindowIcon(icon);
        verticalLayout_4 = new QVBoxLayout(Mickey);
        verticalLayout_4->setObjectName("verticalLayout_4");
        MickeyTabs = new QTabWidget(Mickey);
        MickeyTabs->setObjectName("MickeyTabs");
        MickeyTabs->setMinimumSize(QSize(450, 0));
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        horizontalLayout_3 = new QHBoxLayout(tab_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        PrefLayout1 = new QVBoxLayout();
        PrefLayout1->setObjectName("PrefLayout1");
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName("groupBox");
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 2, -1, 2);
        RelativeCB = new QRadioButton(groupBox);
        RelativeCB->setObjectName("RelativeCB");
        RelativeCB->setChecked(true);

        horizontalLayout_2->addWidget(RelativeCB);

        AbsoluteCB = new QRadioButton(groupBox);
        AbsoluteCB->setObjectName("AbsoluteCB");

        horizontalLayout_2->addWidget(AbsoluteCB);


        PrefLayout1->addWidget(groupBox);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_3 = new QLabel(tab_2);
        label_3->setObjectName("label_3");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        SensSlider = new QSlider(tab_2);
        SensSlider->setObjectName("SensSlider");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SensSlider->sizePolicy().hasHeightForWidth());
        SensSlider->setSizePolicy(sizePolicy1);
        SensSlider->setMinimumSize(QSize(0, 0));
        SensSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(SensSlider, 0, 1, 1, 1);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName("label_4");
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName("label_5");
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        CurveSlider = new QSlider(tab_2);
        CurveSlider->setObjectName("CurveSlider");
        CurveSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(CurveSlider, 2, 1, 1, 1);

        DZSlider = new QSlider(tab_2);
        DZSlider->setObjectName("DZSlider");
        DZSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(DZSlider, 1, 1, 1, 1);

        label_7 = new QLabel(tab_2);
        label_7->setObjectName("label_7");
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_7, 4, 0, 1, 1);

        StepOnly = new QCheckBox(tab_2);
        StepOnly->setObjectName("StepOnly");

        gridLayout->addWidget(StepOnly, 4, 1, 1, 1);

        label_13 = new QLabel(tab_2);
        label_13->setObjectName("label_13");

        gridLayout->addWidget(label_13, 3, 0, 1, 1);

        SmoothingSlider = new QSlider(tab_2);
        SmoothingSlider->setObjectName("SmoothingSlider");
        SmoothingSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(SmoothingSlider, 3, 1, 1, 1);


        PrefLayout1->addLayout(gridLayout);

        ApplyButton = new QPushButton(tab_2);
        ApplyButton->setObjectName("ApplyButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ApplyButton->sizePolicy().hasHeightForWidth());
        ApplyButton->setSizePolicy(sizePolicy2);
        ApplyButton->setMinimumSize(QSize(0, 0));

        PrefLayout1->addWidget(ApplyButton);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        PrefLayout1->addItem(verticalSpacer);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        PrefLayout1->addItem(verticalSpacer_3);

        CalibrateButton = new QPushButton(tab_2);
        CalibrateButton->setObjectName("CalibrateButton");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(CalibrateButton->sizePolicy().hasHeightForWidth());
        CalibrateButton->setSizePolicy(sizePolicy3);

        PrefLayout1->addWidget(CalibrateButton);


        horizontalLayout_3->addLayout(PrefLayout1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_8 = new QLabel(tab_2);
        label_8->setObjectName("label_8");
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_8);

        frame_4 = new QFrame(tab_2);
        frame_4->setObjectName("frame_4");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy4);
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_8 = new QVBoxLayout(frame_4);
        verticalLayout_8->setContentsMargins(2, 2, 2, 2);
        verticalLayout_8->setObjectName("verticalLayout_8");
        PrefLayout = new QVBoxLayout();
        PrefLayout->setSpacing(1);
        PrefLayout->setObjectName("PrefLayout");

        verticalLayout_8->addLayout(PrefLayout);


        verticalLayout_3->addWidget(frame_4);


        horizontalLayout_3->addLayout(verticalLayout_3);

        MickeyTabs->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        horizontalLayout_7 = new QHBoxLayout(tab_3);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        frame_5 = new QFrame(tab_3);
        frame_5->setObjectName("frame_5");
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_5);
        verticalLayout_5->setObjectName("verticalLayout_5");
        frame_3 = new QFrame(frame_5);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_3);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        HotkeyStack = new QGridLayout();
        HotkeyStack->setObjectName("HotkeyStack");
        HotkeyStack->setHorizontalSpacing(2);

        horizontalLayout_6->addLayout(HotkeyStack);


        verticalLayout_5->addWidget(frame_3);

        frame_2 = new QFrame(frame_5);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_10 = new QLabel(frame_2);
        label_10->setObjectName("label_10");

        verticalLayout_2->addWidget(label_10);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        label_6 = new QLabel(frame_2);
        label_6->setObjectName("label_6");

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 3, 1, 1);

        CenterTimeout = new QSpinBox(frame_2);
        CenterTimeout->setObjectName("CenterTimeout");
        CenterTimeout->setMinimum(1);
        CenterTimeout->setValue(10);

        gridLayout_3->addWidget(CenterTimeout, 0, 1, 1, 1);

        label_2 = new QLabel(frame_2);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        CalibrationTimeout = new QSpinBox(frame_2);
        CalibrationTimeout->setObjectName("CalibrationTimeout");
        CalibrationTimeout->setMinimum(1);
        CalibrationTimeout->setValue(10);

        gridLayout_3->addWidget(CalibrationTimeout, 1, 1, 1, 1);

        label_11 = new QLabel(frame_2);
        label_11->setObjectName("label_11");

        gridLayout_3->addWidget(label_11, 0, 2, 1, 1);

        label_12 = new QLabel(frame_2);
        label_12->setObjectName("label_12");

        gridLayout_3->addWidget(label_12, 1, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);


        verticalLayout_5->addWidget(frame_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);


        horizontalLayout_7->addWidget(frame_5);

        MickeyTabs->addTab(tab_3, QString());

        verticalLayout_4->addWidget(MickeyTabs);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        HelpButton = new QPushButton(Mickey);
        HelpButton->setObjectName("HelpButton");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(HelpButton->sizePolicy().hasHeightForWidth());
        HelpButton->setSizePolicy(sizePolicy5);

        horizontalLayout->addWidget(HelpButton);

        RecenterButton = new QPushButton(Mickey);
        RecenterButton->setObjectName("RecenterButton");
        sizePolicy5.setHeightForWidth(RecenterButton->sizePolicy().hasHeightForWidth());
        RecenterButton->setSizePolicy(sizePolicy5);

        horizontalLayout->addWidget(RecenterButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        QuitButton = new QPushButton(Mickey);
        QuitButton->setObjectName("QuitButton");
        sizePolicy5.setHeightForWidth(QuitButton->sizePolicy().hasHeightForWidth());
        QuitButton->setSizePolicy(sizePolicy5);

        horizontalLayout->addWidget(QuitButton);


        verticalLayout_4->addLayout(horizontalLayout);

        frame = new QFrame(Mickey);
        frame->setObjectName("frame");
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame);
        horizontalLayout_5->setContentsMargins(1, 1, 1, 1);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label = new QLabel(frame);
        label->setObjectName("label");
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy6);

        horizontalLayout_4->addWidget(label);

        StatusLabel = new QLabel(frame);
        StatusLabel->setObjectName("StatusLabel");

        horizontalLayout_4->addWidget(StatusLabel);


        horizontalLayout_5->addLayout(horizontalLayout_4);


        verticalLayout_4->addWidget(frame);


        retranslateUi(Mickey);
        QObject::connect(QuitButton, &QPushButton::pressed, Mickey, qOverload<>(&QWidget::close));
        QObject::connect(AbsoluteCB, &QRadioButton::clicked, SensSlider, &QSlider::setDisabled);
        QObject::connect(AbsoluteCB, &QRadioButton::clicked, DZSlider, &QSlider::setDisabled);
        QObject::connect(AbsoluteCB, &QRadioButton::clicked, CurveSlider, &QSlider::setDisabled);
        QObject::connect(AbsoluteCB, &QRadioButton::clicked, StepOnly, &QCheckBox::setDisabled);
        QObject::connect(RelativeCB, &QRadioButton::clicked, SensSlider, &QSlider::setEnabled);
        QObject::connect(RelativeCB, &QRadioButton::clicked, DZSlider, &QSlider::setEnabled);
        QObject::connect(RelativeCB, &QRadioButton::clicked, CurveSlider, &QSlider::setEnabled);
        QObject::connect(RelativeCB, &QRadioButton::clicked, StepOnly, &QCheckBox::setEnabled);

        MickeyTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Mickey);
    } // setupUi

    void retranslateUi(QWidget *Mickey)
    {
        Mickey->setWindowTitle(QCoreApplication::translate("Mickey", "Mickey, the virtual mouse", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Mickey", "Tracking mode", nullptr));
        RelativeCB->setText(QCoreApplication::translate("Mickey", "Relative", nullptr));
        AbsoluteCB->setText(QCoreApplication::translate("Mickey", "Absolute", nullptr));
        label_3->setText(QCoreApplication::translate("Mickey", "Curvature", nullptr));
        label_4->setText(QCoreApplication::translate("Mickey", "Deadzone", nullptr));
        label_5->setText(QCoreApplication::translate("Mickey", "Sensitivity", nullptr));
        label_7->setText(QCoreApplication::translate("Mickey", "Step only", nullptr));
        StepOnly->setText(QString());
        label_13->setText(QCoreApplication::translate("Mickey", "Smoothing", nullptr));
        ApplyButton->setText(QCoreApplication::translate("Mickey", "Apply Changes", nullptr));
        CalibrateButton->setText(QCoreApplication::translate("Mickey", "Calibrate", nullptr));
        label_8->setText(QCoreApplication::translate("Mickey", "Sensitivity curve preview", nullptr));
        MickeyTabs->setTabText(MickeyTabs->indexOf(tab_2), QCoreApplication::translate("Mickey", "Tracking Setup", nullptr));
        label_10->setText(QCoreApplication::translate("Mickey", "Timeouts setup", nullptr));
        label_6->setText(QCoreApplication::translate("Mickey", "Calibration timeout", nullptr));
        label_2->setText(QCoreApplication::translate("Mickey", "Centering timeout", nullptr));
        label_11->setText(QCoreApplication::translate("Mickey", "seconds", nullptr));
        label_12->setText(QCoreApplication::translate("Mickey", "seconds", nullptr));
        MickeyTabs->setTabText(MickeyTabs->indexOf(tab_3), QCoreApplication::translate("Mickey", "Misc. Setup", nullptr));
        HelpButton->setText(QCoreApplication::translate("Mickey", "Help", nullptr));
        RecenterButton->setText(QCoreApplication::translate("Mickey", "Recenter", nullptr));
        QuitButton->setText(QCoreApplication::translate("Mickey", "Quit", nullptr));
        label->setText(QCoreApplication::translate("Mickey", "Status: ", nullptr));
        StatusLabel->setText(QCoreApplication::translate("Mickey", "Tracking", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mickey: public Ui_Mickey {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MICKEY_H
