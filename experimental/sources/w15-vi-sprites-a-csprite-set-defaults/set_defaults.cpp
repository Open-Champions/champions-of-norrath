// Observed: SetDefaults__9VICSpriteR7VIScene at 0x010b1690, 424 bytes. Arguments (this, scene); every failure returns -1 at once. Steps, in order:
// store zero at this+4492; call AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(this, word +424, word +436, float +432, scene), store the result at
// +4496 (even when negative) and return -1 if negative. When the word at +404 is 1, store zero at +4508 and do the same with the word at +428,
// storing the result at +4512. Then call AttachSkin__9VIHSpriteiR7VIScene(this, word +3856, scene), store the result at +4412 (even when
// negative) and return -1 if negative. Then store at +4528 the 64-bit sum GetTime__7VIScene(scene) + (rand() % 20000, signed remainder) + 20000
// (GetTime is called before rand). Floats are stored as 0.0 at +4424, +4428, +4432, +4436, +4444; 1.0 at +4440; 2.0 at +4448; 0.5 at +4452. If the
// word at scene+19356 is zero it returns zero. Otherwise it returns zero when SetHair__9VICSpriteiiiR7VIScene(this, 0, 2, 1, scene) is
// non-negative and -1 when it is negative.
// Inferred: +424/+428 are animation ids with the playback type at +436 and a rate at +432, +4496/+4512 receive the play ids, and +4528 is a
// next-event time in the scene's clock a random 0..19999 units plus 20000 ahead of now (the unit is not known). The floats at +4424.. are default
// vectors (a zero vector, the vector (0, 1, 0) and two scalars).
// Unknown: what +4528 schedules, what the condition at scene+19356 is, and the meaning of SetHair's arguments (0, 2, 1).
extern "C" {
int AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(void* object, int id, int playback, float value, void* scene);
int AttachSkin__9VIHSpriteiR7VIScene(void* object, int id, void* scene);
long long GetTime__7VIScene(void* scene);
int rand(void);
int SetHair__9VICSpriteiiiR7VIScene(void* object, int a, int b, int c, void* scene);

int SetDefaults__9VICSpriteR7VIScene(void* object, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<int*>(bytes + 4492) = 0;
    int id = AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(object, *reinterpret_cast<int*>(bytes + 424), *reinterpret_cast<int*>(bytes + 436),
                                                              *reinterpret_cast<float*>(bytes + 432), scene);
    *reinterpret_cast<int*>(bytes + 4496) = id;
    if (id < 0) return -1;
    if (*reinterpret_cast<int*>(bytes + 404) == 1) {
        *reinterpret_cast<int*>(bytes + 4508) = 0;
        id = AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(object, *reinterpret_cast<int*>(bytes + 428), *reinterpret_cast<int*>(bytes + 436),
                                                              *reinterpret_cast<float*>(bytes + 432), scene);
        *reinterpret_cast<int*>(bytes + 4512) = id;
        if (id < 0) return -1;
    }
    id = AttachSkin__9VIHSpriteiR7VIScene(object, *reinterpret_cast<int*>(bytes + 3856), scene);
    *reinterpret_cast<int*>(bytes + 4412) = id;
    if (id < 0) return -1;
    long long now = GetTime__7VIScene(scene);
    int roll = rand();
    *reinterpret_cast<float*>(bytes + 4424) = 0.0f;
    *reinterpret_cast<float*>(bytes + 4428) = 0.0f;
    *reinterpret_cast<float*>(bytes + 4432) = 0.0f;
    *reinterpret_cast<float*>(bytes + 4436) = 0.0f;
    *reinterpret_cast<float*>(bytes + 4440) = 1.0f;
    *reinterpret_cast<float*>(bytes + 4444) = 0.0f;
    *reinterpret_cast<float*>(bytes + 4448) = 2.0f;
    *reinterpret_cast<float*>(bytes + 4452) = 0.5f;
    *reinterpret_cast<long long*>(bytes + 4528) = now + roll % 20000 + 20000;
    if (*reinterpret_cast<int*>(static_cast<unsigned char*>(scene) + 19356) == 0) return 0;
    return SetHair__9VICSpriteiiiR7VIScene(object, 0, 2, 1, scene) >= 0 ? 0 : -1;
}
}
