// Observed GetYaw__6Camera at 0x0026fc40, 8 bytes.
// Returns the signed halfword at camera+104. Nothing is stored. Nothing here
// was executed on the EE.
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" int GetYaw__6Camera(const unsigned char* camera) {
    return *reinterpret_cast<const AliasedShort*>(camera + 104);
}
