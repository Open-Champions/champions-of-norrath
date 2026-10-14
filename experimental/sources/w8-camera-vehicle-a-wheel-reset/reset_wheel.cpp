// Observed Reset__5Wheel at 0x002783a8, 20 bytes.
// Clears four words of the wheel: +392, +368, +360 and +364. Nothing else is
// read or written and nothing is returned. The meaning of those fields is not
// established here. Nothing here was executed on the EE.
typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void Reset__5Wheel(unsigned char* wheel) {
    *reinterpret_cast<AliasedWord*>(wheel + 392) = 0;
    *reinterpret_cast<AliasedWord*>(wheel + 368) = 0;
    *reinterpret_cast<AliasedWord*>(wheel + 360) = 0;
    *reinterpret_cast<AliasedWord*>(wheel + 364) = 0;
}
