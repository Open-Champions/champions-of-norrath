// Observed lightningSwordParticle at 0x007d63d0, a local BSS symbol; the next symbol,
// flameSwordParticle, is 0x007d6420, so the extent is 80 bytes (the size of the neighbouring
// ParticleDef objects). The file image holds zero; the particle-definition constructor fills it in
// during static initialization (the authored w8-fx-particles-init-fx initializer calls the
// constructor on its own file-local array of the same name, with the values 111, 112, 154, 60, 33,
// 30, 28, -30, 15.0f, 30.0f, 0.1f, 0, 0, 6, 0). The authored run-weapon routine passes this
// address to P_AddParticle. Caution: a program that links both this definition and that
// initializer fills a different object from the one the run-weapon routine reads; the initializer's
// array needs to become this global before the pair is meaningful. Same situation as
// ballSparkParticle in w11-data-a. Nothing here was executed on the EE.
extern "C" {
unsigned char lightningSwordParticle[80];
}
static_assert(sizeof(lightningSwordParticle) == 80u, "lightningSwordParticle extent");
