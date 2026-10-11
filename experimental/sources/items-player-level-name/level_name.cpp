// Observed levelName at 0x34b660, 322 bytes, all zero. main writes offsets 0, 80,
// 81, and 242 later. The separate 80-byte view at 0x34b752 is the image tail
// of this object and stays its own symbol.
extern "C" unsigned char levelName[322] = {};
static_assert(sizeof levelName == 322, "levelName extent");
