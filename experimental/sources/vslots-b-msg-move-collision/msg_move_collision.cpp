// Observed msg_moveCollision__8CreaturePP10GameObject at 0x113738, 52 bytes.
// The object is not read. The pointer list is a sequence of 4-byte words.
// A zero first word returns. Otherwise the later words are read until a zero
// word. Nothing is stored. Unknown: why the walk result is discarded.
extern "C" void msg_moveCollision__8CreaturePP10GameObject(void* self, void* list) {
    (void)self;
    // The loads are the whole body. A plain local would be deleted because
    // nothing is stored.
    auto* cursor = static_cast<volatile unsigned*>(list);
    if (*cursor == 0u) return;
    ++cursor;
    while (*cursor != 0u) ++cursor;
}
