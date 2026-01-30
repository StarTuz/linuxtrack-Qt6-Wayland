/********************************************************************************
** Form generated from reading UI file 'ltr_gui.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LTR_GUI_H
#define UI_LTR_GUI_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ltr_gui
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *pix_box;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *ogl_box;
    QHBoxLayout *horizontalLayout;
    QPushButton *startButton;
    QPushButton *recenterButton;
    QPushButton *pauseButton;
    QPushButton *wakeButton;
    QPushButton *stopButton;
    QLabel *status;

    void setupUi(QWidget *Ltr_gui)
    {
        if (Ltr_gui->objectName().isEmpty())
            Ltr_gui->setObjectName("Ltr_gui");
        Ltr_gui->resize(695, 509);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ltr/linuxtrack.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Ltr_gui->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(Ltr_gui);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(Ltr_gui);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        pix_box = new QVBoxLayout();
        pix_box->setObjectName("pix_box");

        verticalLayout_3->addLayout(pix_box);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        ogl_box = new QVBoxLayout();
        ogl_box->setObjectName("ogl_box");

        verticalLayout_5->addLayout(ogl_box);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        startButton = new QPushButton(Ltr_gui);
        startButton->setObjectName("startButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startButton->sizePolicy().hasHeightForWidth());
        startButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(startButton);

        recenterButton = new QPushButton(Ltr_gui);
        recenterButton->setObjectName("recenterButton");

        horizontalLayout->addWidget(recenterButton);

        pauseButton = new QPushButton(Ltr_gui);
        pauseButton->setObjectName("pauseButton");
        sizePolicy.setHeightForWidth(pauseButton->sizePolicy().hasHeightForWidth());
        pauseButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pauseButton);

        wakeButton = new QPushButton(Ltr_gui);
        wakeButton->setObjectName("wakeButton");
        sizePolicy.setHeightForWidth(wakeButton->sizePolicy().hasHeightForWidth());
        wakeButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(wakeButton);

        stopButton = new QPushButton(Ltr_gui);
        stopButton->setObjectName("stopButton");
        sizePolicy.setHeightForWidth(stopButton->sizePolicy().hasHeightForWidth());
        stopButton->setSizePolicy(sizePolicy);
        stopButton->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(stopButton);


        verticalLayout->addLayout(horizontalLayout);

        status = new QLabel(Ltr_gui);
        status->setObjectName("status");
        status->setFrameShape(QFrame::StyledPanel);
        status->setFrameShadow(QFrame::Plain);

        verticalLayout->addWidget(status);


        retranslateUi(Ltr_gui);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Ltr_gui);
    } // setupUi

    void retranslateUi(QWidget *Ltr_gui)
    {
        Ltr_gui->setWindowTitle(QCoreApplication::translate("Ltr_gui", "Tracking window", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Ltr_gui", "Camera View", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Ltr_gui", "3D View", nullptr));
        startButton->setText(QCoreApplication::translate("Ltr_gui", "Start", nullptr));
        recenterButton->setText(QCoreApplication::translate("Ltr_gui", "Recenter", nullptr));
        pauseButton->setText(QCoreApplication::translate("Ltr_gui", "Pause", nullptr));
        wakeButton->setText(QCoreApplication::translate("Ltr_gui", "Resume", nullptr));
        stopButton->setText(QCoreApplication::translate("Ltr_gui", "Stop", nullptr));
        status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Ltr_gui: public Ui_Ltr_gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LTR_GUI_H
