// Observed lightSetAmbientOffset__FPs at 0x0013e730, extent 116. For each of
// four channels i: reads signed halfword offset[i] from the argument, adds the
// unsigned byte at scene+48+i, clamps negative sums to 0 (a sum of 0 or less
// stores 0; there is no upper clamp), and stores the result as a halfword into
// charAmbient[i] at 0x00486850. Returns nothing.
// Observed: the four bytes at scene+48..51 are read as unsigned bytes.
// Inferred: they are a base ambient colour of the current scene record.
// Unknown: the rest of the scene record layout and the channel order. scene
// (0x004f4060) and charAmbient are defined by other packages.
extern "C" {
extern unsigned char scene[];
extern unsigned char charAmbient[8];
void lightSetAmbientOffset__FPs(const short* offset) {
    short* dest = reinterpret_cast<short*>(charAmbient);
    for (int i = 0; i < 4; ++i) {
        int sum = static_cast<int>(scene[48 + i]) + offset[i];
        dest[i] = static_cast<short>(sum > 0 ? sum : 0);
    }
}
}
