#include "widget.h"
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 1. 设置无边框、始终置顶、不在任务栏显示
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    // 2. 设置背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    // 3. 设置初始大小
    resize(200, 200);

    // 4. 加个临时标签当占位图（测试用）
    QLabel *testLabel = new QLabel("Hello Pet!", this);
    testLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold; background-color: transparent;");
    testLabel->move(50, 80);
}

Widget::~Widget()
{
}