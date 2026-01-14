#ifndef TRENDCHARTVIEW_H
#define TRENDCHARTVIEW_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class TrendChartView;
}

class TrendChartView : public QDialog
{
    Q_OBJECT

public:
    explicit TrendChartView(QWidget *parent = nullptr,QString targetId = "");
    ~TrendChartView();

private:
    Ui::TrendChartView *ui;
};

#endif // TRENDCHARTVIEW_H
