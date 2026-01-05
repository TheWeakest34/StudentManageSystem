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
}

void LoginView::on_btSignIn_clicked()
{
    QString status = IDataBase::getInstance().userLogin(ui->UserName->text(),ui->Password->text());

    //if(status == "true"){
        mainWindow = new MainWindow;
        mainWindow->show();
        this->hide();
    //}

    //else{
        //ui->ErrorLabel->setText(status);
        //ui->ErrorLabel->setStyleSheet("QLabel { color : red; }");
    //}
}

void LoginView::ReturnToLogin()
{
    this->show();
    ui->UserName->setFocus();
}

void LoginView::on_btSignUp_clicked()
{
    signUpView = new SignUpView;
    signUpView->show();
    this->hide();

    connect(signUpView,SIGNAL(gotoLoginView()),this,SLOT(ReturnToLogin()));
}

