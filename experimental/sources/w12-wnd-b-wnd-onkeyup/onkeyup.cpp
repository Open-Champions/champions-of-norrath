// Observed: OnKeyUp__5VIWndi at 0x010c3538 is 88 bytes. With (this, key): when this is the address of
// the global object sWndMcErr (0x01190a20) it tail-calls OnKeyUp__10VIWndMcErri(this, key).
// Otherwise it reads the parent word at +20; if that is nonzero it calls
// the entry at byte offset 32 of the parent's table (table word at parent+32, entry = halfword
// adjustment then a function address) with (parent + adjustment, key). With a zero parent it returns.
// Inferred: this is the base-class key-release handler that forwards the event up the window tree,
// and the table entry at offset 32 is the OnKeyUp slot (the class tables list it fifth).
// Unknown: the key argument's range and what the McErr window does with it.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
extern unsigned char sWndMcErr[];
void OnKeyUp__10VIWndMcErri(void* self, int key);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

void OnKeyUp__5VIWndi(void* self, int key) {
    if (self == sWndMcErr) {
        OnKeyUp__10VIWndMcErri(self, key);
        return;
    }
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* parent = reinterpret_cast<unsigned char*>(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(object + 20)));
    if (parent) {
        const VtEntry* entry = reinterpret_cast<const VtEntry*>(
                                   NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(parent + 32))) + 4;
        entry->function(parent + entry->delta, key);
    }
}
}
