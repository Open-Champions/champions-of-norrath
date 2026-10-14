// Observed addCommander__14WoodElfSoldier at 0x00249ce8, 88 bytes.
// Scans the ten list heads of the class list _14WoodElfSoldier$m_commanderList (0x0042dd90, 40
// bytes) from the first, and links the object into the first head whose word is zero by calling
// listOnewayLink__FP8ListHeadT0Pv with that head, the object and the object. If all ten heads are
// nonzero nothing is linked. Nothing is returned.
// Inferred: the ten heads are commander slots. Unknown: what a head's nonzero word designates.
// Nothing here ran on the EE.
extern "C" {
extern unsigned WoodElfSoldier_commanderList[10] __asm__("_14WoodElfSoldier$m_commanderList");
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value);
}

extern "C" void addCommander__14WoodElfSoldier(void* self) {
    unsigned* head = WoodElfSoldier_commanderList;
    for (int n = 0; n < 10; ++n, ++head) {
        if (*head == 0u) {
            listOnewayLink__FP8ListHeadT0Pv(head, self, self);
            return;
        }
    }
}
