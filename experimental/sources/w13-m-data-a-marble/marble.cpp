// Observed marble at 0x004848ec, a four-byte pointer word in .data. The file bytes are
// zero, and the neighbouring invHeader word is at 0x004848e0. The inventory frame draw
// passes marble as the first argument of a texture draw. The texture is looked up at load
// time by inventory setup, which is outside this unit.
// The neighbouring invHeader is defined by another unit, so this unit does not define it.
// Unknown: the exact visibility the integrator will choose, and the texture type.
extern "C" {
void* marble;
}
static_assert(sizeof marble == sizeof(void*), "marble width");
