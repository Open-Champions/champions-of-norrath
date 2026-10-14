#define memset norrath_host_memset
#define printf norrath_host_printf
#include "read.cpp"
#undef memset
#undef printf

#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

extern "C" int gKeyboardAttached = -1;

struct Event { unsigned code, flags_a, flags_b, dir; };

static std::vector<std::string> calls;
static int enumerate_result, open_result, hint_result, hint_word, read_result;
static std::vector<Event> script;
static unsigned char translate_value;
static std::vector<std::pair<int, int>> queued;  // key, direction
static std::vector<void*> translate_elements;
static void* dequeue_out;
static unsigned char* read_buffer;
static std::string printed;
static unsigned memset_size;

extern "C" void* norrath_host_memset(void* d, int v, unsigned n) { std::memset(d, v, n); memset_size = n; calls.push_back("memset"); return d; }
extern "C" int norrath_host_printf(const char* f, ...) { printed = f; calls.push_back("printf"); return 0; }
extern "C" int lgKbdEnumerate(int i, void*) { calls.push_back("enumerate" + std::to_string(i)); return enumerate_result; }
extern "C" int lgKbdOpen(int i, void* handle) { calls.push_back("open" + std::to_string(i)); if (open_result == 0) *static_cast<int*>(handle) = 77; return open_result; }
extern "C" int lgKbdSetRepeatRate(int i, int r) { calls.push_back("repeat" + std::to_string(i) + "," + std::to_string(r)); return 0; }
extern "C" int lgKbdEnumHint(void* hint) { calls.push_back("hint"); *static_cast<int*>(hint) = hint_word; return hint_result; }
extern "C" int lgKbdRead(int handle, void* buffer) {
    calls.push_back("read" + std::to_string(handle));
    unsigned char* b = static_cast<unsigned char*>(buffer);
    read_buffer = b;
    std::memset(b, 0, 208);
    unsigned short count = static_cast<unsigned short>(script.size());
    std::memcpy(b + 2, &count, 2);
    for (size_t i = 0; i < script.size(); ++i) {
        unsigned short v[4] = {static_cast<unsigned short>(script[i].code), static_cast<unsigned short>(script[i].flags_a),
                               static_cast<unsigned short>(script[i].flags_b), static_cast<unsigned short>(script[i].dir)};
        std::memcpy(b + 4 + 16 * i + 4, &v[0], 2);
        std::memcpy(b + 4 + 16 * i + 8, &v[1], 2);
        std::memcpy(b + 4 + 16 * i + 10, &v[2], 2);
        std::memcpy(b + 4 + 16 * i + 12, &v[3], 2);
    }
    return read_result;
}
extern "C" int lgKbdClose(int handle) { calls.push_back("close" + std::to_string(handle)); return 0; }
extern "C" unsigned char TranslateEvent__10VIKeyboardP11_lgKbdEvent(void*, void* element) {
    calls.push_back("translate");
    translate_elements.push_back(element);
    return translate_value;
}
extern "C" void PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey(void*, void* key) {
    const unsigned char* k = static_cast<const unsigned char*>(key);
    queued.push_back({k[0], k[1]});
    calls.push_back("queue");
}
extern "C" int DequeueKeyEvent__10VIKeyboardP12VIConsoleKey(void*, void* key) {
    dequeue_out = key;
    calls.push_back("dequeue");
    return 4321;
}

struct Board {
    alignas(16) unsigned char bytes[560];
    int& word(int offset) { return *reinterpret_cast<int*>(bytes + offset); }
    Board() {
        std::memset(bytes, 0x5a, sizeof bytes);
        word(0) = 1;
        word(4) = 33;
        calls.clear(); queued.clear(); translate_elements.clear(); script.clear(); printed.clear();
        hint_result = 0; hint_word = 0; read_result = 0; translate_value = 0x31;
        gKeyboardAttached = -1;
    }
};

// Independent expectation, built from the documented mapping.
static int expected_key(const Event& e, bool* translated, bool* flag_always) {
    *translated = false;
    *flag_always = false;
    int fb = (e.flags_b & 0x22) ? 4 : 13;
    bool in_a = e.code >= 83 && e.code <= 103;
    if (in_a) {
        if (e.code == 88) return fb;
        if ((e.flags_a & 1) == 0) {
            switch (e.code) {
            case 89: return 23; case 90: return 20; case 91: return 22; case 92: return 17;
            case 93: return -1;
            case 94: return 18; case 95: return 24; case 96: return 19; case 97: return 21;
            }
        }
        *translated = true;
        return 0;
    }
    switch (e.code) {
    case 40: return fb;
    case 73: return 25; case 74: return 24; case 75: return 21; case 76: return 26; case 77: return 23;
    case 78: return 22; case 79: return 18; case 80: return 17; case 81: return 20; case 82: return 19;
    }
    *translated = true;
    *flag_always = true;
    return 0;
}

