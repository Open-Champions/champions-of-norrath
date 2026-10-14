// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// The string-library entry points are replaced by small models that allocate reps (16-byte header, then characters) in the address
// arena. The expected trace, login arguments and pool contents are written out from the semantics in the source comment.
#include <cstdio>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#define strlen norrath_test_strlen   // route the source's strlen to the stub without touching the library's own calls
#include "on_login.cpp"
#undef strlen

unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

static std::string trace;
static unsigned heapTop, narrowRefs, extraCapacity;
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
    unsigned a = __builtin_va_arg(ap, unsigned); int b = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    trace += std::string("printf:") + f + std::to_string(a) + "," + std::to_string(b) + ";";
    return 0;
}
unsigned long norrath_test_strlen(const char* s) { unsigned long n = 0; while (s[n]) ++n; trace += std::string("strlen:") + s + ";"; return n; }
void* GetAppPtr__5VIWnd(void* r) { trace += "app:" + off(r, 0x400) + ";"; return arena_at(0xa00); }
void UnLock__11VIGUIDialog(void* d) { trace += "unlock:" + off(d, 0x600) + ";"; }
void strncpyWide__FPUsPCUsUi(unsigned short* out, const unsigned short* in, unsigned n) {
    trace += "copy:" + off(out, 0x100) + "," + std::to_string(n) + ";";
    for (unsigned i = 0; i < n; ++i) { out[i] = in[i]; if (!in[i]) break; }
}
int strlenWide__FPCUs(const unsigned short* t) { int n = 0; while (t[n]) ++n; trace += "len:" + off(t, 0x100) + ";"; return n; }
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep) {
    trace += "cloneW;";
    unsigned char* r = static_cast<unsigned char*>(rep);
    unsigned n = norrath_aoc::load_word(r);
    unsigned char* p = newRep(n, norrath_aoc::load_word(r + 4), 2, 1);
    for (unsigned i = 0; i <= n; ++i) reinterpret_cast<unsigned short*>(p + 16)[i] = reinterpret_cast<unsigned short*>(r + 16)[i];
    return p + 16;
}
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep) {
    trace += "cloneC;";
    unsigned char* r = static_cast<unsigned char*>(rep);
    return newRep(norrath_aoc::load_word(r), norrath_aoc::load_word(r + 4), 1, 1) + 16;
}
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(
    void* string, unsigned position, unsigned count, const char* source, unsigned length) {
    trace += "replaceC:" + std::to_string(position) + "," + std::to_string(count) + "," + source + "," + std::to_string(length) + ";";
    unsigned char* p = newRep(length, length + extraCapacity, 1, 1);
    for (unsigned i = 0; i <= length; ++i) p[16 + i] = static_cast<unsigned char>(source[i]);
    *static_cast<unsigned char**>(string) = p + 16;
}
void resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs(void* string, unsigned length, unsigned short fill) {
    trace += "resizeW:" + std::to_string(length) + "," + std::to_string(fill) + ";";
    unsigned char* p = newRep(length, length + extraCapacity, 2, narrowRefs);
    for (unsigned i = 0; i <= length; ++i) reinterpret_cast<unsigned short*>(p + 16)[i] = fill;
    *static_cast<unsigned char**>(string) = p + 16;
}
void alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag) {
    trace += "allocW:" + std::to_string(length) + "," + std::to_string(flag) + ";";
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned char* old = *handle;
    unsigned char* p = newRep(length, length + extraCapacity, 2, 1);
    for (unsigned i = 0; i <= length; ++i) reinterpret_cast<unsigned short*>(p + 16)[i] = reinterpret_cast<unsigned short*>(old)[i];
    norrath_aoc::store_word(old - 8, norrath_aoc::load_word(old - 8) - 1);
    *handle = p + 16;
}
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs(
    void* string, unsigned position, unsigned count, unsigned repeat, unsigned short unit) {
    trace += "fill:" + std::to_string(position) + "," + std::to_string(count) + "," + std::to_string(repeat) + "," + std::to_string(unit) + ";";
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned short* old = reinterpret_cast<unsigned short*>(*handle);
    unsigned n = norrath_aoc::load_word(*handle - 16);
    unsigned char* p = newRep(n + repeat, n + repeat + extraCapacity, 2, 1);
    unsigned short* data = reinterpret_cast<unsigned short*>(p + 16);
    for (unsigned i = 0; i < position; ++i) data[i] = old[i];
    for (unsigned i = 0; i < repeat; ++i) data[position + i] = unit;
    data[n + repeat] = 0;
    *handle = p + 16;
}
void login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(void* api, const unsigned short* first, const unsigned short* second, void* data) {
    std::string a, b;
    unsigned la = norrath_aoc::load_word(reinterpret_cast<const unsigned char*>(first) - 16);
    unsigned lb = norrath_aoc::load_word(reinterpret_cast<const unsigned char*>(second) - 16);
    for (unsigned i = 0; i < la; ++i) a += first[i] ? static_cast<char>(first[i]) : '0';
    for (unsigned i = 0; i < lb; ++i) b += second[i] ? static_cast<char>(second[i]) : '0';
    trace += "login:" + off(api, 0x100) + "," + a + "," + b + "," + std::to_string(data != nullptr) + ";";
}
void __wrap_free(void*) { trace += "free;"; }
}

