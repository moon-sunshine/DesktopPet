#include "widget.h"
#include <QApplication>
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 1. 窗口基本设置（无边框、置顶、透明）
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(300, 300); // 设置窗口初始大小

    // 2. 创建显示图片的标签
    petLabel = new QLabel(this);
    petLabel->resize(300, 300);

    // 3. 加载图片
    // 注意：这里用的是绝对路径，如果你放在其他盘，请修改前面的路径
    QPixmap pixmap("D:/qt_project/DesktopPet/pet.png");

    if (pixmap.isNull()) {
        // 如果图片没找到，用文字提示，防止程序崩溃
        petLabel->setText("图片没找到\n请把 pet.png 放到 D:/qt_project/DesktopPet/");
        petLabel->setAlignment(Qt::AlignCenter);
        petLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
    } else {
        // 让图片自适应窗口大小，保持比例平滑缩放
        petLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // 4. 关键一步：让鼠标事件穿透 Label，传给主窗口，否则拖不动
    petLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

Widget::~Widget()
{
}

// 鼠标按下事件：记录鼠标按下的位置
void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Qt 6 推荐用 globalPosition().toPoint()
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

// 鼠标移动事件：让窗口跟着鼠标跑
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
        event->accept();
    }
}

// 右键菜单事件：添加退出按钮
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *quitAction = menu.addAction("退出桌宠");
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    menu.exec(event->globalPos()); // 在鼠标位置弹出菜单
}