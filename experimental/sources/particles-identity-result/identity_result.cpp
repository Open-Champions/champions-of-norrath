// identityResult at 0x006ca1c0, 1600 bytes, to emergencyResult.
// BSS is zero. animInit writes the identity words later.
extern "C" {
unsigned char identityResult[1600];
}
static_assert(sizeof identityResult == 1600u, "identityResult extent");
