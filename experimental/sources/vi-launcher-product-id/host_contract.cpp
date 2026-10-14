#define strcpy norrath_host_strcpy
#include "product_id.cpp"
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
    if (sizeof sProductId != 32) return 1;
    if (std::strcmp(sProductId, "BASLUS-20470EQONLINE") != 0) return 2;
    if (sProductId[21] != '\0' || sProductId[31] != '\0') return 3;
    const char* text = "BASLUS-20565EQONLINE";
    VIMemCardSetProductId__FPCc(text);
    if (calls != 1 || seen_destination != sProductId || seen_source != text) return 4;
    if (std::strcmp(sProductId, text) != 0) return 5;
    return 0;
}
