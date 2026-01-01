/********************************************************************************
** Form generated from reading UI file 'clip_edit.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIP_EDIT_H
#define UI_CLIP_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClipEditForm
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_30;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *ClipA;
    QDoubleSpinBox *ClipB;
    QLabel *label_34;
    QLabel *label_35;
    QDoubleSpinBox *ClipC;
    QLabel *label_38;
    QDoubleSpinBox *ClipD;
    QSpacerItem *horizontalSpacer;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *ClipLeft;
    QRadioButton *ClipRight;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *ClipLeds;
    QRadioButton *ClipReflective;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ClipEditForm)
    {
        if (ClipEditForm->objectName().isEmpty())
            ClipEditForm->setObjectName("ClipEditForm");
        ClipEditForm->resize(489, 345);
        verticalLayout_5 = new QVBoxLayout(ClipEditForm);
        verticalLayout_5->setObjectName("verticalLayout_5");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_30 = new QLabel(ClipEditForm);
        label_30->setObjectName("label_30");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_30->sizePolicy().hasHeightForWidth());
        label_30->setSizePolicy(sizePolicy);
        label_30->setPixmap(QPixmap(QString::fromUtf8(":/ltr/clip_small.png")));
        label_30->setScaledContents(false);
        label_30->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_30);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        frame = new QFrame(ClipEditForm);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label = new QLabel(frame);
        label->setObjectName("label");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(label);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        ClipA = new QDoubleSpinBox(frame);
        ClipA->setObjectName("ClipA");
        ClipA->setMinimum(1.000000000000000);
        ClipA->setMaximum(999.990000000000009);
        ClipA->setValue(30.000000000000000);

        gridLayout_3->addWidget(ClipA, 0, 1, 1, 1);

        ClipB = new QDoubleSpinBox(frame);
        ClipB->setObjectName("ClipB");
        ClipB->setMinimum(1.000000000000000);
        ClipB->setMaximum(999.990000000000009);
        ClipB->setValue(40.000000000000000);

        gridLayout_3->addWidget(ClipB, 1, 1, 1, 1);

        label_34 = new QLabel(frame);
        label_34->setObjectName("label_34");

        gridLayout_3->addWidget(label_34, 1, 0, 1, 1);

        label_35 = new QLabel(frame);
        label_35->setObjectName("label_35");

        gridLayout_3->addWidget(label_35, 2, 0, 1, 1);

        ClipC = new QDoubleSpinBox(frame);
        ClipC->setObjectName("ClipC");
        ClipC->setMinimum(1.000000000000000);
        ClipC->setMaximum(999.990000000000009);
        ClipC->setValue(70.000000000000000);

        gridLayout_3->addWidget(ClipC, 2, 1, 1, 1);

        label_38 = new QLabel(frame);
        label_38->setObjectName("label_38");

        gridLayout_3->addWidget(label_38, 3, 0, 1, 1);

        ClipD = new QDoubleSpinBox(frame);
        ClipD->setObjectName("ClipD");
        ClipD->setMinimum(1.000000000000000);
        ClipD->setMaximum(999.990000000000009);
        ClipD->setValue(80.000000000000000);

        gridLayout_3->addWidget(ClipD, 3, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);


        verticalLayout->addWidget(frame);

        frame_3 = new QFrame(ClipEditForm);
        frame_3->setObjectName("frame_3");
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_4 = new QLabel(frame_3);
        label_4->setObjectName("label_4");
        QFont font;
        font.setBold(true);
        label_4->setFont(font);

        verticalLayout_4->addWidget(label_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        ClipLeft = new QRadioButton(frame_3);
        ClipLeft->setObjectName("ClipLeft");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ClipLeft->sizePolicy().hasHeightForWidth());
        ClipLeft->setSizePolicy(sizePolicy3);
        ClipLeft->setChecked(true);

        horizontalLayout_5->addWidget(ClipLeft);

        ClipRight = new QRadioButton(frame_3);
        ClipRight->setObjectName("ClipRight");

        horizontalLayout_5->addWidget(ClipRight);


        verticalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout->addWidget(frame_3);

        frame_2 = new QFrame(ClipEditForm);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_2 = new QLabel(frame_2);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        verticalLayout_3->addWidget(label_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        ClipLeds = new QRadioButton(frame_2);
        ClipLeds->setObjectName("ClipLeds");
        sizePolicy3.setHeightForWidth(ClipLeds->sizePolicy().hasHeightForWidth());
        ClipLeds->setSizePolicy(sizePolicy3);
        ClipLeds->setChecked(true);

        horizontalLayout_4->addWidget(ClipLeds);

        ClipReflective = new QRadioButton(frame_2);
        ClipReflective->setObjectName("ClipReflective");

        horizontalLayout_4->addWidget(ClipReflective);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(frame_2);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_5->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        retranslateUi(ClipEditForm);

        QMetaObject::connectSlotsByName(ClipEditForm);
    } // setupUi

    void retranslateUi(QWidget *ClipEditForm)
    {
        ClipEditForm->setWindowTitle(QCoreApplication::translate("ClipEditForm", "Form", nullptr));
        label_30->setText(QString());
        label->setText(QCoreApplication::translate("ClipEditForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Model Dimmensions</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("ClipEditForm", "A [mm]", nullptr));
        label_34->setText(QCoreApplication::translate("ClipEditForm", "B [mm]", nullptr));
        label_35->setText(QCoreApplication::translate("ClipEditForm", "C [mm]", nullptr));
        label_38->setText(QCoreApplication::translate("ClipEditForm", "D [mm]", nullptr));
        label_4->setText(QCoreApplication::translate("ClipEditForm", "On which side of your head the clip is?", nullptr));
        ClipLeft->setText(QCoreApplication::translate("ClipEditForm", "Left", nullptr));
        ClipRight->setText(QCoreApplication::translate("ClipEditForm", "Right", nullptr));
        label_2->setText(QCoreApplication::translate("ClipEditForm", "Model Type", nullptr));
        ClipLeds->setText(QCoreApplication::translate("ClipEditForm", "LEDs", nullptr));
        ClipReflective->setText(QCoreApplication::translate("ClipEditForm", "Reflective", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClipEditForm: public Ui_ClipEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIP_EDIT_H
