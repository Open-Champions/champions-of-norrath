// Observed: DestroyPool__t5VISet1ZP5VIWnd at 0x0120bc28 is 88 bytes (weak). With (this) it sets i to
// Begin__Ct5VISet1ZP5VIWnd(this) and, while i differs from End__Ct5VISet1ZP5VIWnd(this) (evaluated again on
// every pass), sets i to Next__Ct5VISet1ZP5VIWndi(this, i). Nothing else is called, nothing is written, and
// it returns 0.
// Inferred: nothing beyond the three accessor calls is done per element; the name suggests more was intended.
// Unknown: the element type of i (an index is assumed), and whether the return is a declared int.
extern "C" {
int Begin__Ct5VISet1ZP5VIWnd(const void* self);
int End__Ct5VISet1ZP5VIWnd(const void* self);
int Next__Ct5VISet1ZP5VIWndi(const void* self, int index);

int DestroyPool__t5VISet1ZP5VIWnd(void* self) {
    for (int i = Begin__Ct5VISet1ZP5VIWnd(self); i != End__Ct5VISet1ZP5VIWnd(self);
         i = Next__Ct5VISet1ZP5VIWndi(self, i)) {
    }
    return 0;
}
}
