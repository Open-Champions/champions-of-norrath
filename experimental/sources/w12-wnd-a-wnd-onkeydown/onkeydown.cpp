// Observed: OnKeyDown__5VIWndi at 0x010c3590 is 88 bytes. With (this, key): when this equals the
// global sWndMcErr (0x01190a20) it tail-calls OnKeyDown__10VIWndMcErri(this, key). Otherwise it
// reads the pointer word at this+20; if nonzero it calls that object's table entry at byte offset
// 40 (table word at object+32), passing the object adjusted by the entry's first halfword and key.
// Inferred: this+20 is the parent window and entry 40 is its OnKeyDown, so the base class passes
// key presses up the parent chain.
// Unknown: nothing beyond the names; the table entry's second halfword is not read.
extern "C" {
extern unsigned char sWndMcErr[0x210];
void OnKeyDown__10VIWndMcErri(void* self, int key);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

void OnKeyDown__5VIWndi(void* self, int key) {
    if (self == sWndMcErr) {
        OnKeyDown__10VIWndMcErri(self, key);
        return;
    }
    unsigned char* parent = reinterpret_cast<unsigned char*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 20)));
    if (parent == 0) return;
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(
                               static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(parent + 32))) + 5;
    entry->function(parent + entry->delta, key);
}
}
