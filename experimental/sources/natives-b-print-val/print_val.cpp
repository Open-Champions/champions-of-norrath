// Observed amxPrintVal at 0x0013ae18, 8 bytes. It returns zero and
// does not read the AMX cells.
extern "C" int amxPrintVal__FP5__amxPi(void*, int*) {
    return 0;
}
