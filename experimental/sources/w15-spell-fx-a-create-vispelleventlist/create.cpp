// Observed: Create__16VISpellEventListiP7VIScene at 0x01000710 is 100 bytes in the pinned SLUS-20565 image. With (this, count,
// scene) it stores the scene in the word at +0, then, if the word at +8 is nonzero, calls Destroy__16VISpellEventList(this). It
// then stores the count at +4, stores __builtin_vec_new(count * 44) in the word at +8 (even when that is null) and returns zero.
// The 44 is the product the original forms from the count with shifts and a subtraction.
// Inferred: +8 holds an array of count records of 44 bytes each and +4 is its length, so a list is rebuilt rather than grown.
// Unknown: the record layout, the allocator's behaviour (an import) and how a failed allocation is handled. Nothing here ran on
// the EE.
extern "C" {
void Destroy__16VISpellEventList(void* self);
unsigned __builtin_vec_new(unsigned size);

int Create__16VISpellEventListiP7VIScene(void* self, int count, void* scene) {
    unsigned char* const bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned*>(bytes) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(scene));
    if (*reinterpret_cast<unsigned*>(bytes + 8) != 0u) Destroy__16VISpellEventList(self);
    *reinterpret_cast<int*>(bytes + 4) = count;
    *reinterpret_cast<unsigned*>(bytes + 8) = __builtin_vec_new(static_cast<unsigned>(count) * 44u);
    return 0;
}
}
