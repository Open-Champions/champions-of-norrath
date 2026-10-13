// Observed: SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3 at 0x01112fb0, 84 bytes. It
// copies the three floats at the last argument to raster + 18924, stores 1 at raster +
// 17896, then calls memcpy(raster + 18936, vectors, count * 12) with the int and pointer
// arguments, and returns 0.
// Inferred: count three-float entries are copied into a table right after one common
// three-float offset, and the raster word is a "pre-translations changed" flag.
// Unknown: the member names, the table's capacity (no bound is checked) and the units.
extern "C" {
#if defined(NORRATH_HOST_CONTRACT)
void* VICopyBytes(void* destination, const void* source, unsigned int length);
#else
void* VICopyBytes(void* destination, const void* source, unsigned int length) __asm__("memcpy");
#endif

int SetPreTranslations__8VIRasteriP7VIVect3RC7VIVect3(void* raster, int count, const void* vectors, const void* offset) {
    unsigned char* r = static_cast<unsigned char*>(raster);
    const float* from = static_cast<const float*>(offset);
    float* to = reinterpret_cast<float*>(r + 18924);
    to[0] = from[0];
    *reinterpret_cast<int*>(r + 17896) = 1;
    to[1] = from[1];
    to[2] = from[2];
    VICopyBytes(r + 18936, vectors, static_cast<unsigned int>(count * 12));
    return 0;
}
}
