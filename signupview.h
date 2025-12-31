#ifndef SIGNUPVIEW_H
#define SIGNUPVIEW_H

#include <QWidget>

namespace Ui {
class SignUpView;
}

class SignUpView : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpView(QWidget *parent = nullptr);
    ~SignUpView();

private slots:
    void on_btCancel_clicked();


    void on_btSignUp_clicked();

signals:
    void gotoLoginView();

private:
    Ui::SignUpView *ui;
};

#endif // SIGNUPVIEW_H
