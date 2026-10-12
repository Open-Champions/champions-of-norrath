// Observed GetPitch__6Camera at 0x0026fc48, 8 bytes.
// Returns the signed halfword at camera+106. Nothing is stored. Nothing here
// was executed on the EE.
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" int GetPitch__6Camera(const unsigned char* camera) {
    return *reinterpret_cast<const AliasedShort*>(camera + 106);
}
