/********************************************************************************
** Form generated from reading UI file 'joy_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOY_SETUP_H
#define UI_JOY_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JoySetup
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout_3;
    QLabel *label;
    QComboBox *PitchCombo;
    QLabel *label_2;
    QComboBox *YawCombo;
    QLabel *label_3;
    QComboBox *RollCombo;
    QFormLayout *formLayout_2;
    QLabel *label_4;
    QComboBox *TXCombo;
    QLabel *label_5;
    QComboBox *TYCombo;
    QLabel *label_6;
    QComboBox *TZCombo;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QRadioButton *EvdevButton;
    QRadioButton *JsButton;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_7;
    QSpinBox *PPSFreq;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *JoySetup)
    {
        if (JoySetup->objectName().isEmpty())
            JoySetup->setObjectName("JoySetup");
        JoySetup->resize(361, 285);
        verticalLayout = new QVBoxLayout(JoySetup);
        verticalLayout->setObjectName("verticalLayout");
        groupBox_2 = new QGroupBox(JoySetup);
        groupBox_2->setObjectName("groupBox_2");
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName("formLayout_3");
        label = new QLabel(groupBox_2);
        label->setObjectName("label");

        formLayout_3->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        PitchCombo = new QComboBox(groupBox_2);
        PitchCombo->addItem(QString());
        PitchCombo->setObjectName("PitchCombo");

        formLayout_3->setWidget(0, QFormLayout::ItemRole::FieldRole, PitchCombo);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        YawCombo = new QComboBox(groupBox_2);
        YawCombo->addItem(QString());
        YawCombo->setObjectName("YawCombo");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::FieldRole, YawCombo);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");

        formLayout_3->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        RollCombo = new QComboBox(groupBox_2);
        RollCombo->addItem(QString());
        RollCombo->setObjectName("RollCombo");

        formLayout_3->setWidget(2, QFormLayout::ItemRole::FieldRole, RollCombo);


        horizontalLayout_2->addLayout(formLayout_3);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName("formLayout_2");
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, label_4);

        TXCombo = new QComboBox(groupBox_2);
        TXCombo->addItem(QString());
        TXCombo->setObjectName("TXCombo");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, TXCombo);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName("label_5");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::LabelRole, label_5);

        TYCombo = new QComboBox(groupBox_2);
        TYCombo->addItem(QString());
        TYCombo->setObjectName("TYCombo");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::FieldRole, TYCombo);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName("label_6");

        formLayout_2->setWidget(2, QFormLayout::ItemRole::LabelRole, label_6);

        TZCombo = new QComboBox(groupBox_2);
        TZCombo->addItem(QString());
        TZCombo->setObjectName("TZCombo");

        formLayout_2->setWidget(2, QFormLayout::ItemRole::FieldRole, TZCombo);


        horizontalLayout_2->addLayout(formLayout_2);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(JoySetup);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        EvdevButton = new QRadioButton(groupBox);
        EvdevButton->setObjectName("EvdevButton");
        EvdevButton->setChecked(true);

        horizontalLayout->addWidget(EvdevButton);

        JsButton = new QRadioButton(groupBox);
        JsButton->setObjectName("JsButton");

        horizontalLayout->addWidget(JsButton);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(JoySetup);
        groupBox_3->setObjectName("groupBox_3");
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName("label_7");

        horizontalLayout_4->addWidget(label_7);

        PPSFreq = new QSpinBox(groupBox_3);
        PPSFreq->setObjectName("PPSFreq");
        PPSFreq->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PPSFreq->setMinimum(1);
        PPSFreq->setMaximum(120);
        PPSFreq->setValue(60);

        horizontalLayout_4->addWidget(PPSFreq);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        horizontalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        QWidget::setTabOrder(PitchCombo, YawCombo);
        QWidget::setTabOrder(YawCombo, RollCombo);
        QWidget::setTabOrder(RollCombo, TXCombo);
        QWidget::setTabOrder(TXCombo, TYCombo);
        QWidget::setTabOrder(TYCombo, TZCombo);
        QWidget::setTabOrder(TZCombo, EvdevButton);
        QWidget::setTabOrder(EvdevButton, JsButton);

        retranslateUi(JoySetup);

        QMetaObject::connectSlotsByName(JoySetup);
    } // setupUi

    void retranslateUi(QWidget *JoySetup)
    {
        JoySetup->setWindowTitle(QCoreApplication::translate("JoySetup", "Form", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("JoySetup", "Axes mapping", nullptr));
        label->setText(QCoreApplication::translate("JoySetup", "Pitch", nullptr));
        PitchCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_2->setText(QCoreApplication::translate("JoySetup", "Yaw", nullptr));
        YawCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_3->setText(QCoreApplication::translate("JoySetup", "Roll", nullptr));
        RollCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_4->setText(QCoreApplication::translate("JoySetup", "X translation", nullptr));
        TXCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_5->setText(QCoreApplication::translate("JoySetup", "Y translation", nullptr));
        TYCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_6->setText(QCoreApplication::translate("JoySetup", "Z translation", nullptr));
        TZCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        groupBox->setTitle(QCoreApplication::translate("JoySetup", "Select interface", nullptr));
        EvdevButton->setText(QCoreApplication::translate("JoySetup", "Evdev", nullptr));
        JsButton->setText(QCoreApplication::translate("JoySetup", "Joystick", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("JoySetup", "Misc", nullptr));
        label_7->setText(QCoreApplication::translate("JoySetup", "Polling frequency", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JoySetup: public Ui_JoySetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOY_SETUP_H
