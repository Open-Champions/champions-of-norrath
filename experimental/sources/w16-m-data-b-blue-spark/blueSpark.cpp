// blueSpark: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm on the reference image: blueSpark at 0x007cb5a0, type b (zero-initialised). The image binding is local.
//   - Callers: w12-hud-loop-b-player-block-sparks/player_block_sparks.cpp declares extern unsigned char blueSpark[] and passes it to P_AddParticle.
//   - w8-net-play-a-global-init-nm-players/global_init.cpp defines a static unsigned char blueSpark[80] and passes it to the particle constructor.
// Inferred:
//   - 80 bytes, the particle-definition record size used by the other particle storages in the published units.
//   - The global binding here is needed for the partial link; the image binding is local, so the owners must reconcile the two.
// Unknown:
//   - The record fields the game constructs at start-up.
//   - Whether the static w8 definition is the same object as the extern one in the image.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
unsigned char blueSpark[80];
}
static_assert(sizeof blueSpark == 80u, "blueSpark extent");
