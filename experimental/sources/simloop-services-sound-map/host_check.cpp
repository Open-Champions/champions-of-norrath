#include <cstdio>
#include <cstring>
extern "C" int nmSounds = 0;
extern "C" unsigned char soundList[48] = {};
static int compares = 0;
extern "C" int strcasecmp(const char* left, const char* right) {
    compares += 1;
    while (*left != 0 && *right != 0) {
        unsigned char a = static_cast<unsigned char>(*left);
        unsigned char b = static_cast<unsigned char>(*right);
        if (a >= 'A' && a <= 'Z') a = static_cast<unsigned char>(a + 32);
        if (b >= 'A' && b <= 'Z') b = static_cast<unsigned char>(b + 32);
        if (a != b) return static_cast<int>(a) - static_cast<int>(b);
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" int soundMapSound__FPci(char*, int);
static char first[] = "miss";
static char second[] = "Hit";
extern "C" const char* norrath_host_sound_name(unsigned word) { return word == 1u ? first : second; }
static void put_ptr(int index, unsigned value) {
    unsigned char* slot = soundList + index * 16;
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    if (soundMapSound__FPci(const_cast<char*>("hit"), 9) != 0) return 1;
    put_ptr(0, 1u);
    put_ptr(1, 2u);
    soundList[4] = 3;
    soundList[16 + 4] = 8;
    nmSounds = 2;
    if (soundMapSound__FPci(const_cast<char*>("hit"), 0) != 8 || compares != 2) return 2;
    std::puts("sound map contract passed");
    return 0;
}
