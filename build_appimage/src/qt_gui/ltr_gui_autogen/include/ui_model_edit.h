/********************************************************************************
** Form generated from reading UI file 'model_edit.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODEL_EDIT_H
#define UI_MODEL_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelEditForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_23;
    QComboBox *ModelSelector;
    QPushButton *CreateModelButton;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *ModelEditorSite;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *ModelPreview;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ModelEditForm)
    {
        if (ModelEditForm->objectName().isEmpty())
            ModelEditForm->setObjectName("ModelEditForm");
        ModelEditForm->resize(617, 322);
        verticalLayout_2 = new QVBoxLayout(ModelEditForm);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_23 = new QLabel(ModelEditForm);
        label_23->setObjectName("label_23");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        label_23->setFont(font);

        horizontalLayout_2->addWidget(label_23);

        ModelSelector = new QComboBox(ModelEditForm);
        ModelSelector->setObjectName("ModelSelector");

        horizontalLayout_2->addWidget(ModelSelector);

        CreateModelButton = new QPushButton(ModelEditForm);
        CreateModelButton->setObjectName("CreateModelButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(CreateModelButton->sizePolicy().hasHeightForWidth());
        CreateModelButton->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(CreateModelButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        ModelEditorSite = new QVBoxLayout();
        ModelEditorSite->setObjectName("ModelEditorSite");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        ModelEditorSite->addItem(horizontalSpacer);


        horizontalLayout->addLayout(ModelEditorSite);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        ModelPreview = new QLabel(ModelEditForm);
        ModelPreview->setObjectName("ModelPreview");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ModelPreview->sizePolicy().hasHeightForWidth());
        ModelPreview->setSizePolicy(sizePolicy2);
        ModelPreview->setMinimumSize(QSize(150, 150));
        ModelPreview->setMaximumSize(QSize(150, 150));
        ModelPreview->setPixmap(QPixmap(QString::fromUtf8(":/ltr/clip_1.png")));
        ModelPreview->setScaledContents(true);

        verticalLayout->addWidget(ModelPreview);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(ModelEditForm);

        QMetaObject::connectSlotsByName(ModelEditForm);
    } // setupUi

    void retranslateUi(QWidget *ModelEditForm)
    {
        ModelEditForm->setWindowTitle(QCoreApplication::translate("ModelEditForm", "Form", nullptr));
        label_23->setText(QCoreApplication::translate("ModelEditForm", "Model Name:", nullptr));
        CreateModelButton->setText(QCoreApplication::translate("ModelEditForm", "Create New", nullptr));
        ModelPreview->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ModelEditForm: public Ui_ModelEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEL_EDIT_H
