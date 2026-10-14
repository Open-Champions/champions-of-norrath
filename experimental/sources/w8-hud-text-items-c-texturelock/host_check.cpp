struct Entry { unsigned char before[8]; unsigned short lockCount; unsigned char after[2]; };
struct Tex { unsigned char before[24]; short cached; short cacheIndex; };
extern "C" Entry cache[8];
extern "C" void textureLock__FP8_texture(Tex*);
Entry cache[8];

int main() {
    for (int i = 0; i < 8; i++) cache[i].lockCount = 10 + i;
    Tex t = {};
    t.cacheIndex = 5;
    textureLock__FP8_texture(&t);
    for (int i = 0; i < 8; i++) if (cache[i].lockCount != 10 + i) return 1;
    t.cached = 1;
    textureLock__FP8_texture(&t);
    for (int i = 0; i < 8; i++)
        if (cache[i].lockCount != (i == 5 ? 16 : 10 + i)) return 2;
    cache[5].lockCount = 0xffff;
    textureLock__FP8_texture(&t);
    if (cache[5].lockCount != 0) return 3;
    return 0;
}
