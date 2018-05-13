#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    spinbox = new QSpinBox(this);
    spinbox->setSingleStep(50);
    spinbox->setRange(10,1000);
    spinbox->setValue(100);
    connect(spinbox,SIGNAL(valueChanged(int)),this,SLOT(setSpeed(int)));
    ui->mainToolBar->addWidget(spinbox);
    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    //slider->setMaximum(10);
    slider->setValue(0);
    slider->setFocusPolicy(Qt::NoFocus);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setSFrame(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(setFrame()));
    ui->mainToolBar->addWidget(slider);
    LS1 = new QLabel("欢迎使用海天鹰动画播放器！");
    LS1->setMinimumSize(500,20);
    LS1->setStyleSheet("padding:0px 3px;");
    LS2 = new QLabel("速度：100%");
    LS2->setMinimumSize(20,20);
    LS2->setStyleSheet("padding:0px 3px;");
    LS3 = new QLabel("帧：0/0");
    LS3->setMinimumSize(20,20);
    LS3->setStyleSheet("padding:0px 3px;");
    LS4 = new QLabel("下帧等待：0 ms");
    LS4->setMinimumSize(20,20);
    LS4->setStyleSheet("padding:0px 3px;");
    ui->statusBar->addWidget(LS1);
    ui->statusBar->addWidget(LS2);
    ui->statusBar->addWidget(LS3);
    ui->statusBar->addWidget(LS4);

    movie = new QMovie;
    //movie->setCacheMode(QMovie::CacheAll);
    connect(movie,SIGNAL(frameChanged(int)),this,SLOT(frameChange(int)));
    ui->label->setMovie(movie);

    connect(new QShortcut(QKeySequence(Qt::Key_Space),this), SIGNAL(activated()),this, SLOT(playPause()));
    connect(new QShortcut(QKeySequence(Qt::Key_Left),this), SIGNAL(activated()),this, SLOT(on_actionLast_triggered()));
    connect(new QShortcut(QKeySequence(Qt::Key_Right),this), SIGNAL(activated()),this, SLOT(on_actionNext_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_triggered()
{
    if(filename == ""){
        filename = QFileDialog::getOpenFileName(this, "打开动画", ".", "动画(*.gif)");
    }else{
        filename = QFileDialog::getOpenFileName(this, "打开动画", filename, "动画(*.gif)");
    }
    if(!filename.isEmpty()){        
        movie->setFileName(filename);
        //ui->label->setMovie(movie);
        movie->start();
        ui->actionPlay->setIcon(QIcon(":/pause.png"));
        //ui->label->resize(movie->currentPixmap().width(),movie->currentPixmap().height());
        //qDebug() << "Size" << movie->frameRect().size();
        qDebug() << "Size" << movie->currentPixmap().size();
        ui->scrollArea->widget()->setMinimumSize(movie->currentPixmap().size());
        ui->label->resize(movie->frameRect().size());
        spinbox->setValue(100);
        slider->setMaximum(movie->frameCount()-1);
        slider->setValue(0);
        LS1->setText("打开 " + filename);
    }
}

void MainWindow::on_action_exportAllFrames_triggered()
{    
    QString dir = "";
    if(filename == ""){
        dir = QFileDialog::getExistingDirectory(this,"保存路径",".", QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    }else{
        dir = QFileDialog::getExistingDirectory(this,"保存路径",filename, QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    }
    if(!dir.isEmpty()){
        for(int n=0; n<movie->frameCount(); n++){
            movie->jumpToFrame(n);
            QImage image = movie->currentImage();
            QString fn = dir + "/" + QString::number(n) + ".jpg";
            image.save(fn);
            LS1->setText("保存 " + fn);
        }
    }
}

void MainWindow::on_action_quit_triggered()
{
    qApp->quit();
}

void MainWindow::on_action_changelog_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "更新历史", "1.0\n2018-05\nQMovie->setCacheMode(QMovie::CacheAll) 在处理大GIF图片时内存暴涨，不得不取消，失去跳帧功能。\n合并播放暂停按钮。\n2017-02\n增加下帧等待时间。\n修复大文件拖动条拖动引起死机的BUG。\n增加一键导出所有帧。\n增加进度条。\n打开GIF图片，暂停、继续、调速、保存当前帧到图片。");
    aboutMB.exec();
}

void MainWindow::on_action_aboutQt_triggered()
{
    QMessageBox::aboutQt(NULL, "关于 Qt");
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "海天鹰GIF播放器 1.0\n一款基于Qt的GIF播放器，可以暂停、继续、调速、保存当前帧到图片。\n作者：黄颖\nE-mail: sonichy@163.com\n主页：sonichy.96.lt");
    aboutMB.setIconPixmap(QPixmap(":/icon.png"));
    aboutMB.exec();
}

void MainWindow::on_actionPlay_triggered()
{
    playPause();
}

void MainWindow::on_actionLast_triggered()
{
    //movie->setPaused(true);
    bool b = movie->jumpToFrame(movie->currentFrameNumber()-1);
    if(b){
        LS1->setText("跳到上一帧成功");
    }else{
        LS1->setText("跳到上一帧失败");
    }
}

void MainWindow::on_actionNext_triggered()
{
    //movie->setPaused(true);
    bool b = movie->jumpToNextFrame();
    if(b){
        LS1->setText("跳到下一帧成功");
    }else{
        LS1->setText("跳到下一帧失败");
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(filename == ""){
        filename = QFileDialog::getSaveFileName(this, "保存图片", ".", "图片(*.jpg)");
    }else{
        filename = QFileDialog::getSaveFileName(this, "保存图片", filename, "图片(*.jpg)");
    }
    if(!filename.isEmpty()){
        QImage image = movie->currentImage();
        image.save(filename);
        LS1->setText("保存 " + filename);
    }
}

void MainWindow::setSpeed(int ps)
{
    movie->setSpeed(ps);
    LS2->setText("速度：" + QString::number(ps) + "%");
}

void MainWindow::setFrame()
{    
    bool b = movie->jumpToFrame(slider->value());
    if(b){
        LS1->setText("跳到第 " + QString::number(slider->value()) + " 帧成功");
    }else{
        LS1->setText("跳到第 " + QString::number(slider->value()) + " 帧失败");
    }
}

void MainWindow::setSFrame(int n)
{
    LS3->setText("帧：" + QString::number(n) + "/" + QString::number(movie->frameCount()-1));
}

void MainWindow::frameChange(int fn)
{
    slider->setValue(fn);
    LS3->setText("帧：" + QString::number(fn) + "/" + QString::number(movie->frameCount()-1));
    LS4->setText("下帧等待：" + QString::number(movie->nextFrameDelay()) + " ms");
}

void MainWindow::playPause()
{
    qDebug() << movie->state();
    if(movie->state() == QMovie::Running){
        movie->setPaused(true);
        ui->actionPlay->setIcon(QIcon(":/play.png"));
        LS1->setText("暂停");
    }else if(movie->state() == QMovie::Paused){
        movie->setPaused(false);
        ui->actionPlay->setIcon(QIcon(":/pause.png"));
        LS1->setText("播放");
    }else if(movie->state() == QMovie::NotRunning){
        movie->start();
        LS1->setText("开始");
    }
}
