// Observed: startProgress stores zero in the word progress and returns.
// Inferred: none.
// Unknown: what progress measures; the readers are in other packages.

extern "C" int progress;

extern "C" void startProgress__Fv() {
    progress = 0;
}
