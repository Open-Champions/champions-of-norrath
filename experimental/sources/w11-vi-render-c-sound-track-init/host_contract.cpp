#include "sound_track_init.cpp"

static int clear_result;
static int set_result;
static int clear_calls, set_calls;
static VISceneSoundTrack* clear_arg;
static VISceneSoundTrack* set_arg;

extern "C" int Clear__17VISceneSoundTrack(VISceneSoundTrack* t) { ++clear_calls; clear_arg = t; return clear_result; }
extern "C" int SetTracks__17VISceneSoundTrack(VISceneSoundTrack* t) { ++set_calls; set_arg = t; return set_result; }

int main() {
    VISceneSoundTrack* t = reinterpret_cast<VISceneSoundTrack*>(0x4000);
    clear_result = -4; set_result = 9;
    if (Init__17VISceneSoundTrack(t) != -1 || clear_calls != 1 || set_calls != 0 || clear_arg != t) return 1;
    clear_result = 0;
    if (Init__17VISceneSoundTrack(t) != 9 || clear_calls != 2 || set_calls != 1 || set_arg != t) return 2;
    clear_result = 3; set_result = -8;
    if (Init__17VISceneSoundTrack(t) != -8 || set_calls != 2) return 3;
    return 0;
}
