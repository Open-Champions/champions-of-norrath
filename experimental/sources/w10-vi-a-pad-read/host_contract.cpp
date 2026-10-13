#define printf norrath_host_printf
#include "read.cpp"
#undef printf

#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

// Scripted SDK. Calls are logged as strings so order and arguments are checked.
static std::vector<std::string> log_calls;
static int req_state, pad_state, mode_term1, mode_term2, act_minus1, act_0, act_1, align_result, read_result;
static unsigned char read_bytes[8];
static const unsigned char* align_seen;
static void* direct_seen;
static std::string printed;

static void note(const char* name, int a = 0, int b = 0, int c = 0, int d = 0) {
    char text[96];
    std::snprintf(text, sizeof text, "%s(%d,%d,%d,%d)", name, a, b, c, d);
    log_calls.push_back(text);
}
extern "C" int scePadGetReqState(int p, int s) { note("req", p, s); return req_state; }
extern "C" int scePadGetState(int p, int s) { note("state", p, s); return pad_state; }
extern "C" int scePadInfoMode(int p, int s, int t, int o) { note("mode", p, s, t, o); return t == 1 ? mode_term1 : mode_term2; }
extern "C" int scePadInfoAct(int p, int s, int a, int o) {
    note("act", p, s, a, o);
    return a == -1 ? act_minus1 : (a == 0 ? act_0 : act_1);
}
extern "C" int scePadSetActAlign(int p, int s, const unsigned char* d) { note("align", p, s); align_seen = d; return align_result; }
extern "C" int scePadSetMainMode(int p, int s, int m, int l) { note("main", p, s, m, l); return 1; }
extern "C" int scePadRead(int p, int s, void* buffer) { note("read", p, s); std::memcpy(buffer, read_bytes, 8); return read_result; }
extern "C" int scePadSetActDirect(int p, int s, void* d) { note("direct", p, s); direct_seen = d; return 1; }
extern "C" float ScaleStick__5VIPadUc(void*, unsigned char v) { return static_cast<float>(v) * 0.25f; }
extern "C" int norrath_host_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    const char* name = va_arg(args, const char*);
    va_end(args);
    printed = std::string(format) + "|" + name;
    return 0;
}

struct Fixture {
    alignas(8) unsigned char pad[96];
    float out[24];
    Fixture() {
        std::memset(pad, 0xa5, sizeof pad);
        for (float& f : out) f = -7.0f;
        int* w = reinterpret_cast<int*>(pad);
        w[0] = 1; w[1] = 2;
        w[8] = -1;   // offset 32
        w[9] = 0;    // offset 36
        log_calls.clear();
        printed.clear();
        req_state = 0; pad_state = 6; mode_term1 = 7; mode_term2 = 0;
        act_minus1 = 2; act_0 = 0; act_1 = 2; align_result = 1; read_result = 8;
        std::memset(read_bytes, 0, 8);
    }
    int& word(int offset) { return *reinterpret_cast<int*>(pad + offset); }
};

static float expected_button(int index, unsigned char b2, unsigned char b3) {
    // (byte offset / 4) -> (byte, bit) per the table in read.cpp; pressed = bit clear.
    struct Map { int index; int byte; int bit; };
    static const Map map[] = {
        {5, 3, 4}, {6, 3, 7}, {7, 3, 6}, {8, 3, 5}, {13, 3, 2}, {14, 3, 0}, {15, 3, 3}, {16, 3, 1},
        {9, 2, 7}, {10, 2, 5}, {11, 2, 4}, {12, 2, 6}, {17, 2, 3}, {18, 2, 0}, {19, 2, 1}, {20, 2, 2}};
    for (const Map& m : map)
        if (m.index == index) return (((m.byte == 3 ? b3 : b2) >> m.bit) & 1) ? 0.0f : 1.0f;
    return -99.0f;
}

