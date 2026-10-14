// Observed: IsPlaying__13VISoundDevicei at 0x011265f0, 28 bytes. Arguments: the device and a playback handle. Calls
// Playback__13VISoundDevicei(device, handle) and returns 1 when the result is nonzero, else 0.
// Inferred: Playback looks a handle up, so this reports whether the handle still names a live playback.
// Unknown: the member name.
extern "C" {
unsigned Playback__13VISoundDevicei(unsigned char* device, int handle);
int IsPlaying__13VISoundDevicei(unsigned char* device, int handle) {
    return Playback__13VISoundDevicei(device, handle) != 0;
}
}
