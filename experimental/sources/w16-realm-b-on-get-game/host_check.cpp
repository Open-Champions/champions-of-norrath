// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// The string-library entry points are replaced by small models that allocate reps (16-byte header, then characters) in the address
// arena; every other callee records its arguments. The expected trace is written out from the semantics in the source comment.
#include <cstdio>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_get_game.cpp"

unsigned vi_wnd_mui;
unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

static std::string trace;
static unsigned heapTop;
static unsigned narrowRefs;
static char* lastDup;
static unsigned short strings[4000];
static unsigned char* newRep(unsigned length, unsigned capacity, unsigned element, unsigned refs) {
    unsigned char* rep = arena_at(0x10000 + heapTop);
    heapTop += (16 + element * (capacity + 1) + 15) & ~15u;
    norrath_aoc::store_word(rep, length); norrath_aoc::store_word(rep + 4, capacity);
    norrath_aoc::store_word(rep + 8, refs); norrath_aoc::store_word(rep + 12, 0);
    return rep;
}
static std::string off(const void* p, unsigned base) { return std::to_string(static_cast<const unsigned char*>(p) - arena_at(base)); }
extern "C" {
int stub_printf(const char* f, ...) __asm__("_printf");
int stub_printf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    int a = __builtin_va_arg(ap, int); int b = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("head:") + std::to_string(a) + "," + std::to_string(b) + ";";
    return 0;
}
int stub_snprintf(char* out, unsigned long size, const char* f, ...) __asm__("_snprintf");
int stub_snprintf(char* out, unsigned long size, const char* f, ...) {
    trace += "snprintf:" + off(out, 0x100) + "," + std::to_string(size) + "," + f + ";";
    return 0;
}
char* stub_strstr(const char* h, const char* n) __asm__("strstr");
char* stub_strstr(const char* h, const char* n) {
    trace += std::string("strstr:") + h + "," + n + ";";
    for (const char* p = h; *p; ++p) if (*p == n[0]) return const_cast<char*>(p);
    return nullptr;
}
int stub_atoi(const char* s) __asm__("_atoi");
int stub_atoi(const char* s) { trace += std::string("atoi:") + s + ";"; int v = 0; while (*s >= '0' && *s <= '9') v = v * 10 + (*s++ - '0'); return v; }
char* __wrap_strdup(const char* s) {
    std::string t = s; lastDup = new char[t.size() + 1]; for (size_t i = 0; i <= t.size(); ++i) lastDup[i] = s[i];
    trace += "strdup:" + t + ";"; return lastDup;
}
void __wrap_free(void* block) { trace += block == lastDup ? "freedup;" : "free;"; }
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third) {
    trace += "get:" + off(table, 0x900) + "," + std::to_string(id) + "," + std::to_string(third) + ";";
    return strings + id;
}
void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, int f) { trace += "settext:" + off(d, 0x600) + "," + std::to_string(t - strings) + "," + std::to_string(f) + ";"; }
void Hide__11VIGUIDialog(void* d) { trace += "hide:" + off(d, 0x600) + ";"; }
void AddPage__20VIWndGenericRendererPCc(void* r, const char* p) { trace += "page:" + off(r, 0x400) + "," + p + ";"; }
void SetStringValue__9VIGUIPagePCcPCUsb(void* p, const char* n, const unsigned short* v, int f) {
    trace += "setw:" + off(p, 0x700) + "," + n + "," + std::to_string(v - strings) + "," + std::to_string(f) + ";";
}
void SetStringValue__9VIGUIPagePCcT1b(void* p, const char* n, const char* v, int f) {
    trace += "setn:" + off(p, 0x700) + "," + n + "=" + v + "," + std::to_string(f) + ";";
}
void doClientDiscovery__Q212RealmService15RealmGatewayAPIPCUsiT1Pv(void* api, const unsigned short* name, int a, int b, void* c) {
    trace += "disc:" + off(api, 0x100) + "," + std::string(1, static_cast<char>(name[0])) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c != nullptr) + ";";
}
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep) {
    trace += "cloneW;";
    unsigned char* r = static_cast<unsigned char*>(rep);
    return newRep(norrath_aoc::load_word(r), norrath_aoc::load_word(r + 4), 2, 1) + 16;
}
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep) {
    trace += "cloneC;";
    unsigned char* r = static_cast<unsigned char*>(rep);
    unsigned n = norrath_aoc::load_word(r);
    unsigned char* p = newRep(n, norrath_aoc::load_word(r + 4), 1, 1);
    for (unsigned i = 0; i <= n; ++i) p[16 + i] = r[16 + i];
    return p + 16;
}
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(
    void* string, unsigned position, unsigned count, const unsigned short* source, unsigned length) {
    trace += "replace:" + std::to_string(position) + "," + std::to_string(count) + "," + std::to_string(length) + ";";
    unsigned char* p = newRep(length, length, 2, 1);
    for (unsigned i = 0; i < length; ++i) reinterpret_cast<unsigned short*>(p + 16)[i] = source[i];
    *static_cast<unsigned char**>(string) = p + 16;
}
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* string, unsigned length, char fill) {
    trace += "resize:" + std::to_string(length) + "," + std::to_string(fill) + ";";
    unsigned char* p = newRep(length, length, 1, narrowRefs);
    for (unsigned i = 0; i <= length; ++i) p[16 + i] = static_cast<unsigned char>(fill);
    *static_cast<unsigned char**>(string) = p + 16;
}
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag) {
    trace += "alloc:" + std::to_string(length) + "," + std::to_string(flag) + ";";
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned char* old = *handle;
    unsigned char* p = newRep(length, length, 1, 1);
    for (unsigned i = 0; i <= length; ++i) p[16 + i] = old[i];
    norrath_aoc::store_word(old - 8, norrath_aoc::load_word(old - 8) - 1);
    *handle = p + 16;
}
}

