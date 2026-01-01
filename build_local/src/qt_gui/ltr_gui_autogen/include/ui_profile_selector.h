/********************************************************************************
** Form generated from reading UI file 'profile_selector.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROFILE_SELECTOR_H
#define UI_PROFILE_SELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProfileSelectorForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_12;
    QComboBox *Profiles;
    QVBoxLayout *AxesSetup;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *NewProfile;
    QPushButton *ImportProfile;
    QPushButton *ExportProfile;
    QPushButton *CopyFromDefault;

    void setupUi(QWidget *ProfileSelectorForm)
    {
        if (ProfileSelectorForm->objectName().isEmpty())
            ProfileSelectorForm->setObjectName("ProfileSelectorForm");
        ProfileSelectorForm->resize(563, 124);
        verticalLayout_2 = new QVBoxLayout(ProfileSelectorForm);
        verticalLayout_2->setObjectName("verticalLayout_2");
        frame = new QFrame(ProfileSelectorForm);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        label_12 = new QLabel(frame);
        label_12->setObjectName("label_12");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);

        horizontalLayout_12->addWidget(label_12);

        Profiles = new QComboBox(frame);
        Profiles->setObjectName("Profiles");
        Profiles->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Profiles->sizePolicy().hasHeightForWidth());
        Profiles->setSizePolicy(sizePolicy1);
        Profiles->setEditable(false);

        horizontalLayout_12->addWidget(Profiles);


        verticalLayout->addLayout(horizontalLayout_12);

        AxesSetup = new QVBoxLayout();
        AxesSetup->setObjectName("AxesSetup");

        verticalLayout->addLayout(AxesSetup);


        verticalLayout_2->addWidget(frame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        NewProfile = new QPushButton(ProfileSelectorForm);
        NewProfile->setObjectName("NewProfile");

        horizontalLayout_9->addWidget(NewProfile);

        ImportProfile = new QPushButton(ProfileSelectorForm);
        ImportProfile->setObjectName("ImportProfile");

        horizontalLayout_9->addWidget(ImportProfile);

        ExportProfile = new QPushButton(ProfileSelectorForm);
        ExportProfile->setObjectName("ExportProfile");

        horizontalLayout_9->addWidget(ExportProfile);

        CopyFromDefault = new QPushButton(ProfileSelectorForm);
        CopyFromDefault->setObjectName("CopyFromDefault");

        horizontalLayout_9->addWidget(CopyFromDefault);


        verticalLayout_2->addLayout(horizontalLayout_9);


        retranslateUi(ProfileSelectorForm);

        QMetaObject::connectSlotsByName(ProfileSelectorForm);
    } // setupUi

    void retranslateUi(QWidget *ProfileSelectorForm)
    {
        ProfileSelectorForm->setWindowTitle(QCoreApplication::translate("ProfileSelectorForm", "Form", nullptr));
        label_12->setText(QCoreApplication::translate("ProfileSelectorForm", "Profile Name", nullptr));
        NewProfile->setText(QCoreApplication::translate("ProfileSelectorForm", "New Profile", nullptr));
        ImportProfile->setText(QCoreApplication::translate("ProfileSelectorForm", "Import Profile", nullptr));
        ExportProfile->setText(QCoreApplication::translate("ProfileSelectorForm", "Export Profile", nullptr));
        CopyFromDefault->setText(QCoreApplication::translate("ProfileSelectorForm", "Copy From Default Profile", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProfileSelectorForm: public Ui_ProfileSelectorForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROFILE_SELECTOR_H
