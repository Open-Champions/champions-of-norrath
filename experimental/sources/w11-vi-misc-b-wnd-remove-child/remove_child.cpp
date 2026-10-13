// Observed: RemoveChild__5VIWndP5VIWnd at 0x010c34b8 is 64 bytes. With (this, child) it keeps
// child in a stack word, calls Find__Ct5VISet1ZP5VIWndRCP5VIWnd(this, &that word), passes the
// result with this to Erase__t5VISet1ZP5VIWndi(this, result), then re-reads the word and stores
// 0 at offset 20 of it. Nothing is returned.
// Inferred: the result of Find is an element index and offset 20 is the parent word.
// Unknown: what Find returns for an absent child and whether Erase accepts it.
extern "C" {
int Find__Ct5VISet1ZP5VIWndRCP5VIWnd(void* set, void* const* key);
void Erase__t5VISet1ZP5VIWndi(void* set, int index);

void RemoveChild__5VIWndP5VIWnd(void* self, void* child) {
    void* key = child;
    Erase__t5VISet1ZP5VIWndi(self, Find__Ct5VISet1ZP5VIWndRCP5VIWnd(self, &key));
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(key) + 20) = 0;
}
}
