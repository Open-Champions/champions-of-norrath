// Observed msg_run__15AutoContainerLP at 0x225680, 104 bytes. If the pointer word at
// object+328 is nonzero, it calls the entry at byte offset 144 of the table whose
// address is the word at that second object's +72, with that object's address adjusted
// by the entry's 16-bit adjustment and a pointer to a copy of the three words at
// object+16. Then it calls msg_run__13AutoContainer on the original object, which is defined
// elsewhere.
// Unknown: what the pointed-to object is and what the table entry does; the three
// words at +16 are copied without being interpreted.

#include "../w8-actor-virtuals-b-common/virtual_call.hpp"

#pragma pack(push, 4)
struct Triple { unsigned a, b, c; };
#pragma pack(pop)

extern "C" void msg_run__13AutoContainer(void* object);

using w8avb::UWord;
using w8avb::Half;

extern "C" void msg_run__15AutoContainerLP(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned other_word = *reinterpret_cast<UWord*>(object + 328);
    if (other_word != 0) {
        unsigned char* other = const_cast<unsigned char*>(w8avb::address_of(other_word));
        const unsigned char* entry = w8avb::address_of(*reinterpret_cast<UWord*>(other + 72)) + 144;
        typedef void (*Function)(void*, Triple);
        Function function = reinterpret_cast<Function>(w8avb::function_of(*reinterpret_cast<const UWord*>(entry + 4)));
        Triple copy = *reinterpret_cast<const Triple*>(object + 16);
        function(other + *reinterpret_cast<const Half*>(entry), copy);
    }
    msg_run__13AutoContainer(self);
}
