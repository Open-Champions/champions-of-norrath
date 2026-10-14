// harm2: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm on the reference image: harm2 at 0x008d6d20, type b (zero-initialised), 0x50 bytes from its neighbours.
//   - Callers: w11-game-a-spell-harm-touch-attack/spell_harm_touch_attack.cpp declares extern unsigned char harm2[].
//   - The record is constructed by a static initializer in a published unit (w8-spell-static-init-a or w15-gameloop-a). The w8 copy is in an anonymous namespace; the w15-gameloop-a copy is global but ships a static-init function with the same name as w8, so it cannot be linked beside w8.
// Inferred:
//   - 80 bytes, the particle-definition record size from the neighbouring addresses and the other storages.
// Unknown:
//   - The record fields the game constructs. This global is zero, not the constructed record.
//   - Which owner keeps the global storage once w15-gameloop-a and w8 are reconciled.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
unsigned char harm2[80];
}
static_assert(sizeof harm2 == 80u, "harm2 extent");
