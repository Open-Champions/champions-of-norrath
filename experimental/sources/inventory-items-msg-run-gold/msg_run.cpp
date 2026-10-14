// Observed msg_run__4Gold at 0x154e68, 152 bytes.
// A non-zero signed byte at object+76 returns at once. Otherwise, when the word
// at object+80 equals frameNm minus 18, lumpFindResource("fx.lmp",
// "inv_drop_coin.anm") is played with soundPlaySequence(result, object, 1.0f).
// Then animAdvanceAnimation runs on the state at object+88 with the point at
// object+16 and two null arguments. When bit 7 of the halfword at object+94 is
// set, the byte at object+76 becomes 1 and objectRemoveFromRunList runs on the
// object (the store comes first).
// Unknown: what the byte at +76, the word at +80 and the flag mean.

extern "C" {
extern int frameNm;
char* lumpFindResource__FPcT0(const char* lump, const char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* second, void* third);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

extern "C" void msg_run__4Gold(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    if (static_cast<signed char>(object[76]) != 0) return;
    const unsigned stamp = unsigned(object[80]) | (unsigned(object[81]) << 8) | (unsigned(object[82]) << 16) |
                           (unsigned(object[83]) << 24);
    if (stamp == static_cast<unsigned>(frameNm - 18)) {
        char* sequence = lumpFindResource__FPcT0("fx.lmp", "inv_drop_coin.anm");
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sequence, object, 1.0f);
    }
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(object + 88, object + 16, nullptr, nullptr);
    const unsigned half = unsigned(object[94]) | (unsigned(object[95]) << 8);
    if ((half & 0x80u) != 0u) {
        object[76] = 1;
        objectRemoveFromRunList__FP10GameObject(object);
    }
}
