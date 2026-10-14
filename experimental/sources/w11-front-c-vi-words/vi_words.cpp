// Observed sMessagePool at 0x011e5c18 (8-byte gap, a pointer slot) and sWndMcErr at
// 0x01190a20 (0x210 bytes), both zero, in the VI region. USocket free releases the pointer
// in sMessagePool; VIWnd focus handling passes sWndMcErr as the object of
// OnMessage__10VIWndMcErr. Inferred: sWndMcErr is a window object of 528 bytes.
// Unknown: its fields. The symbol sWndMcErr is global in the image; sMessagePool is local.
extern "C" {
void* sMessagePool;
unsigned char sWndMcErr[0x210];
}
