#include "mainwindow.hpp"

#include <QPushButton>
#include <QPalette>
#include <QVBoxLayout>

using namespace Qt;

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent){

    setWindowTitle("MediaPlayer");
    resize(800, 600);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QPalette pal = centralWidget->palette();
    pal.setColor(QPalette::Window, QColor(128, 128, 128));
    centralWidget->setPalette(pal);
    centralWidget->setAutoFillBackground(true);

    play = new PlayButton(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->addStretch(2);
    layout->addWidget(play, 0, AlignCenter);
    layout->addStretch(1);
}

PlayButton::PlayButton(QWidget* parent):QPushButton(parent){
    setText("Здарова ебать");
    setFixedSize(110,50);
}