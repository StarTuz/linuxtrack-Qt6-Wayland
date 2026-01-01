/********************************************************************************
** Form generated from reading UI file 'scp_form.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCP_FORM_H
#define UI_SCP_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCPForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTabWidget *SCPTab;
    QWidget *SCPPitch1;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *SCPPitch;
    QWidget *SCPYaw1;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *SCPYaw;
    QWidget *SCPRoll1;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *SCPRoll;
    QWidget *SCPX1;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *SCPX;
    QWidget *SCPY1;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *SCPY;
    QWidget *SCPZ1;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *SCPZ;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *SCPCloseButton;

    void setupUi(QWidget *SCPForm)
    {
        if (SCPForm->objectName().isEmpty())
            SCPForm->setObjectName("SCPForm");
        SCPForm->resize(639, 520);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ltr/linuxtrack.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        SCPForm->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(SCPForm);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        SCPTab = new QTabWidget(SCPForm);
        SCPTab->setObjectName("SCPTab");
        SCPTab->setEnabled(true);
        SCPPitch1 = new QWidget();
        SCPPitch1->setObjectName("SCPPitch1");
        verticalLayout_3 = new QVBoxLayout(SCPPitch1);
        verticalLayout_3->setObjectName("verticalLayout_3");
        SCPPitch = new QVBoxLayout();
        SCPPitch->setObjectName("SCPPitch");

        verticalLayout_3->addLayout(SCPPitch);

        SCPTab->addTab(SCPPitch1, QString());
        SCPYaw1 = new QWidget();
        SCPYaw1->setObjectName("SCPYaw1");
        verticalLayout_4 = new QVBoxLayout(SCPYaw1);
        verticalLayout_4->setObjectName("verticalLayout_4");
        SCPYaw = new QVBoxLayout();
        SCPYaw->setObjectName("SCPYaw");

        verticalLayout_4->addLayout(SCPYaw);

        SCPTab->addTab(SCPYaw1, QString());
        SCPRoll1 = new QWidget();
        SCPRoll1->setObjectName("SCPRoll1");
        verticalLayout_5 = new QVBoxLayout(SCPRoll1);
        verticalLayout_5->setObjectName("verticalLayout_5");
        SCPRoll = new QVBoxLayout();
        SCPRoll->setObjectName("SCPRoll");

        verticalLayout_5->addLayout(SCPRoll);

        SCPTab->addTab(SCPRoll1, QString());
        SCPX1 = new QWidget();
        SCPX1->setObjectName("SCPX1");
        verticalLayout_6 = new QVBoxLayout(SCPX1);
        verticalLayout_6->setObjectName("verticalLayout_6");
        SCPX = new QVBoxLayout();
        SCPX->setObjectName("SCPX");

        verticalLayout_6->addLayout(SCPX);

        SCPTab->addTab(SCPX1, QString());
        SCPY1 = new QWidget();
        SCPY1->setObjectName("SCPY1");
        verticalLayout_7 = new QVBoxLayout(SCPY1);
        verticalLayout_7->setObjectName("verticalLayout_7");
        SCPY = new QVBoxLayout();
        SCPY->setObjectName("SCPY");

        verticalLayout_7->addLayout(SCPY);

        SCPTab->addTab(SCPY1, QString());
        SCPZ1 = new QWidget();
        SCPZ1->setObjectName("SCPZ1");
        verticalLayout_8 = new QVBoxLayout(SCPZ1);
        verticalLayout_8->setObjectName("verticalLayout_8");
        SCPZ = new QVBoxLayout();
        SCPZ->setObjectName("SCPZ");

        verticalLayout_8->addLayout(SCPZ);

        SCPTab->addTab(SCPZ1, QString());

        verticalLayout->addWidget(SCPTab);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        SCPCloseButton = new QPushButton(SCPForm);
        SCPCloseButton->setObjectName("SCPCloseButton");

        horizontalLayout->addWidget(SCPCloseButton);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(SCPForm);

        SCPTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SCPForm);
    } // setupUi

    void retranslateUi(QWidget *SCPForm)
    {
        SCPForm->setWindowTitle(QCoreApplication::translate("SCPForm", "Sensitivity curves setup", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPPitch1), QCoreApplication::translate("SCPForm", "Pitch", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPYaw1), QCoreApplication::translate("SCPForm", "Yaw", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPRoll1), QCoreApplication::translate("SCPForm", "Roll", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPX1), QCoreApplication::translate("SCPForm", "X Translation", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPY1), QCoreApplication::translate("SCPForm", "Y Translation", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPZ1), QCoreApplication::translate("SCPForm", "Z Translation", nullptr));
        SCPCloseButton->setText(QCoreApplication::translate("SCPForm", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCPForm: public Ui_SCPForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCP_FORM_H
