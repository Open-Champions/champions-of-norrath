// Observed fsListBack at 0x00486910, a 4-byte BSS word in SLUS_205.65, next to fsList
// (0x0048690c). getFreeFrameState__Fv reads and writes both as pointers: fsListBack is the
// front node of the frame-state list that gets recycled, fsList the last node. Zero at
// start. fsList itself is defined by the animation-initialisation source.
extern "C" {
unsigned char* fsListBack;
}
