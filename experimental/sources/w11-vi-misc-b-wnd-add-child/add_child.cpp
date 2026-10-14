// Observed: AddChild__5VIWndP5VIWnd at 0x010c3480 is 52 bytes. With (this, child) it keeps child
// in a stack word, calls Insert__t5VISet1ZP5VIWndRCP5VIWnd(this, &that word), then re-reads the
// word and stores this at offset 20 of it. Nothing is returned.
// Inferred: the set at offset 0 holds child pointers and offset 20 is the parent word.
// Unknown: the result of Insert (ignored here) and the behavior for an already present child.
extern "C" {
void Insert__t5VISet1ZP5VIWndRCP5VIWnd(void* set, void* const* key);

void AddChild__5VIWndP5VIWnd(void* self, void* child) {
    void* key = child;
    Insert__t5VISet1ZP5VIWndRCP5VIWnd(self, &key);
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(key) + 20) =
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self));
}
}
