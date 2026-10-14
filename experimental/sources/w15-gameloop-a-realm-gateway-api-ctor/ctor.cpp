#include "../w9-animation-object-c-common/mem.hpp"
// Observed __Q212RealmService15RealmGatewayAPIUiPCcs at 0x011241d0, 128 bytes, 48-byte frame. With (this, a, name, b)
// where b is a 16-bit value that the routine sign-extends itself (the argument register is not trusted; the source
// declares it as a plain int and narrows it): it stores a at this + 4 and the address of _vt$Q212RealmService15RealmGatewayAPI
// (0x01214760) at this + 8, allocates 144 bytes with __builtin_new, and when the block is not null constructs the core
// object in it with __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(block, name, b,
// this). The block pointer (possibly null) is stored at this + 0. Nothing is returned.
// Inferred: the wrapper owns a heap "core" object that keeps a back pointer to the wrapper.
// Unknown: the roles of a and b (the VIRealmInterface constructor passes 2 and -24735) and of the core's 144 bytes.
extern "C" {
extern unsigned char vt_realm_gateway_api[] __asm__("_vt$Q212RealmService15RealmGatewayAPI");
void* __builtin_new(unsigned size);
void __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(void* core, const char* name, short b,
                                                                                    void* api);

void __Q212RealmService15RealmGatewayAPIUiPCcs(void* self, unsigned a, const char* name, int b) {
    using namespace norrath_aoc;
    unsigned char* bytes = static_cast<unsigned char*>(self);
    store_word(bytes + 4, a);
    store_pointer(bytes + 8, vt_realm_gateway_api);
    void* core = __builtin_new(144);
    if (core != nullptr)
        __Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI(core, name, static_cast<short>(b), self);
    store_pointer(bytes, static_cast<unsigned char*>(core));
}
}
