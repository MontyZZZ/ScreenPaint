#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QToolBar>

#include "drawscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // 主要功能toolbar  线，笔，形状...
    void createMainToolBar();


public slots:
    void startDraw();
    void initTool();
    void setPenSize();
    void setPenColor();
    void colorActionCheckedFalse();
    void toolActionCheckedFalse();

    void clearScene();
    void closeScene();
    void undo();
    void redo();
    void save();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    // 主体绘制区间
    DrawScene*           m_mScene;
    QGraphicsView*       m_view;
    QScreen *            m_screen;
    bool                 m_isStartDraw;

    // action
    QAction*             m_lineAction;
    QAction*             m_penAction;
    QAction*             m_rectAction;
    QAction*             m_ellipseAction;
    QAction*             m_arrowAction;
    QAction*             m_textAction;
    QAction*             m_thinAction;
    QAction*             m_thickAction;
    QAction*             m_whiteAction;
    QAction*             m_blackAction;
    QAction*             m_redAction;
    QAction*             m_blueAction;
    QAction*             m_undoAction;
    QAction*             m_redoAction;
    QAction*             m_clearAction;
    QAction*             m_saveAction;


};
#endif // MAINWINDOW_H
