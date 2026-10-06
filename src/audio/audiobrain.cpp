#include "audiobrain.hpp"
#include <alsa/asoundlib.h>
#include <thread>
#include <cstdlib>
#include <chrono>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.hpp"
#include "minimp3_ex.hpp"

AudioPlayer::AudioPlayer(QObject* parent) : QObject(parent){}

void AudioPlayer::play(const QString& path){
    std::thread([this, path](){

        mp3dec_t mp3d;
        mp3dec_file_info_t info{};
        mp3dec_init(&mp3d);
        mp3dec_load(&mp3d, path.toStdString().c_str(), &info, nullptr, nullptr);

        snd_pcm_open(&pcm_, "default", SND_PCM_STREAM_PLAYBACK, 0);

        snd_pcm_hw_params_t* hw;
        snd_pcm_hw_params_alloca(&hw);
        snd_pcm_hw_params_any(pcm_, hw);
        snd_pcm_hw_params_set_access(pcm_, hw, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(pcm_, hw, SND_PCM_FORMAT_S16_LE);

        unsigned int rate = info.hz;
        snd_pcm_hw_params_set_rate_near(pcm_, hw, &rate, 0);
        snd_pcm_hw_params_set_channels(pcm_, hw, info.channels);
        snd_pcm_hw_params(pcm_, hw);

        const int channels  = info.channels;
        int16_t* ptr = info.buffer;
        const size_t chunk = 1024;
        size_t frames_total = info.samples / channels;
        size_t frames_left = frames_total;

        const auto track_duration_ms = std::chrono::milliseconds(int64_t(frames_total) * 1000 / info.hz);
        auto start_time = std::chrono::steady_clock::now();

        emit progressChanged(0);

        while (isPlaying == true) {
            size_t to_write = std::min(chunk, frames_left);

            if (frames_left > 0){
                snd_pcm_sframes_t written = snd_pcm_writei(pcm_, ptr, to_write);
                
                if (written < 0) {
                    int err = snd_pcm_recover(pcm_, written, 0);
                    if (err < 0) break;
                    continue;
                }

                ptr += written * channels;
                frames_left -= written;
            }

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time);
            
            if (isPlaying){
                int percent = int(100.0 * elapsed.count() / track_duration_ms.count());
                emit progressChanged(percent);
            }

            if (frames_left == 0 && elapsed >= track_duration_ms) {
                break;
            }
            if (frames_left == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        snd_pcm_drain(pcm_);
        snd_pcm_close(pcm_);
        free(info.buffer);

        isPlaying = false;
    }).detach();
}

void AudioPlayer::setPath(const QString& path){
    currentPath = path;
}

void AudioPlayer::playSelected(){
    if(isPlaying == false){
        isPlaying = true;
        play(currentPath);
    }
    else if(isPaused == false && isPlaying == true){
        isPaused = !isPaused;
    }
}

void AudioPlayer::forcedStop(){
    isPlaying = false;
    isPaused  = false;

    if (pcm_) {
        snd_pcm_drop(pcm_);
        snd_pcm_prepare(pcm_);
    }

    emit progressChanged(0);
}