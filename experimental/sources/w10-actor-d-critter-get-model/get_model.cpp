// Observed getModel__7Critter at 0x0030eca0, 8 bytes (weak).
// Returns the pointer-sized word stored at object+1036 and reads nothing else.
// Inferred: that word is the model header the draw routine passes on (msg_draw__7Critter reads
// the same word as its second argument). Unknown: its type name. Nothing here ran on the EE.
#if defined(W10_ACTOR_D_HOST)
extern unsigned char* w10_actor_d_host_base;  // host builds keep 32-bit words as offsets from this base
#endif

extern "C" void* getModel__7Critter(void* self) {
    unsigned word = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 1036);
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_host_base + word;
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
