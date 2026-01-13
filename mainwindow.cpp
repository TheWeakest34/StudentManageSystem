#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    IDataBase &db = IDataBase::getInstance();
    for(int i=0 ; i<db.TableCount() ; i++)   //数据库中有多少个非空表就创建多少个页面
        on_addNewTab_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
     delete studentEditView;
}

void MainWindow::initTableView(QTableView *tableview)
{
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableview->setSelectionMode(QAbstractItemView::SingleSelection);
    tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableview->setAlternatingRowColors(true);
}

void MainWindow::on_btEdit_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();   //通过索引来找到对应模型
    QModelIndex curIndex = IDataBase::getInstance().studentSelections[tabIndex]->currentIndex();

    if(curIndex.row() < 0){
        QMessageBox::warning(this,"提示","请先选择一行!",QMessageBox::Ok);
        return;
    }

    studentEditView = new StudentEditView(this,curIndex.row(),tabIndex);
    studentEditView->show();
}


void MainWindow::on_btSearch_clicked()
{
    QString SearchWay = ui->SearchBy->currentText();
    QString SearchText = ui->SearchText->text();
    QString filter;

    if(SearchWay == "姓名")
        filter = QString("Name like '%%1%'").arg(SearchText);
    else if(SearchWay == "学号")
        filter = QString("ID like '%%1%'").arg(SearchText);
    else if(SearchWay == "班级")
        filter = QString("Class like '%%1%'").arg(SearchText);
    else{
        QMessageBox::warning(this,"错误","请选择一种查询方式",QMessageBox::Ok);
        return;
    }

    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase::getInstance().searchStudent(tabIndex,filter);
}


void MainWindow::on_btAdd_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();
    int curRow = IDataBase::getInstance().addNewStudent(tabIndex);
    studentEditView = new StudentEditView(this,curRow,tabIndex);
    studentEditView->show();
}


void MainWindow::on_btDelete_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase::getInstance().deleteStudent(tabIndex);
}


void MainWindow::on_btOrder_clicked()
{
    IDataBase &db = IDataBase::getInstance();
    QString sortBy = ui->OrderBy->currentText();
    Qt::SortOrder order;

    QString fieldName;
    if(sortBy == "学号")
        fieldName = "ID";
    else if(sortBy == "姓名")
        fieldName = "Name";
    else if(sortBy == "性别")
        fieldName = "Sex";
    else if(sortBy == "班级")
        fieldName = "Class";
    else if(sortBy == "数学成绩")
        fieldName = "MathScore";
    else if(sortBy == "C语言成绩")
        fieldName = "CScore";
    else if(sortBy == "Java成绩")
        fieldName = "JavaScore";
    else if(sortBy == "总分")
        fieldName = "TotalScore";
    else{
        QMessageBox::warning(this,"错误","请选择一种排序方式",QMessageBox::Ok);
        return;
    }

    // 确定排序方向（升序/降序）
    if(ui->AscOrDesc->isChecked())
        order = Qt::AscendingOrder;
    else
        order = Qt::DescendingOrder;

    // 执行排序
    int tabIndex = ui->tabWidget->currentIndex();
    int columnIndex = db.studentTableModels[tabIndex]->fieldIndex(fieldName);
    db.studentTableModels[tabIndex]->setSort(columnIndex, order);
    db.studentTableModels[tabIndex]->select();
}

void MainWindow::on_Filter_clicked()
{
    QString Class = ui->Class->currentText();
    QString course = ui->Course->currentText();
    QString Min = ui->Min->text();
    QString Max = ui->Max->text();
    QString range = QString("BETWEEN %1 AND %2").arg(Min,Max);
    QString filter;

    if(!Class.isEmpty() && Class != "所有班级")
        filter = QString("Class = '%1' AND ").arg(Class);
    if(course == "数学")
        filter.append("MathScore " + range);
    else if(course == "C语言")
        filter.append("CScore " + range);
    else if(course == "Java")
        filter.append("JavaScore " + range);
    else
        filter.append("MathScore " + range + " AND CScore " + range + " AND JavaScore " + range);

    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase::getInstance().searchStudent(tabIndex,filter);
}

void MainWindow::on_addNewTab_clicked()
{
    int count = ui->tabWidget->count();
    QString tabName = QString("第%1次考试").arg(count+1);

    QTableView *newTable = new QTableView(this);
    initTableView(newTable);

    IDataBase &db = IDataBase::getInstance();
    if(db.initNewModel(count)){
        newTable->setModel(db.studentTableModels[count]);
        newTable->setSelectionModel(db.studentSelections[count]);
    }

    QWidget *tabContainer = new QWidget(this);   //创建布局
    QVBoxLayout *tabLayout = new QVBoxLayout(tabContainer);
    tabLayout->addWidget(newTable);

    ui->tabWidget->addTab(tabContainer,tabName);
    ui->tabWidget->setCurrentIndex(count);
}

