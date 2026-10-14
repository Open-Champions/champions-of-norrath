// Observed: OnDrawSelf__5VIWnd at 0x010c3508 is 44 bytes. When this equals the address of the
// global object sWndMcErr (0x01190a20) it tail-calls OnDrawSelf__10VIWndMcErr(this); otherwise it
// returns at once.
// Inferred: the base class draws nothing itself; the one global instance of the memory-card error
// window routes to its own drawing. This is the slot-3 entry of the base class table.
// Unknown: whether the identity test reflects source-level code or a compiler-folded check.
extern "C" {
extern unsigned char sWndMcErr[0x210];
void OnDrawSelf__10VIWndMcErr(void* self);

void OnDrawSelf__5VIWnd(void* self) {
    if (self == sWndMcErr) OnDrawSelf__10VIWndMcErr(self);
}
}
