#include "sound_device_init.cpp"

#include <cstring>
#include <string>
#include <vector>

static unsigned char* object_bytes;
static std::vector<std::string> trace;

static std::string at(const void* p) {
    return std::to_string(static_cast<const unsigned char*>(p) - object_bytes);
}

extern "C" void Init__7VILList(VILList* list) { trace.push_back("I" + at(list)); }
extern "C" void AddTail__7VILListP7VILNode(VILList* list, VILNode* node) {
    trace.push_back("A" + at(list) + ">" + at(node));
}

int main() {
    alignas(8) static unsigned char object[4600];
    std::memset(object, 0xa5, sizeof object);
    object_bytes = object;
    if (Init__13VISoundDevice(reinterpret_cast<VISoundDevice*>(object)) != 0) return 1;

    std::vector<std::string> expected = {"I3284", "I3296"};
    for (int i = 0; i < 46; ++i) expected.push_back("A3296>" + std::to_string(8 + 56 * i));
    if (trace != expected) return 2;

    unsigned char want[4600];
    std::memset(want, 0xa5, sizeof want);
    auto put32 = [&](int off, unsigned v) { std::memcpy(want + off, &v, 4); };
    float one = 1.0f;
    unsigned one_bits;
    std::memcpy(&one_bits, &one, 4);
    std::memset(want + 0, 0, 8);
    std::memset(want + 2584, 0, 512);
    std::memset(want + 3100, 0, 184);
    put32(3096, 0);
    put32(3308, 0x16062);
    put32(3384, 0);
    put32(3404, 0);
    put32(3408, 0);
    put32(3412, 1);
    put32(3420, 0);
    put32(3424, one_bits);
    put32(3428, one_bits);
    put32(3432, 1);
    want[3436] = want[3692] = want[3948] = want[4204] = 0;
    put32(4460, 0);
    put32(4464, 0);
    put32(4468, one_bits);
    put32(4472, 0);
    for (int i = 0; i < 4600; ++i)
        if (object[i] != want[i]) return 3;
    return 0;
}
