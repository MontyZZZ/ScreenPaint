#include "mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QGraphicsView>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(200, 300, 200, 300);
    //resize(200, 300);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    m_isStartDraw = false;

    m_mScene = new DrawScene(this);

    m_view = new QGraphicsView(m_mScene);
    m_view->setWindowFlag(Qt::FramelessWindowHint);
    m_view->setStyleSheet("padding: 0px; border: 0px;");
    m_view->setMouseTracking(true);

    // 消除残影
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // 去掉滚动条
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(m_mScene, &DrawScene::closeScene, this, &MainWindow::closeScene);
    connect(m_mScene, &DrawScene::saveScene, this, &MainWindow::save);

    createMainToolBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMainToolBar()
{
    QToolBar* firstToolBar = new QToolBar(this);
    //m_mainToolBar->setAllowedAreas(Qt::LeftToolBarArea);
    firstToolBar->setFloatable(false);
    firstToolBar->setMovable(false);
//    firstToolBar->setStyleSheet("QToolBar{hover:{color: blue};border:none;}");
//    firstToolBar->setStyleSheet("border: none;padding-left: 0px;padding-right: 0px;margin: 2px;");
    addToolBar(Qt::LeftToolBarArea, firstToolBar);

    m_lineAction = new QAction(QIcon(":/icon/line.png"), "Line");
    m_lineAction->setObjectName("Line");
    m_lineAction->setCheckable(true);
    firstToolBar->addAction(m_lineAction);

    m_rectAction = new QAction(QIcon(":/icon/rect.png"), "Rect");
    m_rectAction->setObjectName("Rect");
    m_rectAction->setCheckable(true);
    firstToolBar->addAction(m_rectAction);

    m_arrowAction = new QAction(QIcon(":/icon/arrow.png"), "Arrow");
    m_arrowAction->setObjectName("Arrow");
    m_arrowAction->setCheckable(true);
    firstToolBar->addAction(m_arrowAction);

    m_thinAction = new QAction(QIcon(":/icon/thin.png"), "Thin");
    m_thinAction->setObjectName("Thin");
    m_thinAction->setCheckable(true);
    m_thinAction->setChecked(true);
    firstToolBar->addAction(m_thinAction);

    m_whiteAction = new QAction(QIcon(":/icon/white.png"), "White");
    m_whiteAction->setObjectName("White");
    m_whiteAction->setCheckable(true);
    firstToolBar->addAction(m_whiteAction);

    m_redAction = new QAction(QIcon(":/icon/red.png"), "Red");
    m_redAction->setObjectName("Red");
    m_redAction->setCheckable(true);
    m_redAction->setChecked(true);
    firstToolBar->addAction(m_redAction);

    m_undoAction = new QAction(QIcon(":/icon/undo.png"), "Undo");
    m_undoAction->setShortcut(QKeySequence::Undo);
    firstToolBar->addAction(m_undoAction);

    m_clearAction = new QAction(QIcon(":/icon/clear.png"), "Clear");
    m_clearAction->setShortcut(QKeySequence::New);
    firstToolBar->addAction(m_clearAction);

    connect(m_lineAction, &QAction::triggered, this, &MainWindow::initTool);
    connect(m_rectAction, &QAction::triggered, this, &MainWindow::initTool);
    connect(m_arrowAction, &QAction::triggered, this, &MainWindow::initTool);
    connect(m_thinAction, &QAction::triggered, this, &MainWindow::setPenSize);
    connect(m_whiteAction, &QAction::triggered, this, &MainWindow::setPenColor);
    connect(m_redAction, &QAction::triggered, this, &MainWindow::setPenColor);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);
    connect(m_clearAction, &QAction::triggered, this, &MainWindow::clearScene);


    // 两行
    this->addToolBarBreak(Qt::LeftToolBarArea);

    // 第二行
    QToolBar* secondBar = new QToolBar(this);
    secondBar->setFloatable(false);
    secondBar->setMovable(false);

    //secondBar->setStyleSheet("border:none;");
    addToolBar(Qt::LeftToolBarArea, secondBar);

    m_penAction = new QAction(QIcon(":/icon/pen.png"), "Pen");
    m_penAction->setObjectName("Pen");
    m_penAction->setCheckable(true);
    secondBar->addAction(m_penAction);

    m_ellipseAction = new QAction(QIcon(":/icon/ellipse.png"), "Ellipse");
    m_ellipseAction->setObjectName("Ellipse");
    m_ellipseAction->setCheckable(true);
    secondBar->addAction(m_ellipseAction);

    m_textAction = new QAction(QIcon(":/icon/text.png"), "Text");
    m_textAction->setObjectName("Text");
    m_textAction->setCheckable(true);
    secondBar->addAction(m_textAction);

    m_thickAction = new QAction(QIcon(":/icon/thick.png"), "Thin");
    m_thickAction->setObjectName("Thick");
    m_thickAction->setCheckable(true);
    secondBar->addAction(m_thickAction);

    m_blackAction = new QAction(QIcon(":/icon/black.png"), "Black");
    m_blackAction->setObjectName("Black");
    m_blackAction->setCheckable(true);
    secondBar->addAction(m_blackAction);

    m_blueAction = new QAction(QIcon(":/icon/blue.png"), "Blue");
    m_blueAction->setObjectName("Blue");
    m_blueAction->setCheckable(true);
    secondBar->addAction(m_blueAction);

    m_redoAction = new QAction(QIcon(":/icon/redo.png"), "Redo");
    m_redoAction->setShortcut(QKeySequence::Redo);
    secondBar->addAction(m_redoAction);

    m_saveAction = new QAction(QIcon(":/icon/save.png"), "Save");
    m_saveAction->setShortcut(QKeySequence::Save);
    secondBar->addAction(m_saveAction);

    connect(m_penAction, &QAction::triggered, this, &MainWindow::initTool);
    connect(m_ellipseAction, &QAction::triggered, this, &MainWindow::initTool);
    connect(m_textAction, &QAction::triggered, this, &MainWindow::initTool);
    connect(m_thickAction, &QAction::triggered, this, &MainWindow::setPenSize);
    connect(m_blackAction, &QAction::triggered, this, &MainWindow::setPenColor);
    connect(m_blueAction, &QAction::triggered, this, &MainWindow::setPenColor);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::startDraw()
{
    this->hide();

    m_screen = QGuiApplication::primaryScreen();
    QRect rect = m_screen->geometry();
    //rect.setHeight(rect.height() - 200);

    int width = rect.width();
    int height = rect.height();
    //qDebug() << width << height;
    m_mScene->setSceneRect(0 ,0, width, height);
    m_view->setGeometry(0 ,0, width, height);

    QPixmap pixmapFullScreen = m_screen->grabWindow(0);
    m_mScene->setBackgroundBrush(QBrush(pixmapFullScreen));
    m_view->show();

    this->show();
}

