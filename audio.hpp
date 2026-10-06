#pragma once
#include <alsa/asoundlib.h>
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
        void setPath(const QString& path);
        void playSelected();
        void forcedStop();

    private:
        snd_pcm_t* pcm_ = nullptr;

        QString currentPath;
        bool isPlaying = false;
        bool isPaused = false;
};