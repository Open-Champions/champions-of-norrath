// Observed SetPitch__6Camerai at 0x0026fb00, 8 bytes.
// Stores the low halfword of the argument at camera+106 and returns nothing.
// No other byte is written. Nothing here was executed on the EE.
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" void SetPitch__6Camerai(unsigned char* camera, int pitch) {
    *reinterpret_cast<AliasedShort*>(camera + 106) = static_cast<short>(pitch);
}
