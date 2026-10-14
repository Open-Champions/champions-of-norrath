/* Host contract. Not a plan input. Addresses are arena offsets; the uncached alias bit
 * is ignored. The expected result is computed by a separate model written from the
 * header comment of sce_fs_rcv_intr.cpp. */
#define NORRATH_HOST_CONTRACT 1
#include "sce_fs_rcv_intr.cpp"
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <vector>
alignas(64) static unsigned char arena[1 << 20];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" unsigned norrath_host_reply_offset = 0x1000;
extern "C" { int _fs_rcv_bufdbl = 0; int ee_retbuf_sel = -7; int _sceFs_q[32]; unsigned char _rcv_data_cmd[1]; }
static std::vector<int> signaled;
extern "C" int iSignalSema(int s) { signaled.push_back(s); return 0; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static uint64_t rng = 0x9e3779b97f4a7c15ull;
static unsigned rnd() { rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17; return static_cast<unsigned>(rng >> 16); }

int main() {
    int bad = 0;
    for (int iteration = 0; iteration < 20000; ++iteration) {
        const unsigned commands[] = {0, 2, 2, 3, 11, 11, 12, 12, 23, 25, 26, 27, 0xffffffffu};
        const unsigned command = commands[rnd() % 13];
        const int statuses[] = {0, 1, 58, -1, -3, -32, -100, 7};
        const int status = statuses[rnd() % 8];
        const unsigned selector = rnd() % 2;
        _fs_rcv_bufdbl = static_cast<int>(rnd() % 2);
        const unsigned use = _fs_rcv_bufdbl ? selector : 0;
        const unsigned packet = 0x100;
        put(packet + 12, selector);
        const unsigned base = 0x1000 + 0x440 * use;
        for (unsigned off = 0; off < 0x880; off += 4) put(0x1000 + off, rnd());
        const unsigned dest = 0x20000 + (rnd() % 3) * 0x10000 + (rnd() % 8);
        const unsigned dest2 = 0x60000 + (rnd() % 3) * 0x1000 + (rnd() % 4);
        for (unsigned a = 0x20000; a < 0x80000; a += 4) put(a, 0x5a5a5a5au);
        put(base, static_cast<unsigned>(status)); put(base + 4, command);
        put(base + 8, 0x30000 + (rnd() % 8)); put(base + 12, rnd() % 200);
        put(base + 16, (rnd() & 1) ? rnd() : 0);
        const unsigned counts1 = rnd() % 5 ? rnd() % 120 : 0;
        const unsigned counts2 = rnd() % 5 ? rnd() % 120 : 0;
        const unsigned lengths[] = {0, 1, 7, 100, 1024, 1025, 5000};
        if (command == 2) { put(base + 20, counts1); put(base + 24, counts2); put(base + 28, dest); put(base + 32, dest2); }
        if (command == 11 || command == 12 || command == 23 || command == 25 || command == 26) put(base + 20, dest);
        if (command == 23 || command == 25 || command == 26) put(base + 24, lengths[rnd() % 7]);
        for (int i = 0; i < 32; ++i) _sceFs_q[i] = (rnd() % 3) ? -1 : static_cast<int>(rnd() % 6);
        int queue_before[32]; std::memcpy(queue_before, _sceFs_q, sizeof queue_before);
        static unsigned char before[1 << 20];
        std::memcpy(before, arena, sizeof arena);
        signaled.clear();
        _sceFs_Rcv_Intr(reinterpret_cast<const unsigned*>(arena + packet), nullptr);
        // Model.
        static unsigned char model[1 << 20];
        std::memcpy(model, before, sizeof model);
        auto mcopy = [&](unsigned to, unsigned from, unsigned n) { std::memmove(model + (to & 0x1fffffffu), model + from, n); };
        const unsigned s0 = base;
        const unsigned h_dest = get(s0 + 8), h_len = get(s0 + 12);
        if (status >= 0) mcopy(h_dest, s0 + 16, h_len);
        auto word = [&](unsigned off) { unsigned v; std::memcpy(&v, model + off, 4); return v; };
        if (command == 2) {
            for (int part = 0; part < 2; ++part) {
                const unsigned n = word(s0 + 20 + 4 * part), to = word(s0 + 28 + 4 * part), from = s0 + (part ? 100 : 36);
                if (static_cast<int>(n) > 0) mcopy(to, from, n);
            }
        } else if (command == 11) {
            if (static_cast<int>(word(h_dest & 0x1fffffffu)) > 0) mcopy(word(s0 + 20), s0 + 24, 324);
        } else if (command == 12) {
            mcopy(word(s0 + 20), s0 + 24, 64);
        } else if (command == 23 || command == 25 || command == 26) {
            unsigned n = word(s0 + 24); if (n > 1024) n = 1024;
            mcopy(word(s0 + 20), s0 + 28, n);
        }
        int queue_model[32]; std::memcpy(queue_model, queue_before, sizeof queue_model);
        std::vector<int> signal_model;
        if (status >= 0) signal_model.push_back(status);
        else for (int i = 0; i < 32; ++i) if (queue_model[i] == -status) { queue_model[i] = -1; break; }
        // Compare, ignoring the packet area that the model does not change either.
        if (std::memcmp(arena, model, sizeof arena) != 0 || std::memcmp(queue_model, _sceFs_q, sizeof queue_model) != 0 ||
            signal_model != signaled || ee_retbuf_sel != static_cast<int>(use)) {
            if (++bad < 5) std::printf("iteration %d (command %u, status %d) differs\\n", iteration, command, status);
        }
    }
    if (bad) return 1;
    std::puts("_sceFs_Rcv_Intr contract passed");
    return 0;
}
