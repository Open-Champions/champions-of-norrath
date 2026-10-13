// Observed lightSetColorScale__Ffff at 0x0013ca40, extent 328.
// (r, g, b) floats. The three scale values are stored to charScaleR, charScaleG
// and charScaleB (0x00486868, 0x0048686c, 0x00486870). Then for each of the
// three colour objects charAmbient (0x00486850), charDirect (0x00486858) and
// charDirect2 (0x00486860), the first three signed halfwords are converted to
// float, multiplied by r, g and b respectively, converted back with truncation
// and stored as halfwords in place. The fourth halfword of each is untouched.
// Inferred: the objects hold per-channel light colours; channel order unknown.
// charAmbient, charDirect, charDirect2 and charScaleR/G/B are defined by another
// package (w8-net-play-a-char-light-state).
extern "C" {
extern unsigned char charAmbient[8];
extern unsigned char charDirect[8];
extern unsigned char charDirect2[8];
extern float charScaleR;
extern float charScaleG;
extern float charScaleB;

void lightSetColorScale__Ffff(float r, float g, float b) {
    short* ambient = reinterpret_cast<short*>(charAmbient);
    short* direct = reinterpret_cast<short*>(charDirect);
    short* direct2 = reinterpret_cast<short*>(charDirect2);
    float ar = static_cast<float>(ambient[0]);
    float ag = static_cast<float>(ambient[1]);
    float ab = static_cast<float>(ambient[2]);
    float dr = static_cast<float>(direct[0]);
    float dg = static_cast<float>(direct[1]);
    float db = static_cast<float>(direct[2]);
    float er = static_cast<float>(direct2[0]);
    float eg = static_cast<float>(direct2[1]);
    float eb = static_cast<float>(direct2[2]);
    charScaleR = r;
    charScaleG = g;
    charScaleB = b;
    ambient[0] = static_cast<short>(static_cast<int>(ar * r));
    ambient[1] = static_cast<short>(static_cast<int>(ag * g));
    ambient[2] = static_cast<short>(static_cast<int>(ab * b));
    direct[0] = static_cast<short>(static_cast<int>(dr * r));
    direct[1] = static_cast<short>(static_cast<int>(dg * g));
    direct[2] = static_cast<short>(static_cast<int>(db * b));
    direct2[0] = static_cast<short>(static_cast<int>(er * r));
    direct2[1] = static_cast<short>(static_cast<int>(eg * g));
    direct2[2] = static_cast<short>(static_cast<int>(eb * b));
}
}
