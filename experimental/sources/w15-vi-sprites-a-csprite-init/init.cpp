// Observed: Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi at 0x010b27c0, 180 bytes. Arguments (this, skeleton, race,
// sex, scale, flag); the three enumerations are plain integers and the scale is a float. It calls Clear__9VICSprite(this) and
// Init__9VIHSprite(this); a negative result from either returns -1 (the second is not called after a failed first). Otherwise it sets bit 0
// of the word at this+400 when the flag is non-zero and clears it when the flag is zero, and stores the sex at this+412, the scale at
// this+208 (float), the skeleton at this+404 and the race at this+408. It returns zero.
// Inferred: this+400 is a flags word whose bit 0 is the last argument; +404, +408 and +412 hold the three classification arguments in the
// order of their parameters.
// Unknown: what the flag bit means, and what the +208 float is (Clear__9VIHSprite also resets it to 1.0).
extern "C" {
int Clear__9VICSprite(void* object);
int Init__9VIHSprite(void* object);

int Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi(void* object, int skeleton, int race, int sex, float scale, int flag) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (Clear__9VICSprite(object) < 0) return -1;
    if (Init__9VIHSprite(object) < 0) return -1;
    unsigned flags = *reinterpret_cast<unsigned*>(bytes + 400);
    if (flag) flags |= 1u;
    else flags &= ~1u;
    *reinterpret_cast<unsigned*>(bytes + 400) = flags;
    *reinterpret_cast<int*>(bytes + 412) = sex;
    *reinterpret_cast<float*>(bytes + 208) = scale;
    *reinterpret_cast<int*>(bytes + 404) = skeleton;
    *reinterpret_cast<int*>(bytes + 408) = race;
    return 0;
}
}
