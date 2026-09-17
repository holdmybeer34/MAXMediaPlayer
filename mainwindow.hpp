#pragma once

#include <QMainWindow>
#include <QPushButton>

class PlayButton : public QPushButton {
    Q_OBJECT

    public:
    explicit PlayButton(QWidget* parent = nullptr);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
    explicit MainWindow(QWidget* parent = nullptr);

    private:
    QPushButton* play;
};