#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "studenteditview.h"
#include <QTableView>

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

    void on_btSearch_clicked();

    void on_btAdd_clicked();

    void on_btDelete_clicked();

    void on_btOrder_clicked();

    void initTableView(QTableView *tableview);

private:
    Ui::MainWindow *ui;
    StudentEditView *studentEditView;
};

#endif // MAINWINDOW_H
