// Observed SetHold__7Vehiclei at 0x00277b80, 8 bytes.
// Stores the argument as a word at vehicle+1024 and returns nothing.
// What the word means is not established here. Nothing here was executed on the EE.
typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void SetHold__7Vehiclei(unsigned char* vehicle, int hold) {
    *reinterpret_cast<AliasedWord*>(vehicle + 1024) = static_cast<unsigned>(hold);
}
