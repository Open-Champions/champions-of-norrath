// Observed draw at 0x00790ff0. Local BSS. dispEnv begins 64 bytes later, so
// this object is 16 words. The file image is zero. engineFrameEnd uses
// offsets 0 through 60.
extern "C" {
unsigned draw[16];
}
static_assert(sizeof(draw) == 64, "sixteen words");