void MainWindow::initTool()
{
    if (!m_isStartDraw) {
        startDraw();
        m_isStartDraw = true;
    }

    toolActionCheckedFalse();
    QString name = sender()->objectName();

    if (name == "Line") {
        m_mScene->setShapeType(ShapeType::Line);
        m_lineAction->setChecked(true);
    } else if (name == "Pen") {
        m_mScene->setShapeType(ShapeType::Pen);
        m_penAction->setChecked(true);
    } else if (name == "Rect") {
        m_mScene->setShapeType(ShapeType::Rect);
        m_rectAction->setChecked(true);
    } else if (name == "Ellipse") {
        m_mScene->setShapeType(ShapeType::Ellipse);
        m_ellipseAction->setChecked(true);
    } else if (name == "Arrow") {
        m_mScene->setShapeType(ShapeType::Arrow);
        m_arrowAction->setChecked(true);
    } else if (name == "Text") {
        m_mScene->setShapeType(ShapeType::Text);
        m_textAction->setChecked(true);
    }
}

void MainWindow::setPenSize()
{
    QString name = sender()->objectName();
    //qDebug() << "send name: " << name;
    if (name == "Thin") {
        m_mScene->getPen().setWidth(3);
        m_thinAction->setChecked(true);
        m_thickAction->setChecked(false);
    } else if (name == "Thick") {
        m_mScene->getPen().setWidth(6);
        m_thinAction->setChecked(false);
        m_thickAction->setChecked(true);
    }
}

void MainWindow::setPenColor()
{
    QString name = sender()->objectName();
    if (name == "White") {
        m_mScene->getPen().setColor(Qt::white);
        colorActionCheckedFalse();
        m_whiteAction->setChecked(true);
    } else if (name == "Black") {
        m_mScene->getPen().setColor(Qt::black);
        colorActionCheckedFalse();
        m_blackAction->setChecked(true);
    } else if (name == "Red") {
        m_mScene->getPen().setColor(Qt::red);
        colorActionCheckedFalse();
        m_redAction->setChecked(true);
    } else if (name == "Blue") {
        m_mScene->getPen().setColor(Qt::blue);
        colorActionCheckedFalse();
        m_blueAction->setChecked(true);
    }
}

void MainWindow::colorActionCheckedFalse()
{
    m_whiteAction->setChecked(false);
    m_blackAction->setChecked(false);
    m_redAction->setChecked(false);
    m_blueAction->setChecked(false);
}

void MainWindow::toolActionCheckedFalse()
{
    m_lineAction->setChecked(false);
    m_penAction->setChecked(false);
    m_rectAction->setChecked(false);
    m_ellipseAction->setChecked(false);
    m_arrowAction->setChecked(false);
    m_textAction->setChecked(false);
}

void MainWindow::clearScene()
{
    m_mScene->clearData();
}

void MainWindow::closeScene()
{
    m_isStartDraw = false;
}

void MainWindow::undo()
{
    m_mScene->undoItem();
}

void MainWindow::redo()
{
    m_mScene->redoItem();
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                   "drawBoarding.png",
                                                    tr("Image files(*.bmp *.jpg  *.png );;All files (*.*)"));

    QSize size = m_screen->geometry().size();
    QImage image(size, QImage::Format_RGB32);
    QPainter painter(&image);
    m_mScene->render(&painter);   //关键函数
    image.save(fileName);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << __FILE__ << __func__;
    switch(event->key())
    {
        case Qt::Key_Escape:
            m_isStartDraw = false;
            m_mScene->clearData();
            m_view->close();
            break;
        default:
            QWidget::keyPressEvent(event);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //qDebug() << event->type();
    m_isStartDraw = false;
    m_mScene->clearData();
    m_view->close();
}


