/********************************************************************************
** Form generated from reading UI file 'prefix_discovery_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFIX_DISCOVERY_DIALOG_H
#define UI_PREFIX_DISCOVERY_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PrefixDiscoveryDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *prefixList;
    QHBoxLayout *horizontalLayout;
    QPushButton *browseButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *selectButton;

    void setupUi(QDialog *PrefixDiscoveryDialog)
    {
        if (PrefixDiscoveryDialog->objectName().isEmpty())
            PrefixDiscoveryDialog->setObjectName("PrefixDiscoveryDialog");
        PrefixDiscoveryDialog->resize(500, 400);
        verticalLayout = new QVBoxLayout(PrefixDiscoveryDialog);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(PrefixDiscoveryDialog);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        prefixList = new QListWidget(PrefixDiscoveryDialog);
        prefixList->setObjectName("prefixList");

        verticalLayout->addWidget(prefixList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        browseButton = new QPushButton(PrefixDiscoveryDialog);
        browseButton->setObjectName("browseButton");

        horizontalLayout->addWidget(browseButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(PrefixDiscoveryDialog);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);

        selectButton = new QPushButton(PrefixDiscoveryDialog);
        selectButton->setObjectName("selectButton");

        horizontalLayout->addWidget(selectButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(PrefixDiscoveryDialog);

        selectButton->setDefault(true);


        QMetaObject::connectSlotsByName(PrefixDiscoveryDialog);
    } // setupUi

    void retranslateUi(QDialog *PrefixDiscoveryDialog)
    {
        PrefixDiscoveryDialog->setWindowTitle(QCoreApplication::translate("PrefixDiscoveryDialog", "Select Wine/Proton Prefix", nullptr));
        label->setText(QCoreApplication::translate("PrefixDiscoveryDialog", "Discovered Wine/Proton prefixes:", nullptr));
        browseButton->setText(QCoreApplication::translate("PrefixDiscoveryDialog", "Browse Manually...", nullptr));
        cancelButton->setText(QCoreApplication::translate("PrefixDiscoveryDialog", "Cancel", nullptr));
        selectButton->setText(QCoreApplication::translate("PrefixDiscoveryDialog", "Select", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrefixDiscoveryDialog: public Ui_PrefixDiscoveryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFIX_DISCOVERY_DIALOG_H
