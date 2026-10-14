// Observed startForces__7Vehicle at 0x00277e10, 32 bytes.
// Stores 0.0f at object+1992, +2012, +2008, +2004, +2000 and +1996 and returns
// nothing. No other byte is touched. Nothing here was executed on the EE.
typedef float AliasedFloat __attribute__((may_alias));

extern "C" void startForces__7Vehicle(unsigned char* object) {
    for (int offset = 1992; offset <= 2012; offset += 4) {
        *reinterpret_cast<AliasedFloat*>(object + offset) = 0.0f;
    }
}
