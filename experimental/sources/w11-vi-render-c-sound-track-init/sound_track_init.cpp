// Observed Init__17VISceneSoundTrack at 0x0114e9b0, 64 bytes. Calls Clear__17VISceneSoundTrack on
// the object; a negative result is returned as -1. Otherwise the call is a tail call and its
// result is returned unchanged: SetTracks__17VISceneSoundTrack(object).
extern "C" {
struct VISceneSoundTrack;
int Clear__17VISceneSoundTrack(VISceneSoundTrack* track);
int SetTracks__17VISceneSoundTrack(VISceneSoundTrack* track);

int Init__17VISceneSoundTrack(VISceneSoundTrack* track) {
    if (Clear__17VISceneSoundTrack(track) < 0) return -1;
    return SetTracks__17VISceneSoundTrack(track);
}
}
