/********************************************************************************
** Form generated from reading UI file 'udp_settings.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UDP_SETTINGS_H
#define UI_UDP_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QKeySequenceEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UdpSettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *connectionGroup;
    QGridLayout *gridLayout;
    QLabel *ipLabel;
    QLineEdit *ipEdit;
    QLabel *portLabel;
    QSpinBox *portSpinBox;
    QLabel *protocolLabel;
    QComboBox *protocolCombo;
    QCheckBox *autoStartCheck;
    QGroupBox *controlsGroup;
    QVBoxLayout *controlsVLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *startStopButton;
    QLabel *statusLabel;
    QHBoxLayout *buttonLayout;
    QPushButton *recenterButton;
    QPushButton *pauseButton;
    QPushButton *installWineButton;
    QGroupBox *hotkeysGroup;
    QGridLayout *hotkeysLayout;
    QCheckBox *enableHotkeysCheck;
    QLabel *recenterKeyLabel;
    QKeySequenceEdit *recenterKeyEdit;
    QLabel *pauseKeyLabel;
    QKeySequenceEdit *pauseKeyEdit;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UdpSettingsDialog)
    {
        if (UdpSettingsDialog->objectName().isEmpty())
            UdpSettingsDialog->setObjectName(QString::fromUtf8("UdpSettingsDialog"));
        UdpSettingsDialog->resize(400, 280);
        verticalLayout = new QVBoxLayout(UdpSettingsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        connectionGroup = new QGroupBox(UdpSettingsDialog);
        connectionGroup->setObjectName(QString::fromUtf8("connectionGroup"));
        gridLayout = new QGridLayout(connectionGroup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ipLabel = new QLabel(connectionGroup);
        ipLabel->setObjectName(QString::fromUtf8("ipLabel"));

        gridLayout->addWidget(ipLabel, 0, 0, 1, 1);

        ipEdit = new QLineEdit(connectionGroup);
        ipEdit->setObjectName(QString::fromUtf8("ipEdit"));

        gridLayout->addWidget(ipEdit, 0, 1, 1, 1);

        portLabel = new QLabel(connectionGroup);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));

        gridLayout->addWidget(portLabel, 1, 0, 1, 1);

        portSpinBox = new QSpinBox(connectionGroup);
        portSpinBox->setObjectName(QString::fromUtf8("portSpinBox"));
        portSpinBox->setMinimum(1024);
        portSpinBox->setMaximum(65535);
        portSpinBox->setValue(4242);

        gridLayout->addWidget(portSpinBox, 1, 1, 1, 1);

        protocolLabel = new QLabel(connectionGroup);
        protocolLabel->setObjectName(QString::fromUtf8("protocolLabel"));

        gridLayout->addWidget(protocolLabel, 2, 0, 1, 1);

        protocolCombo = new QComboBox(connectionGroup);
        protocolCombo->addItem(QString());
        protocolCombo->addItem(QString());
        protocolCombo->setObjectName(QString::fromUtf8("protocolCombo"));

        gridLayout->addWidget(protocolCombo, 2, 1, 1, 1);

        autoStartCheck = new QCheckBox(connectionGroup);
        autoStartCheck->setObjectName(QString::fromUtf8("autoStartCheck"));
        autoStartCheck->setChecked(true);

        gridLayout->addWidget(autoStartCheck, 3, 0, 1, 2);


        verticalLayout->addWidget(connectionGroup);

        controlsGroup = new QGroupBox(UdpSettingsDialog);
        controlsGroup->setObjectName(QString::fromUtf8("controlsGroup"));
        controlsVLayout = new QVBoxLayout(controlsGroup);
        controlsVLayout->setObjectName(QString::fromUtf8("controlsVLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        startStopButton = new QPushButton(controlsGroup);
        startStopButton->setObjectName(QString::fromUtf8("startStopButton"));

        horizontalLayout->addWidget(startStopButton);

        statusLabel = new QLabel(controlsGroup);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        horizontalLayout->addWidget(statusLabel);


        controlsVLayout->addLayout(horizontalLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        recenterButton = new QPushButton(controlsGroup);
        recenterButton->setObjectName(QString::fromUtf8("recenterButton"));

        buttonLayout->addWidget(recenterButton);

        pauseButton = new QPushButton(controlsGroup);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));

        buttonLayout->addWidget(pauseButton);


        controlsVLayout->addLayout(buttonLayout);


        verticalLayout->addWidget(controlsGroup);

        installWineButton = new QPushButton(UdpSettingsDialog);
        installWineButton->setObjectName(QString::fromUtf8("installWineButton"));

        verticalLayout->addWidget(installWineButton);

        hotkeysGroup = new QGroupBox(UdpSettingsDialog);
        hotkeysGroup->setObjectName(QString::fromUtf8("hotkeysGroup"));
        hotkeysLayout = new QGridLayout(hotkeysGroup);
        hotkeysLayout->setObjectName(QString::fromUtf8("hotkeysLayout"));
        enableHotkeysCheck = new QCheckBox(hotkeysGroup);
        enableHotkeysCheck->setObjectName(QString::fromUtf8("enableHotkeysCheck"));

        hotkeysLayout->addWidget(enableHotkeysCheck, 0, 0, 1, 2);

        recenterKeyLabel = new QLabel(hotkeysGroup);
        recenterKeyLabel->setObjectName(QString::fromUtf8("recenterKeyLabel"));

        hotkeysLayout->addWidget(recenterKeyLabel, 1, 0, 1, 1);

        recenterKeyEdit = new QKeySequenceEdit(hotkeysGroup);
        recenterKeyEdit->setObjectName(QString::fromUtf8("recenterKeyEdit"));

        hotkeysLayout->addWidget(recenterKeyEdit, 1, 1, 1, 1);

        pauseKeyLabel = new QLabel(hotkeysGroup);
        pauseKeyLabel->setObjectName(QString::fromUtf8("pauseKeyLabel"));

        hotkeysLayout->addWidget(pauseKeyLabel, 2, 0, 1, 1);

        pauseKeyEdit = new QKeySequenceEdit(hotkeysGroup);
        pauseKeyEdit->setObjectName(QString::fromUtf8("pauseKeyEdit"));

        hotkeysLayout->addWidget(pauseKeyEdit, 2, 1, 1, 1);


        verticalLayout->addWidget(hotkeysGroup);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(UdpSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(UdpSettingsDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, UdpSettingsDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, UdpSettingsDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(UdpSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *UdpSettingsDialog)
    {
        UdpSettingsDialog->setWindowTitle(QCoreApplication::translate("UdpSettingsDialog", "UDP Bridge Settings", nullptr));
        connectionGroup->setTitle(QCoreApplication::translate("UdpSettingsDialog", "Connection", nullptr));
        ipLabel->setText(QCoreApplication::translate("UdpSettingsDialog", "Target IP:", nullptr));
        ipEdit->setText(QCoreApplication::translate("UdpSettingsDialog", "127.0.0.1", nullptr));
        portLabel->setText(QCoreApplication::translate("UdpSettingsDialog", "Port:", nullptr));
        protocolLabel->setText(QCoreApplication::translate("UdpSettingsDialog", "Protocol:", nullptr));
        protocolCombo->setItemText(0, QCoreApplication::translate("UdpSettingsDialog", "OpenTrack (6 doubles)", nullptr));
        protocolCombo->setItemText(1, QCoreApplication::translate("UdpSettingsDialog", "FreeTrack", nullptr));

        autoStartCheck->setText(QCoreApplication::translate("UdpSettingsDialog", "Auto-start when tracking begins", nullptr));
        controlsGroup->setTitle(QCoreApplication::translate("UdpSettingsDialog", "Manual Controls", nullptr));
        startStopButton->setText(QCoreApplication::translate("UdpSettingsDialog", "Start UDP Stack", nullptr));
        statusLabel->setText(QCoreApplication::translate("UdpSettingsDialog", "Status: Stopped", nullptr));
        recenterButton->setText(QCoreApplication::translate("UdpSettingsDialog", "Recenter", nullptr));
        pauseButton->setText(QCoreApplication::translate("UdpSettingsDialog", "Pause/Resume", nullptr));
        installWineButton->setText(QCoreApplication::translate("UdpSettingsDialog", "Install UDP Bridge (Wine/Proton)...", nullptr));
        hotkeysGroup->setTitle(QCoreApplication::translate("UdpSettingsDialog", "Hotkeys (Wine/Proton)", nullptr));
        enableHotkeysCheck->setText(QCoreApplication::translate("UdpSettingsDialog", "Enable hotkey utility", nullptr));
        recenterKeyLabel->setText(QCoreApplication::translate("UdpSettingsDialog", "Recenter key:", nullptr));
        recenterKeyEdit->setKeySequence(QCoreApplication::translate("UdpSettingsDialog", "F12", nullptr));
        pauseKeyLabel->setText(QCoreApplication::translate("UdpSettingsDialog", "Pause key:", nullptr));
        pauseKeyEdit->setKeySequence(QCoreApplication::translate("UdpSettingsDialog", "Pause", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UdpSettingsDialog: public Ui_UdpSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UDP_SETTINGS_H
