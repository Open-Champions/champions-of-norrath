// Observed canQueue__5Spell at 0x00314630, 8 bytes (weak symbol in the image): returns zero and
// reads nothing, so the single `this` argument is not used. Which Spell subclasses override it,
// and what the caller does with the answer, is unknown. Nothing here ran on the EE.
extern "C" int canQueue__5Spell(void* self) {
    (void)self;
    return 0;
}
