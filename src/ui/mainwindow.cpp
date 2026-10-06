#include "ui/additive.hpp"
#include "ui/mainwindow.hpp"
#include "audio/audiobrain.hpp"

#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QPaintEvent>
#include <QListWidget>
#include <QDir>
#include <QFileInfo>

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent){

    setWindowTitle("MediaPlayer");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QPalette pal = centralWidget->palette();
    pal.setColor(QPalette::Window,QColor(128, 128, 128));
    centralWidget->setPalette(pal);
    centralWidget->setAutoFillBackground(true);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);


    playerPage = new QWidget(centralWidget);
    {
        QVBoxLayout* playerVlayout = new QVBoxLayout(playerPage);
        playerVlayout->setContentsMargins(10,10,10,10);

        playlistButton = new QPushButton("≡", playerPage);
        playlistButton->setFixedSize(25, 25);

        QHBoxLayout* topBar = new QHBoxLayout();
        {
        topBar->addStretch(1);
        topBar->addWidget(playlistButton);
        }

        QHBoxLayout* centreLayout = new QHBoxLayout();
        {
        play = new QPushButton("▶", playerPage);
        play->setFixedSize(40,40);
        stop = new QPushButton("■", playerPage);
        stop->setFixedSize(40,40);

        centreLayout->addStretch(1);
        centreLayout->addWidget(play);
        centreLayout->addSpacing(10);
        centreLayout->addWidget(stop);
        centreLayout->addStretch(1);

        progress = new ProgressBar(playerPage);
        
        playerVlayout->addLayout(topBar);
        playerVlayout->addStretch(1);
        playerVlayout->addLayout(centreLayout);
        playerVlayout->addSpacing(20);
        playerVlayout->addWidget(progress);
        playerVlayout->addStretch(1);
        }
    }

    player = new AudioPlayer(this);
    playlistPanel = new PlaylistPanel(centralWidget);
    playlistPanel->hide();

    connect(playlistPanel,
            &PlaylistPanel::trackSelected,
            player,
            &AudioPlayer::setPath
    );
    connect(playlistPanel,
            &PlaylistPanel::trackDoubleClicked,
            player,
            [this](const QString& path){
                player->setPath(path);
                player->playSelected();
                playlistPanel->hide();
            }
    );
    connect(play,
            &QPushButton::clicked,
            player,
            &AudioPlayer::playSelected
    );
    connect(player,
            &AudioPlayer::progressChanged,
            progress,
            &ProgressBar::setPercent
    );
    connect(stop,
            &QPushButton::clicked,
            player,
            &AudioPlayer::forcedStop
    );

    playlistPanel->loadDirectory("/home/maxim/Документы/GitHub/MAXMediaPlayer/music");

    connect(playlistButton,
        &QPushButton::clicked,
        this,
        &MainWindow::togglePlaylist
    );

    mainLayout->addWidget(playerPage, 1);
    mainLayout->addWidget(playlistPanel, 0);
}

void MainWindow::togglePlaylist() {
    if (playlistPanel->isVisible()) {
        playlistPanel->hide();
    } else {
        playlistPanel->show();
    }
}