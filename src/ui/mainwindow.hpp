#pragma once

#include "ui/additive.hpp"
#include "audio/audiobrain.hpp"
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QListWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private slots:
        void togglePlaylist();

    private:
        QWidget* playerPage;
        PlaylistPanel* playlistPanel;
        QPushButton* playlistButton;

        QPushButton* play;
        QPushButton* stop;
        AudioPlayer* player;
        ProgressBar* progress;
};