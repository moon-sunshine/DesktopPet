#include "widget.h"
#include <QApplication>
#include <QPixmap>
#include <QSize>
#include <QDir>
#include <QScreen>
#include <QFile>
#include <QDebug> // 用来输出调试信息

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 1. 窗口设置
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(300, 300);
    currentScale = 1.0f; // 初始缩放比例

    // 2. 创建显示标签
    petLabel = new QLabel(this);
    petLabel->resize(300, 300);
    petLabel->setAlignment(Qt::AlignCenter);

    // 3. 加载素材（保持不变）
    QString appDir = QCoreApplication::applicationDirPath();
    QString assetsDir = appDir + "/assets";
    QString gifPath = assetsDir + "/pet.gif";
    QString pngPath = assetsDir + "/pet.png";

    petMovie = new QMovie(gifPath, QByteArray(), this);
    if (petMovie->isValid()) {
        petMovie->setScaledSize(QSize(300, 300));
        petLabel->setMovie(petMovie);
        petMovie->start();
    } else {
        QPixmap pixmap(pngPath);
        if (pixmap.isNull()) {
            petLabel->setText("找不到 pet.gif 或 pet.png");
            petLabel->setStyleSheet("color: red; font-weight: bold; background: transparent;");
        } else {
            petLabel->setPixmap(pixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    petLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    initTray();
}

Widget::~Widget()
{
    if (petMovie->isValid()) petMovie->stop();
}

// 核心新增：缩放桌宠（中心固定模式）
void Widget::resizePet(float factor)
{
    // 1. 先算出当前窗口的中心点
    int centerX = x() + width() / 2;
    int centerY = y() + height() / 2;

    // 2. 计算新的缩放比例
    currentScale *= factor;
    // 限制缩放范围：0.5倍到3倍之间
    if (currentScale < 0.5f) currentScale = 0.5f;
    if (currentScale > 3.0f) currentScale = 3.0f;

    int newSize = static_cast<int>(300 * currentScale);

    // 3. 根据中心点，反推新的左上角坐标
    int newX = centerX - newSize / 2;
    int newY = centerY - newSize / 2;

    // 4. 边界检查：防止放大后中心点把窗口推出屏幕外
    QScreen *screen = QApplication::screenAt(QPoint(centerX, centerY));
    if (screen) {
        QRect screenRect = screen->availableGeometry();
        int boundX = qBound(screenRect.left() - newSize + 50, newX, screenRect.right() - 50);
        int boundY = qBound(screenRect.top() - newSize + 50, newY, screenRect.bottom() - 50);
        // setGeometry 一次性完成位置和大小设置，比分开写 move 和 resize 更流畅
        setGeometry(boundX, boundY, newSize, newSize);
    } else {
        setGeometry(newX, newY, newSize, newSize);
    }

    // 5. 更新内部 Label 和动画尺寸
    petLabel->resize(newSize, newSize);
    if (petMovie->isValid()) {
        petMovie->setScaledSize(QSize(newSize, newSize));
    } else {
        // 静态图逻辑
        QString appDir = QCoreApplication::applicationDirPath();
        QPixmap pixmap(appDir + "/assets/pet.png");
        if (!pixmap.isNull()) {
            petLabel->setPixmap(pixmap.scaled(newSize, newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void Widget::initTray()
{
    // ... 保持原有的托盘初始化代码（与你刚才合并的一模一样） ...
    trayMenu = new QMenu(this);
    QAction *showAction = trayMenu->addAction("显示/隐藏桌宠");
    QAction *quitAction = trayMenu->addAction("退出");

    connect(showAction, &QAction::triggered, this, [this]() { this->setVisible(!this->isVisible()); });
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QString appDir = QCoreApplication::applicationDirPath();
    QString iconPath = appDir + "/assets/icon.png";
    trayIcon = new QSystemTrayIcon(this);
    if (QFile::exists(iconPath)) {
        trayIcon->setIcon(QIcon(iconPath));
    } else {
        trayIcon->setIcon(QIcon::fromTheme("applications-system"));
    }
    trayIcon->setToolTip("桌面宠物");
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) { this->setVisible(!this->isVisible()); }
    });
}

// 鼠标按下：记录两个位置
void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressPos = event->globalPosition().toPoint(); // 记录屏幕绝对位置（用来判断点击）
        dragPosition = pressPos - frameGeometry().topLeft(); // 记录窗口相对偏移（用来拖拽）
        event->accept();
    }
}

// 鼠标松开：判断是“点击”还是“拖拽”
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint releasePos = event->globalPosition().toPoint();
        // 如果鼠标松开时的位置和按下时的位置相差很小（比如小于5个像素），说明是“点击”
        if ((releasePos - pressPos).manhattanLength() < 5) {
            qDebug() << "点击了桌宠！这里可以触发点击动画或音效";
            // 以后有素材了，可以在这里切换成 click.gif
        }
        event->accept();
    }
}

// 鼠标移动（保持不变）
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint newPos = event->globalPosition().toPoint() - dragPosition;
        QScreen *screen = QApplication::screenAt(event->globalPosition().toPoint());
        if (screen) {
            QRect screenRect = screen->availableGeometry();
            int x = qBound(screenRect.left() - width() + 50, newPos.x(), screenRect.right() - 50);
            int y = qBound(screenRect.top() - height() + 50, newPos.y(), screenRect.bottom() - 50);
            move(x, y);
        } else {
            move(newPos);
        }
        event->accept();
    }
}

// 右键菜单：加入缩放功能
void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *zoomInAction = menu.addAction("放大");
    QAction *zoomOutAction = menu.addAction("缩小");
    menu.addSeparator(); // 加一条分割线
    QAction *quitAction = menu.addAction("退出桌宠");

    connect(zoomInAction, &QAction::triggered, this, [this]() { resizePet(1.2f); });  // 每次放大20%
    connect(zoomOutAction, &QAction::triggered, this, [this]() { resizePet(0.8f); }); // 每次缩小20%
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    menu.exec(event->globalPos());
}

void Widget::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
}