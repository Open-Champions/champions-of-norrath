// Observed netStartLevel at 0x00135108, 8 bytes: it stores zero to the word
// localPredictedPlayer (gp-relative sdata) and returns.
// Inferred: none. The name is the image's own symbol; the meaning of the
// stored value is not established here.
// Unknown: who reads localPredictedPlayer.
extern "C" int localPredictedPlayer;

extern "C" void netStartLevel__Fv() {
    localPredictedPlayer = 0;
}
