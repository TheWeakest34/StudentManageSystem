#include "loginview.h"
#include "ui_loginview.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->show();
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_btSignIn_clicked()
{
    mainWindow = new MainWindow;
    mainWindow->show();
    this->hide();
}

void LoginView::ReturnToLogin()
{
    this->show();
}

void LoginView::on_btSignUp_clicked()
{
    signUpView = new SignUpView;
    signUpView->show();
    this->hide();

    connect(signUpView,SIGNAL(gotoLoginView()),this,SLOT(ReturnToLogin()));
}

