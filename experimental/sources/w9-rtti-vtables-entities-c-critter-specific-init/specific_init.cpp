#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed specificInit__7CritterPcT1 at 0x1f1240, 1488 bytes. Arguments: the
// object, a species name, and a resource-file name. The species name is matched
// in this order, with strcasecmp unless marked (t) for strticmp__FPCcT0; the
// first match runs its block and returns. Flag words are the 32-bit word at
// object+12 (W12), the 64-bit word at +160 (D160) and the word at +1076
// (W1076). Observed strings are at 0x4851a0 to 0x485228 and 0x46de18 to
// 0x46deb8.
//   "bat": W12 |= 1, D160 |= 0x100000.
//   "kitten" (t): W1076 &= ~2; +1024 = 180, +1028 = 120, +1020 = 180; when
//     findTagString(object, "name", 0) returns a string, it is compared (t)
//     with "kitten0" .. "kitten3" in turn and on the first match bit i of the
//     word at +244 is set and the loop ends; then +1048 = lumpFindResource
//     (resource, "jump_xy.anm").
//   "pengo": +1024 = 360, +1028 = 120, +1020 = 480; +1048 = "pengo_cluck.anm",
//     +1052 = "pengo_flap.anm" (both lumpFindResource__FPcT0 on the resource).
//   "shark": byte +40 = 40.
//   "crab": W12 |= 1; +1024 = +1020 = 180; the single at +224 = signed eRand
//     * 2^-16 + 1.5.
//   "raven": D160 |= 0x100000; W1076 |= 1; +1084 = 200; +1028 = 240;
//     +1048 = "hop.anm", +1052 = "flutter.anm"; then, when the string at
//     levelName+0x51 equals (t) "kelsel" or "intro", D160 |= 0x2000000.
//   "fish" (t): W12 |= 1; the single at +1080 += signed eRand * 2^-16 * 60.0.
//   "seagull": D160 |= 0x100000; findTagInt(object, "oneshot", 0) nonzero calls
//     the virtual function at byte offset 184 of the table at +72 with 14,
//     zero calls it with 15; then +1084 = 200, W12 |= 1, +1028 = 360,
//     W1076 |= 1, +1048 = "stretch.anm", +1052 = "preen.anm".
//   "rat": W12 |= 1; the single at +224 = 0x3f999999.
//   "bullfrog": W12 |= 1; +1024 = +1020 = 360; the single at +224 = 0.75.
//   "catfish": W1076 |= 4, W12 |= 1; the single at +168 = 0x3e4ccccc.
//   "fishdead": W1076 |= 4; the single at +1080 = 8.0; +168 = 0x3e4ccccc.
//   "drake": nothing when findTagString(object, "path", 0) is nonzero;
//     otherwise D160 |= 0x100000, virtual slot 184 with 15, W12 |= 1,
//     W1076 |= 1, +1084 = 200, +1028 = 360, +1048 = "idlea.anm",
//     +1052 = "idleb.anm".
//   "bflya" or "bflyb": W12 |= 1.
//   "orca": byte +40 = 120.
// Inferred: the names are creature species; levelName is the existing global
// level-name buffer. Unknown: the meaning of the fields and flag bits.
extern "C" {
extern char levelName[];
int strcasecmp(const char* left, const char* right);
int strticmp__FPCcT0(const char* left, const char* right);
int sprintf(char* destination, const char* format, ...);
int eRand__10GameObject(void* object);
char* findTagString__10GameObjectPci(void* object, char* tag, int occurrence);
int findTagInt__10GameObjectPci(void* object, char* tag, int occurrence);
void* lumpFindResource__FPcT0(char* lump, char* name);
}

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline bool same(const char* left, const char* right) { return strcasecmp(left, right) == 0; }
__attribute__((always_inline)) inline bool same_t(const char* left, const char* right) { return strticmp__FPCcT0(left, right) == 0; }
__attribute__((always_inline)) inline void set_bits(Address address_of_word, unsigned bits) { write_word(address_of_word, read_word(address_of_word) | bits); }
__attribute__((always_inline)) inline void set_bits64(Address address_of_word, unsigned long long bits) { write_u64(address_of_word, read_u64(address_of_word) | bits); }
__attribute__((always_inline)) inline void call_slot_184(void* self, int argument) {
    typedef void (*SlotCall)(void*, int);
    void* adjusted;
    SlotCall call = slot_function<SlotCall>(self, 184u, &adjusted);
    call(adjusted, argument);
}
__attribute__((always_inline)) inline void lump(void* self, unsigned offset, char* resource, const char* name) {
    write_word(address(self) + offset, address_bits(lumpFindResource__FPcT0(resource, const_cast<char*>(name))));
}
}

