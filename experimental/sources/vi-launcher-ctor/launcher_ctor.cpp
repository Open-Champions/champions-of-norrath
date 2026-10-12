// Observed __10VILauncher at 0x0106c258, 8 bytes. It returns without
// writing the object. The VILauncher size is not established here.
extern "C" {
struct VILauncher;
void __10VILauncher(VILauncher* object) {
    (void)object;
}
}
