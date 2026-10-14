// Observed: CreateSpellEffect__19VISpellEffectSystemi at 0x01002c68 is 168 bytes in the pinned SLUS-20565 image. With (this,
// flag) it allocates 48 bytes with __builtin_new and, when that succeeds, runs the constructor __13VISpellEffect on the block.
// A null allocation returns -1. Otherwise Create__13VISpellEffect(effect, scene) is called with the scene held in the word at +0
// of the system. A negative result destroys the effect with _$_13VISpellEffect(effect, 3) and returns -1. A non-negative result
// increments the word at +4 of the effect when flag is nonzero, then returns the result of the pool insertion
// Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect(system + 1112, &effect), where the effect pointer is passed by reference.
// Inferred: this is the spell-effect factory of the system and the word at +4 is a reference count taken for the caller (flag).
// Unknown: the pool's return value meaning (an import), the constructor's and destructor's bodies, and the type of the flag.
// Nothing here ran on the EE.
extern "C" {
void* __builtin_new(unsigned size);
void __13VISpellEffect(void* self);
int Create__13VISpellEffectP7VIScene(void* self, void* scene);
void _$_13VISpellEffect(void* self, int flags);
int Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect(void* pool, void* const* item);

int CreateSpellEffect__19VISpellEffectSystemi(void* self, int flag) {
    unsigned char* const system = static_cast<unsigned char*>(self);
    void* effect = __builtin_new(48u);
    if (effect != nullptr) __13VISpellEffect(effect);
    if (effect == nullptr) return -1;
    if (Create__13VISpellEffectP7VIScene(effect, *reinterpret_cast<void**>(system)) < 0) {
        _$_13VISpellEffect(effect, 3);
        return -1;
    }
    if (flag != 0) ++*reinterpret_cast<int*>(static_cast<unsigned char*>(effect) + 4);
    return Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect(system + 1112, &effect);
}
}
