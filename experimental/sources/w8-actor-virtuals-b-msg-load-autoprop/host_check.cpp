#include <cstdio>
#include <cstring>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void set_state_stub(void* self, int state);
extern "C" void* norrath_w8avb_host_function(unsigned word) {
    return word == 11 ? reinterpret_cast<void*>(&set_state_stub) : nullptr;
}
#include "msg_load.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char memory[1024];
static int calls;
static void* seen;
static int seen_state;
static int stored_at_call;
static void set_state_stub(void* self, int state) {
    ++calls;
    seen = self;
    seen_state = state;
    std::memcpy(&stored_at_call, memory + 376, 4);
}

int main() {
    std::memset(memory, 0x44, sizeof memory);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    unsigned table = 512;
    std::memcpy(memory + 72, &table, 4);
    short adjust = 16;
    std::memcpy(memory + 512 + 144, &adjust, 2);
    unsigned function = 11;
    std::memcpy(memory + 512 + 144 + 4, &function, 4);
    unsigned other = 5;
    std::memcpy(memory + 512 + 136 + 4, &other, 4);  // the advance entry must not be used
    alignas(8) unsigned char stream[16];
    std::memset(stream, 0x2e, sizeof stream);
    int state = -3;
    std::memcpy(stream, &state, 4);
    int used = msg_load__8AutoPropPc(memory, reinterpret_cast<const char*>(stream));
    expect(used == 4, "consumes four bytes");
    expect(calls == 1, "one slot call");
    expect(seen == memory + 16, "object address is adjusted");
    expect(seen_state == -3, "the stream word is the argument");
    expect(stored_at_call == -3, "the word is stored before the call");
    expect(memory[375] == 0x44 && memory[380] == 0x44, "neighbouring bytes untouched");
    return failures ? 1 : 0;
}
