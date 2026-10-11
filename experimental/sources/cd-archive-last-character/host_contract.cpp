#include <cstdio>
extern "C" char* norrath_archive_last_character(const char* text, int character);
int main() {
    char name[] = "ab.c.def";
    char* dot = norrath_archive_last_character(name, 46);
    if (dot != name + 4) return 1;
    if (norrath_archive_last_character(name, '.') != name + 4) return 2;
    if (norrath_archive_last_character("noext", '.') != nullptr) return 3;
    char empty[] = "";
    if (norrath_archive_last_character(empty, '.') != nullptr) return 4;
    if (norrath_archive_last_character(empty, 0) != empty) return 5;
    if (norrath_archive_last_character(name, 0) != name + 8) return 6;
    char high[] = {'a', static_cast<char>(0xff), 'a', static_cast<char>(0xff), 0};
    if (norrath_archive_last_character(high, 0xff) != high + 3) return 7;
    if (norrath_archive_last_character(high, -1) != high + 3) return 8;
    char dots[] = "...";
    if (norrath_archive_last_character(dots, '.') != dots + 2) return 9;
    if (norrath_archive_last_character("abc", 0x12e) != norrath_archive_last_character("abc", '.')) return 11;
    std::puts("last character contract passed");
    return 0;
}
