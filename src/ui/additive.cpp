#include "ui/additive.hpp"

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
#include <qlistwidget.h>

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

    connect(playlist,
            &QListWidget::itemClicked,
            this,
            &PlaylistPanel::onItemClicked
    );

    connect(playlist,
            &QListWidget::itemDoubleClicked,
            this,
            &PlaylistPanel::onItemDoubleClicked
    );
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

    if (playlist->count() > 0){
        playlist->setCurrentRow(0);
        QListWidgetItem* first = playlist->item(0);
        emit trackSelected(first->data(Qt::UserRole).toString());
    }
}

void PlaylistPanel::onItemClicked(QListWidgetItem* item) {
    QString path = item->data(Qt::UserRole).toString();
    emit trackSelected(path);
}

void PlaylistPanel::onItemDoubleClicked(QListWidgetItem* item) {
    QString path = item->data(Qt::UserRole).toString();
    emit trackDoubleClicked(path);
}