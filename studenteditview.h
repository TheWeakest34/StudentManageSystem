#ifndef STUDENTEDITVIEW_H
#define STUDENTEDITVIEW_H

#include <QDialog>

namespace Ui {
class StudentEditView;
}

class StudentEditView : public QDialog
{
    Q_OBJECT

public:
    explicit StudentEditView(QWidget *parent = nullptr);
    ~StudentEditView();

private:
    Ui::StudentEditView *ui;
};

#endif // STUDENTEDITVIEW_H
