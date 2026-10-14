// Observed: OnMessage__20VIWndGenericRendererP5VIWndiii at 0x010bdcf8 is 548 bytes. With (this, from,
// message, a, b) it first calls printf("!!Entered OnMessage, id == %i\n", message), then selects:
//  - 4352: AddChild(this, from);
//  - 307: SetFocus(this, this, true);
//  - 4356: SetFocus(this, from, true);
//  - 4354: printf("!!!Remove Child.....\n"), store from at this + 1412, then
//    printf("!!!removing child, param = %i\n", a);
//  - 4358: when a is 3 store 1 in the global word mcerror (0x01192c4c); printf("DONE!  code = %i\n",
//    a); store zero in the global word mcerring (0x01192c58); SetFocus(this, this, true); add one to
//    the word at this + 1408;
//  - 308: nothing of its own;
//  - any other message: printf("!!Yo, its the default!\n") and OnMessage__5VIWndP5VIWndiii(this,
//    from, message, a, b).
// Then, for every message, forwarding: when the word at this + 52 is nonzero, the target is the
// window at this + 56 if that word is nonzero and IsOpen__11VIGUIDialog(it) is true, otherwise the
// object at this + 52 (re-read after the IsOpen call). The target's table word at target + 120 is
// followed to the entry at byte offset 32 (halfword adjustment, then function address), which is
// called with (target + adjustment, from, message, a, b). Nothing is returned.
// Inferred: the objects at this + 52 and + 56 are a child receiver and a dialog; the entry at table
// offset 32 takes the message arguments.
// Unknown: what the messages mean, why the state words change, the classes of the two objects, and
// whether the message in the first printf is an int (the format says %i).
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
int printf(const char* format, ...);
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* a, void* b);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
void AddChild__5VIWndP5VIWnd(void* self, void* child);
int IsOpen__11VIGUIDialog(void* dialog);
extern int mcerror;
extern int mcerring;

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, void*, void*);
};

static inline void* pointer_of(unsigned word) {
    return NORRATH_TRANSLATE(word);
}
static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

void OnMessage__20VIWndGenericRendererP5VIWndiii(void* self, void* from, int message, void* a, void* b) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    printf("!!Entered OnMessage, id == %i\n", message);
    switch (message) {
    case 4352:
        AddChild__5VIWndP5VIWnd(self, from);
        break;
    case 307:
        SetFocus__5VIWndP5VIWndb(self, self, true);
        break;
    case 4356:
        SetFocus__5VIWndP5VIWndb(self, from, true);
        break;
    case 4354:
        printf("!!!Remove Child.....\n");
        *reinterpret_cast<unsigned*>(bytes + 1412) = word_of(from);
        printf("!!!removing child, param = %i\n", word_of(a));
        break;
    case 4358:
        if (word_of(a) == 3u) mcerror = 1;
        printf("DONE!  code = %i\n", word_of(a));
        mcerring = 0;
        SetFocus__5VIWndP5VIWndb(self, self, true);
        *reinterpret_cast<int*>(bytes + 1408) = *reinterpret_cast<int*>(bytes + 1408) + 1;
        break;
    case 308:
        break;
    default:
        printf("!!Yo, its the default!\n");
        OnMessage__5VIWndP5VIWndiii(self, from, message, a, b);
        break;
    }
    if (*reinterpret_cast<unsigned*>(bytes + 52) == 0) return;
    unsigned char* target;
    void* dialog = pointer_of(*reinterpret_cast<unsigned*>(bytes + 56));
    if (dialog != nullptr && IsOpen__11VIGUIDialog(dialog) != 0)
        target = static_cast<unsigned char*>(pointer_of(*reinterpret_cast<unsigned*>(bytes + 56)));
    else
        target = static_cast<unsigned char*>(pointer_of(*reinterpret_cast<unsigned*>(bytes + 52)));
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(pointer_of(*reinterpret_cast<unsigned*>(target + 120))) + 4;
    entry->function(target + entry->delta, from, message, a, b);
}
}
