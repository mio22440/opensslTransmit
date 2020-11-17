/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSend_file;
    QAction *actionReceive_file;
    QAction *actionport;
    QWidget *centralwidget;
    QPushButton *pb_choose_file;
    QTextEdit *te_file_path;
    QTextEdit *te_srv_ip;
    QLabel *lbl_tips_srv_ip;
    QLabel *lbl_tips_file_path;
    QPushButton *pb_apply;
    QGraphicsView *gv_logo;
    QLabel *lbl_welcome;
    QLabel *lbl_error_setting;
    QLabel *lbl_tips_port;
    QTextEdit *te_port;
    QMenuBar *mb_choose_mode;
    QMenu *menumode;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(581, 472);
        actionSend_file = new QAction(MainWindow);
        actionSend_file->setObjectName(QString::fromUtf8("actionSend_file"));
        actionReceive_file = new QAction(MainWindow);
        actionReceive_file->setObjectName(QString::fromUtf8("actionReceive_file"));
        actionport = new QAction(MainWindow);
        actionport->setObjectName(QString::fromUtf8("actionport"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pb_choose_file = new QPushButton(centralwidget);
        pb_choose_file->setObjectName(QString::fromUtf8("pb_choose_file"));
        pb_choose_file->setGeometry(QRect(30, 180, 89, 25));
        te_file_path = new QTextEdit(centralwidget);
        te_file_path->setObjectName(QString::fromUtf8("te_file_path"));
        te_file_path->setGeometry(QRect(140, 180, 331, 61));
        te_srv_ip = new QTextEdit(centralwidget);
        te_srv_ip->setObjectName(QString::fromUtf8("te_srv_ip"));
        te_srv_ip->setGeometry(QRect(30, 290, 231, 31));
        lbl_tips_srv_ip = new QLabel(centralwidget);
        lbl_tips_srv_ip->setObjectName(QString::fromUtf8("lbl_tips_srv_ip"));
        lbl_tips_srv_ip->setGeometry(QRect(30, 250, 151, 31));
        lbl_tips_file_path = new QLabel(centralwidget);
        lbl_tips_file_path->setObjectName(QString::fromUtf8("lbl_tips_file_path"));
        lbl_tips_file_path->setGeometry(QRect(30, 140, 161, 16));
        pb_apply = new QPushButton(centralwidget);
        pb_apply->setObjectName(QString::fromUtf8("pb_apply"));
        pb_apply->setGeometry(QRect(210, 380, 151, 31));
        gv_logo = new QGraphicsView(centralwidget);
        gv_logo->setObjectName(QString::fromUtf8("gv_logo"));
        gv_logo->setGeometry(QRect(160, 20, 271, 81));
        lbl_welcome = new QLabel(centralwidget);
        lbl_welcome->setObjectName(QString::fromUtf8("lbl_welcome"));
        lbl_welcome->setGeometry(QRect(260, 130, 81, 16));
        lbl_error_setting = new QLabel(centralwidget);
        lbl_error_setting->setObjectName(QString::fromUtf8("lbl_error_setting"));
        lbl_error_setting->setGeometry(QRect(120, 320, 371, 51));
        lbl_tips_port = new QLabel(centralwidget);
        lbl_tips_port->setObjectName(QString::fromUtf8("lbl_tips_port"));
        lbl_tips_port->setGeometry(QRect(310, 250, 67, 31));
        te_port = new QTextEdit(centralwidget);
        te_port->setObjectName(QString::fromUtf8("te_port"));
        te_port->setGeometry(QRect(370, 290, 101, 31));
        MainWindow->setCentralWidget(centralwidget);
        mb_choose_mode = new QMenuBar(MainWindow);
        mb_choose_mode->setObjectName(QString::fromUtf8("mb_choose_mode"));
        mb_choose_mode->setGeometry(QRect(0, 0, 581, 22));
        mb_choose_mode->setDefaultUp(false);
        mb_choose_mode->setNativeMenuBar(true);
        menumode = new QMenu(mb_choose_mode);
        menumode->setObjectName(QString::fromUtf8("menumode"));
        MainWindow->setMenuBar(mb_choose_mode);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        mb_choose_mode->addAction(menumode->menuAction());
        menumode->addAction(actionSend_file);
        menumode->addSeparator();
        menumode->addAction(actionReceive_file);
        menumode->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSend_file->setText(QCoreApplication::translate("MainWindow", "Send file", nullptr));
        actionReceive_file->setText(QCoreApplication::translate("MainWindow", "Receive file", nullptr));
        actionport->setText(QCoreApplication::translate("MainWindow", "port", nullptr));
        pb_choose_file->setText(QCoreApplication::translate("MainWindow", "choose file", nullptr));
        lbl_tips_srv_ip->setText(QCoreApplication::translate("MainWindow", "ip address of recever:", nullptr));
        lbl_tips_file_path->setText(QCoreApplication::translate("MainWindow", "file path to send:", nullptr));
        pb_apply->setText(QCoreApplication::translate("MainWindow", "send", nullptr));
        lbl_welcome->setText(QCoreApplication::translate("MainWindow", "welcome !", nullptr));
        lbl_error_setting->setText(QString());
        lbl_tips_port->setText(QCoreApplication::translate("MainWindow", "port:", nullptr));
        menumode->setTitle(QCoreApplication::translate("MainWindow", "mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
