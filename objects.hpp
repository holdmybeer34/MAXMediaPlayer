#pragma once
#include "audio.hpp"
#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <QString>

class ButtonPP : public QPushButton {
    Q_OBJECT

    public:
    explicit ButtonPP(QWidget* parent = nullptr);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
    explicit MainWindow(QWidget* parent = nullptr);

    private:
    ButtonPP* play;
    AudioPlayer* player;
};