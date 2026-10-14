// Observed: hubNames at 0x00438b10 is 20 bytes in the initialized data: five 32-bit string
// pointers to "kelethin", "gnomehub", "beachhub", "elfhub" and "airhub".
// Inferred: the level names of the five hub areas, since the jump-gate hub reset walks them with
// the hub index.
// Unknown: who else reads them. The strings are placed by the build profile, not as in the image.
extern "C" const char* hubNames[5];
const char* hubNames[5] = {"kelethin", "gnomehub", "beachhub", "elfhub", "airhub"};
