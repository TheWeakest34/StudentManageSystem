#ifndef STUDENTEDITVIEW_H
#define STUDENTEDITVIEW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlTableModel>

namespace Ui {
class StudentEditView;
}

class StudentEditView : public QDialog
{
    Q_OBJECT

public:
    explicit StudentEditView(QWidget *parent = nullptr,int index = 0,int tabIndex = 0);
    ~StudentEditView();

private slots:

    void on_btSave_clicked();

    void on_btCancel_clicked();

private:
    Ui::StudentEditView *ui;
    QDataWidgetMapper *dataMapper;
    QSqlTableModel *tableModel;
    int curTabIndex;
};

#endif // STUDENTEDITVIEW_H
