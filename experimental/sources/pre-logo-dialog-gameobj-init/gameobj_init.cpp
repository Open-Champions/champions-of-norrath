// Observed gameobjInit__Fv at 0x00155ba0, 44 bytes.
// It passes createByName__FPcG6Point3sPPc at 0x0014c410 to
// objectSetCreateByNameFunction, then passes
// neverSaveAlwaysLoadFromLump__FPc at 0x00155b30 to
// objectSetFilterFunction, and returns. Those four symbols are not
// defined here.
extern "C" void createByName__FPcG6Point3sPPc();
extern "C" void neverSaveAlwaysLoadFromLump__FPc();
extern "C" void objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(void (*create)());
extern "C" void objectSetFilterFunction__FPFPc_i(void (*filter)());

extern "C" void gameobjInit__Fv() {
    objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject(createByName__FPcG6Point3sPPc);
    objectSetFilterFunction__FPFPc_i(neverSaveAlwaysLoadFromLump__FPc);
}
