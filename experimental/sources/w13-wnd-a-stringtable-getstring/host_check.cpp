// Host check only; not a build input. Addresses stored in the table are small numbers mapped to host text.
#define NORRATH_HOST_CONTRACT 1
#include "get_string.cpp"
#include <cstdio>
#include <cstring>

static unsigned short block_text[4][4] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};
static unsigned entries[12];
extern "C" const unsigned short* norrath_host_text(unsigned address) {
    if (address == 0x100) return reinterpret_cast<const unsigned short*>(entries);
    return block_text[address - 1];
}

int main() {
    bool ok = true;
    int table[4] = {3, 4, 0, 0x100};
    for (int i = 0; i < 12; ++i) entries[i] = i % 5 == 4 ? 0u : 1u + i % 4;
    for (int id = -1; id <= 5; ++id) {
        for (int column = -1; column <= 4; ++column) {
            const unsigned short* got = GetString__13VIStringTableii(table, id, column);
            const bool inside = id >= 1 && id <= 3 && column >= 0 && column <= 3;
            const unsigned entry = inside ? entries[(id - 1) * 4 + column] : 0u;
            if (entry) ok = ok && got == block_text[entry - 1];
            else ok = ok && got[0] == 0x28 && got[5] == 0x29 && got[6] == 0 && got[1] == 0x6e;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
