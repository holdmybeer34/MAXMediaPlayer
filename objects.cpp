#include "objects.hpp"
#include "audio.hpp"

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
#include <algorithm>

PlaylistPanel::PlaylistPanel(QWidget* parent):QWidget(parent){
    setStyleSheet(
        "PlaylistPanel {border : 2px solid black;}"
    );

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(128,128,128));
    setPalette(pal);
    setAutoFillBackground(true);

    setFixedWidth(300);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10,10,10,10);

    playlist = new QListWidget(this);
    playlist->setStyleSheet(
        "QListWidget {"
        "   background: #2b2b2b;"
        "   color: white;"
        "   font-size: 14px;"
        "   border: none;"
        "}"
        "QListWidget::item { padding: 8px; }"
        "QListWidget::item:hover { background: #3a3a3a; }"
        "QListWidget::item:selected { background: #4a90d9; }"
    );

    layout->addWidget(playlist);

    connect(playlist, &QListWidget::itemClicked, this, &PlaylistPanel::onItemClicked);
}

void PlaylistPanel::loadDirectory(const QString& directory){
    playlist->clear();

    QDir dir(directory);
    QStringList filters;
    filters << "*.mp3";

    QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Name);

    for (const QFileInfo& file : files) {
        QListWidgetItem* item = new QListWidgetItem(file.fileName());
        item->setData(Qt::UserRole, file.absoluteFilePath());
        playlist->addItem(item);
    }
}

void PlaylistPanel::onItemClicked(QListWidgetItem* item) {
    QString path = item->data(Qt::UserRole).toString();
    emit trackSelected(path);
}

ProgressBar::ProgressBar(QWidget* parent):QWidget(parent){
    setFixedHeight(10);
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

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    playerPage = new QWidget(centralWidget);
    {
        QVBoxLayout* layout = new QVBoxLayout(playerPage);
        layout->setContentsMargins(10,10,10,10);

        QHBoxLayout* topBar = new QHBoxLayout();
        topBar->addStretch(1);

        switchButton = new QPushButton("≡", playerPage);
        switchButton->setFixedSize(40, 40);
        topBar->addWidget(switchButton);

        play = new QPushButton("▶", playerPage);
        progress = new ProgressBar(centralWidget);

        layout->addLayout(topBar);
        layout->addStretch(1);
        layout->addWidget(play, 0, Qt::AlignCenter);
        layout->addSpacing(20);
        layout->addWidget(progress);
        layout->addStretch(1);
    }

    playlistPanel = new PlaylistPanel(centralWidget);
    playlistPanel->hide();
    playlistPanel->loadDirectory("/home/maxim/Документы/GitHub/MAXMediaPlayer/Music");

    player = new AudioPlayer(this);

    connect(switchButton, &QPushButton::clicked, this, &MainWindow::togglePlaylist);
    connect(playlistPanel, &PlaylistPanel::trackSelected, player, &AudioPlayer::setPath);
    connect(play, &QPushButton::clicked, player, &AudioPlayer::playSelected);
    connect(player, &AudioPlayer::progressChanged, progress, &ProgressBar::setPercent);

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