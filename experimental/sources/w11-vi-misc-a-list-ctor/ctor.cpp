// Observed: __7VILList at 0x01072788 is 20 bytes. It tail-calls Init__7VILList
// with the same object pointer and does nothing else.
extern "C" {
void Init__7VILList(void* list);

void __7VILList(void* list) {
    Init__7VILList(list);
}
}
