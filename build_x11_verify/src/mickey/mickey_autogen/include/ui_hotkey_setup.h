/********************************************************************************
** Form generated from reading UI file 'hotkey_setup.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOTKEY_SETUP_H
#define UI_HOTKEY_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "my_line_edit.h"

QT_BEGIN_NAMESPACE

class Ui_HotKeySetupDlg
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *mainLayout;
    QLabel *label;
    myLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *HotKeySetupDlg)
    {
        if (HotKeySetupDlg->objectName().isEmpty())
            HotKeySetupDlg->setObjectName("HotKeySetupDlg");
        HotKeySetupDlg->setWindowModality(Qt::ApplicationModal);
        HotKeySetupDlg->resize(194, 103);
        verticalLayout = new QVBoxLayout(HotKeySetupDlg);
        verticalLayout->setObjectName("verticalLayout");
        mainLayout = new QVBoxLayout();
        mainLayout->setObjectName("mainLayout");
        label = new QLabel(HotKeySetupDlg);
        label->setObjectName("label");

        mainLayout->addWidget(label);

        lineEdit = new myLineEdit(HotKeySetupDlg);
        lineEdit->setObjectName("lineEdit");

        mainLayout->addWidget(lineEdit);


        verticalLayout->addLayout(mainLayout);

        buttonBox = new QDialogButtonBox(HotKeySetupDlg);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(HotKeySetupDlg);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, HotKeySetupDlg, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, HotKeySetupDlg, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(HotKeySetupDlg);
    } // setupUi

    void retranslateUi(QDialog *HotKeySetupDlg)
    {
        HotKeySetupDlg->setWindowTitle(QCoreApplication::translate("HotKeySetupDlg", "Set HotKey", nullptr));
        label->setText(QCoreApplication::translate("HotKeySetupDlg", "Press the desired hotkey:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HotKeySetupDlg: public Ui_HotKeySetupDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOTKEY_SETUP_H
