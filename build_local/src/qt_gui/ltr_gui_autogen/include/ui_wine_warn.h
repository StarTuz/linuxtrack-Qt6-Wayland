/********************************************************************************
** Form generated from reading UI file 'wine_warn.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINE_WARN_H
#define UI_WINE_WARN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QCheckBox *DontShow;
    QSpacerItem *horizontalSpacer;
    QPushButton *OKButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->setWindowModality(Qt::ApplicationModal);
        Dialog->resize(400, 103);
        verticalLayout = new QVBoxLayout(Dialog);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(Dialog);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        DontShow = new QCheckBox(Dialog);
        DontShow->setObjectName("DontShow");

        horizontalLayout->addWidget(DontShow);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        OKButton = new QPushButton(Dialog);
        OKButton->setObjectName("OKButton");
        OKButton->setAutoDefault(true);

        horizontalLayout->addWidget(OKButton);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(OKButton, DontShow);

        retranslateUi(Dialog);

        OKButton->setDefault(true);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Wine not available", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Wine is not available.\n"
"You'll not be able to install NP firmware and Wine plugin!", nullptr));
        DontShow->setText(QCoreApplication::translate("Dialog", "Don't show this warning again.", nullptr));
        OKButton->setText(QCoreApplication::translate("Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINE_WARN_H
