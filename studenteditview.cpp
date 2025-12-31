#include "studenteditview.h"
#include "ui_studenteditview.h"

StudentEditView::StudentEditView(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudentEditView)
{
    ui->setupUi(this);
}

StudentEditView::~StudentEditView()
{
    delete ui;
}
