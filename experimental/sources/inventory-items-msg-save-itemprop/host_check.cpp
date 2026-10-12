#include <cstring>

extern "C" int msg_save__8ItemPropPc(void* object, unsigned char* buffer);

static int order[4], steps;
static void* packed;
static void* unpacked;
static unsigned char* prop_buffer;
static unsigned char* prop_object;
static int prop_result = 5;
static unsigned char at_pack[72];

extern "C" void pack__4itemPQ24item11description(void* description) {
    order[steps++] = 1;
    packed = description;
    // The copy must see the packed form: change the bytes here.
    unsigned char* p = static_cast<unsigned char*>(description);
    for (int i = 0; i < 72; ++i) p[i] = static_cast<unsigned char>(p[i] ^ 0x5a);
}
extern "C" void unpack__4itemPQ24item11description(void* description) {
    order[steps++] = 3;
    unpacked = description;
    unsigned char* p = static_cast<unsigned char*>(description);
    for (int i = 0; i < 72; ++i) p[i] = static_cast<unsigned char>(p[i] ^ 0x5a);
}
extern "C" int msg_save__4PropPc(void* object, unsigned char* buffer) {
    order[steps++] = 4;
    prop_object = static_cast<unsigned char*>(object);
    prop_buffer = buffer;
    return prop_result;
}

int main() {
    unsigned char object[400];
    unsigned char out[200];
    for (int i = 0; i < 400; ++i) object[i] = static_cast<unsigned char>(i);
    std::memset(out, 0xee, sizeof out);
    std::memcpy(at_pack, object + 328, 72);
    int n = msg_save__8ItemPropPc(object, out);
    if (n != 77) return 1;
    if (steps != 3 || order[0] != 1 || order[1] != 3 || order[2] != 4) return 2;
    if (packed != object + 328 || unpacked != object + 328) return 3;
    if (prop_object != object || prop_buffer != out + 72) return 4;
    for (int i = 0; i < 72; ++i) if (out[i] != static_cast<unsigned char>(at_pack[i] ^ 0x5a)) return 5;   // copied after pack
    for (int i = 0; i < 72; ++i) if (object[328 + i] != at_pack[i]) return 6;                              // restored by unpack
    for (int i = 72; i < 200; ++i) if (out[i] != 0xee) return 7;
    prop_result = 0;
    steps = 0;
    if (msg_save__8ItemPropPc(object, out) != 72) return 8;
    return 0;
}
