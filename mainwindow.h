#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QLabel>
#include <QSlider>
#include <QMovie>
#include <QDragEnterEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dropEvent(QDropEvent*);

private:
    Ui::MainWindow *ui;
    QMovie *movie;
    QString filename = "";
    QSpinBox *spinbox;
    QSlider *slider;
    QLabel *LS1, *LS2, *LS3, *LS4;

private slots:
    void on_action_open_triggered();
    void on_action_exportAllFrames_triggered();
    void on_action_quit_triggered();
    void on_action_changelog_triggered();
    void on_action_aboutQt_triggered();
    void on_action_about_triggered();
    void on_actionPlay_triggered();
    void on_actionLast_triggered();
    void on_actionNext_triggered();
    void on_actionSave_triggered();
    void open(QString fileName);
    void setSpeed(int ps);
    void setSFrame(int n);
    void setFrame();
    void frameChange(int fn);
    void playPause();
};

#endif // MAINWINDOW_H
