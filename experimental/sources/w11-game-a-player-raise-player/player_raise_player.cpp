// Observed playerRaisePlayer__FiG6Point3 at 0x001ab388, 312 bytes. Arguments: a player number and a
// position (a 12-byte Point3 passed by reference in this ABI; the image first copies it to a local).
// Records are 23984 bytes apart at `player`; the word at record +21872 is the player's object.
//   1. When netPlayerIsLocal__Fi(player) is nonzero, gameNetKick__Fi(1) runs.
//   2. When the record's object pointer is nonzero, the object's virtual function in the second
//      slot of its table (table pointer at object +72, slot at table +8: a signed halfword that is
//      added to the object pointer to form `this`, then a function pointer at slot +4) is called
//      with 3.
//   3. blockAlloc__Fi(1408). Only when it returns non-null: objectFindEmptySpot__FG6Point3iiPc
//      writes a spot into a local Point3 from the copied position with 96, 24 and a null char
//      pointer; syncRandState advances (state * 1664525 + 1013904223, stored before the
//      constructor call); the Player constructor __6PlayerG6Point3sii runs on the new block with
//      the spot, the high halfword of the new state as a short, the player number and 0.
//   4. Always: the float at record +172 is set to the float at record +176 times 0.5, and
//      resurectEffect__6Player runs on the object pointer now in the record (the constructor is
//      what puts the new object there; this routine never stores it).
// Inferred: the slot-2 call at step 2 releases the dead player's old object and the routine
// replaces it with a fresh player (a raise from the dead) at an open spot near `position` with
// half the value kept at +176. Unknown: the meaning of the floats at +172/+176 and of slot 2.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char player[];
extern int syncRandState;
int netPlayerIsLocal__Fi(int player_number);
void gameNetKick__Fi(int value);
void* blockAlloc__Fi(int size);
void objectFindEmptySpot__FG6Point3iiPc(float* result, const float* near, int width, int height, char* flag);
void __6PlayerG6Point3sii(void* self, float* position, short yaw, int player_number, int extra);
void resurectEffect__6Player(void* object);
}

namespace {
inline unsigned char* load_pointer(const unsigned char* at) {
    unsigned char* value;
    __builtin_memcpy(&value, at, sizeof value);
    return value;
}
}  // namespace

extern "C" void playerRaisePlayer__FiG6Point3(int player_number, const float* position) {
    float where[3] = {position[0], position[1], position[2]};
    if (netPlayerIsLocal__Fi(player_number) != 0) gameNetKick__Fi(1);
    unsigned char* record = player + player_number * 23984;
    unsigned char* object = load_pointer(record + 21872);
    if (object != nullptr) {
        const unsigned char* slot = load_pointer(object + 72) + 8;
        short adjust;
        __builtin_memcpy(&adjust, slot, sizeof adjust);
        void (*function)(void*, int) = reinterpret_cast<void (*)(void*, int)>(load_pointer(slot + 4));
        function(object + adjust, 3);
    }
    void* block = blockAlloc__Fi(1408);
    if (block != nullptr) {
        float spot[3];
        objectFindEmptySpot__FG6Point3iiPc(spot, where, 96, 24, nullptr);
        syncRandState = static_cast<int>(static_cast<unsigned>(syncRandState) * 1664525u + 1013904223u);
        __6PlayerG6Point3sii(block, spot, static_cast<short>(syncRandState >> 16), player_number, 0);
    }
    *reinterpret_cast<float*>(record + 172) = *reinterpret_cast<float*>(record + 176) * 0.5f;
    resurectEffect__6Player(load_pointer(record + 21872));
}
