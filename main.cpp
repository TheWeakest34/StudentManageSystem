#include "loginview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(R"(
        /* ========== 按钮优化 ========== */
        QPushButton {
            background-color: #409EFF;
            color: white;
            border-radius: 6px;
            padding: 6px 12px;
            border: none;
        }
        QPushButton:hover {
            background-color: #66B1FF;
        }
        QPushButton:pressed {
            background-color: #3A8EE6;
        }
        /* 取消/次要按钮用灰色 */
        QPushButton#btCancel, QPushButton#cancelBtn {
            background-color: #F4F4F5;
            color: #606266;
            border: 1px solid #DCDFE6;
        }
        QPushButton#btCancel:hover {
            background-color: #E9E9EB;
            color: #409EFF;
            border-color: #C6E2FF;
        }

        /* ========== 输入框优化 ========== */
        QLineEdit, QComboBox, QDoubleSpinBox, QSpinBox {
            border: 1px solid #DCDFE6;
            border-radius: 6px;
            padding: 4px 8px;
            background: white;
            min-height: 20px;
        }
        QLineEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QSpinBox:focus {
            border: 1px solid #409EFF;
            outline: none;
        }

        /* ========== 分组框 ========== */
        QGroupBox {
            font-weight: bold;
            color: #606266;
            border: 1px solid #DCDFE6;
            border-radius: 6px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        QComboBox {
            border-radius: 6px;
        }

        QTabWidget::pane {
            border: 1px solid #DCDFE6;
            background: white;
            border-radius: 4px;
        }

        QTabBar::tab {
            background: #F5F7FA;
            border: 1px solid #E4E7ED;
            padding: 8px 16px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }

        /* 选中状态的Tab */
        QTabBar::tab:selected {
            background: #409EFF; /* 选中背景色 */
            color: white; /* 选中文字颜色 */
            border-color: #409EFF;
            border-bottom-color: #fff; /* 底部边框为白色，与内容区域融合 */
        }

        /* 未选中状态的Tab */
        QTabBar::tab:!selected {
            color: #606266;
        }
    )");
    LoginView l;
    l.show();
    return a.exec();
}
