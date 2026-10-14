#include "sound_device_ctor.cpp"

#include <cstring>
#include <string>
#include <vector>

static unsigned char* object_bytes;
static std::vector<std::string> trace;

static std::string at(const void* p) {
    return std::to_string(static_cast<const unsigned char*>(p) - object_bytes);
}

extern "C" void __7VILNode(VILNode* node) { trace.push_back("N" + at(node)); }
extern "C" void __7VILList(VILList* list) { trace.push_back("L" + at(list)); }
extern "C" void Init__7VILList(VILList* list) { trace.push_back("I" + at(list)); }
extern "C" void AddTail__7VILListP7VILNode(VILList* list, VILNode* node) {
    trace.push_back("A" + at(list) + ">" + at(node));
}

int main() {
    alignas(8) static unsigned char object[4600];
    std::memset(object, 0xa5, sizeof object);
    object_bytes = object;
    __13VISoundDevice(reinterpret_cast<VISoundDevice*>(object));

    std::vector<std::string> expected;
    for (int i = 0; i < 46; ++i) expected.push_back("N" + std::to_string(8 + 56 * i));
    for (int off : {3284, 3296, 3312, 3324, 3340, 3352}) expected.push_back("L" + std::to_string(off));
    expected.push_back("I3296");
    for (int i = 0; i < 46; ++i)
        expected.push_back("A3296>" + std::to_string(8 + 56 * i));
    if (trace != expected) return 1;

    // Bytes that must have been written, and the value each should hold.
    unsigned char want[4600];
    std::memset(want, 0xa5, sizeof want);
    auto put32 = [&](int off, unsigned v) { std::memcpy(want + off, &v, 4); };
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
    put32(4472, 0);
    want[3948] = 0;
    for (int i = 0; i < 4600; ++i)
        if (object[i] != want[i]) return 2;
    return 0;
}
