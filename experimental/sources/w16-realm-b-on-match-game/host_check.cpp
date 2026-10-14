// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// The string-library entry points are replaced by small models that allocate reps (16-byte header, then characters) in the address
// arena; every other callee records its arguments. The expected trace is written out row by row from the semantics in the source comment.
#include <cstdio>
#include <string>
#include <vector>
#define strlen norrath_test_strlen   // route the source's strlen to the stub without touching the library's own calls
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_match_game.cpp"
#undef strlen

unsigned vi_wnd_mui;
unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

static const unsigned OBJ = 0x20000, RENDERER = 0x2b000, DIALOG = 0x2b100, PAGE = 0x2b200, TABLE = 0x2b300, COMBO = 0x2c000, LISTS = 0x2d000,
                      TEXTS = 0x2e000, BLOBS = 0x30000, HEAP = 0x40000;
static std::string trace;
static unsigned heapTop, narrowRefs, extraCapacity;
static bool objectExists, castSucceeds;
static char* lastDup;
static unsigned short strings[4000];
static char pageName[] = "xFindGamey";
static unsigned char* newRep(unsigned length, unsigned capacity, unsigned element, unsigned refs) {
    unsigned char* rep = arena_at(HEAP + heapTop);
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
    int a = __builtin_va_arg(ap, int), b = __builtin_va_arg(ap, int), c = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += "head:" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
    return 0;
}
int stub_sprintf(char* out, const char* f, ...) __asm__("_sprintf");
int stub_sprintf(char* out, const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    const char* a = __builtin_va_arg(ap, const char*); const char* b = __builtin_va_arg(ap, const char*);
    __builtin_va_end(ap);
    std::string line = std::string(a) + " (" + b + ")";
    trace += std::string("sprintf:") + f + "|" + a + "|" + b + ";";
    for (size_t i = 0; i <= line.size(); ++i) out[i] = line[i];
    return static_cast<int>(line.size());
}
void* stub_memset(void* d, int v, unsigned long n) __asm__("_memset");
void* stub_memset(void* d, int v, unsigned long n) {
    trace += "memset:" + off(d, OBJ) + "," + std::to_string(v) + "," + std::to_string(n) + ";";
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned long i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
char* stub_strcpy(char* d, const char* s) __asm__("_strcpy");
char* stub_strcpy(char* d, const char* s) {
    trace += "strcpy:" + off(d, OBJ) + "," + s + ";";
    char* r = d; while ((*d++ = *s++)) {} return r;
}
unsigned long norrath_test_strlen(const char* s) { unsigned long n = 0; while (s[n]) ++n; return n; }
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
    trace += "get:" + off(table, TABLE) + "," + std::to_string(id) + "," + std::to_string(third) + ";";
    return strings + id;
}
void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, int f) {
    trace += "settext:" + off(d, DIALOG) + "," + (t >= strings && t < strings + 4000 ? "str" + std::to_string(t - strings) : "buf") + "," + std::to_string(f) + ";";
}
int snprintf16__8VIStringPUsiPCUse(unsigned short* out, int count, const unsigned short* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    unsigned n = __builtin_va_arg(ap, unsigned);
    __builtin_va_end(ap);
    trace += "sn16:" + std::to_string(count) + "," + std::to_string(f - strings) + "," + std::to_string(n) + ";";
    out[0] = 'B'; out[1] = 0;
    return 1;
}
void AddPage__20VIWndGenericRendererPCc(void* r, const char* p) { trace += "page:" + off(r, RENDERER) + "," + p + ";"; }
const char* GetName__9VIGUIPage(void* p) { trace += "name:" + off(p, PAGE) + ";"; return pageName; }
void* GetObjectByName__9VIGUIPagePCc(void* p, const char* n) {
    trace += "obj:" + off(p, PAGE) + "," + n + ";";
    return objectExists ? arena_at(COMBO) : nullptr;
}
void* __dynamic_cast_2(void* function, const void* target, int offsetValue, void* adjusted, const void* source, void* object) {
    (void)function; (void)target; (void)source;
    trace += "cast:" + std::to_string(offsetValue) + "," + off(adjusted, COMBO) + "," + off(object, COMBO) + ";";
    return castSucceeds ? arena_at(COMBO) : nullptr;
}
void __tf13VIGUIComboBox() {}
void __tf11VIGUIObject() {}
void Clear__13VIGUIComboBox(void* c) { trace += "clear:" + off(c, COMBO) + ";"; }
void AddOption__13VIGUIComboBoxPc(void* c, const char* t) { trace += "option:" + off(c, COMBO) + "," + t + ";"; }
void SetValue__13VIGUIComboBoxi(void* c, int v) { trace += "value:" + off(c, COMBO) + "," + std::to_string(v) + ";"; }
void SetStringValue__9VIGUIPagePCcT1b(void* p, const char* n, const char* v, int f) {
    trace += "setvalue:" + off(p, PAGE) + "," + n + "=" + off(v, OBJ) + "," + std::to_string(f) + ";";
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
    trace += "replaceW:" + std::to_string(position) + "," + std::to_string(count) + "," + std::to_string(length) + ";";
    unsigned char* p = newRep(length, length + extraCapacity, 2, 1);
    for (unsigned i = 0; i < length; ++i) reinterpret_cast<unsigned short*>(p + 16)[i] = source[i];
    *static_cast<unsigned char**>(string) = p + 16;
}
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
    void* string, unsigned position, unsigned count, const char* source, unsigned length) {
    trace += "replaceC:" + off(string, OBJ) + "," + std::to_string(position) + "," + std::to_string(count) + "," + source + "," + std::to_string(length) + ";";
    unsigned char* p = newRep(length, length + extraCapacity, 1, 1);
    for (unsigned i = 0; i <= length; ++i) p[16 + i] = static_cast<unsigned char>(source[i]);
    norrath_aoc::store_pointer(static_cast<unsigned char*>(string), p + 16);
}
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* string, unsigned length, char fill) {
    trace += "resizeC:" + std::to_string(length) + "," + std::to_string(fill) + ";";
    unsigned char* p = newRep(length, length + extraCapacity, 1, narrowRefs);
    for (unsigned i = 0; i <= length; ++i) p[16 + i] = static_cast<unsigned char>(fill);
    *static_cast<unsigned char**>(string) = p + 16;
}
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag) {
    trace += "allocC:" + std::to_string(length) + "," + std::to_string(flag) + ";";
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned char* old = *handle;
    unsigned char* p = newRep(length, length + extraCapacity, 1, 1);
    for (unsigned i = 0; i <= length; ++i) p[16 + i] = old[i];
    norrath_aoc::store_word(old - 8, norrath_aoc::load_word(old - 8) - 1);
    *handle = p + 16;
}
}

