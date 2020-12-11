#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set defaultvalue
    this->setValues();
    this->setVisibility();
    this->setComponentText();
    this->setSigAndSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
    //free(this->filePath);
}

//my functions
int MainWindow::setSigAndSlot(void){
    connect(this->ui->actionSend_file, &QAction::triggered, this, &MainWindow::setModeSender);
    connect(this->ui->actionReceive_file, &QAction::triggered, this, &MainWindow::setModeReceiver);

    connect(this->ui->pb_choose_file, &QPushButton::released, this, &MainWindow::getFilePath);

    connect(this->ui->pb_apply, &QPushButton::released, this, &MainWindow::sendOrReceiveFile);
    return 1;
}

int MainWindow::sendOrReceiveFile(void){
    //local var
    int ret;
    QString tmpQs;
    QByteArray tmpQba;
    this->ui->lbl_error_setting->setText("");

    //get arg from edit text
    tmpQs = this->ui->te_file_path->toPlainText();
    tmpQba = tmpQs.toLatin1();
    this->filePath = tmpQba.data();
    if(this->ui->te_file_path->document()->isEmpty()){
        this->filePath = NULL;
    }
    qDebug() << this->filePath << endl;

    tmpQs = this->ui->te_srv_ip->toPlainText();
    tmpQba = tmpQs.toLatin1();
    this->ipSrv = tmpQba.data();

    tmpQs = this->ui->te_port->toPlainText();
    tmpQba = tmpQs.toLatin1();
    this->port = tmpQba.data();

    if((this->ipSrv == NULL) || (this->port == NULL)){
        this->ui->lbl_error_setting->setText("ip or port not set yet");
        qDebug() << "ip or port not set yet" << endl;
        return 1;
    }

    //send file
    if(this->mode == 0){
        if(this->filePath == NULL){
            this->ui->lbl_error_setting->setText("error: no file selected !");
            return -1;
        }
        ret = this->sslCsl.startAsClient(this->ipSrv, this->port, this->filePath);
    }
    else if(this->mode == 1){
        ret = this->sslCsl.startAsServer(this->port, this->filePath);
    }
    else{
        this->ui->lbl_error_setting->setText("error: error on var mode");
        qDebug() << "error on var mode" << endl;
        ret = -1;
    }

    if(ret == -1){
        this->ui->lbl_error_setting->setText("error: fail to send or receive");
    }
    return 1;
}

int MainWindow::setValues(void){
    //local var

    this->mode = -1;
    this->ipSrv = NULL;
    this->port = NULL;
    this->filePath = NULL;

    //set logo image
    QString filename = "../../../imgs/logo.png";
    QImage *tmpImg = new QImage;
    if(!tmpImg->load(filename)){
        printf("fail to load logo\n");
        return -1;
    }
    //scale the logo
    QImage *imgScaled = new QImage;
    *imgScaled = tmpImg->scaled(
                280,
                140,
                Qt::KeepAspectRatio);
    this->ui->lbl_logo->setPixmap(QPixmap::fromImage(*imgScaled));
    //qDebug() << this->filePath << endl;
    return 1;
}

int MainWindow::setVisibility(void){
    switch(this->mode){
    case -1:
        this->ui->lbl_error_setting->setVisible(false);
        this->ui->lbl_logo->setVisible(true);
        this->ui->lbl_tips_port->setVisible(false);
        this->ui->lbl_tips_file_path->setVisible(false);
        this->ui->lbl_tips_guide->setVisible(true);
        this->ui->lbl_tips_srv_ip->setVisible(false);
        this->ui->lbl_welcome->setVisible(true);
        this->ui->pb_apply->setVisible(false);
        this->ui->pb_choose_file->setVisible(false);
        this->ui->te_file_path->setVisible(false);
        this->ui->te_port->setVisible(false);
        this->ui->te_srv_ip->setVisible(false);

        break;
    case 0:
        this->ui->lbl_error_setting->setVisible(true);
        this->ui->lbl_logo->setVisible(true);
        this->ui->lbl_tips_port->setVisible(true);
        this->ui->lbl_tips_file_path->setVisible(true);
        this->ui->lbl_tips_guide->setVisible(false);
        this->ui->lbl_tips_srv_ip->setVisible(true);
        this->ui->lbl_welcome->setVisible(false);
        this->ui->pb_apply->setVisible(true);
        this->ui->pb_choose_file->setVisible(true);
        this->ui->te_file_path->setVisible(true);
        this->ui->te_port->setVisible(true);
        this->ui->te_srv_ip->setVisible(true);
        break;
    case 1:
        this->ui->lbl_error_setting->setVisible(true);
        this->ui->lbl_logo->setVisible(true);
        this->ui->lbl_tips_port->setVisible(true);
        this->ui->lbl_tips_file_path->setVisible(true);
        this->ui->lbl_tips_guide->setVisible(false);
        this->ui->lbl_tips_srv_ip->setVisible(false);
        this->ui->lbl_welcome->setVisible(false);
        this->ui->pb_apply->setVisible(true);
        this->ui->pb_choose_file->setVisible(false);
        this->ui->te_file_path->setVisible(true);
        this->ui->te_port->setVisible(true);
        this->ui->te_srv_ip->setVisible(false);
        break;
    default:
        abort();
        break;
    }
    return 1;
}

int MainWindow::setComponentText(void){
    switch(this->mode){
    case -1:
        break;
    case 0:
        this->ui->lbl_tips_file_path->setText("file path to send:");
        this->ui->pb_apply->setText("send");
        break;
    case 1:
        this->ui->lbl_tips_file_path->setText("file path to receive:");
        this->ui->pb_apply->setText("receive");
        break;
    default:
        abort();
        break;
    }
    return 1;
}

//slots

void MainWindow::setModeSender(){
    //set mode var
    this->mode = 0;
    //set visibility
    this->setVisibility();
    this->setComponentText();
}

void MainWindow::setModeReceiver(){
    //set mode var
    this->mode = 1;
    //set visibility
    this->setVisibility();
    this->setComponentText();
}

void MainWindow::getFilePath(){
     QString qFilePath = QFileDialog::getOpenFileName(
                this,
                "open",
                "../"
                );
    this->ui->te_file_path->setText(qFilePath);
    //transfer from QString to char*
    QByteArray qba = qFilePath.toLatin1();
    this->filePath = qba.data();
    //strcpy(this->testFilePath, this->filePath);
    qDebug() << this->filePath << endl;
}


