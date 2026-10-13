// Observed bActive__3Web at 0x0020fbd0, 12 bytes: returns 1 when the word at web+44 is nonzero, else 0.
// The word is written by reset__3Web (to 1) and by breakWeb__3Web (to 0); the callers (the BlackWidow and
// SpiderQueen run functions) use a zero result to leave their web state. Unknown: what else writes +44.
// Nothing here ran on the EE.
extern "C" int bActive__3Web(const void* self) {
    return *reinterpret_cast<const unsigned*>(static_cast<const unsigned char*>(self) + 44) != 0u;
}
