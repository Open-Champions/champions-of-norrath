// Host contract for doNewWorld__FPc. Not an EE plan input.
#define strcpy host_strcpy
#define strlen host_strlen
#define strcat host_strcat
#define strncmp host_strncmp
#define atoi host_atoi
#include "do_new_world.cpp"

char levelName[322];
unsigned char* currentRunningObject = 0;

static char trace[512];
static int traceLength = 0;
static void note(const char* text) {
    while (*text) trace[traceLength++] = *text++;
    trace[traceLength] = 0;
}

extern "C" char* strcpy(char* to, const char* from) {
    char* start = to;
    while ((*to++ = *from++)) {}
    return start;
}
extern "C" unsigned strlen(const char* text) { unsigned n = 0; while (text[n]) ++n; return n; }
extern "C" char* strcat(char* to, const char* from) {
    char* start = to;
    while (*to) ++to;
    while ((*to++ = *from++)) {}
    note("c");
    return start;
}
extern "C" int strncmp(const char* a, const char* b, unsigned count) {
    for (unsigned i = 0; i < count; ++i) {
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        if (!a[i]) return 0;
    }
    return 0;
}
extern "C" int atoi(const char* text) {
    int sign = 1, v = 0;
    if (*text == '-') { sign = -1; ++text; }
    while (*text >= '0' && *text <= '9') v = v * 10 + (*text++ - '0');
    return sign * v;
}

int main() {
    // No running object: marker byte -1.
    levelName[80] = 5;
    doNewWorld__FPc((char*)"abc");
    if (host_strncmp(levelName, "abc", 4) != 0 || (signed char)levelName[80] != -1) return 1;
    // Object with no matching argument.
    char* none[] = {(char*)"x=1", (char*)"orient=2", 0};
    unsigned char object[64] = {};
    *(char***)(object + 44) = none;
    currentRunningObject = object;
    doNewWorld__FPc((char*)"lvl");
    if ((signed char)levelName[80] != -1 || host_strncmp(levelName, "lvl", 4) != 0) return 2;
    // Matching argument; the last match wins and the walk continues past it.
    char* hit[] = {(char*)"a", (char*)"orientation=3", (char*)"b", (char*)"orientation=200", (char*)"c", 0};
    *(char***)(object + 44) = hit;
    doNewWorld__FPc((char*)"two");
    if ((unsigned char)levelName[80] != 200) return 3;
    char* one[] = {(char*)"orientation=42", 0};
    *(char***)(object + 44) = one;
    traceLength = 0; trace[0] = 0;
    doNewWorld__FPc((char*)"x");
    if (levelName[80] != 42 || traceLength != 0) return 4;
    // Empty list.
    char* empty[] = {0};
    *(char***)(object + 44) = empty;
    doNewWorld__FPc((char*)"");
    if ((signed char)levelName[80] != -1 || levelName[0] != 0) return 5;
    // Negative value keeps the low byte.
    char* neg[] = {(char*)"orientation=-3", 0};
    *(char***)(object + 44) = neg;
    doNewWorld__FPc((char*)"n");
    if ((signed char)levelName[80] != -3) return 6;
    // Not found: strcat is called twice for a nonempty name, once for an empty name; found: never.
    traceLength = 0; trace[0] = 0;
    *(char***)(object + 44) = none;
    doNewWorld__FPc((char*)"q");
    if (traceLength != 2) return 7;
    traceLength = 0; trace[0] = 0;
    doNewWorld__FPc((char*)"");
    if (traceLength != 1) return 8;
    return 0;
}
