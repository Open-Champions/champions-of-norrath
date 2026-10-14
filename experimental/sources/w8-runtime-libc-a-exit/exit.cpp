// Observed exit at 0x002ffb08, 168 bytes. It reads _impure_ptr (0x00448480) and
// then the word at impure+328, the head of the registered-function lists. For
// each list node (next at +0, count at +4, up to 32 function pointers from +8)
// the registered functions are called from the last to the first; the count is
// re-read at every node and not modified. After the last node, the word at
// impure+60 is called if nonzero with the impure pointer as its argument, and the routine tail-calls _exit at
// 0x00100248 with the original status. The first node is embedded in the
// impure image at +332 (this is where atexit at 0x002ff8d8 appends).
// Inferred: impure+60 is a stream-flush hook; its name is not claimed.
// Unknown: _exit's behavior, which is a separate routine outside this unit.
struct NorrathExitReent;
struct NorrathExitNode {
    NorrathExitNode* next;
    int count;
    void (*functions[32])();
};

struct NorrathExitReent {
    unsigned char before_hook[60];
    void (*hook)(NorrathExitReent*);
    unsigned char gap[264];
    NorrathExitNode* list_head;
    NorrathExitNode first_node;
};

#if defined(__mips__)
static_assert(sizeof(void*) == 4, "EE pointer width");
static_assert(__builtin_offsetof(NorrathExitReent, hook) == 60, "hook word");
static_assert(__builtin_offsetof(NorrathExitReent, list_head) == 328, "list head word");
static_assert(__builtin_offsetof(NorrathExitReent, first_node) == 332, "embedded first node");
static_assert(sizeof(NorrathExitNode) == 136, "node of 32 functions");
#endif

extern "C" {
extern unsigned char* norrath_factory_impure_pointer_word;
void _exit(int status);

void exit(int status) {
    NorrathExitReent* reent = reinterpret_cast<NorrathExitReent*>(norrath_factory_impure_pointer_word);
    for (NorrathExitNode* node = reent->list_head; node != nullptr; node = node->next) {
        for (int index = node->count - 1; index >= 0; --index) node->functions[index]();
    }
    if (reent->hook != nullptr) reent->hook(reent);
    _exit(status);
}
}
