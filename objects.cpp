#include "objects.hpp"
#include "audio.hpp"
#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QPaintEvent>
#include <algorithm>

ButtonPP::ButtonPP(QWidget* parent):QPushButton(parent){
    setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    setIconSize(QSize(30,30));
    setFixedSize(50,50);
}

ProgressBar::ProgressBar(QWidget* parent):QWidget(parent){
    setMinimumHeight(20);
}

void ProgressBar::setPercent(int percent){
    percent = std::clamp(percent, 0, 100);

    if (percent == percent_) return;
    percent_ = percent;
    update();
}

void ProgressBar::paintEvent(QPaintEvent*) {
    QPainter progr(this);

    progr.fillRect(rect(), QColor(255, 255, 255));

    int fillWidth = width() * percent_ / 100;
    progr.fillRect(0, 0, fillWidth, height(), QColor(0, 0, 0));
}

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent){

    setWindowTitle("MediaPlayer");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QPalette pal = centralWidget->palette();
    pal.setColor(QPalette::Window, QColor(128, 128, 128));
    centralWidget->setPalette(pal);
    centralWidget->setAutoFillBackground(true);
    
    progress = new ProgressBar(centralWidget);
    progress->setFixedHeight(10);

    play = new ButtonPP(centralWidget);
    player = new AudioPlayer(this);
    connect(play, &QPushButton::clicked, player, &AudioPlayer::ButtonPPFunc);
    connect(player, &AudioPlayer::progressChanged, progress, &ProgressBar::setPercent);
    
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->addStretch(1);
    layout->addWidget(play, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(progress);
    layout->addStretch(1);
}