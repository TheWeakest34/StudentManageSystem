#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include "signupview.h"
#include "mainwindow.h"

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:
    void on_btSignIn_clicked();
    void ReturnToLogin();

    void on_btSignUp_clicked();

private:
    Ui::LoginView *ui;
    SignUpView *signUpView;
    MainWindow *mainWindow;
};

#endif // LOGINVIEW_H
