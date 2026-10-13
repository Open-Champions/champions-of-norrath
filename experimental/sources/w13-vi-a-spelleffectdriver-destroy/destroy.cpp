// Observed: Destroy__19VISpellEffectDriver at 0x01182c38 is 200 bytes. If the word at +4 of the
// driver is zero it does nothing. Otherwise: if Length__C7VILList(driver + 72) is non-zero and
// the word at +100 is non-zero it calls Reset__14VICameraEffect(*(word at +92)); in every
// case it then stores zero at +100. It then repeatedly calls RemHead__7VILList(driver + 72)
// and, for each non-null node, calls _$_18VISpellEventDriver(node, 3). After the list is
// empty it stores zero at +0, -1 at +8 and +12, zero at +16, -1 into the eight words
// +20, +24, ..., +48, zero at +4 and zero at +92. Nothing is returned.
// Inferred: +4 is an "active" flag, +72 a list of owned event drivers that are deleted (flag
// 3 = free the memory as well), +92 a camera effect that is reset when the event list was
// non-empty and a flag at +100 was set; the eight -1 words are a slot table (probably handles).
// Unknown: the meaning of +0, +8, +12, +16, +100 and of the eight slots, and what
// Reset__14VICameraEffect does.
// The word at +92 is a 32-bit target address; a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Length__C7VILList(const void* list);
void Reset__14VICameraEffect(void* effect);
void* RemHead__7VILList(void* list);
void _$_18VISpellEventDriver(void* driver, int flags);

void Destroy__19VISpellEffectDriver(unsigned char* driver) {
    if (*reinterpret_cast<int*>(driver + 4) == 0) {
        return;
    }
    unsigned char* list = driver + 72;
    if (Length__C7VILList(list) != 0 && *reinterpret_cast<int*>(driver + 100) != 0) {
        Reset__14VICameraEffect(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(driver + 92)));
    }
    *reinterpret_cast<int*>(driver + 100) = 0;
    void* node;
    while ((node = RemHead__7VILList(list)) != 0) {
        _$_18VISpellEventDriver(node, 3);
    }
    *reinterpret_cast<int*>(driver + 0) = 0;
    *reinterpret_cast<int*>(driver + 12) = -1;
    *reinterpret_cast<int*>(driver + 4) = 0;
    *reinterpret_cast<int*>(driver + 92) = 0;
    *reinterpret_cast<int*>(driver + 8) = -1;
    *reinterpret_cast<int*>(driver + 16) = 0;
    for (int i = 7; i >= 0; --i) {
        *reinterpret_cast<int*>(driver + 20 + 4 * i) = -1;
    }
}
}
