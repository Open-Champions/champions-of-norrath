// Observed SetYaw__6Camerai at 0x0026faf8, 8 bytes.
// Stores the low halfword of the argument at camera+104 and returns nothing.
// No other byte is written. Nothing here was executed on the EE.
typedef short __attribute__((may_alias)) AliasedShort;

extern "C" void SetYaw__6Camerai(unsigned char* camera, int yaw) {
    *reinterpret_cast<AliasedShort*>(camera + 104) = static_cast<short>(yaw);
}
