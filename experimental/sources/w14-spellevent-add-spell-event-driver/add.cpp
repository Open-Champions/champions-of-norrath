// Observed: AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi at 0x01182ef0 is 216 bytes
// in the pinned SLUS-20565 image. Arguments: (this, list, id); returns the new object or zero.
//   1. p = __builtin_new(100). If p is non-null it initialises it inline (the image carries its own
//      copy of the event-driver constructor body): __7VILNode(p); reads the word at this+4 into a
//      local before the next calls; __7VILList(p+76); __7VILList(p+88); then stores this at p+16, the
//      word read from this+4 at p+20, the id at p+24, the list at p+72, zero at p+28, +32, +36, +40,
//      +44 and +48, and -1 at p+52, +56, +60, +64 and +68.
//   2. Whether or not the allocation worked it calls AddTail__7VILListP7VILNode(this+72, p) and
//      returns p. A null p is passed on.
// Inferred: this is a spell effect driver whose +72 list owns event drivers (the destroy routine
// removes them from that list), +4 holds the scene (the event driver's +20 is a scene in its
// Process routine) and the object built here is a VISpellEventDriver for a script list and id.
// Unknown: the meaning of the id, and whether the original source called the real constructor (the
// image has an out-of-line one at 0x01182590 that this body does not call). Nothing here ran on the EE.
extern "C" {
void* __builtin_new(unsigned size);
void __7VILNode(void* node);
void __7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

void* AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi(unsigned char* self, void* list, int id) {
    unsigned char* p = static_cast<unsigned char*>(__builtin_new(100));
    if (p != 0) {
        __7VILNode(p);
        unsigned scene = *reinterpret_cast<unsigned*>(self + 4);
        __7VILList(p + 76);
        __7VILList(p + 88);
        unsigned* w = reinterpret_cast<unsigned*>(p);
        w[4] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self));
        w[18] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(list));
        w[5] = scene;
        w[6] = static_cast<unsigned>(id);
        w[17] = 0xffffffffu;
        w[7] = 0;
        w[8] = 0;
        w[9] = 0;
        w[10] = 0;
        w[11] = 0;
        w[12] = 0;
        w[13] = 0xffffffffu;
        w[14] = 0xffffffffu;
        w[15] = 0xffffffffu;
        w[16] = 0xffffffffu;
    }
    AddTail__7VILListP7VILNode(self + 72, p);
    return p;
}
}
