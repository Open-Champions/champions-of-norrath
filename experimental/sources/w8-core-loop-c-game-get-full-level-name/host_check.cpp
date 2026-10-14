// Host contract for gameGetFullLevelName. Not an EE plan input.
// The two library calls are renamed so the host C library is not redefined.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#define sprintf hostSprintf
#define strcpy hostStrcpy
#include "game_get_full_level_name.cpp"
#undef sprintf
#undef strcpy

extern "C" {
char levelName[322];
static int sprintfCalls = 0, strcpyCalls = 0;
int hostSprintf(char* destination, const char* format, ...) {
    ++sprintfCalls;
    va_list args;
    va_start(args, format);
    int n = std::vsnprintf(destination, 200, format, args);
    va_end(args);
    return n;
}
char* hostStrcpy(char* destination, const char* source) {
    ++strcpyCalls;
    char* r = destination;
    while ((*destination++ = *source++) != 0) {}
    return r;
}
}

int main() {
    char out[200];
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName + 0, "decoy");
    std::strcpy(levelName + 81, "town");
    std::memset(out, 'x', sizeof out);
    gameGetFullLevelName__FPc(out);
    if (std::strcmp(out, "town") != 0 || strcpyCalls != 1 || sprintfCalls != 0) return 1;
    std::strcpy(levelName + 161, "night");
    gameGetFullLevelName__FPc(out);
    if (std::strcmp(out, "town_night") != 0 || strcpyCalls != 1 || sprintfCalls != 1) return 2;
    // A byte at offset 160 does not select the two-field form.
    std::memset(levelName + 161, 0, 20);
    levelName[160] = 'z';
    gameGetFullLevelName__FPc(out);
    if (std::strcmp(out, "town") != 0 || strcpyCalls != 2) return 3;
    return 0;
}
