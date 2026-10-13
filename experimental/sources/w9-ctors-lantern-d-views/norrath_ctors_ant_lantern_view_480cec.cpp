// Observed .lit4 word at 0x480cec. The bits are 0x3f666666. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480cec;
const float norrath_ctors_ant_lantern_view_480cec = 0x1.cccccc0000000p-1f;
