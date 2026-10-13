extern "C" void init_ramps(void*) asm("initRamps__18SkillDiseaseEffect");
int main() {
    alignas(8) unsigned char bytes[80];
    for (unsigned i = 0; i < 80; ++i) bytes[i] = 0xa5;
    init_ramps(bytes);
    for (unsigned i = 0; i < 80; ++i) {
        unsigned char expected = 0xa5;
        if (i == 14 || i == 15 || i == 56 || i == 57 || i == 66 || i == 67) expected = i % 2 == 0 ? 1 : 0;
        if (i == 24) expected = 3;
        if (i == 25) expected = 0;
        if ((i >= 36 && i <= 39) || (i >= 76 && i <= 79)) {
            const unsigned char one[4] = {0, 0, 0x80, 0x3f};
            expected = one[i >= 76 ? i - 76 : i - 36];
        }
        if (bytes[i] != expected) return 1;
    }
    return 0;
}
