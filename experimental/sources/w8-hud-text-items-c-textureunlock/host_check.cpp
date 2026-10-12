struct Entry { unsigned char before[8]; unsigned short lockCount; unsigned char after[2]; };
struct Tex { unsigned char before[26]; short cacheIndex; };
extern "C" Entry cache[8];
extern "C" void textureUnlock__FP8_texture(Tex*);
Entry cache[8];

int main() {
    for (int i = 0; i < 8; i++) cache[i].lockCount = 10 + i;
    Tex t = {};
    t.cacheIndex = 5;
    textureUnlock__FP8_texture(&t);
    for (int i = 0; i < 8; i++)
        if (cache[i].lockCount != (i == 5 ? 14 : 10 + i)) return 1;
    cache[5].lockCount = 0;
    textureUnlock__FP8_texture(&t);
    if (cache[5].lockCount != 0xffff) return 2;
    return 0;
}
