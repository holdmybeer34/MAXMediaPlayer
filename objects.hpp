#pragma once
#include "audio.hpp"
#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <QString>
#include <QWidget>
#include <QPaintEvent>

class ButtonPP : public QPushButton {
    Q_OBJECT

    public:
        explicit ButtonPP(QWidget* parent = nullptr);
};

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

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        ButtonPP* play;
        AudioPlayer* player;
        ProgressBar* progress;
};