// Observed detach__11FireBomblet at 0x0028b828, 8 bytes: stores zero in the word at object+172 and
// returns. Which field that is (the callers pass the object a bomblet was attached to elsewhere, then
// call this) is unknown; the word's role is not established here. Nothing here ran on the EE.
extern "C" void detach__11FireBomblet(void* self) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 172) = 0;
}
