#include <cstdio>
#include <cstring>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void* slot_stub_address;
extern "C" void* norrath_w8avb_host_function(unsigned word) {
    return word == 5 ? slot_stub_address : nullptr;
}
#include "run.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char memory[2048];
static std::vector<int> order;  // 1 = slot call, 2 = base call
static void* slot_self;
static Triple slot_triple;
static void* base_self;

static void slot_stub(void* self, Triple t) {
    order.push_back(1);
    slot_self = self;
    slot_triple = t;
    // writing to the by-value copy must not reach the object
    t.a = 0xdeadbeef;
}
extern "C" void msg_run__13AutoContainer(void* object) {
    order.push_back(2);
    base_self = object;
}

static void put(unsigned off, unsigned value) { std::memcpy(memory + off, &value, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, memory + off, 4); return v; }

static void setup(unsigned other_word) {
    std::memset(memory, 0x5a, sizeof memory);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    put(16, 0x11111111u);
    put(20, 0x22222222u);
    put(24, 0x33333333u);
    put(328, other_word);
    put(1024 + 72, 1536);  // the second object's table
    short adjust = -40;
    std::memcpy(memory + 1536 + 144, &adjust, 2);
    put(1536 + 144 + 4, 5);
    put(1536 + 136 + 4, 9);  // the neighbouring entry must not be used
    order.clear();
    slot_self = base_self = nullptr;
}

int main() {
    slot_stub_address = reinterpret_cast<void*>(&slot_stub);
    setup(1024);
    msg_run__15AutoContainerLP(memory);
    expect(order.size() == 2 && order[0] == 1 && order[1] == 2, "slot call, then the base method");
    expect(slot_self == memory + 1024 - 40, "second object address is adjusted");
    expect(slot_triple.a == 0x11111111u && slot_triple.b == 0x22222222u && slot_triple.c == 0x33333333u, "the three words at +16 are passed");
    expect(base_self == memory, "base method gets the original object");
    expect(get(16) == 0x11111111u && get(20) == 0x22222222u && get(24) == 0x33333333u, "the object's words are not modified");
    setup(0);
    msg_run__15AutoContainerLP(memory);
    expect(order.size() == 1 && order[0] == 2 && base_self == memory, "null second object: only the base method");
    return failures ? 1 : 0;
}
