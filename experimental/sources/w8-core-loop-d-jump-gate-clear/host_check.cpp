// Host contract for jumpGateClear__Fv. Not an EE plan input.
extern "C" int jumping = 0;
static void (*removed[4])();
static int n;
extern "C" void jumpGateTaskRun__Fv() {}
extern "C" void jumpGateTaskDraw__Fv() {}
extern "C" void engineRemoveTask__FPFv_v(void (*f)()) { if (n < 4) removed[n++] = f; }
#include "jump_gate_clear.cpp"
int main() {
    jumping = 1; jumpGateClear__Fv();
    if (n != 0) return 1;
    jumping = 0; jumpGateClear__Fv();
    if (n != 2 || removed[0] != jumpGateTaskRun__Fv || removed[1] != jumpGateTaskDraw__Fv) return 2;
    jumping = -1; jumpGateClear__Fv();
    if (n != 2) return 3;
    return 0;
}
