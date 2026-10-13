// Observed: OnMessage__5VIWndP5VIWndiii at 0x010c35e8 is 88 bytes. With (this, from, message, a, b):
// when this is the address of the global object sWndMcErr (0x01190a20) it tail-calls
// OnMessage__10VIWndMcErrP5VIWndiii with all arguments unchanged. Otherwise it reads the parent
// word at +20; if that is nonzero it calls the entry at byte offset 48 of the parent's table
// (table word at parent+32; entry = halfword adjustment then function address) with
// (parent + adjustment, from, message, a, b) unchanged. With a zero parent it returns.
// Inferred: the base-class message handler forwards unhandled messages to the parent window; the
// entry at offset 48 is the OnMessage slot (the class tables list it seventh).
// Unknown: the return value (nothing is read from the forwarded call) and message meanings.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
extern unsigned char sWndMcErr[];
void OnMessage__10VIWndMcErrP5VIWndiii(void* self, void* from, int message, int a, int b);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, int, int);
};

void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, int a, int b) {
    if (self == sWndMcErr) {
        OnMessage__10VIWndMcErrP5VIWndiii(self, from, message, a, b);
        return;
    }
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* parent = reinterpret_cast<unsigned char*>(NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(object + 20)));
    if (parent) {
        const VtEntry* entry = reinterpret_cast<const VtEntry*>(
                                   NORRATH_TRANSLATE(*reinterpret_cast<unsigned*>(parent + 32))) + 6;
        entry->function(parent + entry->delta, from, message, a, b);
    }
}
}
