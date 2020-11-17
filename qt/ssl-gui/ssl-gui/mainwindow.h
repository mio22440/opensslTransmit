#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>

#include "../../../include/sslConsole.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int setSigAndSlot(void);//call connect to set signals and slots

    int sendOrReceiveFile(void);

    int setValues(void);
    int setVisibility(void);
    int setComponentText(void);

    void setModeSender();
    void setModeReceiver();
    void getFilePath();


private:
    Ui::MainWindow *ui;

    int mode;//-1: default, 0: send, 1:receive
    char *ipSrv;
    char *port;
    char *filePath;
    char testFilePath[500];
    sslConsole sslCsl;


};
#endif // MAINWINDOW_H