static int slotOf(unsigned bytes) { return bytes < 129 ? static_cast<int>(((bytes + 7) >> 3) - 1) : -1; }
static void put_wide(unsigned at, const std::string& s) {
    unsigned short* p = reinterpret_cast<unsigned short*>(arena_at(at));
    for (size_t i = 0; i < s.size(); ++i) p[i] = static_cast<unsigned char>(s[i]);
    p[s.size()] = 0;
}

int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(OBJ);
    store_pointer(object + 12, arena_at(RENDERER));
    store_pointer(arena_at(RENDERER) + 56, arena_at(DIALOG));
    store_pointer(arena_at(RENDERER) + 52, arena_at(PAGE));
    store_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui), arena_at(TABLE));
    store_word(arena_at(TABLE) + 48, 77);
    store_pointer(arena_at(COMBO) + 148, arena_at(COMBO + 0x200));      // the object's table word
    *reinterpret_cast<short*>(arena_at(COMBO + 0x200)) = 5;           // adjustment halfword
    store_word(arena_at(COMBO + 0x200) + 4, 0x1234);
    const std::string comboGet = "obj:0,GameName;";
    const std::string comboCast = "cast:0,5,0;";
    auto dialogMessage = [](int id) { return "get:0," + std::to_string(id) + ",77;settext:0,str" + std::to_string(id) + ",0;"; };

    // non-row paths
    struct Simple { int status; unsigned count; bool object; bool cast; std::string tail; };
    const Simple simple[] = {
        {1, 3, true, true, dialogMessage(1861)}, {2, 3, true, true, dialogMessage(1852)}, {-1, 0, true, true, dialogMessage(1852)},
        {1, 0, true, true, dialogMessage(1861)},
        {0, 0, true, true, comboGet + comboCast + "clear:0;" + dialogMessage(1852)},
        {0, 0, true, false, comboGet + comboCast + dialogMessage(1852)},
        {0, 0, false, false, comboGet + dialogMessage(1852)}};
    for (const auto& c : simple) {
        trace.clear(); objectExists = c.object; castSucceeds = c.cast;
        onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv(object, 4, c.status, c.count, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        std::string want = "head:4," + std::to_string(c.status) + "," + std::to_string(c.count) + ";" + c.tail;
        if (trace != want) { ok = false; std::fputs(("simple status " + std::to_string(c.status) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout); }
    }

    // result rows
    const unsigned count = 3;
    const std::string list4[count] = {"Alpha:11", "Beta", ""};
    const std::string list5[count] = {"Desc one", "", "Third description"};
    const std::string hostData[count] = {"host:9000", "plain", "a:b"};
    const unsigned blobSize[count] = {300, 0, 220};
    const int ids[count] = {11, 22, 33};
    std::string tails[count];
    for (unsigned i = 0; i < count; ++i) { tails[i] = std::string(hostData[i]); tails[i].resize(blobSize[i] > 0 ? 260 : 0, 'z'); }
    for (const char* pname : {"xFindGamey", "Other"})
    for (int selectedIndex : {-1, 0, 1, 2, 5})
    for (unsigned flagW : {0u, 1u}) for (unsigned flagC : {0u, 1u}) for (unsigned refs : {1u, 2u}) for (unsigned extra : {0u, 130u}) {
        for (char& ch : pageName) ch = 0;
        for (size_t k = 0; pname[k]; ++k) pageName[k] = pname[k];
        trace.clear(); heapTop = 0; narrowRefs = refs; extraCapacity = extra; objectExists = true; castSucceeds = true;
        for (int i = 0; i < 16; ++i) norrath_free_list[i] = 0;
        store_word(norrath_nil_rep_ushort + 12, flagW); store_word(norrath_nil_rep_ushort + 8, 50);
        store_word(norrath_nil_rep_char + 12, flagC); store_word(norrath_nil_rep_char + 8, 60);
        // lists: 4-byte pointer words to wide strings; blobs: 8-byte records
        std::vector<std::string> blobText(count);
        for (unsigned i = 0; i < count; ++i) {
            put_wide(TEXTS + 0x200 * i, list4[i]); put_wide(TEXTS + 0x100 + 0x200 * i, list5[i]);
            store_pointer(arena_at(LISTS) + 4 * i, arena_at(TEXTS + 0x200 * i));
            store_pointer(arena_at(LISTS + 0x40) + 4 * i, arena_at(TEXTS + 0x100 + 0x200 * i));
            char* d = reinterpret_cast<char*>(arena_at(BLOBS + 0x400 + 0x200 * i));
            const std::string& full = blobSize[i] ? tails[i] : hostData[i];
            for (size_t k = 0; k <= full.size(); ++k) d[k] = full[k];
            store_pointer(arena_at(BLOBS) + 8 * i, arena_at(BLOBS + 0x400 + 0x200 * i));
            store_word(arena_at(BLOBS) + 8 * i + 4, blobSize[i]);
            for (unsigned base : {4284u, 4684u}) {
                unsigned char* rep = newRep(2, 2, 1, 1);
                rep[16] = rep[17] = 'z'; rep[18] = 0;
                store_pointer(object + base + 4 * i, rep + 16);
            }
        }
        store_word(arena_at(COMBO) + 272, static_cast<unsigned>(selectedIndex));
        for (unsigned i = 0; i < 100; ++i) store_word(object + 3480 + 4 * i, 0x5000 + i);
        if (selectedIndex >= 0 && selectedIndex < 100) store_word(object + 3480 + 4 * selectedIndex, 22);
        for (unsigned i = 0; i < 100; ++i) { object[9992 + 256 * i] = 'Q'; object[0x8b1c + 24 * i] = 'R'; }
        const int expectSelected = selectedIndex >= 0 ? (selectedIndex < 100 ? 22 : static_cast<int>(0x5000 + selectedIndex)) : -1;
        const unsigned char* names = arena_at(LISTS);
        const unsigned char* descriptions = arena_at(LISTS + 0x40);
        onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv(object, 4, 0, count, names, descriptions, nullptr, ids,
                                                                              arena_at(BLOBS), nullptr);

        std::string want = "head:4,0,3;name:0;strstr:" + std::string(pname) + ",FindGame;";
        if (std::string(pname).find("FindGame") == std::string::npos) want += "get:0,1851,77;sn16:4095,1851,3;settext:0,buf,0;page:0,data/ui/FindGame.xml;";
        want += comboGet + comboCast + "clear:0;memset:" + std::to_string(0x8b1c) + ",0,2400;";
        std::vector<unsigned> released;
        auto rel = [&](unsigned bytes) { released.push_back(bytes); if (bytes > 128) want += "free;"; };
        auto narrowWork = [&](unsigned length) {   // one wide-to-narrow conversion; returns the trace for the steps up to the copy
            std::string t;
            if (flagC) t += "cloneC;";
            t += "resizeC:" + std::to_string(length) + ",0;";
            if (refs >= 2 && length > 0) t += "allocC:" + std::to_string(length) + ",1;";
            if (length > 0) t += "cloneC;";
            return t;
        };
        unsigned match = 0;
        for (unsigned i = 0; i < count; ++i) {
            const unsigned l5 = list5[i].size(), l4 = list4[i].size();
            // first block: list5 text
            if (flagW) want += "cloneW;";
            want += "replaceW:0,4294967295," + std::to_string(l5) + ";";
            std::string data = blobSize[i] ? tails[i] : hostData[i];
            want += "strstr:" + data + ",:;";
            size_t colon = data.find(':');
            std::string cut = colon == std::string::npos ? data : data.substr(0, colon);
            want += narrowWork(l5);
            if (l5 > 0) rel(l5 + extra + 16);            // working narrow string released after the copy
            want += "sprintf:%s (%s)|" + list5[i] + "|" + (blobSize[i] != 0 ? cut : std::string("?")) + ";";
            if (l5 > 0) rel(l5 + extra + 16);            // the copy is the last holder
            else if (refs == 1) rel(extra + 16);
            if (colon != std::string::npos) want += "strcpy:" + std::to_string(9992 + 256 * i) + "," + data.substr(colon + 1) + ";";
            if (blobSize[i] >= 220) want += "strcpy:" + std::to_string(0x8b1c + 24 * i) + "," + data.substr(220) + ";";
            want += "option:0," + list5[i] + " (" + (blobSize[i] != 0 ? cut : std::string("?")) + ");";
            if (expectSelected == ids[i]) { want += "value:0," + std::to_string(i) + ";"; match = i; }
            // second block: list4 text duplicated
            if (flagW) want += "cloneW;";
            want += "replaceW:0,4294967295," + std::to_string(l4) + ";";
            want += narrowWork(l4);
            if (l4 > 0) rel(l4 + extra + 16);
            want += "strdup:" + list4[i] + ";";
            if (l4 > 0) rel(l4 + extra + 16); else if (refs == 1) rel(extra + 16);
            rel(2 * (l4 + extra) + 16);
            want += "strstr:" + list4[i] + ",:;";
            size_t c4 = list4[i].find(':');
            if (c4 != std::string::npos) {
                std::string head = list4[i].substr(0, c4), tail = list4[i].substr(c4 + 1);
                want += "atoi:" + tail + ";replaceC:" + std::to_string(4284 + 4 * i) + ",0,4294967295," + head + "," + std::to_string(head.size()) + ";";
            }
            want += "freedup;";
            // third block: list5 text assigned into the second table
            if (flagW) want += "cloneW;";
            want += "replaceW:0,4294967295," + std::to_string(l5) + ";";
            want += narrowWork(l5);
            if (l5 > 0) rel(l5 + extra + 16);
            rel(18);                                          // the old slot string (capacity 2, one holder) is released
            // the copy is then shared into the slot (count 2) and released (count 1): no block is freed here
            rel(2 * (l5 + extra) + 16);                       // third wide string
            rel(2 * (l5 + extra) + 16);                       // first wide string, released last
        }
        want += "setvalue:0,HelpText=" + std::to_string(9992 + 256 * match) + ",0;";
        bool state = true;
        for (unsigned i = 0; i < count; ++i) {
            const unsigned char* slot2 = load_pointer(object + 4684 + 4 * i);
            state = state && load_word(slot2 - 16) == list5[i].size() && std::string(reinterpret_cast<const char*>(slot2), list5[i].size()) == list5[i];
            const unsigned char* slot1 = load_pointer(object + 4284 + 4 * i);
            size_t c4 = list4[i].find(':');
            if (c4 != std::string::npos) {
                state = state && load_word(slot1 - 16) == c4 && std::string(reinterpret_cast<const char*>(slot1), c4) == list4[i].substr(0, c4);
                state = state && load_word(object + 3884 + 4 * i) == static_cast<unsigned>(std::stoi(list4[i].substr(c4 + 1)));
            } else {
                state = state && load_word(slot1 - 16) == 2 && slot1[0] == 'z';
            }
            state = state && load_word(object + 3480 + 4 * i) == static_cast<unsigned>(ids[i]);
            std::string data = blobSize[i] ? tails[i] : hostData[i];
            size_t colon = data.find(':');
            state = state && (colon == std::string::npos ? object[9992 + 256 * i] == 0 : std::string(reinterpret_cast<const char*>(object + 9992 + 256 * i)) == data.substr(colon + 1));
        }
        unsigned expectSlots = 0;
        { bool seen[16] = {}; for (unsigned bytes : released) { int sl = slotOf(bytes); if (sl >= 0 && !seen[sl]) { seen[sl] = true; ++expectSlots; } } }
        unsigned pooled = 0;
        for (int i = 0; i < 16; ++i) if (norrath_free_list[i] != 0) ++pooled;
        bool good = trace == want && pooled == expectSlots && state;
        if (!good) {
            ok = false;
            std::fputs(("page " + std::string(pname) + " sel " + std::to_string(selectedIndex) + " W" + std::to_string(flagW) + " C" + std::to_string(flagC) + " r" + std::to_string(refs) + " x" + std::to_string(extra) +
                        "\n got  " + trace + "\n want " + want + "\n pooled " + std::to_string(pooled) + " want " + std::to_string(expectSlots) + " state " + std::to_string(state) + "\n").c_str(), stdout);
            break;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
