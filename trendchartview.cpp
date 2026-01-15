#include "trendchartview.h"
#include "ui_trendchartview.h"
#include "idatabase.h"
#include <QtCharts>

TrendChartView::TrendChartView(QWidget *parent,QString targetId)
    : QDialog(parent)
    , ui(new Ui::TrendChartView)
{
    ui->setupUi(this);

    // 获取数据
    QVector<IDataBase::TrendData> data = IDataBase::getInstance().getTrendData(targetId);

    if(data.isEmpty()) {
        QMessageBox::information(this, "提示", "暂无数据可生成图表");
        return;
    }

    // 图表 1：三个单科
    QStringList subjects = {"数学", "C语言", "Java"};
    QVector<QLineSeries*> seriesList;
    QVector<QColor> colors = {Qt::blue, Qt::green, Qt::red};

    // 创建单科的折线
    for(int s = 0; s < 3; s++) {
        QLineSeries *series = new QLineSeries();
        series->setName(subjects[s]);
        series->setColor(colors[s]);

        for(int i = 0; i < data.size(); i++) {
            series->append(i+1, data[i].scores[s]);
        }
        seriesList.append(series);
    }

    // 创建图表
    QChart *chartSubjects = new QChart();
    for(auto *series : seriesList) {
        chartSubjects->addSeries(series);
    }

    QString titleText = targetId.isEmpty() ? "全年级" : ("学生 " + targetId);
    chartSubjects->setTitle(QString("%1 - 单科成绩变化趋势").arg(titleText));

    // 创建坐标轴
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("考试次数");
    axisX->setTickCount(data.size());
    axisX->setLabelFormat("%d");

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("分数");
    axisY->setRange(0, 100); // 单科满分100

    chartSubjects->addAxis(axisX, Qt::AlignBottom);
    chartSubjects->addAxis(axisY, Qt::AlignLeft);
    for(auto *series : seriesList) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
    ui->trendGraph->setChart(chartSubjects);

    // 图表 2：总分 (ui->trendGraph_2)
    QLineSeries *totalSeries = new QLineSeries();
    totalSeries->setName("总分");
    totalSeries->setColor(Qt::magenta);

    for(int i = 0; i < data.size(); i++) {
        totalSeries->append(i+1, data[i].scores[3]);
    }

    QChart *chartTotal = new QChart();
    chartTotal->addSeries(totalSeries);
    chartTotal->setTitle(QString("%1 - 总分变化趋势").arg(titleText));

    // 创建坐标轴
    QValueAxis *axisX2 = new QValueAxis();
    axisX2->setTitleText("考试次数");
    axisX2->setTickCount(data.size());
    axisX2->setLabelFormat("%d");

    QValueAxis *axisY2 = new QValueAxis();
    axisY2->setTitleText("分数");
    axisY2->setRange(0, 300); // 总分满分300

    chartTotal->addAxis(axisX2, Qt::AlignBottom);
    chartTotal->addAxis(axisY2, Qt::AlignLeft);
    totalSeries->attachAxis(axisX2);
    totalSeries->attachAxis(axisY2);

    ui->trendGraph_2->setChart(chartTotal);
}

TrendChartView::~TrendChartView()
{
    delete ui;

    if(ui->trendGraph->chart()) {
        delete ui->trendGraph->chart();
    }
    if(ui->trendGraph_2->chart()) {
        delete ui->trendGraph_2->chart();
    }
}
