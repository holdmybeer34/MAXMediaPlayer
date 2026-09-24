#include "audio.hpp"
#include "objects.hpp"
#include <alsa/asoundlib.h>
#include <thread>
#include <cstdlib>
#include <chrono>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.hpp"
#include "minimp3_ex.hpp"

AudioPlayer::AudioPlayer(QObject* parent) : QObject(parent){}

void AudioPlayer::ClicktoPlay(){
    if (isPlaying){
        return;
    }
    play("/home/maxim/Prog/MediaPlayer/Music/Caramella Girls - Caramelldansen.mp3");
}

void AudioPlayer::ClicktoPause(){
    if (isPlaying){
        isPaused = !isPaused;
        isPlaying = !isPlaying;
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

        long chunk_size = 2048;
        long total_frames = info.samples / info.channels;
        long frames_written = 0;
        short* buffer_ptr = (short*)info.buffer;

        while(frames_written < total_frames){
            
            if (isPaused){
                while(isPaused) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }

            long to_write = std::min(chunk_size, total_frames - frames_written);
            long written = snd_pcm_writei(pcm, buffer_ptr + (frames_written * info.channels), to_write);

            if (written < 0) written = snd_pcm_recover(pcm, written, 0);
            
            if (written > 0) {
                frames_written += written;
            } 
            else {
                break;
            }
        }

        snd_pcm_drain(pcm);
        snd_pcm_close(pcm);
        free(info.buffer);

        isPlaying = false;
    }).detach();
}