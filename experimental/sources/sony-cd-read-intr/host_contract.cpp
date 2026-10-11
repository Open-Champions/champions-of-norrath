#include <cstdio>
#include <map>
#define NORRATH_HOST_CONTRACT 1
#include "read_intr.cpp"

static std::map<unsigned, unsigned char> memory;
static void* callback_argument = nullptr;
extern "C" int sceCdCbfunc_num = 19;
extern "C" unsigned norrath_sony_cd_load_word(unsigned address) {
    unsigned value = 0;
    for (int byte = 0; byte < 4; ++byte) value |= static_cast<unsigned>(memory[address + byte]) << (8 * byte);
    return value;
}
extern "C" unsigned char norrath_sony_cd_load_byte(unsigned address) { return memory[address]; }
extern "C" void norrath_sony_cd_store_byte(unsigned address, unsigned char value) { memory[address] = value; }
extern "C" void _sceCd_cd_callback(void* argument) { callback_argument = argument; }
static void put_word(unsigned address, unsigned value) {
    for (int byte = 0; byte < 4; ++byte) memory[address + byte] = static_cast<unsigned char>(value >> (8 * byte));
}
int main() {
    const unsigned header = 0x20000010u;
    put_word(header, 2);
    put_word(header + 4, 1);
    put_word(header + 8, 0x1000u);
    put_word(header + 12, 0x2000u);
    memory[header + 16] = 0xab;
    memory[header + 17] = 0xcd;
    memory[header + 80] = 0x11;
    auto* argument = reinterpret_cast<void*>(static_cast<std::uintptr_t>(header & ~0x20000000u));
    _sceCd_cd_read_intr(argument);
    if (memory[0x20001000u] != 0xab || memory[0x1000u] != 0xab) return 1;
    if (memory[0x20001001u] != 0xcd || memory[0x1001u] != 0xcd) return 2;
    if (memory[0x20002000u] != 0x11 || memory[0x2000u] != 0x11) return 3;
    if (callback_argument != &sceCdCbfunc_num) return 4;
    memory.clear();
    put_word(header, 0);
    put_word(header + 4, -1);
    callback_argument = nullptr;
    _sceCd_cd_read_intr(argument);
    if (!memory.empty() && memory.size() > 8) return 5;
    if (callback_argument != &sceCdCbfunc_num) return 6;
    std::puts("cd read intr contract passed");
    return 0;
}
