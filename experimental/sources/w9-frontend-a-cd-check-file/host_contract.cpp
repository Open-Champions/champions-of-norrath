#include "cd_check_file.cpp"

#include <cstdio>
#include <cstring>

unsigned char* currentLayerDir;
// Only the lower-case flag (bit 2) of the real table matters to this function.
extern "C" const unsigned char _ctype_[257] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0};

extern "C" int sprintf(char* buffer, const char* format, ...) {
    if (std::strcmp(format, "%s;1") != 0) return -1;
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const char* name = __builtin_va_arg(args, const char*);
    __builtin_va_end(args);
    std::strcpy(buffer, name);
    std::strcat(buffer, ";1");
    return static_cast<int>(std::strlen(buffer));
}
extern "C" int strcmp(const char* left, const char* right) { return std::memcmp(left, right, std::strlen(left) + 1) == 0 ? 0 : 1; }

int main() {
    static unsigned char table[0x6004];
    std::memcpy(table, "ABC.BIN;1", 10);
    std::memcpy(table + 32, "HELLO.TXT;1", 12);
    currentLayerDir = table;
    int count = 2;
    std::memcpy(table + 0x6000, &count, 4);
    if (cdCheckForFileInCurrentLayer__FPc(const_cast<char*>("abc.bin")) != 1) return 1;
    if (cdCheckForFileInCurrentLayer__FPc(const_cast<char*>("Hello.TXT")) != 1) return 2;
    if (cdCheckForFileInCurrentLayer__FPc(const_cast<char*>("HELLO.TXT")) != 1) return 3;
    if (cdCheckForFileInCurrentLayer__FPc(const_cast<char*>("other")) != 0) return 4;
    count = 1;
    std::memcpy(table + 0x6000, &count, 4);
    if (cdCheckForFileInCurrentLayer__FPc(const_cast<char*>("hello.txt")) != 0) return 5;
    count = 0;
    std::memcpy(table + 0x6000, &count, 4);
    if (cdCheckForFileInCurrentLayer__FPc(const_cast<char*>("abc.bin")) != 0) return 6;
    std::puts("cd check file passed");
    return 0;
}
