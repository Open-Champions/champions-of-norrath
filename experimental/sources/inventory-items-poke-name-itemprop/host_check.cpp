#include <cstdio>
#include <cstring>
#include <string>

extern "C" int msg_getPokeName__8ItemPropPUsi(void* object, unsigned short* text, int);
extern "C" int frameNm = 0x1234;

static std::string log_text;
static const unsigned short pickup_result[2] = {'P', 0};
static const unsigned short name_result[2] = {'N', 0};
static unsigned short* name_description;
static int name_mode = -1;
static const char* key_seen;
static unsigned short* dest_seen;

extern "C" unsigned short* textGetString__FPc(const char* key) {
    key_seen = key;
    log_text += "T";
    return const_cast<unsigned short*>(pickup_result);
}
extern "C" void ustrcat__FPUsT0(unsigned short* destination, const unsigned short* source) {
    dest_seen = destination;
    log_text += source == pickup_result ? "[pick]" : source == name_result ? "[name]" : "[?]";
}
extern "C" void ustrcat__FPUsPc(unsigned short* destination, const char* source) {
    dest_seen = destination;
    log_text += std::string("<") + source + ">";
}
extern "C" unsigned short* getName__4itemPQ24item11descriptioni(void* description, int mode) {
    name_description = static_cast<unsigned short*>(description);
    name_mode = mode;
    log_text += "G";
    return const_cast<unsigned short*>(name_result);
}
extern "C" int sprintf(char* buffer, const char* format, ...);

int main() {
    unsigned char object[400];
    std::memset(object, 0, sizeof object);
    unsigned short text[16] = {};
    short count = 1;
    std::memcpy(object + 332, &count, 2);
    if (msg_getPokeName__8ItemPropPUsi(object, text, 77) != 1) return 1;
    int stored;
    std::memcpy(&stored, object + 260, 4);
    if (stored != 0x1234) return 2;
    if (!key_seen || std::strcmp(key_seen, "PickUp") != 0) return 3;
    if (static_cast<void*>(name_description) != object + 328 || name_mode != 1) return 4;
    if (dest_seen != text) return 5;
    if (log_text != "T[pick]< >G[name]") return 6;
    log_text.clear();
    count = 2;
    std::memcpy(object + 332, &count, 2);
    frameNm = 99;
    msg_getPokeName__8ItemPropPUsi(object, text, 0);
    if (log_text != "T[pick]< >G[name]< x 2>") return 7;
    std::memcpy(&stored, object + 260, 4);
    if (stored != 99) return 8;
    log_text.clear();
    count = -3;                                  // negative counts are not shown
    std::memcpy(object + 332, &count, 2);
    msg_getPokeName__8ItemPropPUsi(object, text, 0);
    if (log_text != "T[pick]< >G[name]") return 9;
    log_text.clear();
    count = 300;
    std::memcpy(object + 332, &count, 2);
    msg_getPokeName__8ItemPropPUsi(object, text, 0);
    if (log_text != "T[pick]< >G[name]< x 300>") return 10;
    return 0;
}