extern "C" void specificInit__7CritterPcT1(void* self, char* species, char* resource) {
    const Address object = address(self);
    if (same(species, "bat")) {
        set_bits(object + 12u, 1u);
        set_bits64(object + 160u, 0x100000ull);
    } else if (same_t(species, "kitten")) {
        write_word(object + 1076u, read_word(object + 1076u) & 0xfffffffdu);
        write_word(object + 1024u, 180u);
        write_word(object + 1028u, 120u);
        write_word(object + 1020u, 180u);
        char* name = findTagString__10GameObjectPci(self, const_cast<char*>("name"), 0);
        if (name != nullptr) {
            for (int index = 0; index < 4; ++index) {
                char buffer[16];
                sprintf(buffer, "kitten%d", index);
                if (strticmp__FPCcT0(name, buffer) == 0) {
                    write_word(object + 244u, read_word(object + 244u) | (1u << index));
                    break;
                }
            }
        }
        lump(self, 1048u, resource, "jump_xy.anm");
    } else if (same(species, "pengo")) {
        write_word(object + 1024u, 360u);
        write_word(object + 1028u, 120u);
        write_word(object + 1020u, 480u);
        lump(self, 1048u, resource, "pengo_cluck.anm");
        lump(self, 1052u, resource, "pengo_flap.anm");
    } else if (same(species, "shark")) {
        write_u8(object + 40u, 40u);
    } else if (same(species, "crab")) {
        set_bits(object + 12u, 1u);
        write_word(object + 1024u, 180u);
        write_word(object + 1020u, 180u);
        const int roll = eRand__10GameObject(self);
        write_single(object + 224u, static_cast<float>(roll) * single_from_bits(0x37800000u) + single_from_bits(0x3fc00000u));
    } else if (same(species, "raven")) {
        set_bits64(object + 160u, 0x100000ull);
        write_word(object + 1076u, read_word(object + 1076u) | 1u);
        write_word(object + 1084u, 200u);
        write_word(object + 1028u, 240u);
        lump(self, 1048u, resource, "hop.anm");
        lump(self, 1052u, resource, "flutter.anm");
        if (same_t(levelName + 0x51, "kelsel") || same_t(levelName + 0x51, "intro")) set_bits64(object + 160u, 0x2000000ull);
    } else if (same_t(species, "fish")) {
        set_bits(object + 12u, 1u);
        const int roll = eRand__10GameObject(self);
        write_single(object + 1080u, read_single(object + 1080u) + static_cast<float>(roll) * single_from_bits(0x37800000u) * single_from_bits(0x42700000u));
    } else if (same(species, "seagull")) {
        set_bits64(object + 160u, 0x100000ull);
        if (findTagInt__10GameObjectPci(self, const_cast<char*>("oneshot"), 0) != 0) call_slot_184(self, 14);
        else call_slot_184(self, 15);
        write_word(object + 1084u, 200u);
        set_bits(object + 12u, 1u);
        write_word(object + 1028u, 360u);
        set_bits(object + 1076u, 1u);
        lump(self, 1048u, resource, "stretch.anm");
        lump(self, 1052u, resource, "preen.anm");
    } else if (same(species, "rat")) {
        set_bits(object + 12u, 1u);
        write_single(object + 224u, single_from_bits(0x3f999999u));
    } else if (same(species, "bullfrog")) {
        set_bits(object + 12u, 1u);
        write_word(object + 1024u, 360u);
        write_single(object + 224u, single_from_bits(0x3f400000u));
        write_word(object + 1020u, 360u);
    } else if (same(species, "catfish")) {
        set_bits(object + 1076u, 4u);
        set_bits(object + 12u, 1u);
        write_single(object + 168u, single_from_bits(0x3e4ccccc));
    } else if (same(species, "fishdead")) {
        set_bits(object + 1076u, 4u);
        write_single(object + 1080u, single_from_bits(0x41000000u));
        write_single(object + 168u, single_from_bits(0x3e4ccccc));
    } else if (same(species, "drake")) {
        if (findTagString__10GameObjectPci(self, const_cast<char*>("path"), 0) != nullptr) return;
        set_bits64(object + 160u, 0x100000ull);
        call_slot_184(self, 15);
        set_bits(object + 12u, 1u);
        set_bits(object + 1076u, 1u);
        write_word(object + 1084u, 200u);
        write_word(object + 1028u, 360u);
        lump(self, 1048u, resource, "idlea.anm");
        lump(self, 1052u, resource, "idleb.anm");
    } else if (same(species, "bflya") || same(species, "bflyb")) {
        set_bits(object + 12u, 1u);
    } else if (same(species, "orca")) {
        write_u8(object + 40u, 120u);
    }
}
