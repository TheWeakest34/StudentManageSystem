#ifndef STATISTICVIEW_H
#define STATISTICVIEW_H

#include <QDialog>

namespace Ui {
class StatisticView;
}

class StatisticView : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticView(QWidget *parent = nullptr,int tabIndex = 0);
    ~StatisticView();

private slots:
    void on_btnstatistic_clicked();

private:
    int curTabIndex;
    Ui::StatisticView *ui;
};

#endif // STATISTICVIEW_H
