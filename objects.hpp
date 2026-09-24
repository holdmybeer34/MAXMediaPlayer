#pragma once
#include "audio.hpp"
#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <QString>

class PlayButton : public QPushButton {
    Q_OBJECT

    public:
    explicit PlayButton(QWidget* parent = nullptr);
};

class PauseButton : public QPushButton {
    Q_OBJECT

    public:
    explicit PauseButton(QWidget* parent = nullptr);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
    explicit MainWindow(QWidget* parent = nullptr);

    private:
    PlayButton* play;
    PauseButton* pause;
    AudioPlayer* player;
};