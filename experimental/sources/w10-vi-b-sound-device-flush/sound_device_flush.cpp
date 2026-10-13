// Observed Flush__13VISoundDevice at 0x01125a38, 388 bytes. Offsets are bytes.
// It repeats this until a full pass completes:
//  - while the word at 3412 of the device is zero it calls
//    IdleTransfer__13VISoundDevice(device), SetIopFunction__FUii(3, 0) and
//    FlushIopCommand__Fb(0), then tests the word again;
//  - otherwise it walks the 128 sound pointers at 2584. For a nonzero entry
//    VISound::Type is read: type 0 takes AsWave and, when the wave word at
//    40 is zero, calls BeginAdpcmTransfer(device, wave+52, wave[60], wave[56],
//    wave+40); type 1 takes AsXm and, when the word at 32 is zero, calls
//    BeginAdpcmTransfer(device, xm+44, xm[20], xm[48], xm+32); when it is
//    nonzero but the word at 28 is zero it calls BeginXmTransfer(device,
//    xm+36, xm[24], xm[20], xm[40], xm+28). Other types are skipped.
//  - After any call the walk stops and starts over from the check of 3412.
// It returns zero after a walk that made no call. The wave and xm layouts
// beyond the words named here are not established.
extern "C" {
struct VISoundDevice;
struct VISound;

int IdleTransfer__13VISoundDevice(VISoundDevice* device);
void SetIopFunction__FUii(unsigned function, int value);
void FlushIopCommand__Fb(bool wait);
int Type__C7VISound(const VISound* sound);
int* AsWave__7VISound(VISound* sound);
int* AsXm__7VISound(VISound* sound);
void BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(VISoundDevice* device, void** destination,
                                                 unsigned size, int bank, int* state);
void BeginXmTransfer__13VISoundDevicePPvUiUiiPi(VISoundDevice* device, void** destination,
                                                unsigned size, unsigned count, int bank,
                                                int* state);

int Flush__13VISoundDevice(VISoundDevice* device) {
    char* base = reinterpret_cast<char*>(device);
    for (;;) {
        if (*reinterpret_cast<int*>(base + 3412) == 0) {
            IdleTransfer__13VISoundDevice(device);
            SetIopFunction__FUii(3, 0);
            FlushIopCommand__Fb(false);
            continue;
        }
        bool started = false;
        unsigned* sounds = reinterpret_cast<unsigned*>(base + 2584);
        for (int i = 0; i < 128 && !started; ++i) {
            VISound* sound = reinterpret_cast<VISound*>(static_cast<unsigned long>(sounds[i]));
            if (!sound) continue;
            int type = Type__C7VISound(sound);
            if (type == 0) {
                int* wave = AsWave__7VISound(sound);
                if (wave[10] == 0) {
                    started = true;
                    wave = AsWave__7VISound(sound);
                    BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(
                        device, reinterpret_cast<void**>(wave + 13), static_cast<unsigned>(wave[15]),
                        wave[14], wave + 10);
                }
            } else if (type == 1) {
                int* xm = AsXm__7VISound(sound);
                if (xm[8] == 0) {
                    started = true;
                    xm = AsXm__7VISound(sound);
                    BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(
                        device, reinterpret_cast<void**>(xm + 11), static_cast<unsigned>(xm[5]),
                        xm[12], xm + 8);
                } else {
                    xm = AsXm__7VISound(sound);
                    if (xm[7] == 0) {
                        started = true;
                        xm = AsXm__7VISound(sound);
                        BeginXmTransfer__13VISoundDevicePPvUiUiiPi(
                            device, reinterpret_cast<void**>(xm + 9), static_cast<unsigned>(xm[6]),
                            static_cast<unsigned>(xm[5]), xm[10], xm + 7);
                    }
                }
            }
        }
        if (!started) return 0;
    }
}
}
