// Observed getModel__3NPC at 0x0030fba0, 8 bytes (a weak, inline-style definition).
// Returns the pointer stored in the word at object+1056 and touches nothing else.
// Inferred from the NPC constructor, which stores the model resource found by
// lump lookup at that offset. Nothing here ran on the EE.
extern "C" void* getModel__3NPC(void* self) {
    return *reinterpret_cast<void**>(static_cast<unsigned char*>(self) + 1056);
}
