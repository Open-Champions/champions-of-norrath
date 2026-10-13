// Observed msg_run__10SpawnMummy at 0x0023b588, 312 bytes. O is the object, frameNm the global frame counter.
//  1. When the signed word at O+80 is below frameNm and the word at O+92 is zero: 1128 bytes are allocated with
//     blockAlloc__Fi. A non-null block gets the Mummy constructor __5MummyG6Point3sPPcPc(block, copy of the point at
//     O+16, (short)eRand__10GameObject(O), null, the word at O+88), then listOnewayLink__FP8ListHeadT0Pv(O+96, block,
//     block) and the 64-bit value at block+160 is or-ed with 0x01000000. The word at O+92 becomes 1 in both cases
//     (also when the allocation fails).
//  2. X is the word at O+100. When X is zero, or when bit 8 of the word at +12 of the object whose address is the word at
//     X+12 is set: if the word at O+96 is non-zero, the entry at offset 184 of the table of the object that is the word
//     at (O+96)+12 is called on that object (adjusted by the entry's signed halfword) with the argument 10.
//  3. When the word at O+92 is non-zero and the word at O+96 is zero: objectAddToDeleteList__FP10GameObject(O).
//  4. msg_run__9DustDevil(O) runs last.
// Inferred: a spawner that makes one Mummy at a delay and watches it (the link at O+96 and the delete once it is gone).
// Table entry 184 is called with the argument 10; the vslots-b record names that slot setState__8Creaturei. Unknown:
// what the words at O+80, +88, +100 and the bit 8 mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
void* blockAlloc__Fi(int size);
int eRand__10GameObject(void* object);
void __5MummyG6Point3sPPcPc(void* self, Point3 position, short a, char** b, char* c);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void objectAddToDeleteList__FP10GameObject(void* object);
void msg_run__9DustDevil(void* self);

void msg_run__10SpawnMummy(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (sword(o, 80) < frameNm && word(o, 92) == 0) {
        unsigned char* const block = static_cast<unsigned char*>(blockAlloc__Fi(1128));
        if (block != nullptr) {
            const short heading = static_cast<short>(eRand__10GameObject(o));
            Point3 position = *reinterpret_cast<Point3*>(o + 16);
            __5MummyG6Point3sPPcPc(block, position, heading, nullptr, reinterpret_cast<char*>(from_word(word(o, 88))));
            listOnewayLink__FP8ListHeadT0Pv(o + 96, block, block);
            dword(block, 160) = dword(block, 160) | 0x01000000ull;
        }
        word(o, 92) = 1;
    }
    bool callEntry = true;
    if (word(o, 100) != 0) callEntry = (word(from_word(word(from_word(word(o, 100)), 12)), 12) & 8u) != 0;
    if (callEntry && word(o, 96) != 0) call_entry(from_word(word(from_word(word(o, 96)), 12)), 184, 10);
    if (word(o, 92) != 0 && word(o, 96) == 0) objectAddToDeleteList__FP10GameObject(o);
    msg_run__9DustDevil(o);
}
}
