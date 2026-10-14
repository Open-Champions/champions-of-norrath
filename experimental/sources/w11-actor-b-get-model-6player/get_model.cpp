// Observed getModel__6Player at 0x0030e828 (weak, 8 bytes): returns the word stored at object+1332.
// Unknown: the pointed-to type (callers pass it where a model header is expected, which is inferred
// from the name only). Nothing here ran on the EE.
extern "C" void* getModel__6Player(void* self) {
    return *reinterpret_cast<void**>(static_cast<unsigned char*>(self) + 1332);
}
