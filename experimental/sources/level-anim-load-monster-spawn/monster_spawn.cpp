#include "../level-anim-load-common/mem.hpp"
// Observed monsterSpawn__FPPcG6Point3sb at 0x0026aa80, 2112 bytes (the megatile sources
// import it as norrath_monster_spawn). Arguments: a null-terminated array of option
// strings, a 12-byte point (copied, and passed on to every spawn call), a sign-extended
// halfword type word, and a flag.
//
// Options are matched by prefix: "type=" (the type text after it), "line=" (int),
// "chance=" (text to double to float), "radius=" (int, default 2000), "countmin=" and
// "countmax=" (ints, default 0), "multitype=" (flag, value unread), "clearradius="
// (int, default -1). Later options of the same kind overwrite earlier ones.
//
// Gate: the global eRandState advances by x * 0x19660d + 0x3c6ef35f. A draw is
// (state >> 16) * 2^-16 as a float. When chance <= draw the state is kept and the routine
// returns, so a missing chance (0.0) never spawns. Otherwise a second draw gives
// count = (int)(countmin + draw * (countmax - countmin + 1)).
//
// Type text: sscanf("%d") reads a leading number n, then entries follow the first comma:
// a plain entry runs to a ',' or '-' (or the end); after a '-' the characters up to ','
// or the end set flags, 'A'/'a' in a second table and 'O'/'o' in a first. A parenthesised
// entry "(...)" scans to ';', '-' or ')', reads flags the same way after a '-', and then skips to
// the matching ')'. Each entry's start offset is recorded (up to 16). Three 16-byte byte
// tables are used: availability (all 1 initially), the 'O' flags and the 'A' flags.
//
// For each of the n entries whose 'A' flag is set, spawn runs once with the first option
// string that contains the entry text from the entry's start to the end of the type text
// (strstr of the option against that tail); an 'O' flag also clears the entry's
// availability and decrements n. Then, with multitype clear, one entry index is drawn
// as (int)(n * draw), moved forward past unavailable entries, and spawn runs count times
// with that entry; with multitype set, each of the count iterations draws its own entry the
// same way and an 'O' entry becomes unavailable and decrements n. The spawn arguments in
// order: point copy, radius, line, type word, the strstr result, filter, the option array,
// clearradius. Nothing is returned.
// Inferred: "O" means once-only and "A" means always, from how the flags are used.
// Unknown: a missing type= option (the image passes a null pointer to sscanf); the semantics inside spawn (0x0026a738), the strstr tail quirk above (it is
// what the instructions do, and the intent is not claimed), and the original's behaviour when
// the type text has more than 16 entries or lacks a comma, where it runs over neighbouring
// stack words and unterminated text. This version stops at the terminator and treats table reads
// beyond the 48 bytes as nonzero; those cases are not tested. The compile is not byte-matched.
extern "C" {
extern int eRandState;
int norrath_factory_strncmp(const char* left, const char* right, unsigned count);
int norrath_factory_atoi(const char* text);
char* norrath_factory_strstr(const char* text, const char* needle);
int norrath_factory_sscanf(const char* text, const char* format, ...);
unsigned long long norrath_player_atof(const char* text);
float norrath_player_double_to_float(unsigned long long value);
void spawn__FG6Point3iisPcbPPci(const unsigned char* point, int radius, int line,
                                short type_word, char* found, bool filter,
                                char** options, int clear_radius);
}
namespace {
struct Tables {
    unsigned char bytes[48];
    unsigned char& at(int base, int index) { return bytes[base + index]; }
    bool valid(int base, int index) const { return base + index >= 0 && base + index < 48; }
    // Out-of-range reads count as nonzero; out-of-range writes are dropped.
    unsigned char get(int base, int index) const {
        return valid(base, index) ? bytes[base + index] : 1;
    }
    void put(int base, int index, unsigned char value) {
        if (valid(base, index)) bytes[base + index] = value;
    }
};
const int kAvailable = 0, kOnce = 16, kAlways = 32;

float draw() {
    const unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(state);
    return static_cast<float>(static_cast<int>(state >> 16)) * (1.0f / 65536.0f);
}

bool starts_with(const char* text, const char* prefix, unsigned length) {
    return norrath_factory_strncmp(text, prefix, length) == 0;
}

// Flag characters up to a ',' (or the end) when plain, as the entry's table writes.
void set_flag(Tables& tables, int entry, char flag) {
    if (flag == 'O' || flag == 'o') tables.put(kOnce, entry, 1);
    else if (flag == 'A' || flag == 'a') tables.put(kAlways, entry, 1);
}

// Moves forward past unavailable entries as the original does: one step per round.
int settle(const Tables& tables, int index) {
    int rounds = 0;
    do {
        const int next = index + 1;
        const unsigned char available = tables.get(kAvailable, index);
        rounds += 1;
        if (available == 0) index = next;
    } while (!(index < rounds));
    return index;
}
}
extern "C" void norrath_monster_spawn(char** arguments, const void* point_in, short type_word,
                                      bool filter) {
    unsigned char point[12];
    for (int i = 0; i != 12; ++i) point[i] = static_cast<const unsigned char*>(point_in)[i];
    const char* text = nullptr;
    int minimum = 0, maximum = 0;
    int radius = 2000, clear_radius = -1, line = 0;
    int multitype = 0;
    float chance = 0.0f;
    for (char** cursor = arguments; *cursor != nullptr; ++cursor) {
        const char* option = *cursor;
        if (starts_with(option, "type=", 5)) text = option + 5;
        else if (starts_with(option, "line=", 5)) line = norrath_factory_atoi(option + 5);
        else if (starts_with(option, "chance=", 7))
            chance = norrath_player_double_to_float(norrath_player_atof(option + 7));
        else if (starts_with(option, "radius=", 7)) radius = norrath_factory_atoi(option + 7);
        else if (starts_with(option, "countmin=", 9)) minimum = norrath_factory_atoi(option + 9);
        else if (starts_with(option, "countmax=", 9)) maximum = norrath_factory_atoi(option + 9);
        else if (starts_with(option, "multitype=", 10)) multitype = 1;
        else if (starts_with(option, "clearradius=", 12))
            clear_radius = norrath_factory_atoi(option + 12);
    }
    if (chance <= draw()) return;
    const int count = static_cast<int>(static_cast<float>(minimum) +
                                       draw() * static_cast<float>(maximum - minimum + 1));
    int entries = 0;
    norrath_factory_sscanf(text, "%d", &entries);
    Tables tables;
    for (int i = 0; i != 16; ++i) {
        tables.bytes[kAvailable + i] = 1;
        tables.bytes[kOnce + i] = 0;
        tables.bytes[kAlways + i] = 0;
    }
    int starts[16];
    for (int i = 0; i != 16; ++i) starts[i] = 0;
    int parsed = 0;
    int at = 0;
    if (text[0] != ',') {
        at = 1;
        while (text[at] != ',' && text[at] != 0) ++at;
    }
    at += 1;
    while (text[at] != 0) {
        const int entry = parsed;
        if (entry < 16) starts[entry] = at;
        if (text[at] == '(') {
            at += 1;
            bool flags = false;
            while (text[at] != 0) {
                const char c = text[at];
                if (c == ';' || c == ')') break;
                at += 1;
                if (c == '-') { flags = true; break; }
            }
            if (flags) {
                const char c = text[at];
                if (c == ';' || c == ')') {
                    at += 1;
                } else {
                    for (;;) {
                        if (text[at] == 0) break;
                        set_flag(tables, entry, text[at]);
                        at += 1;
                        const char next = text[at];
                        if (next == ';' || next == ')') { at += 1; break; }
                    }
                }
            }
            int depth = 1;
            while (text[at] != 0) {
                const char c = text[at];
                if (c == '(') depth += 1;
                else if (c == ')') depth -= 1;
                at += 1;
                if (depth == 0) break;
            }
        } else {
            while (text[at] != 0 && text[at] != ',' && text[at] != '-') ++at;
            if (text[at] == '-') {
                at += 1;
                while (text[at] != 0 && text[at] != ',') {
                    set_flag(tables, entry, text[at]);
                    at += 1;
                }
            }
        }
        parsed += 1;
        if (text[at] != 0) at += 1;
    }
    const int original_entries = entries;
    char* found;
    auto find = [&](const char* needle) {
        found = nullptr;
        for (char** cursor = arguments; *cursor != nullptr;) {
            found = norrath_factory_strstr(*cursor++, needle);
            if (found != nullptr) break;
        }
    };
    auto start_of = [&](int index) -> int { return (index >= 0 && index < 16) ? starts[index] : 0; };
    for (int index = 0; index < original_entries; ++index) {
        if (tables.get(kAlways, index) == 0) continue;
        find(text + start_of(index));
        spawn__FG6Point3iisPcbPPci(point, radius, line, type_word, found, filter, arguments,
                                   clear_radius);
        if (tables.get(kOnce, index) != 0) {
            tables.put(kAvailable, index, 0);
            entries -= 1;
        }
    }
    const char* chosen = nullptr;
    if (multitype == 0) {
        int index = static_cast<int>(static_cast<float>(entries) * draw());
        if (!(index < multitype)) index = settle(tables, index);
        chosen = text + start_of(index);
    }
    if (count <= 0) return;
    for (int remaining = count; remaining != 0; --remaining) {
        if (multitype != 0) {
            int index = static_cast<int>(static_cast<float>(entries) * draw());
            if (index >= 0) index = settle(tables, index);
            chosen = text + start_of(index);
            if (tables.get(kOnce, index) != 0) {
                tables.put(kAvailable, index, 0);
                entries -= 1;
            }
        }
        find(chosen);
        spawn__FG6Point3iisPcbPPci(point, radius, line, type_word, found, filter, arguments,
                                   clear_radius);
    }
}
