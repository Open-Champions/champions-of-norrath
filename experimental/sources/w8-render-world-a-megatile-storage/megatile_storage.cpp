// Storage that the megatile routines of this package reach by name. Addresses are
// from the pinned SLUS_205.65 symbol table; the file bytes of both objects are
// zero. The reviewed -G0 profile does not recreate .sdata or .sbss, so they are
// ordinary zero storage here.
//   mgrg        0x00485c54  4 bytes, the pointer megatileInit__FP12MegatileRootPv
//                           stores its root argument in. The name is the image's.
//   mt_tiledata 0x00434068  320 bytes, 32 records of 10 signed bytes, to mt_ebx.
//                           getMegatileCenter__FG6Point2 and
//                           getMegatileOriginalCenter__FG6Point2 read it. Its
//                           writers are not part of this package.
//   zeroTile    0x008d5570  72 bytes, .bss, to slideDustParticle at 0x008d55b8. Its
//                           first word is cleared by the constructor wrapper
//                           _GLOBAL_$I$edgearray; nothing else here writes it.
extern "C" {
void* mgrg;
alignas(4) signed char mt_tiledata[320];
int zeroTile[18];
}
