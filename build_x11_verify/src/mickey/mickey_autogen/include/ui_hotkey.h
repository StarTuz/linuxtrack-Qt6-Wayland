/********************************************************************************
** Form generated from reading UI file 'hotkey.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOTKEY_H
#define UI_HOTKEY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HotKeySetup
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *AssignButton;

    void setupUi(QWidget *HotKeySetup)
    {
        if (HotKeySetup->objectName().isEmpty())
            HotKeySetup->setObjectName("HotKeySetup");
        HotKeySetup->resize(331, 45);
        horizontalLayout = new QHBoxLayout(HotKeySetup);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 2, -1, 2);
        AssignButton = new QPushButton(HotKeySetup);
        AssignButton->setObjectName("AssignButton");

        horizontalLayout->addWidget(AssignButton);


        retranslateUi(HotKeySetup);

        QMetaObject::connectSlotsByName(HotKeySetup);
    } // setupUi

    void retranslateUi(QWidget *HotKeySetup)
    {
        HotKeySetup->setWindowTitle(QCoreApplication::translate("HotKeySetup", "Form", nullptr));
        AssignButton->setText(QCoreApplication::translate("HotKeySetup", "Assign", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HotKeySetup: public Ui_HotKeySetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOTKEY_H
