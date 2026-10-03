#pragma once
#include "audio.hpp"
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QPaintEvent>
#include <QListWidget>
#include <QDir>

class ProgressBar : public QWidget {
    Q_OBJECT

    public:
        explicit ProgressBar(QWidget* parent = nullptr);

    public slots:
        void setPercent(int percent);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        int percent_ = 0;
};

class PlaylistPanel : public QWidget {
    Q_OBJECT

    public:
        explicit PlaylistPanel(QWidget* parent = nullptr);

        void loadDirectory(const QString& directory);

    signals:
        void trackSelected(const QString& filepath);

    private slots:
        void onItemClicked(QListWidgetItem* item);

    private:
        QListWidget* playlist;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private slots:
        void togglePlaylist();

    private:
        QWidget* playerPage;
        PlaylistPanel* playlistPanel;
        QPushButton* switchButton;

        QPushButton* play;
        AudioPlayer* player;
        ProgressBar* progress;
};