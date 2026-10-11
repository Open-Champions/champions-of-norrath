// Observed helpMessageInit__Fv at 0x001c7828, 48 bytes.
// It registers helpMessageFrame__Fv, at 0x001c4b18, with priority 50,
// the bytes "HelpMessage" at 0x0046a868, and flag 1. Those bytes have no
// ELF symbol. The frame function and engineAddTask are not defined here.
extern "C" void helpMessageFrame__Fv();
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);

extern "C" void helpMessageInit__Fv() {
    engineAddTask__FPFv_viPci(helpMessageFrame__Fv, 50, const_cast<char*>("HelpMessage"), 1);
}
