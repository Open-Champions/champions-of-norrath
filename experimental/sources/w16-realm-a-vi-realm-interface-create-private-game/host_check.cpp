// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin -w host_check.cpp
#define NORRATH_HOST_ARENA 1
#include "create_private_game.cpp"
#include "../w16-realm-a-common/host_string_model.hpp"
#include <cstdio>

StringRep nilRepWide, nilRepNarrow;
unsigned free_list[16];
static std::string trace;
static unsigned char* object;
static int wide_clones, narrow_clones, unshares, frees;
static int resize_references = 1;   // reference count given to the buffer made by resize
static struct { const void* first; const void* second; const void* third; void* context; unsigned short second_first; int calls; } base_call;
extern "C" unsigned norrath_host_word_of(const void* p) { return hostmodel::word_of(p); }
extern "C" void* norrath_host_pointer_of(unsigned w) { return hostmodel::pointer_of(w); }
extern "C" {
int snprintf(char* d, unsigned long n, const char* f, ...) {
    trace += "snprintf(" + std::to_string((unsigned char*)d - object) + "," + std::to_string(n) + "," + f + ");";
    return 0;
}
void __wrap_free(void*) { ++frees; }
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(void* self, unsigned pos, unsigned n, const unsigned short* t, unsigned len) {
    trace += "replace(" + std::to_string(pos) + "," + std::to_string(n) + "," + std::to_string(len) + ");";
    hostmodel::set_word(self, hostmodel::new_rep(t, len, 2, len, 1, 0));
}
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(StringRep* rep) { ++wide_clones; return (unsigned char*)hostmodel::pointer_of(hostmodel::new_rep((unsigned char*)rep + 16, rep->length, 2, rep->length, 1, 0)); }
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep* rep) { ++narrow_clones; return (unsigned char*)hostmodel::pointer_of(hostmodel::new_rep((unsigned char*)rep + 16, rep->length, 1, rep->length, 1, 0)); }
void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* self, unsigned n, char c) {
    trace += "resize(" + std::to_string(n) + "," + std::to_string(c) + ");";
    std::string fill(n, c);
    hostmodel::set_word(self, hostmodel::new_rep(fill.data(), n, 1, n, resize_references, 0));
}
void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* self, unsigned n, bool f) {
    ++unshares;
    unsigned old = hostmodel::word_at(self);
    StringRep* r = hostmodel::rep_of_word(old);
    r->references -= 1;
    hostmodel::set_word(self, hostmodel::new_rep(hostmodel::pointer_of(old), r->length, 1, r->length, 1, 0));
}
void createPrivateGame__Q212RealmService15RealmGatewayAPIPCUsN21Pv(void*, const unsigned short* a, const unsigned short* b, const unsigned short* c, void* ctx) {
    trace += "base;";
    base_call = {a, b, c, ctx, *b, base_call.calls + 1};
}
}

static void reset(int wide_nil_shared, int narrow_nil_shared) {
    hostmodel::reset_arena();
    nilRepWide = {0, 0, 5, wide_nil_shared};
    nilRepNarrow = {0, 0, 7, narrow_nil_shared};
    hostmodel::map_static(&nilRepWide, 16, 0x00f00000);
    hostmodel::map_static(&nilRepNarrow, 16, 0x00f00100);
    hostmodel::map_static(free_list, sizeof free_list, 0x00f01000);
    for (int i = 0; i < 16; ++i) free_list[i] = 0;
    trace.clear(); wide_clones = narrow_clones = unshares = frees = 0; resize_references = 1;
    base_call = {};
}
static int chain_length(unsigned index) {
    int n = 0;
    for (unsigned w = free_list[index]; w != 0 && n < 10; w = *(unsigned*)hostmodel::pointer_of(w)) ++n;
    return n;
}
int main() {
    static unsigned char buf[4000];
    object = buf;
    bool ok = true;
    unsigned short text[] = {0x41, 0x142, 0x7f, 0};
    unsigned short empty[] = {0};
    // ordinary text: low bytes are kept; the flag forces a clone for the copy; the empty strings are shared
    reset(0, 0);
    std::memset(buf, 0x5a, sizeof buf);
    createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(buf, -9, (const unsigned short*)0x11, text, (const unsigned short*)0x33, (void*)0x44);
    ok = ok && *(int*)(buf + 3472) == -9 && buf[3471] == 0x5a && buf[3476] == 0x5a;
    ok = ok && trace == "replace(0,4294967295,3);resize(3,0);snprintf(276,1023,AB\x7f);base;";
    ok = ok && base_call.first == (void*)0x11 && base_call.second == text && base_call.third == (void*)0x33 && base_call.context == (void*)0x44;
    ok = ok && nilRepWide.references == 6 && nilRepNarrow.references == 8;
    ok = ok && narrow_clones == 1 && wide_clones == 0 && unshares == 0 && frees == 0;
    ok = ok && chain_length(2) == 3 && chain_length(1) == 0;     // narrow, its copy, the wide copy: 3, 3 and 6 bytes + 16
    // empty text: no loop, the empty literal as format, the buffers are shared and the last owner frees them
    reset(0, 0);
    createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(buf, 5, (const unsigned short*)0x11, empty, (const unsigned short*)0x33, (void*)0x44);
    ok = ok && trace == "replace(0,4294967295,0);resize(0,0);snprintf(276,1023,);base;";
    ok = ok && narrow_clones == 0 && chain_length(1) == 2;
    // shared empty representations are cloned, not counted
    reset(1, 1);
    createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(buf, 5, (const unsigned short*)0x11, empty, (const unsigned short*)0x33, (void*)0x44);
    ok = ok && wide_clones == 1 && narrow_clones == 1 && nilRepWide.references == 5 && nilRepNarrow.references == 7;
    // a buffer from resize already held twice is unshared before the first store only
    reset(0, 0);
    resize_references = 2;
    createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(buf, 5, (const unsigned short*)0x11, text, (const unsigned short*)0x33, (void*)0x44);
    ok = ok && unshares == 1;
    // long text (129 characters): all three buffers exceed the free-list size and go to __wrap_free
    reset(0, 0);
    static unsigned short longtext[130];
    for (int i = 0; i < 129; ++i) longtext[i] = 'a' + i % 26;
    createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(buf, 5, (const unsigned short*)0x11, longtext, (const unsigned short*)0x33, (void*)0x44);
    ok = ok && frees == 3 && chain_length(2) == 0;
    // exactly 112 bytes of narrow data (112 + 16 = 128) still use the free list
    reset(0, 0);
    static unsigned short mid[113];
    for (int i = 0; i < 112; ++i) mid[i] = 'b';
    createPrivateGame__16VIRealmInterfaceiPCUsN22Pv(buf, 5, (const unsigned short*)0x11, mid, (const unsigned short*)0x33, (void*)0x44);
    ok = ok && frees == 1 && chain_length(15) == 2;              // the wide copy (224 + 16 bytes) is freed; the two narrow ones go to index 15
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
