/********************************************************************************
** Form generated from reading UI file 'chsettings.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHSETTINGS_H
#define UI_CHSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AcceptSettings
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *FBString;
    QHBoxLayout *horizontalLayout;
    QPushButton *RevertButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *KeepButton;

    void setupUi(QWidget *AcceptSettings)
    {
        if (AcceptSettings->objectName().isEmpty())
            AcceptSettings->setObjectName("AcceptSettings");
        AcceptSettings->resize(386, 124);
        verticalLayout = new QVBoxLayout(AcceptSettings);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(AcceptSettings);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        FBString = new QLabel(AcceptSettings);
        FBString->setObjectName("FBString");

        verticalLayout->addWidget(FBString);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        RevertButton = new QPushButton(AcceptSettings);
        RevertButton->setObjectName("RevertButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RevertButton->sizePolicy().hasHeightForWidth());
        RevertButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(RevertButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        KeepButton = new QPushButton(AcceptSettings);
        KeepButton->setObjectName("KeepButton");
        sizePolicy.setHeightForWidth(KeepButton->sizePolicy().hasHeightForWidth());
        KeepButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(KeepButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AcceptSettings);

        QMetaObject::connectSlotsByName(AcceptSettings);
    } // setupUi

    void retranslateUi(QWidget *AcceptSettings)
    {
        AcceptSettings->setWindowTitle(QCoreApplication::translate("AcceptSettings", "Change settings?", nullptr));
        label->setText(QCoreApplication::translate("AcceptSettings", "Do you want to keep these settings?", nullptr));
        FBString->setText(QCoreApplication::translate("AcceptSettings", "Will automatically revert back in X seconds...", nullptr));
        RevertButton->setText(QCoreApplication::translate("AcceptSettings", "Revert back...", nullptr));
        KeepButton->setText(QCoreApplication::translate("AcceptSettings", "Keep...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AcceptSettings: public Ui_AcceptSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHSETTINGS_H
