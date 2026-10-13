// Observed dismount__6Player at 0x001aba50, 64 bytes. With i the signed byte at object+292:
// the float at object + i*32 + 280 is set to 1.0; the word at +1400 is set to 2; the word at
// +1088 is set to the global word frameNm; and the float at object + i*32 + 284 is set to a
// float constant of the executable's small-data pool (observed bits 0xbdaaaaaa,
// about -0.0833333). The byte at +292 is read a second time after the word stores. Nothing is
// returned. Nothing here ran on the EE; what these fields mean is unknown.
extern "C" {
extern unsigned frameNm;

void dismount__6Player(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    *reinterpret_cast<float*>(o + static_cast<int>(static_cast<signed char>(o[292])) * 32 + 280) = 1.0f;
    *reinterpret_cast<int*>(o + 1400) = 2;
    *reinterpret_cast<unsigned*>(o + 1088) = frameNm;
    *reinterpret_cast<float*>(o + static_cast<int>(static_cast<signed char>(o[292])) * 32 + 284) = __builtin_bit_cast(float, 0xbdaaaaaau);
}
}