int main() {
    // State 0: enumerate fails -> nothing else but the dequeue.
    {
        Board b; b.word(0) = 0; enumerate_result = 1;
        int r = Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
        if (r != 4321 || dequeue_out != b.bytes + 400 || gKeyboardAttached != 0 || b.word(528) != 0 || b.word(0) != 0) return 1;
        if (calls.size() != 2 || calls[0] != "enumerate0") return 2;
    }
    // State 0: open fails.
    {
        Board b; b.word(0) = 0; enumerate_result = 0; open_result = 5;
        Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
        if (b.word(4) != -1 || b.word(0) != 0) return 3;
        if (calls.size() != 3 || calls[1] != "open0") return 4;
    }
    // State 0: open ok.
    {
        Board b; b.word(0) = 0; b.word(12) = 9; enumerate_result = 0; open_result = 0;
        Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
        if (b.word(4) != 77 || b.word(0) != 1 || b.word(12) != 0 || memset_size != 512) return 5;
        for (int i = 16; i < 528; ++i) if (b.bytes[i] != 0) return 6;
        if (b.bytes[11] != 0x5a || b.bytes[528 + 4] != 0x5a) return 7;
        if (calls.size() != 5 || calls[2] != "repeat0,0" || calls[3] != "memset") return 8;
    }
    // State 1: hint failure or hint word 1 closes; other hint words do not.
    {
        Board b; hint_result = 1;
        Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
        if (b.word(0) != 0 || gKeyboardAttached != 1 || calls[1] != "close33") return 10;
        Board c; hint_word = 1;
        Read__10VIKeyboardR12VIConsoleKey(c.bytes, c.bytes + 400);
        if (c.word(0) != 0) return 11;
        for (const std::string& s : calls) if (s.rfind("read", 0) == 0) return 12;
        Board d; hint_word = 2;
        Read__10VIKeyboardR12VIConsoleKey(d.bytes, d.bytes + 400);
        if (d.word(0) != 1) return 13;
    }
    // Read status handling.
    {
        const int closing[] = {static_cast<int>(0x80000000u), static_cast<int>(0x80000004u), static_cast<int>(0x80000006u), static_cast<int>(0x8000000du)};
        for (int status : closing) {
            Board b; read_result = status;
            script.push_back({20, 0, 0, 1});
            Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
            if (b.word(0) != 0 || !queued.empty()) return 20;
            bool printed_overflow = printed == "Keyboard Buffer Overflow? Resetting";
            if (printed_overflow != (status == static_cast<int>(0x8000000du))) return 21;
        }
        const int ignored[] = {1, -1, 5, static_cast<int>(0x80000001u), static_cast<int>(0x80000005u), static_cast<int>(0x8000000eu), 0x7fffffff};
        for (int status : ignored) {
            Board b; read_result = status;
            script.push_back({20, 0, 0, 1});
            Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
            if (b.word(0) != 1 || !queued.empty()) return 22;
            if (calls.back() != "dequeue") return 23;
        }
        Board c; hint_result = 0; c.word(0) = 7;  // unknown state does nothing
        Read__10VIKeyboardR12VIConsoleKey(c.bytes, c.bytes + 400);
        if (calls.size() != 1) return 24;
    }
    // Every code with each flag combination and direction.
    for (unsigned code = 0; code < 256; ++code) {
        for (unsigned fa = 0; fa < 2; ++fa) {
            for (unsigned fb = 0; fb < 0x44; fb += 2) {
                for (unsigned dir = 0; dir < 3; ++dir) {
                    for (int tv = 0; tv < 2; ++tv) {
                        Board b;
                        // pre-set the stored key for the up path.
                        b.bytes[16 + 2 * (code % 256)] = 0x44;
                        Event e{code, fa, fb, dir};
                        script.push_back(e);
                        translate_value = tv ? 0x31 : 0;
                        int r = Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
                        if (r != 4321) return 30;
                        bool translated, always;
                        int want = expected_key(e, &translated, &always);
                        bool down = dir == 1;
                        unsigned char* slot = b.bytes + 16 + 2 * code;
                        int key;
                        if (want == -1) {
                            if (!queued.empty() || !translate_elements.empty()) return 31;
                            continue;
                        }
                        if (translated) {
                            if (down) {
                                if (translate_elements.size() != 1 || translate_elements[0] != read_buffer + 4) return 32;
                                key = translate_value;
                                if (slot[0] != key) return 33;
                            } else {
                                if (!translate_elements.empty()) return 34;
                                key = 0x44;  // the previously stored key is returned and cleared
                                if (slot[0] != 0) return 35;
                            }
                            if (always || key != 0) {
                                if (slot[1] != (down ? 1 : 0)) return 36;
                            } else if (slot[1] != 0x5a) {
                                return 37;
                            }
                        } else {
                            key = want;
                            if (!translate_elements.empty()) return 38;
                            if (slot[0] != 0x44) return 39;  // table untouched for fixed keys
                            if (slot[1] != 0x5a) return 40;
                        }
                        if (key == 0) {
                            if (!queued.empty()) return 41;
                        } else {
                            if (queued.size() != 1 || queued[0].first != key || queued[0].second != (down ? 1 : 0)) return 42;
                        }
                    }
                }
            }
        }
    }
    // Several events in one read are handled in order, and the count is honored.
    {
        Board b;
        script.push_back({89, 0, 0, 1});
        script.push_back({90, 0, 0, 0});
        script.push_back({40, 0, 0x02, 1});
        script.push_back({41, 0, 0, 1});
        Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
        if (queued.size() != 4) return 50;
        if (queued[0] != std::make_pair(23, 1) || queued[1] != std::make_pair(20, 0) ||
            queued[2] != std::make_pair(4, 1) || queued[3] != std::make_pair(0x31, 1)) return 51;
        if (translate_elements.size() != 1 || translate_elements[0] != read_buffer + 4 + 16 * 3) return 52;  // the element in the read buffer
    }
    {
        Board b;  // zero events: only the dequeue follows the read
        Read__10VIKeyboardR12VIConsoleKey(b.bytes, b.bytes + 400);
        if (!queued.empty()) return 53;
    }
    return 0;
}
