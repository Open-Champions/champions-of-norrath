// Observed resetPlayerAnimations__Fi at 0x001a1c98, 448 bytes.
// Argument: a player number. object is the pointer word at record+21872 of the
// 23984-byte player record and b is the signed byte at record+23582. Nothing
// happens when object is zero or the float at record+172 is not above 0.0
// (a NaN passes the test). Otherwise, in order:
//  - floats at object+304 and +312 become 1.0 and the halfword at object+324
//    becomes 4613;
//  - the word at object+296 is set. When the signed byte at object+1106 is zero
//    it is the word at (holder + 104*b + 68), where holder is the structure the
//    word at object+36 points to. When it is non-zero it is the result of
//    lumpFindResource(first word of holder, name), where name is one of six
//    16-byte slots of the reset-name view chosen by b: 1 -> slot 0, 2 -> slot 1,
//    3 -> slot 2, 4 -> slot 3, 7 -> slot 4, anything else -> slot 5;
//  - the halfword at +420 becomes 4613, the word at +336 zero, and then from the
//    holder row (holder + 104*b) the words at +76, +84, +92 go to object +328,
//    +360, +392; words +344, +368, +376, +400, +408, +432 and +440 become zero and
//    halfwords +356 and +388 become 4613;
//  - animResetFramestates runs on the embedded state at object+288.
// Unknown: what the 104-byte rows and the object fields are called.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern const unsigned char norrath_inventory_reset_resource_names_view[];
char* lumpFindResource__FPcT0(char* lump, char* name);
void animResetFramestates__FP14AnimationState(void* state);
}

namespace {
unsigned& word(unsigned char* base, unsigned offset) { return *reinterpret_cast<unsigned*>(base + offset); }
float& real(unsigned char* base, unsigned offset) { return *reinterpret_cast<float*>(base + offset); }
short& half(unsigned char* base, unsigned offset) { return *reinterpret_cast<short*>(base + offset); }
}

extern "C" void resetPlayerAnimations__Fi(int owner) {
    using namespace inventory_items;
    unsigned char* record = player_record(owner);
    if (word(record, 21872) == 0) return;
    unsigned char* object = const_cast<unsigned char*>(resolve<unsigned char>(word(record, 21872)));
    if (*reinterpret_cast<float*>(record + 172) <= 0.0f) return;
    const int row = static_cast<signed char>(record[23582]);
    real(object, 312) = 1.0f;
    half(object, 324) = 4613;
    const signed char use_lump = static_cast<signed char>(object[1106]);
    real(object, 304) = 1.0f;
    if (use_lump != 0) {
        int slot;
        switch (static_cast<signed char>(record[23582])) {
            case 1: slot = 0; break;
            case 2: slot = 1; break;
            case 3: slot = 2; break;
            case 4: slot = 3; break;
            case 7: slot = 4; break;
            default: slot = 5; break;
        }
        const unsigned char* holder = resolve<unsigned char>(word(object, 36));
        char* resource = lumpFindResource__FPcT0(
            const_cast<char*>(resolve<char>(*reinterpret_cast<const unsigned*>(holder))),
            const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_reset_resource_names_view) + 16 * slot));
        object = const_cast<unsigned char*>(resolve<unsigned char>(word(record, 21872)));
        word(object, 296) = word_of(resource);
    } else {
        const unsigned char* holder = resolve<unsigned char>(word(object, 36));
        word(object, 296) = *reinterpret_cast<const unsigned*>(holder + 104 * row + 68);
    }
    const unsigned char* holder = resolve<unsigned char>(word(object, 36));
    const unsigned char* source = holder + 104 * row;
    half(object, 420) = 4613;
    word(object, 336) = 0;
    const unsigned third = *reinterpret_cast<const unsigned*>(source + 92);
    const unsigned first = *reinterpret_cast<const unsigned*>(source + 76);
    const unsigned second = *reinterpret_cast<const unsigned*>(source + 84);
    word(object, 328) = first;
    word(object, 360) = second;
    word(object, 392) = third;
    word(object, 344) = 0;
    half(object, 356) = 4613;
    word(object, 368) = 0;
    word(object, 376) = 0;
    half(object, 388) = 4613;
    word(object, 400) = 0;
    word(object, 408) = 0;
    word(object, 432) = 0;
    word(object, 440) = 0;
    animResetFramestates__FP14AnimationState(object + 288);
}
