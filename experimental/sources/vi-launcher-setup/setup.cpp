// Observed Setup__10VILauncher at 0x0106c558, 20 bytes. It returns the
// result of setup_main. The launcher pointer is not read.
extern "C" {
struct VILauncher;
int setup_main__Fv();

int Setup__10VILauncher(VILauncher* object) {
    (void)object;
    return setup_main__Fv();
}
}
