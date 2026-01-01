/********************************************************************************
** Form generated from reading UI file 'calibration.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATION_H
#define UI_CALIBRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Calibration
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *CalibrationText;
    QLabel *FBString;
    QHBoxLayout *horizontalLayout;
    QPushButton *CancelButton;

    void setupUi(QWidget *Calibration)
    {
        if (Calibration->objectName().isEmpty())
            Calibration->setObjectName("Calibration");
        Calibration->resize(382, 145);
        verticalLayout = new QVBoxLayout(Calibration);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(Calibration);
        label->setObjectName("label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(20);
        label->setFont(font);

        verticalLayout->addWidget(label);

        CalibrationText = new QLabel(Calibration);
        CalibrationText->setObjectName("CalibrationText");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(CalibrationText->sizePolicy().hasHeightForWidth());
        CalibrationText->setSizePolicy(sizePolicy1);
        CalibrationText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout->addWidget(CalibrationText);

        FBString = new QLabel(Calibration);
        FBString->setObjectName("FBString");

        verticalLayout->addWidget(FBString);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        CancelButton = new QPushButton(Calibration);
        CancelButton->setObjectName("CancelButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(CancelButton->sizePolicy().hasHeightForWidth());
        CancelButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(CancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Calibration);

        QMetaObject::connectSlotsByName(Calibration);
    } // setupUi

    void retranslateUi(QWidget *Calibration)
    {
        Calibration->setWindowTitle(QCoreApplication::translate("Calibration", "Calibration instructions", nullptr));
        label->setText(QCoreApplication::translate("Calibration", "Calibration instructions", nullptr));
        CalibrationText->setText(QCoreApplication::translate("Calibration", "TextLabel", nullptr));
        FBString->setText(QCoreApplication::translate("Calibration", "Center position will be confirmed in X seconds...", nullptr));
        CancelButton->setText(QCoreApplication::translate("Calibration", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Calibration: public Ui_Calibration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATION_H
