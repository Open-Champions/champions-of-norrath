// Observed msg_collision__7VehicleP10GameObject at 0x2778a0, extent 64.
// The other object's flag word at +12 is tested against 0x40. When that bit is
// set and the other object's word at +1328 is nonzero, the function returns
// that word and links nothing. Otherwise it links the other object into the
// one-way list headed at vehicle+2136, passing the other object as both the
// second and third argument, and returns the callee's low word.
// Unknown: what the flag bit and word 1328 mean.
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);

namespace {
typedef unsigned __attribute__((may_alias)) AliasedWord;
}

extern "C" unsigned msg_collision__7VehicleP10GameObject(void* vehicle, void* other) {
    unsigned char* o = static_cast<unsigned char*>(other);
    if ((*reinterpret_cast<AliasedWord*>(o + 12) & 0x40u) != 0u) {
        const unsigned marker = *reinterpret_cast<AliasedWord*>(o + 1328);
        if (marker != 0u) return marker;
    }
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(
        norrath_list_oneway_link(static_cast<unsigned char*>(vehicle) + 2136, other, other)));
}
