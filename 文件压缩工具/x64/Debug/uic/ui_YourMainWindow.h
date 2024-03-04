/********************************************************************************
** Form generated from reading UI file 'YourMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YOURMAINWINDOW_H
#define UI_YOURMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_YourMainWindowClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QTextEdit *textEdit1;
    QPushButton *selectFileButton;
    QLineEdit *lineEditFilePath;
    QLineEdit *lineEditOutput;
    QPushButton *compressButton;
    QPushButton *decompressButton;
    QTextEdit *dialog;
    QPushButton *showEncodingTableButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *YourMainWindowClass)
    {
        if (YourMainWindowClass->objectName().isEmpty())
            YourMainWindowClass->setObjectName(QString::fromUtf8("YourMainWindowClass"));
        YourMainWindowClass->resize(1276, 919);
        centralWidget = new QWidget(YourMainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(440, 50, 401, 41));
        QFont font;
        font.setPointSize(20);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(false);
        textEdit1 = new QTextEdit(centralWidget);
        textEdit1->setObjectName(QString::fromUtf8("textEdit1"));
        textEdit1->setGeometry(QRect(160, 320, 931, 201));
        selectFileButton = new QPushButton(centralWidget);
        selectFileButton->setObjectName(QString::fromUtf8("selectFileButton"));
        selectFileButton->setGeometry(QRect(160, 550, 471, 61));
        QFont font1;
        font1.setPointSize(15);
        selectFileButton->setFont(font1);
        lineEditFilePath = new QLineEdit(centralWidget);
        lineEditFilePath->setObjectName(QString::fromUtf8("lineEditFilePath"));
        lineEditFilePath->setGeometry(QRect(660, 550, 431, 61));
        lineEditOutput = new QLineEdit(centralWidget);
        lineEditOutput->setObjectName(QString::fromUtf8("lineEditOutput"));
        lineEditOutput->setGeometry(QRect(720, 640, 371, 61));
        compressButton = new QPushButton(centralWidget);
        compressButton->setObjectName(QString::fromUtf8("compressButton"));
        compressButton->setGeometry(QRect(160, 640, 251, 61));
        compressButton->setFont(font1);
        decompressButton = new QPushButton(centralWidget);
        decompressButton->setObjectName(QString::fromUtf8("decompressButton"));
        decompressButton->setGeometry(QRect(440, 640, 241, 61));
        decompressButton->setFont(font1);
        dialog = new QTextEdit(centralWidget);
        dialog->setObjectName(QString::fromUtf8("dialog"));
        dialog->setGeometry(QRect(160, 140, 931, 141));
        showEncodingTableButton = new QPushButton(centralWidget);
        showEncodingTableButton->setObjectName(QString::fromUtf8("showEncodingTableButton"));
        showEncodingTableButton->setGeometry(QRect(160, 730, 941, 61));
        showEncodingTableButton->setFont(font1);
        YourMainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(YourMainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1276, 22));
        YourMainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(YourMainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        YourMainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(YourMainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        YourMainWindowClass->setStatusBar(statusBar);

        retranslateUi(YourMainWindowClass);

        QMetaObject::connectSlotsByName(YourMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *YourMainWindowClass)
    {
        YourMainWindowClass->setWindowTitle(QApplication::translate("YourMainWindowClass", "YourMainWindow", nullptr));
        label->setText(QApplication::translate("YourMainWindowClass", "\346\226\207\344\273\266\345\216\213\347\274\251\345\267\245\345\205\267", nullptr));
        selectFileButton->setText(QApplication::translate("YourMainWindowClass", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        compressButton->setText(QApplication::translate("YourMainWindowClass", "\345\216\213\347\274\251", nullptr));
        decompressButton->setText(QApplication::translate("YourMainWindowClass", "\350\247\243\345\216\213", nullptr));
        showEncodingTableButton->setText(QApplication::translate("YourMainWindowClass", "\346\237\245\347\234\213\347\274\226\347\240\201\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class YourMainWindowClass: public Ui_YourMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YOURMAINWINDOW_H