static int slotOf(unsigned bytes) { return bytes < 129 ? static_cast<int>(((bytes + 7) >> 3) - 1) : -1; }

int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    store_pointer(object + 12, arena_at(0x400));
    store_pointer(arena_at(0x400) + 56, arena_at(0x600));
    store_pointer(arena_at(0x400) + 52, arena_at(0x700));
    store_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui), arena_at(0x900));
    store_word(arena_at(0x900) + 48, 77);
    for (const char* c = "Game"; *c; ++c) object[276 + (c - "Game")] = static_cast<unsigned char>(*c);
    object[280] = 0;
    object[16] = 'a'; object[17] = 0;
    // non-zero statuses
    for (int status : {1, 2, -1, 5}) {
        trace.clear();
        static const unsigned short empty[] = {0};
        onGetGame__16VIRealmInterfaceiiPCUsT3iPv(object, 8, status, empty, empty, 3, nullptr);
        std::string want = "head:8," + std::to_string(status) + ";get:0," + (status == 1 ? "1861" : "1853") + ",77;settext:0," + (status == 1 ? "1861" : "1853") + ",0;";
        if (trace != want) { ok = false; std::fputs(("status " + std::to_string(status) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout); }
    }
    const char* addresses[] = {"host:1234", "example.com:80", "nocolon", "", ":99", "a:b:7", "h:", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx:65000", "y0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"};
    for (const char* narrow : addresses)
    for (unsigned flagW : {0u, 1u}) for (unsigned flagC : {0u, 1u}) for (unsigned refs : {1u, 2u}) {
        static unsigned short text[300];
        unsigned length = 0;
        for (const char* c = narrow; *c; ++c) text[length++] = static_cast<unsigned short>(*c + (length % 2 ? 0x100 : 0));
        text[length] = 0;
        trace.clear(); heapTop = 0; narrowRefs = refs;
        for (int i = 0; i < 16; ++i) norrath_free_list[i] = 0;
        store_word(norrath_nil_rep_ushort + 12, flagW); store_word(norrath_nil_rep_ushort + 8, 50);
        store_word(norrath_nil_rep_char + 12, flagC); store_word(norrath_nil_rep_char + 8, 60);
        store_word(object + 3372, 0xabcd); store_word(object + 3880, 0);
        onGetGame__16VIRealmInterfaceiiPCUsT3iPv(object, 8, 0, text, nullptr, 0x40, nullptr);

        std::string want = "head:8,0;page:0,data/ui/GameClnt.xml;";
        if (flagW) want += "cloneW;";
        want += "replace:0,4294967295," + std::to_string(length) + ";";
        if (flagC) want += "cloneC;";
        want += "resize:" + std::to_string(length) + ",0;";
        std::vector<unsigned> released;   // byte sizes of the blocks released: narrow, copy, wide (0 when still shared)
        if (length > 0) {
            if (refs >= 2) want += "alloc:" + std::to_string(length) + ",1;";
            want += "cloneC;";
            released = {length + 16, length + 16, 2 * length + 16};
        } else {
            released = {0u, refs == 1 ? 16u : 0u, 16u};
        }
        if (released[0] > 128) want += "free;";
        want += "strdup:" + std::string(narrow) + ";";
        if (released[1] > 128) want += "free;";
        if (released[2] > 128) want += "free;";
        std::string host = narrow;
        size_t colon = host.find(':');
        want += "strstr:" + host + ",:;";
        unsigned expectPort = 0xabcd;
        if (colon != std::string::npos) {
            std::string tail = host.substr(colon + 1);
            int v = 0; for (char ch : tail) { if (ch < '0' || ch > '9') break; v = v * 10 + (ch - '0'); }
            expectPort = static_cast<unsigned>(v);
            want += "atoi:" + tail + ";snprintf:2348,1023," + host.substr(0, colon) + ";";
        }
        want += "freedup;disc:0,a,64,0,0;get:0,1839,77;setw:0,ServerStatus,1839,1;setn:0,GameName=Game,0;hide:0;";
        unsigned expectSlots = 0;
        { bool seen[16] = {}; for (unsigned bytes : released) { int s = bytes ? slotOf(bytes) : -1; if (s >= 0 && !seen[s]) { seen[s] = true; ++expectSlots; } } }
        unsigned pooled = 0;
        for (int i = 0; i < 16; ++i) if (norrath_free_list[i] != 0) ++pooled;
        bool good = trace == want && pooled == expectSlots && load_word(object + 3372) == expectPort && load_word(object + 3880) == 0x40 &&
                    load_word(norrath_nil_rep_ushort + 8) == (flagW ? 50u : 51u) && load_word(norrath_nil_rep_char + 8) == (flagC ? 60u : 61u);
        if (!good) {
            ok = false;
            std::fputs(("addr " + std::string(narrow).substr(0, 20) + " W" + std::to_string(flagW) + " C" + std::to_string(flagC) + " r" + std::to_string(refs) +
                        "\n got  " + trace + "\n want " + want + "\n pooled " + std::to_string(pooled) + " want " + std::to_string(expectSlots) + "\n").c_str(), stdout);
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