void MainWindow::on_CloseTab_clicked()
{
    int index = ui->tabWidget->currentIndex();
    IDataBase &db = IDataBase::getInstance();
    db.studentTableModels.remove(index);
    db.studentSelections.remove(index);
    QWidget *tab = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    delete tab;
}

void MainWindow::on_Import_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择CSV文件",
                                                    QDir::currentPath(),"CSV 文件 (*.csv)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件: " + filePath);
        return;
    }

    // 获取当前标签页索引及对应的数据模型
    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase &db = IDataBase::getInstance();
    QSqlTableModel *model = db.studentTableModels[tabIndex];

    QTextStream in(&file);
    QStringList headers;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;   //跳过空行

        // 分割CSV字段（处理带引号的字段）
        QStringList fields;
        QString currentField;
        bool inQuotes = false;

        for (int i = 0; i < line.length(); ++i) {
            QChar c = line[i];
            if (c == '"')
                inQuotes = !inQuotes;
            else if (c == ',' && !inQuotes) {
                fields.append(currentField.trimmed());
                currentField.clear();
            }
            else
                currentField += c;
        }
        fields.append(currentField.trimmed());

        if (headers.isEmpty()) {
            headers = fields;
            continue;
        }

        // 创建新记录并填充数据
        QSqlRecord record = model->record();
        for (int i = 0; i < headers.size(); i++) {
            QString fieldName = headers[i]; // 从CSV表头获取字段名
            QString fieldValue = fields[i];

            if(fieldValue.startsWith('\'') && fieldValue.length() > 1)   //处理以单引号开头的数字串
                fieldValue = fieldValue.mid(1);

            int colIndex = model->fieldIndex(fieldName); // 匹配表格模型中的字段
            if (fieldName.contains("Score", Qt::CaseInsensitive)) {
                double score = fieldValue.toDouble();   //将成绩转化为数字
                record.setValue(colIndex,score);
            }
            else
                record.setValue(colIndex, fieldValue);
        }
        model->insertRecord(-1, record);
    }
    model->submitAll();
    file.close();
    QMessageBox::information(this, "成功", "导入成功" + filePath, QMessageBox::Ok);
}

void MainWindow::on_Export_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this,"导出CSV文件",QDir::currentPath() +
                                                    "/成绩导出.csv","CSV 文件 (*.csv);;所有文件 (*.*)");
    if (filePath.isEmpty())
        return;

    //获取当前标签页的模型
    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase &db = IDataBase::getInstance();
    QSqlTableModel *model = db.studentTableModels[tabIndex];
    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "提示", "当前标签页无数据可导出！", QMessageBox::Ok);
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法写入文件: " + filePath + "\n" + file.errorString());
        return;
    }
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << "\xEF\xBB\xBF";

    //写入表头
    QStringList headerLine;
    for (int col = 0; col < model->columnCount(); col++) {
        QString fieldName = model->headerData(col, Qt::Horizontal).toString();
        headerLine.append(fieldName);
    }
    out << formatCsvLine(headerLine) << "\n";

    //写入数据行
    for (int row = 0; row < model->rowCount(); row++) {
        QStringList dataLine;
        for (int col = 0; col < model->columnCount(); col++) {
            QVariant cellValue = model->data(model->index(row, col));
            QString valueStr = cellValue.toString();

            // 成绩字段格式化
            QString fieldName = model->headerData(col, Qt::Horizontal).toString();
            if (fieldName == "MathScore" || fieldName == "CScore" ||
                fieldName == "JavaScore" || fieldName == "TotalScore") {
                double score = cellValue.toDouble();
                valueStr = QString::number(score, 'g', 5);
            }

            dataLine.append(valueStr);
        }
        out << formatCsvLine(dataLine) << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", "数据已导出至：\n" + filePath, QMessageBox::Ok);
}

QString MainWindow::formatCsvLine(const QStringList &fields)
{
    QStringList formattedFields;
    for (const QString &field : fields) {
        QString processed = field;
        //替换双引号为两个双引号（CSV转义规则）
        processed.replace("\"", "\"\"");
        //若包含逗号/换行符/双引号，整体加双引号
        if (processed.contains(',') || processed.contains('\n') || processed.contains('"'))
            processed = "\"" + processed + "\"";
        formattedFields.append(processed);
    }
    return formattedFields.join(",");
}

void MainWindow::on_scoreStatistics_triggered()
{
    int tabIndex = ui->tabWidget->currentIndex();
    statisticView = new StatisticView(this,tabIndex);
    statisticView->show();
}

