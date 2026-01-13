#include "statisticview.h"
#include "ui_statisticview.h"
#include "idatabase.h"
#include <QMessageBox>

StatisticView::StatisticView(QWidget *parent,int tabIndex)
    : QDialog(parent)
    , ui(new Ui::StatisticView)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    curTabIndex = tabIndex;
}

StatisticView::~StatisticView()
{
    delete ui;
}

void StatisticView::on_btnstatistic_clicked()
{
    QString subject = ui->subject->currentText();

    if(subject.isEmpty()){
        QMessageBox::warning(this, "提示", "请选择统计科目！", QMessageBox::Ok);
        return;
    }

    IDataBase::ScoreStat stat = IDataBase::getInstance().getStatistics(curTabIndex,subject);

    ui->count->setText(QString::number(stat.totalCount));
    ui->avg->setText(QString::number(stat.avg));
    ui->max->setText(QString::number(stat.max, 'f', 2));
    ui->min->setText(QString::number(stat.min, 'f', 2));
    ui->passRate->setText(QString::number(stat.passRate, 'f', 2) + "%");
    ui->excellentRate->setText(QString::number(stat.excellentRate, 'f', 2) + "%");
}