static int slotOf(unsigned bytes) { return bytes < 129 ? static_cast<int>(((bytes + 7) >> 3) - 1) : -1; }

int main() {
    using namespace norrath_aoc;
    bool ok = true;
    unsigned char* object = arena_at(0x100);
    store_pointer(object + 12, arena_at(0x400));
    store_pointer(arena_at(0x400) + 56, arena_at(0x600));
    static const unsigned short name[] = {'T', 'e', 's', 't', 0};
    // status 0: copy the name
    for (int i = 0; i < 100; ++i) object[16 + i] = 0x77;
    store_word(object + 272, 999);
    trace.clear();
    onLogin__16VIRealmInterfaceUiiPCUsPv(object, 5, 0, name, nullptr);
    if (trace != "app:0;unlock:0;copy:16,256;len:16;printf:VIRealmInterface::onLogin(%i, %i)\n5,0;" || load_word(object + 272) != 4 ||
        object[16] != 'T' || object[24] != 0) { ok = false; std::fputs(("status 0: " + trace + "\n").c_str(), stdout); }
    for (unsigned flagW : {0u, 1u}) for (unsigned flagC : {0u, 1u}) for (unsigned refs : {1u, 2u}) for (unsigned extra : {0u, 130u}) for (int status : {1, -7}) {
        trace.clear(); heapTop = 0; narrowRefs = refs; extraCapacity = extra;
        for (int i = 0; i < 16; ++i) norrath_free_list[i] = 0;
        store_word(norrath_nil_rep_ushort + 12, flagW); store_word(norrath_nil_rep_ushort + 8, 50);
        store_word(norrath_nil_rep_char + 12, flagC); store_word(norrath_nil_rep_char + 8, 60);
        onLogin__16VIRealmInterfaceUiiPCUsPv(object, 6, status, name, nullptr);
        std::string want = "app:0;unlock:0;";
        unsigned wideBytes = 2 * (3 + extra) + 16, narrowBytes = 3 + extra + 16;
        for (const char* lit : {"foo", "bar"}) {
            if (flagC) want += "cloneC;";
            want += std::string("strlen:") + lit + ";replaceC:0,4294967295," + lit + ",3;";
            if (flagW) want += "cloneW;";
            want += "resizeW:3,0;";
            if (refs >= 2) want += "allocW:3,1;";
            want += "cloneW;";                         // the flag written by the widening loop forces a real copy
            if (wideBytes > 128) want += "free;";      // the working wide string is released
            want += "fill:3,0,1,0;";
            if (narrowBytes > 128) want += "free;";    // the narrow string is released last
        }
        want += "login:0,foo0,bar0,0;";
        if (2 * (4 + extra) + 16 > 128) want += "free;free;";   // both final copies are the last holders of their one-unit-longer blocks
        want += "printf:VIRealmInterface::onLogin(%i, %i)\n6," + std::to_string(status) + ";";
        unsigned expectSlots = extra ? 0 : (slotOf(wideBytes) == slotOf(narrowBytes) ? 1 : 2);
        unsigned pooled = 0;
        for (int i = 0; i < 16; ++i) if (norrath_free_list[i] != 0) ++pooled;
        bool good = trace == want && pooled == expectSlots &&
                    load_word(norrath_nil_rep_ushort + 8) == (flagW ? 50u : 52u) && load_word(norrath_nil_rep_char + 8) == (flagC ? 60u : 62u);
        if (!good) {
            ok = false;
            std::fputs(("W" + std::to_string(flagW) + " C" + std::to_string(flagC) + " r" + std::to_string(refs) + " x" + std::to_string(extra) +
                        "\n got  " + trace + "\n want " + want + "\n pooled " + std::to_string(pooled) + " want " + std::to_string(expectSlots) + "\n").c_str(), stdout);
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
