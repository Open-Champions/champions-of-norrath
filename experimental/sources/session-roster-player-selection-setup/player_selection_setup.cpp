// Observed playerSelectionSetup__Fv at 0x002804e8, 32 bytes. It calls init__Fv
// (0x0027d600) once and returns. No argument is passed and no result is read.
// What init__Fv sets up is not established here.
extern "C" void init__Fv();

extern "C" void playerSelectionSetup__Fv() {
    init__Fv();
}
