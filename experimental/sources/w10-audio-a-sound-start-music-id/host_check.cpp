#include "sound_start_music_id.cpp"
#include <cstdio>
#include <cstring>
float soundMusicVolume, musicDuckAmount;
static int volume_calls, start_calls, last_index, last_id, last_flags;
static float last_volume;
void soundSetStreamVolume__Fif(int index, float volume) { ++volume_calls; last_index = index; last_volume = volume; }
void soundStartDialogStream__Fiii(int index, int id, int flags) {
    ++start_calls; last_index += index * 100; last_id = id; last_flags = flags;
}
int main() {
    soundMusicVolume = 0.04f;
    musicDuckAmount = 1.0f;
    soundStartMusic__Fii(3, 0);
    if (volume_calls != 0 || start_calls != 0) return 1;
    const float volumes[] = {0.05f, 0.5f, 1.0f};
    const int flags[] = {0, 1, 2, 0x20, 0x100};
    for (float volume : volumes) {
        for (int f : flags) {
            soundMusicVolume = volume;
            musicDuckAmount = 0.75f;
            volume_calls = start_calls = 0;
            soundStartMusic__Fii(42, f);
            if (volume_calls != 1 || start_calls != 1 || last_index != 0) return 2;
            if (last_id != 42 || last_flags != (f | 0x22)) return 3;
            if (std::memcmp(&last_volume, &(const float&)(0.75f * volume * 0.46875f), 4) != 0) return 4;
        }
    }
    std::puts("sound_start_music_id passed");
    return 0;
}
