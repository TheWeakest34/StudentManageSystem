#include "signupview.h"
#include "ui_signupview.h"

SignUpView::SignUpView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignUpView)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
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

