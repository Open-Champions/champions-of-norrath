// Host check for runMusicDucking__Fv. Not an EE plan input.
#include <cmath>
#include <string>
extern "C" {
float musicDuckAmount = 1.0f;
float desiredMusicDuckAmount = 1.0f;
float soundMusicVolume = 0.5f;
}
#include "run_music_ducking.cpp"

static std::string trace;
static int status2 = 0, status0 = 0;
static float vol_seen = -1.0f;
extern "C" int soundGetStreamPlayStatus__Fi(int index) {
    trace += "G" + std::to_string(index);
    return index == 2 ? status2 : status0;
}
extern "C" void soundSetStreamVolume__Fif(int index, float volume) {
    trace += "V" + std::to_string(index);
    vol_seen = volume;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main() {
    // Not ducked, already at 1.0: no change, no volume call.
    runMusicDucking__Fv();
    if (trace != "G2" || !near(musicDuckAmount, 1.0f) || !near(desiredMusicDuckAmount, 1.0f)) return 1;
    // Stream 2 playing: desired drops to 0.2167, amount steps down 0.01; volume set when stream 0 has bit.
    trace.clear(); status2 = 0x20; status0 = 0x20;
    runMusicDucking__Fv();
    if (trace != "G2G0V0") return 2;
    if (!near(desiredMusicDuckAmount, 0.2f + 1.0f / 60.0f) || !near(musicDuckAmount, 0.99f)) return 3;
    if (vol_seen != musicDuckAmount * soundMusicVolume * 0.46875f) return 4;
    // Stream 0 without the bit: the amount still changes but no volume call.
    trace.clear(); status0 = 0;
    runMusicDucking__Fv();
    if (trace != "G2G0" || !near(musicDuckAmount, 0.98f)) return 5;
    // Release: desired climbs 1/60 per call; amount rises only when below it.
    trace.clear(); status2 = 0; musicDuckAmount = 0.5f; desiredMusicDuckAmount = 0.9f;
    runMusicDucking__Fv();
    if (!near(desiredMusicDuckAmount, 0.9f + 1.0f / 60.0f) || !near(musicDuckAmount, 0.51f)) return 6;
    // Step never overshoots the desired value.
    musicDuckAmount = 0.995f; desiredMusicDuckAmount = 1.0f;
    runMusicDucking__Fv();
    if (!near(musicDuckAmount, 1.0f)) return 7;
    musicDuckAmount = 0.2f; desiredMusicDuckAmount = 0.1f; status2 = 0;
    runMusicDucking__Fv();   // desired becomes 0.11667; amount steps down 0.01 only
    if (!near(musicDuckAmount, 0.19f)) return 8;
    musicDuckAmount = 0.12f; desiredMusicDuckAmount = 0.1f;
    runMusicDucking__Fv();   // amount 0.12 is above the new desired 0.116667 and is clamped to it
    if (!near(musicDuckAmount, 0.116667f)) return 9;
    return 0;
}
