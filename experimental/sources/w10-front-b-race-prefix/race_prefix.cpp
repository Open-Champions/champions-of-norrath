// Observed in the pinned SLUS_205.65 image: racePrefix at 0x0041fcc0 is 40 bytes, ten pointers to
// short NUL-terminated strings that sit in eight-byte cells starting at 0x00484550: "bm", "bf",
// "wem", "wef", "hem", "hef", "em", "ef", "dem", "def". The player constructor indexes it with the
// word at record+84 and feeds the chosen prefix to sprintf together with numbers and a letter to
// build resource names.
// Inferred: pairs of entries (male then female) for five races, from the repeated m/f endings.
// Unknown: which race each pair names; the index values are not tied to a race here.
extern "C" {
const char* racePrefix[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};
}
