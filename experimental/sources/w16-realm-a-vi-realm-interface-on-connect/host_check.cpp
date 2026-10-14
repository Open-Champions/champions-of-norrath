// Host check only; not a build input. Build: c++ -std=c++17 -O0 -fno-builtin -w host_check.cpp
#define NORRATH_HOST_ARENA 1
#include "on_connect.cpp"
#include "../w16-realm-a-common/host_string_model.hpp"
#include <cstdio>

StringRep nilRepWide, nilRepNarrow;
unsigned free_list[16];
int connected;
static std::string trace;
static bool inside_stub = true;   // libc++ string code calls the strlen stub below; only calls from the routine are traced
struct Guard { bool saved; Guard() : saved(inside_stub) { inside_stub = true; } ~Guard() { inside_stub = saved; } };
static int wide_clones, narrow_clones, unshares, frees;
static int resize_references = 1, clone_shared;
static std::string login_user, login_password;
static void* login_context;
extern "C" unsigned norrath_host_word_of(const void* p) { return hostmodel::word_of(p); }
extern "C" void* norrath_host_pointer_of(unsigned w) { return hostmodel::pointer_of(w); }
static std::string wide_text(const unsigned short* t) {
    std::string s;
    for (; *t; ++t) s += std::to_string(*t) + ",";
    return s;
}
extern "C" {
int printf(const char* f, ...) { Guard g; trace += std::string("printf(") + f + ");"; return 0; }
unsigned long strlen(const char* t) { unsigned long n = 0; while (t[n]) ++n; if (!inside_stub) { Guard g; trace += "strlen(" + std::to_string(n) + ");"; } return n; }
void __wrap_free(void*) { ++frees; }
void login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(void*, const unsigned short* user, const unsigned short* password, void* context) {
    Guard g;
    trace += "login;";
    login_user = wide_text(user); login_password = wide_text(password); login_context = context;
}
unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(StringRep* rep) { ++wide_clones; return (unsigned char*)hostmodel::pointer_of(hostmodel::new_rep((unsigned char*)rep + 16, rep->length, 2, rep->length, 1, clone_shared)); }
unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep* rep) { ++narrow_clones; return (unsigned char*)hostmodel::pointer_of(hostmodel::new_rep((unsigned char*)rep + 16, rep->length, 1, rep->length, 1, 0)); }
void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(void* self, unsigned pos, unsigned n, const char* t, unsigned len) {
    Guard g;
    trace += "replaceN(" + std::to_string(pos) + "," + std::to_string(n) + "," + t + "," + std::to_string(len) + ");";
    hostmodel::set_word(self, hostmodel::new_rep(t, len, 1, len, 1, 0));
}
void resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs(void* self, unsigned n, unsigned short c) {
    Guard g;
    trace += "resizeW(" + std::to_string(n) + "," + std::to_string(c) + ");";
    std::vector<unsigned short> fill(n, c);
    hostmodel::set_word(self, hostmodel::new_rep(fill.data(), n, 2, n, resize_references, 0));
}
void alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib(void* self, unsigned n, bool f) {
    ++unshares;
    unsigned old = hostmodel::word_at(self);
    StringRep* r = hostmodel::rep_of_word(old);
    r->references -= 1;
    hostmodel::set_word(self, hostmodel::new_rep(hostmodel::pointer_of(old), r->length, 2, r->length, 1, 0));
}
void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs(void* self, unsigned pos, unsigned n1, unsigned n2, unsigned short c) {
    Guard g;
    trace += "append(" + std::to_string(pos) + "," + std::to_string(n1) + "," + std::to_string(n2) + "," + std::to_string(c) + ");";
    unsigned old = hostmodel::word_at(self);
    StringRep* r = hostmodel::rep_of_word(old);
    std::vector<unsigned short> data((unsigned short*)hostmodel::pointer_of(old), (unsigned short*)hostmodel::pointer_of(old) + r->length);
    data.push_back(c);
    r->references -= 1;
    hostmodel::set_word(self, hostmodel::new_rep(data.data(), data.size(), 2, data.size(), 1, 0));
}
}

static void reset() {
    hostmodel::reset_arena();
    nilRepWide = {0, 0, 5, 0};
    nilRepNarrow = {0, 0, 7, 0};
    hostmodel::map_static(&nilRepWide, 16, 0x00f00000);
    hostmodel::map_static(&nilRepNarrow, 16, 0x00f00100);
    hostmodel::map_static(free_list, sizeof free_list, 0x00f01000);
    for (int i = 0; i < 16; ++i) free_list[i] = 0;
    trace.clear(); wide_clones = narrow_clones = unshares = frees = 0; resize_references = 1; clone_shared = 0;
    login_user.clear(); login_password.clear(); login_context = (void*)1; connected = 0;
}
static int chain_length(unsigned index) {
    int n = 0;
    for (unsigned w = free_list[index]; w != 0 && n < 20; w = *(unsigned*)hostmodel::pointer_of(w)) ++n;
    return n;
}
static void run() { inside_stub = false; onConnect__16VIRealmInterface(nullptr); inside_stub = true; }
#define CHECK(c) do { if (!(c)) { ok = false; std::puts(("line " + std::to_string(__LINE__)).c_str()); } } while (0)
int main() {
    bool ok = true;
    reset();
    run();
    CHECK(connected == 1);
    CHECK(trace == "printf(Connected to realm interface\n);"
                   "strlen(3);replaceN(0,4294967295,foo,3);resizeW(3,0);append(3,0,1,0);"
                   "strlen(3);replaceN(0,4294967295,bar,3);resizeW(3,0);append(3,0,1,0);login;");
    CHECK(login_user == "102,111,111," && login_password == "98,97,114," && login_context == nullptr);
    CHECK(unshares == 0 && frees == 0);
    CHECK(wide_clones == 2 && narrow_clones == 0);                 // one per literal: the buffer whose flag the store loop set
    CHECK(nilRepWide.references == 7 && nilRepNarrow.references == 9);
    CHECK(chain_length(2) == 8);                                   // four blocks per literal reach the free list (all 6..8 data bytes)
    // a buffer from resize already held twice is unshared once per literal
    reset();
    resize_references = 2;
    run();
    CHECK(unshares == 2);
    // when the first copy comes back flagged, the next copy is cloned as well
    reset();
    clone_shared = 1;
    run();
    CHECK(wide_clones == 4 && login_user == "102,111,111,");
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
