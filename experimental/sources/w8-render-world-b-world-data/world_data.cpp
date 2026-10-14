// World-level storage observed in the executable (SLUS_205.65):
//   world            0x004844a0, 4 bytes, .sdata, zero
//   worldDrawEnable  0x0048412c, 1 byte,  .sdata, initial value 1
//   worldViewCenter  0x0037ee90, 12 bytes, .data, zero (three floats)
// Names and sizes come from the executable's symbol table; the initial values
// were read from the file image. The meaning of the world header is not
// established here.
#if defined(NORRATH_HOST_WORLD_DATA)
#define NORRATH_LOADED(name)
#else
#define NORRATH_LOADED(name) __attribute__((section(name)))
#endif

extern "C" {
void* world NORRATH_LOADED(".sdata") = nullptr;
unsigned char worldDrawEnable NORRATH_LOADED(".sdata") = 1;
float worldViewCenter[3] NORRATH_LOADED(".data") = {0.0f, 0.0f, 0.0f};
}
