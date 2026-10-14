#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__7Critteri at 0x1f0f48, 756 bytes. The halfword at
// object+572 is the current state; the animation state is at object+288.
// 1. A state other than 12 becomes 12 when findTagString__10GameObjectPci
//    (object, "path", 0) returns nonzero.
// 2. With current state (signed halfword) 13, a request outside {13, 14}
//    becomes 13; with current 14 the request becomes 14; with current 15 or 16
//    a request outside {15, 16} becomes the current value.
// 3. A request of 17 or more (unsigned) is replaced by the current state; so
//    are the requests 2-6, 8-11 below. Every path ends by storing the low 16
//    bits of the final request at +572.
// Per state (animAddTransitionTo is called on object+288):
//   0: eRand mod 120 plus the words at +808 and +1024 goes to +256; a second
//      eRand mod 60 plus +808 and +1028 goes to +1032; transition to the header
//      word at +1044, rate 0.3 (0x3e999999), flags 3.
//   1: eRand mod 120 plus +808 and +1020 goes to +256; resetRouting; a 12-byte
//      point from getSmartRandomDestination(range 200) is copied to +272; the
//      header is the word at +1060 when it is nonzero and flag bits 0xc00000
//      of the 64-bit word at +160 are set, otherwise the word at +1056; rate
//      0.1 (0x3dcccccc), flags 2051.
//   7: when the word at +1064 is zero the state stays the current one. Else
//      eRand mod 120 plus 60 plus +808 goes to +256, resetRouting, and a
//      transition to the +1064 header with rate 0.01 (0x3c23d70a), flags 2051.
//   12: transition to the +1056 header, rate 0.01, flags 2051.
//   15: when bit 1 of the word at +1076 is set, +1080 becomes 0; then as 13.
//   13: +808 plus 300 goes to +256; an eRand mod 60 plus +808 and +1028 goes
//      to +1032; transition to the +1068 header, rate 0.3, flags 3.
//   14 and 16: +808 goes to +256; bit 1 of the flag word at +12 is set;
//      transition to the +1072 header, rate 0.01, flags 2051.
// Unknown: the names of the states and of the fields at +256, +272, +1020 to
// +1080.
extern "C" {
int eRand__10GameObject(void* object);
char* findTagString__10GameObjectPci(void* object, char* tag, int occurrence);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
}

namespace {
void transition(rtti_c::Address object, unsigned header_offset, unsigned rate_bits, int flags) {
    using namespace rtti_c;
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
        bytes(object + 288u), pointer(read_word(object + header_offset)), single_from_bits(rate_bits), flags);
}
}

extern "C" void setState__7Critteri(void* self, int requested) {
    using namespace rtti_c;
    const Address object = address(self);
    unsigned state = static_cast<unsigned>(requested);
    if (state != 12u && findTagString__10GameObjectPci(self, const_cast<char*>("path"), 0) != nullptr) state = 12u;
    const int current = read_i16(object + 572u);
    const unsigned current_bits = read_u16(object + 572u);
    if (current == 13) {
        if (state - 13u >= 2u) state = 13u;
    } else if (current == 14) {
        state = 14u;
    }
    if (current == 15) {
        if (state - 15u >= 2u) state = 15u;
    } else if (current == 16) {
        if (state - 15u >= 2u) state = 16u;
    }
    switch (state) {
    case 0u: {
        const unsigned first = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + read_word(object + 1024u) + first);
        const unsigned second = static_cast<unsigned>(eRand__10GameObject(self)) % 60u;
        write_word(object + 1032u, read_word(object + 808u) + read_word(object + 1028u) + second);
        transition(object, 1044u, 0x3e999999u, 3);
        break;
    }
    case 1u: {
        const unsigned first = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + read_word(object + 1020u) + first);
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        if ((read_u64(object + 160u) & 0xc00000ull) != 0ull && read_word(object + 1060u) != 0u) {
            transition(object, 1060u, 0x3dccccccu, 2051);
        } else {
            transition(object, 1056u, 0x3dccccccu, 2051);
        }
        break;
    }
    case 7u:
        if (read_word(object + 1064u) == 0u) {
            state = current_bits;
            break;
        }
        write_word(object + 256u, read_word(object + 808u) + 60u + static_cast<unsigned>(eRand__10GameObject(self)) % 120u);
        resetRouting__8Creature(self);
        transition(object, 1064u, 0x3c23d70au, 2051);
        break;
    case 12u:
        transition(object, 1056u, 0x3c23d70au, 2051);
        break;
    case 15u:
        if ((read_word(object + 1076u) & 1u) != 0u) write_word(object + 1080u, 0u);
        // fall through
    case 13u: {
        write_word(object + 256u, read_word(object + 808u) + 300u);
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(self)) % 60u;
        write_word(object + 1032u, read_word(object + 808u) + read_word(object + 1028u) + roll);
        transition(object, 1068u, 0x3e999999u, 3);
        break;
    }
    case 14u:
    case 16u:
        write_word(object + 256u, read_word(object + 808u));
        write_word(object + 12u, read_word(object + 12u) | 1u);
        transition(object, 1072u, 0x3c23d70au, 2051);
        break;
    default:
        state = static_cast<unsigned>(current);
        break;
    }
    write_u16(object + 572u, state);
}
