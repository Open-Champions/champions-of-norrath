// Observed hudAddHelpMessage__FiPUsT1 at 0x001c2378, 264 bytes.
// An id other than -1 returns when netPlayerIsLocal is zero. A zero
// netNetworkMode byte accepts the message. Any other mode walks
// playersAlive until nmPlayersAlive and calls msgHudSetPlayerBusy with the
// sign-extended low byte of the first local index and 1. No local index
// leaves the message unset. An accepted message loads "butn_message.anm"
// from "fx.lmp", plays that sequence at 1.0 with a null object, marks the
// returned sequence unpausable, flares interface 0, stores 1 in
// helpMessageOn, stores the two text pointers in title and body, and stores
// 90 in controlPaused. Those symbols are not defined here.
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" unsigned char netNetworkMode;
extern "C" int nmPlayersAlive;
extern "C" int playersAlive[];
extern "C" void msgHudSetPlayerBusy__Fcc(int player, int busy);
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
extern "C" void soundMakeSequenceUnpausable__Fi(int sequence);
extern "C" void coreInterfaceFlare__Fi(int kind);
extern "C" signed char helpMessageOn;
extern "C" unsigned short* title;
extern "C" unsigned short* body;
extern "C" int controlPaused;

extern "C" void hudAddHelpMessage__FiPUsT1(int id, unsigned short* first, unsigned short* second) {
    int accepted = 0;
    if (id != -1 && netPlayerIsLocal__Fi(id) == 0) return;
    if (netNetworkMode == 0) {
        accepted = 1;
    } else {
        for (int cursor = 0; cursor < nmPlayersAlive; ++cursor) {
            const int index = playersAlive[cursor];
            if (netPlayerIsLocal__Fi(index) == 0) continue;
            msgHudSetPlayerBusy__Fcc(static_cast<signed char>(index), 1);
            accepted = 1;
            break;
        }
    }
    if (accepted == 0) return;
    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("butn_message.anm"));
    const int sequence = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, nullptr, 1.0f);
    soundMakeSequenceUnpausable__Fi(sequence);
    coreInterfaceFlare__Fi(0);
    helpMessageOn = 1;
    title = first;
    body = second;
    controlPaused = 90;
}
