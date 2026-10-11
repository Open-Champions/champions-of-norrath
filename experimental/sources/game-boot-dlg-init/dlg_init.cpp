// Observed DLG_Init at 0x001d37a0, 104 bytes.
// It asks lumpFindResource for the unnamed bytes "dlgbox.lmp" at 0x0046cdd8
// and "box.tex" at 0x00485048, and stores that result in boxTex before either
// task is entered. It then registers runDialog at 0x001d2940 with 110, the
// unnamed bytes "Dialog Demon Run" at 0x0046cde8, and 0; then drawDialog at
// 0x001d2f28 with 110, the unnamed bytes "Dialog Demon Draw" at 0x0046ce00,
// and 1. After both registrations return it stores 0 in curDlg.
// Those four text addresses have no ELF symbol. curDlg's symbol size is 0;
// the store is one word. What the tasks do, and what curDlg means past that
// word, was not executed.
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name);
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag);
extern "C" void runDialog__Fv();
extern "C" void drawDialog__Fv();
extern "C" char* boxTex;
extern "C" unsigned curDlg;

extern "C" void DLG_Init__Fv() {
    boxTex = lumpFindResource__FPcT0(
        const_cast<char*>("dlgbox.lmp"), const_cast<char*>("box.tex"));
    engineAddTask__FPFv_viPci(runDialog__Fv, 110, const_cast<char*>("Dialog Demon Run"), 0);
    engineAddTask__FPFv_viPci(drawDialog__Fv, 110, const_cast<char*>("Dialog Demon Draw"), 1);
    curDlg = 0;
}
