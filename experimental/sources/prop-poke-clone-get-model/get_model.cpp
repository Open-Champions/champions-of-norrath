// Observed getModel__10GameObject at 0x3130e0, 8 bytes, weak. jr ra with
// zero in v0. No byte of the object is read or written. Overrides exist in
// derived tables; this is only the base slot.
extern "C" void* getModel__10GameObject(void* self) {
    (void)self;
    return nullptr;
}
