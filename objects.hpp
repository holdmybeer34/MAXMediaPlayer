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
        void trackDoubleClicked(const QString& filepath);

    public slots:
        void onItemClicked(QListWidgetItem* item);
        void onitemDoubleClicked(QListWidgetItem* item);

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
        QPushButton* playlistButton;

        QPushButton* play;
        QPushButton* stop;
        AudioPlayer* player;
        ProgressBar* progress;
};