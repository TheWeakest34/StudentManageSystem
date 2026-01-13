#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);

    //this->setWindowFlag(Qt::FramelessWindowHint);
}

LoginView::~LoginView()
{
    delete ui;
    delete mainWindow;
    delete signUpView;
}

void LoginView::on_btSignIn_clicked()
{
    QString status = IDataBase::getInstance().userLogin(ui->UserName->text(),ui->Password->text());

    //if(status == "true"){
        mainWindow = new MainWindow;
        mainWindow->show();
        this->close();
    //}

    //else{
        //ui->ErrorLabel->setText(status);
        //ui->ErrorLabel->setStyleSheet("QLabel { color : red; }");
    //}
}

void LoginView::ReturnToLogin(QString userName,QString password)
{
    this->show();
    ui->UserName->setFocus();
    ui->UserName->setText(userName);
    ui->Password->setText(password);
}

void LoginView::on_btSignUp_clicked()
{
    if(signUpView == nullptr)   //防止用户多次点击注册和取消而导致重复创建
        signUpView = new SignUpView;
    signUpView->show();
    this->hide();

    connect(signUpView,&SignUpView::gotoLoginView,this,&LoginView::ReturnToLogin);
}

