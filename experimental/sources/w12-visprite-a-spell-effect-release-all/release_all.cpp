// Observed: ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary at
// 0x010025b8, 204 bytes. With the object as this, pool = this + 1112 and dict
// the second argument (a reference, passed as an address):
//   Loop: i = Begin__Ct6VIPool1ZP13VISpellEffect(pool); if i equals
//   End__Ct6VIPool1ZP13VISpellEffect(pool) the loop ends. Otherwise e is the
//   word at (word at pool + 8) + 12 * i; the pool's entries are 12 bytes with
//   the effect pointer first.
//     The word at e + 4 is decremented and stored; if the new value is above
//     zero, the loop restarts (Begin is called again, which finds the same
//     entry).
//     If the word at e + 0 is non-zero, Remove__12VIDictionaryR10VIResource(dict, e).
//     r = Destroy__13VISpellEffect(e); if r is negative the loop restarts.
//     Otherwise _$_13VISpellEffect(e, 3) is called and then
//     Erase__t6VIPool1ZP13VISpellEffecti(pool, i).
//   After the loop Clear__t6VIPool1ZP13VISpellEffect(pool) is called and the
//   result is zero.
// Inferred: e + 4 is a reference count and e + 0 is a dictionary link that is
// non-zero while the effect is registered; the pool iterator is an index.
// Unknown: the member names and the meaning of a negative Destroy result (the
// image simply retries).
extern "C" {
int Begin__Ct6VIPool1ZP13VISpellEffect(void* pool);
int End__Ct6VIPool1ZP13VISpellEffect(void* pool);
void Erase__t6VIPool1ZP13VISpellEffecti(void* pool, int index);
void Clear__t6VIPool1ZP13VISpellEffect(void* pool);
void Remove__12VIDictionaryR10VIResource(void* dictionary, void* resource);
int Destroy__13VISpellEffect(void* effect);
void _$_13VISpellEffect(void* effect, int flags);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary(void* self, void* dictionary) {
    char* pool = static_cast<char*>(self) + 1112;
    for (;;) {
        int i = Begin__Ct6VIPool1ZP13VISpellEffect(pool);
        if (i == End__Ct6VIPool1ZP13VISpellEffect(pool)) break;
        char* entries = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(pool + 8));
        char* e = NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(entries + 12 * i));
        int count = *reinterpret_cast<int*>(e + 4) - 1;
        *reinterpret_cast<int*>(e + 4) = count;
        if (count > 0) continue;
        if (*reinterpret_cast<int*>(e)) Remove__12VIDictionaryR10VIResource(dictionary, e);
        if (Destroy__13VISpellEffect(e) < 0) continue;
        _$_13VISpellEffect(e, 3);
        Erase__t6VIPool1ZP13VISpellEffecti(pool, i);
    }
    Clear__t6VIPool1ZP13VISpellEffect(pool);
    return 0;
}
}
