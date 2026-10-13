// Host contract for msg_collision__17VehicleBodySphereP10GameObject; not an EE input. 32-bit words are
// offsets from a host base and the table call is logged.
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2) {
    calls += "call(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) +
             "," + std::to_string(argument_count) + "," + std::to_string(a1) + ");";
    return 7;
}
#include "msg_collision.cpp"

int main() {
    static unsigned char arena[0x1000] __attribute__((aligned(16)));
    w12_spell_c_host_base = arena;
    unsigned char* sphere = arena + 0x100;
    unsigned char* owner = arena + 0x400;
    unsigned char* table = arena + 0x800;
    unsigned char* other = arena + 0xc00;
    auto put32 = [](unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); };
    put32(sphere + 76, static_cast<unsigned>(owner - arena));
    put32(owner + 72, static_cast<unsigned>(table - arena));
    short adjust = -24;
    std::memcpy(table + 48, &adjust, 2);
    put32(table + 52, 0x1234);
    put32(table + 20, 0x9999);   // a decoy entry at another byte
    int bad = 0;
    msg_collision__17VehicleBodySphereP10GameObject(sphere, other);
    const std::string want = "call(4660," + std::to_string(owner - arena - 24) + ",1," + std::to_string(other - arena) + ");";
    if (calls != want) { std::puts(calls.c_str()); std::puts("fail: forwarded call"); bad = 1; }
    return bad;
}
