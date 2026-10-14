#include <cstdio>
#include <cstring>
static unsigned char dialog[160];
static int finds = 0;
static int starts = 0;
static int start_index = -1;
static int start_flags = -1;
static unsigned start_begin = 0;
static unsigned start_length = 0;
static int start_half = 0;
static const char* start_name = nullptr;
extern "C" unsigned char* soundFindDialog__FPc(char*) {
    finds += 1;
    return finds == 1 ? nullptr : dialog;
}
extern "C" char* soundGetDialogFilename__Fv() { return const_cast<char*>("dialog.bin"); }
extern "C" int soundStartStream__FiPciiiiii(int index, char* filename, int flags, unsigned begin, unsigned length,
                                            unsigned, int half, unsigned) {
    starts += 1;
    start_index = index;
    start_name = filename;
    start_flags = flags;
    start_begin = begin;
    start_length = length;
    start_half = half;
    return 42;
}
extern "C" int soundStartDialogStream__FiPci(int, char*, int);
static void put(int offset, unsigned value) {
    dialog[offset] = static_cast<unsigned char>(value);
    dialog[offset + 1] = static_cast<unsigned char>(value >> 8);
    dialog[offset + 2] = static_cast<unsigned char>(value >> 16);
    dialog[offset + 3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    std::memset(dialog, 0, sizeof dialog);
    if (soundStartDialogStream__FiPci(1, const_cast<char*>("none"), 0) != 0 || starts != 0) return 1;
    put(64, 10);
    put(144, 10);
    if (soundStartDialogStream__FiPci(1, const_cast<char*>("same"), 0) == 42 || starts != 0) return 2;
    put(144, 30);
    put(68, 5);
    dialog[72] = 0xfe;
    dialog[73] = 0xff;
    put(76, 9);
    if (soundStartDialogStream__FiPci(2, const_cast<char*>("line"), 0x22) != 42) return 3;
    if (start_index != 2 || start_flags != 0x22 || start_begin != 10u || start_length != 20u) return 4;
    if (start_half != -2 || std::strcmp(start_name, "dialog.bin") != 0) return 5;
    std::puts("dialog stream contract passed");
    return 0;
}
