// Observed _$_6Player at 0x001947d0, 348 bytes (weak). Steps, in order:
//   1. the 6Player table pointer (_vt$6Player) is stored at object+72;
//   2. when netPrediction is zero: if the byte at +1106 is non-zero, thereIsACutPlayer is cleared, else the
//      word at player+21872+index*23984 is cleared (index is the word at +960; a player record is 23984
//      bytes); then, in both cases, the five words at player+21876+index*23984 (+0..+16) are cleared;
//   3. enumerateAlivePlayers__Fv(); 4. listDestroy__FP8ListHead on +1396; 5. _$_14AnimationState on +1356
//   with flag 2; 6. _$_5Light on +1268 and on +1208 with flag 2; 7. listDestroy on +1192;
//   8. the five 24-byte elements at +968 are walked from the last to the first, calling listDestroy on
//   element+8 (the walk is guarded by a test of the element-array address against null, as in the image);
//   9. _$_8Creature(object, 0); 10. bit 0 of the flags decides the final blockFree__FPv (a tail call).
// Inferred: this is the Player destructor, from its symbol name only; the cleared words belong to the
// global player table entry for this object's index. Unknown: what the cleared words mean.
// Nothing here ran on the EE.
extern "C" {
extern unsigned char vt_6Player[] __asm__("_vt$6Player");
extern signed char netPrediction;
extern char thereIsACutPlayer;
extern unsigned char player[];
void enumerateAlivePlayers__Fv();
void listDestroy__FP8ListHead(void* head);
void animation_state_destructor(void* state, int flags) __asm__("_$_14AnimationState");
void light_destructor(void* light, int flags) __asm__("_$_5Light");
void creature_destructor(void* self, int flags) __asm__("_$_8Creature");
void blockFree__FPv(void* block);
}

extern "C" void destructor_Player(void* self, int flags) __asm__("_$_6Player");
extern "C" void destructor_Player(void* self, int flags) {
    unsigned char* object = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(object + 72) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_6Player));
    if (netPrediction == 0) {
        if (*reinterpret_cast<signed char*>(object + 1106) != 0)
            thereIsACutPlayer = 0;
        else
            *reinterpret_cast<int*>(player + 21872 + *reinterpret_cast<int*>(object + 960) * 23984) = 0;
        for (int i = 0; i < 5; ++i)
            *reinterpret_cast<int*>(player + 21876 + (*reinterpret_cast<int*>(object + 960) * 5996 + i) * 4) = 0;
    }
    enumerateAlivePlayers__Fv();
    unsigned char* elements = object + 968;
    listDestroy__FP8ListHead(object + 1396);
    animation_state_destructor(object + 1356, 2);
    light_destructor(object + 1268, 2);
    light_destructor(object + 1208, 2);
    listDestroy__FP8ListHead(object + 1192);
    __asm__("" : "+r"(elements));
    if (elements != nullptr) {
        unsigned char* element = object + 1088;
        if (elements != element) {
            do {
                element -= 24;
                listDestroy__FP8ListHead(element + 8);
            } while (elements != element);
        }
    }
    creature_destructor(object, 0);
    if ((flags & 1) != 0) blockFree__FPv(object);
}
