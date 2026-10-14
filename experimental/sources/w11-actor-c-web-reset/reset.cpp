// Observed reset__3Web at 0x0020fae0, 12 bytes: stores 1 in the word at web+44 and nothing else.
// bActive__3Web reads that word; breakWeb__3Web clears it. Called by the BlackWidow and SpiderQueen
// set-state functions on an embedded web. Unknown: the field's name. Nothing here ran on the EE.
extern "C" void reset__3Web(void* self) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 44) = 1u;
}
