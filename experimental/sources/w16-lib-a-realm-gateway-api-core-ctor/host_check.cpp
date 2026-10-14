// Host check, not a plan input. Pointer words are compared as their low 32 bits (the host pointer is wider).
#include <cstdio>
#include <cstring>
#define strlen norrath_host_strlen
#include "ctor.cpp"
#undef strlen
StringRep nilRep = {0, 0, 1, 0};
unsigned char vt_realm_gateway_api_core[8];
static int order[8], steps; static unsigned gen_args[11]; static const char* gen_name; static short gen_b; static void* gen_self;
static void* enc_self; static void* rep_self; static unsigned rep_pos, rep_count, rep_len; static const char* rep_text; static unsigned rep_word_seen;
static unsigned char clone_block[32]; static int clone_calls;
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
extern "C" unsigned norrath_host_strlen(const char* s) { return std::strlen(s); }
extern "C" void __Q210GenericAPI14GenericAPICorePCcsUiUiUiUiUiUiUiUi(void* s, const char* n, short b, unsigned a, unsigned b2, unsigned c, unsigned d, unsigned e, unsigned f, unsigned g, unsigned h) {
    order[steps++] = 1; gen_self = s; gen_name = n; gen_b = b; unsigned v[8] = {a, b2, c, d, e, f, g, h}; std::memcpy(gen_args, v, sizeof v);
}
extern "C" void __9Encryptor(void* s) { order[steps++] = 2; enc_self = s; }
extern "C" unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(StringRep*) { ++clone_calls; return clone_block; }
extern "C" void replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi(void* s, unsigned p, unsigned c, const char* t, unsigned l) {
    order[steps++] = 3; rep_self = s; rep_pos = p; rep_count = c; rep_text = t; rep_len = l; rep_word_seen = *static_cast<unsigned*>(s);
}
int main() {
    alignas(8) unsigned char obj[144]; std::memset(obj, 0xaa, sizeof obj);
    int api;
    __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(obj, "name!", 0x12349661, &api);
    if (steps != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    const unsigned expect[8] = {60, 5, 0, 90, 32, 32, 120, 64};
    if (gen_self != obj || std::strcmp(gen_name, "name!") || gen_b != -27039 || std::memcmp(gen_args, expect, sizeof expect)) return 2;
    const unsigned* w = reinterpret_cast<const unsigned*>(obj);
    if (w[26] != low(&api) || w[27] != 0 || w[25] != low(vt_realm_gateway_api_core) || w[28] != 0) return 3;
    if (enc_self != obj + 116) return 4;
    if (rep_self != obj + 132 || rep_pos != 0 || rep_count != 0xffffffffu || rep_text == nullptr || std::strcmp(rep_text, "name!") || rep_len != 5) return 5;
    if (rep_word_seen != low(&nilRep) + 16 || nilRep.references != 2 || clone_calls != 0) return 6;
    if (*reinterpret_cast<const short*>(obj + 136) != -27039) return 7;
    nilRep.shared = 1; steps = 0;
    __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(obj, "", 5, &api);
    if (clone_calls != 1 || rep_word_seen != low(clone_block) || nilRep.references != 2 || rep_len != 0) return 8;
    std::puts("realm gateway api core ctor check passed");
    return 0;
}
