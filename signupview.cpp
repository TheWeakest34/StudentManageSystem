#include "signupview.h"
#include "ui_signupview.h"
#include "idatabase.h"
#include <QMessageBox>

SignUpView::SignUpView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignUpView)
{
    ui->setupUi(this);

    //this->setWindowFlag(Qt::FramelessWindowHint);
}

SignUpView::~SignUpView()
{
    delete ui;
}

void SignUpView::on_btCancel_clicked()
{
    emit gotoLoginView();
    this->close();
}

void SignUpView::on_btSignUp_clicked()
{
    QString userName = ui->UserName->text();
    QString password = ui->Password->text();
    QString confirmPassword = ui->ConfirmPassword->text();

    QString status = IDataBase::getInstance().userSignUp(userName,password,confirmPassword);

    if(status == "true"){
        QMessageBox::information(this,"成功","注册成功！",QMessageBox::Ok);
        emit gotoLoginView();
    }
    else{   //显示错误信息
        ui->ErrorLabel->setText(status);
        ui->ErrorLabel->setStyleSheet("QLabel { color : red; }");
    }
}

