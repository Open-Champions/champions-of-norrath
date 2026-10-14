// firestormParticle3: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm on the reference image: firestormParticle3 at 0x008d6e10, type b (zero-initialised), 80 bytes apart from its neighbours.
//   - Callers: w15-spell-fx-a-msg-run-firestormflare/msg_run.cpp declares extern unsigned char firestormParticle1[] and passes it to P_AddParticle.
//   - w10-actor-f-static-init-firestorm-flare/static_init.cpp constructs a record of the same name, but in an anonymous namespace, so it is local and not this symbol.
// Inferred:
//   - 80 bytes, the particle-definition record size from the neighbouring addresses and the sibling storages.
// Unknown:
//   - The record fields the game constructs. This global is zero; the static-init record is a separate, local object.
//   - Whether the image intends one object for both bindings.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
unsigned char firestormParticle3[80];
}
static_assert(sizeof firestormParticle3 == 80u, "firestormParticle3 extent");
