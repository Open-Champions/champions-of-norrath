// Observed c.1089 at 0x007d0988 in zero-filled storage. gameBoot passes it to __6Camera behind a guard word and
// stores it in gameCam. The ELF gives it no size. The next symbol, worldBoundingBox (8-aligned), is 0x180 (384)
// bytes later, so the Camera object lies within [377, 384] bytes; the constructor writes through offset 136.
// 384 is used as the gap, not as a measured sizeof(Camera).
// Inferred: this is a function-local static Camera, from its numbered name and the guard.
// Unknown: sizeof(Camera) and the field layout beyond offset 136.
extern "C" {
alignas(8) unsigned char norrath_core_loop_a_camera_static[384] asm("c.1089");
}
