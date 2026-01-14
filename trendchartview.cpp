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

    // 创建4条折线
    QStringList subjects = {"数学", "C语言", "Java", "总分"};
    QVector<QLineSeries*> seriesList;
    QVector<QColor> colors = {Qt::blue, Qt::green, Qt::red, Qt::magenta};

    for(int s = 0; s < 4; s++) {
        QLineSeries *series = new QLineSeries();
        series->setName(subjects[s]);
        series->setColor(colors[s]);

        for(int i = 0; i < data.size(); i++) {
            series->append(i, data[i].scores[s]);
        }
        seriesList.append(series);
    }

    // 创建图表
    QChart *chart = new QChart();
    for(auto *series : seriesList) {
        chart->addSeries(series);
    }

    // 设置标题
    QString title = targetId.isEmpty() ? "全年级" : ("学生 " + targetId);
    chart->setTitle(QString("%1 - 成绩变化趋势").arg(title));

    // 创建坐标轴
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("考试次数");
    axisX->setTickCount(data.size());
    axisX->setLabelFormat("%d");

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("分数");
    axisY->setRange(0, 300); // 总分最高300

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for(auto *series : seriesList) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    ui->trendGraph->setChart(chart);
}

TrendChartView::~TrendChartView()
{
    delete ui;
}
