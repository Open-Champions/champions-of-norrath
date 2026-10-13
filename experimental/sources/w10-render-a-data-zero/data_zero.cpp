// Storage objects that the image keeps zero at load: the first four are 80-, 80-, 64- and
// 128-byte uninitialised-data objects, the last three are single zero words or a byte in the
// small-data area.
//   ParticleEmitterTestParticle  0x006ec740  80 bytes (BSS; next object starts 0x50 later)
//   droplet                      0x006ebaa0  80 bytes (BSS; next object starts 0x50 later)
//   castLightColor               0x007cb730  64 bytes (BSS; next object starts 0x40 later)
//   fontGrid                     0x008d5ab8  128 bytes (BSS; next object starts 0x80 later)
//   displayTextDisable           0x0048434c  4 bytes (.sdata, file word zero)
//   animLoadBuffer               0x0048501e  1 byte  (.sdata, file byte zero)
//   bevel                        0x004848f0  4 bytes (.sdata, file word zero)
// Observed: the symbol table names, addresses and spacing, and the zero file contents. The
// 80-byte objects are the same size as the other particle definitions in the image; the
// 64-byte object is declared as a 4 by 4 array of ints because the authored player recall
// code stores four ints into each of four rows. bevel is declared as a pointer because the
// authored inventory frame code treats it as one; animLoadBuffer as a byte because the
// authored sound code assigns a comparison result to it.
// Unknown: what fontGrid holds, and the types of anything not written by authored code.
extern "C" {
unsigned char ParticleEmitterTestParticle[80];
unsigned char droplet[80];
int castLightColor[4][4];
unsigned char fontGrid[128];
int displayTextDisable;
unsigned char animLoadBuffer;
void* bevel;
}
static_assert(sizeof ParticleEmitterTestParticle == 80u && sizeof droplet == 80u, "particle extents");
static_assert(sizeof castLightColor == 64u && sizeof fontGrid == 128u, "extents");
