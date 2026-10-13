// Observed .lit4 word at 0x480b3c. The bits are 0x3eb33333. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480b3c;
const float norrath_ctors_ant_lantern_view_480b3c = 0x1.6666660000000p-2f;