int main() {
    // Full success read: every button combination of the two bytes is mapped correctly.
    for (int b2 = 0; b2 < 256; b2 += 5) {
        for (int b3 = 0; b3 < 256; b3 += 3) {
            Fixture f;
            read_bytes[0] = 0; read_bytes[2] = static_cast<unsigned char>(b2); read_bytes[3] = static_cast<unsigned char>(b3);
            read_bytes[4] = 4; read_bytes[5] = 8; read_bytes[6] = 12; read_bytes[7] = 16;
            int r = Read__5VIPadR12VIConsolePad(f.pad, f.out);
            if (r != 1) return 1;
            if (f.out[0] != -7.0f) return 2;
            if (f.out[1] != 1.0f || f.out[2] != 2.0f || f.out[3] != 3.0f || f.out[4] != 4.0f) return 3;
            for (int index = 5; index <= 20; ++index)
                if (f.out[index] != expected_button(index, static_cast<unsigned char>(b2), static_cast<unsigned char>(b3))) return 4;
            if (f.out[21] != -7.0f) return 5;
        }
    }

    {   // call order and state bookkeeping of the full success path
        Fixture f;
        Read__5VIPadR12VIConsolePad(f.pad, f.out);
        const char* expected[] = {"state(1,2,0,0)", "mode(1,2,1,0)", "mode(1,2,2,0)", "act(1,2,-1,0)", "act(1,2,0,3)",
                                  "act(1,2,1,3)", "align(1,2,0,0)", "read(1,2,0,0)"};
        if (log_calls.size() != 8) return 10;
        for (int i = 0; i < 8; ++i) if (log_calls[i] != expected[i]) return 11;
        if (f.word(32) != 2 || f.word(36) != 1 || f.word(88) != 6) return 12;
        static const unsigned char want[6] = {0, 1, 0xff, 0xff, 0xff, 0xff};
        if (std::memcmp(align_seen, want, 6) != 0) return 13;
    }
    {   // pending request busy: returns 0 and nothing else happens
        Fixture f;
        f.word(36) = 1; req_state = 2;
        if (Read__5VIPadR12VIConsolePad(f.pad, f.out) != 0 || log_calls.size() != 1 || f.word(36) != 1) return 20;
    }
    {   // request done: clears 36 only
        Fixture f;
        f.word(36) = 1; req_state = 0; f.word(32) = 2;
        Read__5VIPadR12VIConsolePad(f.pad, f.out);
        if (f.word(36) != 0 || f.word(32) != 2) return 21;
    }
    {   // request failed: clears 32 and 36, then word 32 is no longer -1 so no act probing
        Fixture f;
        f.word(36) = 1; req_state = 1; f.word(32) = 2;
        Read__5VIPadR12VIConsolePad(f.pad, f.out);
        if (f.word(36) != 0 || f.word(32) != 0) return 22;
        for (const std::string& s : log_calls) if (s.rfind("act", 0) == 0) return 23;
    }
    {   // not ready path with an actuator set
        Fixture f;
        f.word(32) = 2; pad_state = 1;
        std::memset(f.pad + 16, 0xcc, 16);
        f.pad[40] = 9; f.pad[41] = 9;
        int r = Read__5VIPadR12VIConsolePad(f.pad, f.out);
        if (r != 0 || printed != "pad not ready (%s)\n|FINDPAD") return 30;
        if (f.word(32) != -1 || f.word(48) != 0 || f.pad[40] || f.pad[41] || f.word(88) != 1) return 31;
        for (int i = 16; i < 32; ++i) if (f.pad[i]) return 32;
        if (direct_seen != f.pad + 40) return 33;
        if (log_calls.size() != 2 || log_calls[1] != "direct(1,2,0,0)") return 34;
        if (f.out[1] != -7.0f) return 35;
    }
    {   // not ready path without an actuator: no direct call; other state names
        Fixture f;
        f.word(32) = 0; pad_state = 7;
        Read__5VIPadR12VIConsolePad(f.pad, f.out);
        if (printed != "pad not ready (%s)\n|ERROR") return 40;
        for (const std::string& s : log_calls) if (s.rfind("direct", 0) == 0) return 41;
        Fixture g;
        pad_state = 0;
        Read__5VIPadR12VIConsolePad(g.pad, g.out);
        if (printed != "pad not ready (%s)\n|DISCONNECT") return 42;
        Fixture h;
        pad_state = 5;
        Read__5VIPadR12VIConsolePad(h.pad, h.out);
        if (printed != "pad not ready (%s)\n|EXECCMD") return 43;
        Fixture i;
        pad_state = 3;
        Read__5VIPadR12VIConsolePad(i.pad, i.out);
        if (printed != "pad not ready (%s)\n|") return 44;
    }
    {   // state 2 is accepted as well
        Fixture f;
        pad_state = 2;
        if (Read__5VIPadR12VIConsolePad(f.pad, f.out) != 1) return 50;
    }
    {   // wrong controller id: main mode set, return 0, no read
        Fixture f;
        mode_term1 = 4; mode_term2 = 0;
        if (Read__5VIPadR12VIConsolePad(f.pad, f.out) != 0) return 60;
        if (log_calls.back() != "main(1,2,1,3)") return 61;
        if (f.word(32) != 0) return 62;
        for (const std::string& s : log_calls) if (s.rfind("read", 0) == 0) return 63;
    }
    {   // extended id replaces a nonzero id only when positive
        Fixture f;
        mode_term1 = 4; mode_term2 = 7;
        if (Read__5VIPadR12VIConsolePad(f.pad, f.out) != 1) return 70;
        Fixture g;
        mode_term1 = 7; mode_term2 = -1;
        if (Read__5VIPadR12VIConsolePad(g.pad, g.out) != 1) return 71;
        Fixture h;
        mode_term1 = 0; mode_term2 = 7;  // zero id: ext is not queried
        if (Read__5VIPadR12VIConsolePad(h.pad, h.out) != 0) return 72;
        for (const std::string& s : log_calls) if (s.rfind("mode(1,2,2", 0) == 0) return 73;
    }
    {   // each actuator probe failure clears word 32 and leaves 36 alone
        for (int stage = 0; stage < 4; ++stage) {
            Fixture f;
            if (stage == 0) act_minus1 = 1;
            if (stage == 1) act_0 = 1;
            if (stage == 2) act_1 = 0;
            if (stage == 3) align_result = 0;
            Read__5VIPadR12VIConsolePad(f.pad, f.out);
            if (f.word(32) != 0 || f.word(36) != 0) return 80 + stage;
        }
        Fixture f;
        mode_term1 = 4;  // wrong id: probing stops at the id check
        Read__5VIPadR12VIConsolePad(f.pad, f.out);
        if (f.word(32) != 0) return 90;
        for (const std::string& s : log_calls) if (s.rfind("act(1,2,0", 0) == 0) return 91;
    }
    {   // read failures return 0 and leave out untouched
        Fixture f;
        read_result = 0;
        if (Read__5VIPadR12VIConsolePad(f.pad, f.out) != 0 || f.out[1] != -7.0f) return 100;
        Fixture g;
        read_bytes[0] = 1;
        if (Read__5VIPadR12VIConsolePad(g.pad, g.out) != 0 || g.out[1] != -7.0f) return 101;
    }
    return 0;
}
