// Observed: MC_ResetAutoCheckPointSave__FPc at 0x0025b0e0 is 100 bytes. With name the argument:
//  1. when asyncMode is 7 or 3 it returns without doing anything else;
//  2. when engineTaskActive__FPc("deathTask") is nonzero it returns;
//  3. otherwise saveCheckPointCount becomes 10 if strcmp(name, "arena1") is zero, else 1.
// Inferred: saveCheckPointCount is the number of frames or steps before the next automatic
// checkpoint save, from its name and from other writers (loadRamSlot sets 99).
// Unknown: what asyncMode values 3 and 7 stand for here, and the unit of the count.

extern "C" {
extern int asyncMode;
extern int saveCheckPointCount;
int engineTaskActive__FPc(const char* name);
int strcmp(const char* left, const char* right);
}

extern "C" void MC_ResetAutoCheckPointSave__FPc(char* name) {
    if (asyncMode == 7 || asyncMode == 3) return;
    if (engineTaskActive__FPc("deathTask")) return;
    saveCheckPointCount = strcmp(name, "arena1") != 0 ? 1 : 10;
}
