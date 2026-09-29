#include "audio.hpp"
#include "objects.hpp"
#include <alsa/asoundlib.h>
#include <thread>
#include <cstdlib>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.hpp"
#include "minimp3_ex.hpp"

AudioPlayer::AudioPlayer(QObject* parent) : QObject(parent){}

void AudioPlayer::ButtonPPFunc(){
    std::lock_guard<std::mutex> lock(m_mutex);

    if(isPlaying == false){
        play("/home/maxim/Prog/MediaPlayer/Music/Caramella Girls - Caramelldansen.mp3");
        isPlaying = true;
    } else {
        isPaused = !isPaused;
    }
}

void AudioPlayer::play(const QString& path){
    isPlaying = true;
    isPaused = false;

    std::thread([this, path](){
        mp3dec_t mp3d;
        mp3dec_file_info_t info{};
        mp3dec_init(&mp3d);
        mp3dec_load(&mp3d, path.toStdString().c_str(), &info, nullptr, nullptr);
        
        snd_pcm_t* pcm = nullptr;
        snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);

        snd_pcm_hw_params_t* hw;
        snd_pcm_hw_params_alloca(&hw);
        snd_pcm_hw_params_any(pcm, hw);
        snd_pcm_hw_params_set_access(pcm, hw, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(pcm, hw, SND_PCM_FORMAT_S16_LE);

        unsigned int rate = info.hz;
        snd_pcm_hw_params_set_rate_near(pcm, hw, &rate, 0);
        snd_pcm_hw_params_set_channels(pcm, hw, info.channels);
        snd_pcm_hw_params(pcm, hw);

        const int channels  = info.channels;
        int16_t*  ptr       = info.buffer;
        size_t    frames_left = info.samples / channels;
        const size_t chunk  = 1024;

        while (frames_left > 0) {
            size_t to_write = std::min(chunk, frames_left);
            snd_pcm_sframes_t written = snd_pcm_writei(pcm, ptr, to_write);

            if (written < 0) {
                snd_pcm_recover(pcm, written, 0);
                continue;
            }

            ptr += written * channels;
            frames_left -= written;
        }

        snd_pcm_drain(pcm);
        snd_pcm_close(pcm);
        free(info.buffer);

        std::lock_guard<std::mutex> lock(m_mutex);
        isPlaying = false;
        isPaused = false;
    }).detach();
}