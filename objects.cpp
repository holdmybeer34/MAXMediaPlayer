#include "objects.hpp"
#include "audio.hpp"
#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>
#include <QStyle>

using namespace Qt;

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent){

    setWindowTitle("MediaPlayer");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QPalette pal = centralWidget->palette();
    pal.setColor(QPalette::Window, QColor(128, 128, 128));
    centralWidget->setPalette(pal);
    centralWidget->setAutoFillBackground(true);
    
    play = new PlayButton(centralWidget);
    player = new AudioPlayer(this);
    connect(play, &QPushButton::clicked, player, &AudioPlayer::ClicktoPlay);
    
    pause = new PauseButton(centralWidget);
    connect(pause, &QPushButton::clicked, player, &AudioPlayer::ClicktoPause);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->addStretch(2);
    layout->addWidget(play, 0, AlignCenter);
    layout->addStretch(1);
}

PlayButton::PlayButton(QWidget* parent):QPushButton(parent){
    setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    setIconSize(QSize(30,30));
    setFixedSize(50,50);
}

PauseButton::PauseButton(QWidget* parent):QPushButton(parent){
    setIconSize(QSize(30,30));
    setFixedSize(50,50);
}