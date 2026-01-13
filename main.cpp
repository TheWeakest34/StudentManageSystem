#include "loginview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginView l;
    l.show();
    return a.exec();
}
/*核心功能：、单科与全科成绩统计、成绩变化趋势图展示；
 * 支持生成统计报表。
 *
 *学生课程成绩批量录入
 *采用SQLite存储数据，结合Model/View实现班级与课程维度的分类筛选，
 */
