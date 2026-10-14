#define strcpy norrath_host_strcpy
#include "product_title.cpp"
#undef strcpy

#include <cstring>

static int calls;
static char* seen_destination;
static const char* seen_source;

extern "C" char* norrath_host_strcpy(char* destination, const char* source) {
    ++calls;
    seen_destination = destination;
    seen_source = source;
    char* cursor = destination;
    while ((*cursor++ = *source++) != '\0') {
    }
    return destination;
}

int main() {
    if (sizeof sProductTitle != 64) return 1;
    if (std::strcmp(sProductTitle, "EverQuest Online\nAdventures") != 0) return 2;
    if (sProductTitle[27] != '\0' || sProductTitle[63] != '\0') return 3;
    const char* text = "other";
    VIMemCardSetProductTitle__FPCc(text);
    if (calls != 1 || seen_destination != sProductTitle || seen_source != text) return 4;
    if (std::strcmp(sProductTitle, "other") != 0) return 5;
    return 0;
}
