/********************************************************************************
** Form generated from reading UI file 'extractor.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTRACTOR_H
#define UI_EXTRACTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *FWCombo;
    QPushButton *DownloadButton;
    QLabel *label;
    QPlainTextEdit *LogView;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *HelpButton;
    QPushButton *AnalyzeSourceButton;
    QPushButton *BrowseInstaller;
    QPushButton *BrowseDir;
    QPushButton *QuitButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(588, 337);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName("verticalLayout");
        frame = new QFrame(Form);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        QFont font;
        font.setBold(true);
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        FWCombo = new QComboBox(frame);
        FWCombo->setObjectName("FWCombo");
        FWCombo->setEditable(true);

        horizontalLayout_2->addWidget(FWCombo);

        DownloadButton = new QPushButton(frame);
        DownloadButton->setObjectName("DownloadButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DownloadButton->sizePolicy().hasHeightForWidth());
        DownloadButton->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(DownloadButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        label = new QLabel(frame);
        label->setObjectName("label");
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        LogView = new QPlainTextEdit(frame);
        LogView->setObjectName("LogView");
        LogView->setLineWrapMode(QPlainTextEdit::NoWrap);
        LogView->setReadOnly(true);

        verticalLayout_2->addWidget(LogView);


        verticalLayout->addWidget(frame);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        HelpButton = new QPushButton(Form);
        HelpButton->setObjectName("HelpButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(HelpButton->sizePolicy().hasHeightForWidth());
        HelpButton->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(HelpButton);

        AnalyzeSourceButton = new QPushButton(Form);
        AnalyzeSourceButton->setObjectName("AnalyzeSourceButton");

        horizontalLayout_3->addWidget(AnalyzeSourceButton);

        BrowseInstaller = new QPushButton(Form);
        BrowseInstaller->setObjectName("BrowseInstaller");

        horizontalLayout_3->addWidget(BrowseInstaller);

        BrowseDir = new QPushButton(Form);
        BrowseDir->setObjectName("BrowseDir");

        horizontalLayout_3->addWidget(BrowseDir);

        QuitButton = new QPushButton(Form);
        QuitButton->setObjectName("QuitButton");
        sizePolicy1.setHeightForWidth(QuitButton->sizePolicy().hasHeightForWidth());
        QuitButton->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(QuitButton);


        verticalLayout->addLayout(horizontalLayout_3);

        QWidget::setTabOrder(FWCombo, DownloadButton);
        QWidget::setTabOrder(DownloadButton, LogView);
        QWidget::setTabOrder(LogView, HelpButton);
        QWidget::setTabOrder(HelpButton, AnalyzeSourceButton);
        QWidget::setTabOrder(AnalyzeSourceButton, BrowseInstaller);
        QWidget::setTabOrder(BrowseInstaller, QuitButton);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Extractor", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "Select Package to Download (any of them should do)", nullptr));
        DownloadButton->setText(QCoreApplication::translate("Form", "Download", nullptr));
        label->setText(QCoreApplication::translate("Form", "Progress information", nullptr));
        HelpButton->setText(QCoreApplication::translate("Form", "Help", nullptr));
        AnalyzeSourceButton->setText(QCoreApplication::translate("Form", "Extract Firmware\n"
"(developer)", nullptr));
        BrowseInstaller->setText(QCoreApplication::translate("Form", "Extract from\n"
"installer", nullptr));
        BrowseDir->setText(QCoreApplication::translate("Form", "Extract from\n"
"unpacked", nullptr));
        QuitButton->setText(QCoreApplication::translate("Form", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTRACTOR_H
