#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "studenteditview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btEdit_clicked();

private:
    Ui::MainWindow *ui;
    StudentEditView *studentEditView;
};

#endif // MAINWINDOW_H
