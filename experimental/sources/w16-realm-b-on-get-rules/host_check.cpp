// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// The string-library entry points are replaced by small models that allocate reps (16-byte header, then characters) in the address
// arena. The expected trace, printed text and pool-list contents are written out from the string semantics in the source comment.
#include <cstdio>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "on_get_rules.cpp"

unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

static std::string trace;
static unsigned heapTop;
static unsigned narrowRefs;
static std::string printed;
static unsigned char* newRep(unsigned length, unsigned capacity, unsigned element, unsigned refs) {
    unsigned char* rep = arena_at(0x10000 + heapTop);
    heapTop += (16 + element * (capacity + 1) + 15) & ~15u;
    norrath_aoc::store_word(rep, length); norrath_aoc::store_word(rep + 4, capacity);
    norrath_aoc::store_word(rep + 8, refs); norrath_aoc::store_word(rep + 12, 0);
    return rep;
}
extern "C" {
int stub_printf(const char* f, ...) __asm__("_printf");
int stub_printf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    if (std::string(f) == "%s\n") { printed = __builtin_va_arg(ap, const char*); trace += "print;"; }
    else { unsigned a = __builtin_va_arg(ap, unsigned); int b = __builtin_va_arg(ap, int); trace += "head:" + std::to_string(a) + "," + std::to_string(b) + ";"; }
    __builtin_va_end(ap);
    return 0;
}
void* GetAppPtr__5VIWnd(void* r) { trace += "app:" + std::to_string(static_cast<unsigned char*>(r) - arena_at(0x400)) + ";"; return arena_at(0xa00); }
int snprintf16__8VIStringPUsiPCce(unsigned short* out, int count, const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    const unsigned short* t = __builtin_va_arg(ap, const unsigned short*);
    __builtin_va_end(ap);
    trace += "fmt:" + std::to_string(count) + "," + f + ";";
    out[0] = t[0];
    return 1;
}
void ReceiveEula__9VIStationiPCUsi(void* s, int status, const unsigned short* text, int n) {
    trace += "eula:" + std::string(s == arena_at(0xa00) ? "app" : "?") + "," + std::to_string(status) + "," + std::to_string(n) + ";";
    (void)text;
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
void __wrap_free(void*) { trace += "free;"; }
}

static int slotOf(unsigned bytes) { return bytes < 129 ? static_cast<int>(((bytes + 7) >> 3) - 1) : -1; }

int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    store_pointer(object + 12, arena_at(0x400));
    for (unsigned length : {0u, 1u, 5u, 56u, 57u, 112u, 113u, 130u})
    for (unsigned flagW : {0u, 1u}) for (unsigned flagC : {0u, 1u}) for (unsigned refs : {1u, 2u}) {
        static unsigned short text[200];
        for (unsigned i = 0; i < length; ++i) text[i] = static_cast<unsigned short>(0x100 * (i % 3) + 'a' + i % 26);
        text[length] = 0;
        trace.clear(); printed = "<unset>"; heapTop = 0; narrowRefs = refs;
        for (int i = 0; i < 16; ++i) norrath_free_list[i] = 0;
        store_word(norrath_nil_rep_ushort + 12, flagW); store_word(norrath_nil_rep_ushort + 8, 50);
        store_word(norrath_nil_rep_char + 12, flagC); store_word(norrath_nil_rep_char + 8, 60);
        onGetRules__16VIRealmInterfaceUiiPCUsUiPv(object, 11, -3, text, length, nullptr);

        std::string want = "head:11,-3;app:0;fmt:614400,%S\n\n\n\n;eula:app,-3," + std::to_string(length + 4) + ";";
        if (flagW) want += "cloneW;";
        want += "replace:0,4294967295," + std::to_string(length) + ";";
        if (flagC) want += "cloneC;";
        want += "resize:" + std::to_string(length) + ",0;";
        // releases in order: narrow, copy, wide. Each entry is the byte size of the block, or 0 when no block is freed.
        std::vector<unsigned> released;
        if (length > 0) {
            if (refs >= 2) want += "alloc:" + std::to_string(length) + ",1;";
            want += "cloneC;";         // the written flag word makes the copy a real clone
            released = {length + 16, length + 16, 2 * length + 16};
        } else {
            // an empty narrow string is shared: the copy adds a reference
            released = {0u, refs == 1 ? 16u : 0u, 16u};   // the shared block dies with the last holder only when it started with one count
        }
        if (released[0] > 128) want += "free;";   // the narrow string is released before the print
        want += "print;";
        for (size_t i = 1; i < released.size(); ++i) if (released[i] > 128) want += "free;";
        std::string expectPrint(length, '\0');
        for (unsigned i = 0; i < length; ++i) expectPrint[i] = static_cast<char>(text[i] & 255);
        unsigned expectSlots = 0;
        { bool seen[16] = {}; for (unsigned bytes : released) { int s = bytes ? slotOf(bytes) : -1; if (s >= 0 && !seen[s]) { seen[s] = true; ++expectSlots; } } }
        unsigned pooled = 0;
        for (int i = 0; i < 16; ++i) if (norrath_free_list[i] != 0) ++pooled;
        bool good = trace == want && printed == expectPrint && pooled == expectSlots &&
                    load_word(norrath_nil_rep_ushort + 8) == (flagW ? 50u : 51u) && load_word(norrath_nil_rep_char + 8) == (flagC ? 60u : 61u);
        if (!good) {
            ok = false;
            std::fputs(("len " + std::to_string(length) + " W" + std::to_string(flagW) + " C" + std::to_string(flagC) + " r" + std::to_string(refs) +
                        "\n got  " + trace + "\n want " + want + "\n printed '" + printed + "' pooled " + std::to_string(pooled) + " want " + std::to_string(expectSlots) + "\n").c_str(), stdout);
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
