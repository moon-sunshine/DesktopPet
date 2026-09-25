#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    // 鼠标事件重写
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    // 右键菜单事件重写
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QLabel *petLabel;       // 显示桌宠图片的标签
    QPoint dragPosition;    // 记录拖拽时的鼠标偏移量
};

#endif // WIDGET_H