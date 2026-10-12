/* Observed InitTLB at 0x002dfd68, 64 bytes. It calls GetMemorySize (service 127).
 * When the result equals 0x02000000 it calls InitTLB32MB and otherwise calls
 * _InitTLB (service 130). It takes no arguments and the result left in the return
 * register is whatever the chosen callee left there. What the two table setups
 * contain is unknown. Compiled output is not claimed to match the original.
 */
extern "C" {
int GetMemorySize();
void InitTLB32MB();
void _InitTLB();
}

extern "C" void InitTLB() {
    if (GetMemorySize() == 0x02000000)
        InitTLB32MB();
    else
        _InitTLB();
}
