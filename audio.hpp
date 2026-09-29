#pragma once
#include <QObject>
#include <QString>

class AudioPlayer : public QObject {
    Q_OBJECT

    public:
        explicit AudioPlayer(QObject* parent = nullptr);
        void play(const QString& path);

    signals:
        void progressChanged(int percent);

    public slots:
        void ButtonPPFunc();

    private:
        bool isPlaying = false;
        bool isPaused = false;
};