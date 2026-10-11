#include <cstdio>

extern "C" int strcmp(const char* left, const char* right);

static int call(const char* left, const char* right) {
    auto* function = strcmp;
    __asm__ __volatile__("" : "+r"(function));
    return function(left, right);
}

static int expect(const char* left, const char* right, int difference) {
    return call(left, right) == difference ? 0 : 1;
}

int main() {
    if (expect("", "", 0)) return 1;
    if (expect("ab", "ab", 0)) return 2;
    if (expect("--net", "--net", 0)) return 3;
    if (expect("--mainmenu", "--main", int('m') - 0)) return 4;
    if (expect("--main", "--mainmenu", 0 - int('m'))) return 5;
    if (expect("abc", "abd", int('c') - int('d'))) return 6;
    if (expect("ab", "abc", 0 - int('c'))) return 7;
    char high[] = {'\x80', '\0'};
    char low[] = {'\x7f', '\0'};
    char full[] = {'\xff', '\0'};
    if (expect(high, low, 0x80 - 0x7f)) return 8;
    if (expect(low, high, 0x7f - 0x80)) return 9;
    if (expect(full, "", 0xff)) return 10;
    if (expect("", full, -0xff)) return 11;
    char long_left[64];
    char long_right[64];
    for (int i = 0; i < 63; ++i) {
        long_left[i] = static_cast<char>('A' + (i % 26));
        long_right[i] = long_left[i];
    }
    long_left[63] = 0;
    long_right[63] = 0;
    if (expect(long_left, long_right, 0)) return 12;
    long_right[40] = 'Z';
    if (expect(long_left, long_right, int(static_cast<unsigned char>(long_left[40])) - int('Z'))) return 13;
    long_left[0] = static_cast<char>(0xff);
    long_right[0] = 1;
    if (expect(long_left, long_right, 0xff - 1)) return 14;
    std::printf("strcmp cases passed\n");
    return 0;
}
