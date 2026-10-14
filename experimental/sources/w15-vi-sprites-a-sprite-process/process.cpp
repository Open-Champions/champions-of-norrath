// Observed: Process__9VIHSpriteiR7VIScene at 0x01064400, 168 bytes. Arguments (this, level, scene). It reads GetTime__7VIScene(scene) and
// compares the full 64-bit value with the 64-bit word at this+200. When they differ: if Size__Ct6VIPool1Z13VIHSpritePlay(this+120) > 0 it
// calls ProcessPlayList__9VIHSpriteiR7VIScene(this, level, scene) and, if that is not negative, ProcessHierarchy__9VIHSprite(this);
// otherwise it calls SetBindPose__9VIHSprite(this). A negative status returns -1 and leaves the time word alone. On success (or when the
// times matched) it calls GetTime again and stores the full 64-bit result at this+200, then returns zero.
// Inferred: this+200 is the time of the last processing, so a sprite is processed at most once per scene time. The integer is passed
// through unchanged to ProcessPlayList, whose own use of it is not examined here.
// Unknown: whether the two GetTime calls can ever return different values.
extern "C" {
long long GetTime__7VIScene(void* scene);
int Size__Ct6VIPool1Z13VIHSpritePlay(const void* pool);
int ProcessPlayList__9VIHSpriteiR7VIScene(void* object, int level, void* scene);
int ProcessHierarchy__9VIHSprite(void* object);
int SetBindPose__9VIHSprite(void* object);

int Process__9VIHSpriteiR7VIScene(void* object, int level, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (GetTime__7VIScene(scene) != *reinterpret_cast<long long*>(bytes + 200)) {
        int status;
        if (Size__Ct6VIPool1Z13VIHSpritePlay(bytes + 120) > 0) {
            status = ProcessPlayList__9VIHSpriteiR7VIScene(object, level, scene);
            if (status >= 0) status = ProcessHierarchy__9VIHSprite(object);
        } else {
            status = SetBindPose__9VIHSprite(object);
        }
        if (status < 0) return -1;
    }
    *reinterpret_cast<long long*>(bytes + 200) = GetTime__7VIScene(scene);
    return 0;
}
}
