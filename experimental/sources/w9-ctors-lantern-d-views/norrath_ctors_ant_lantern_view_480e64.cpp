// Observed .lit4 word at 0x480e64. The bits are 0x3e4ccccc. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480e64;
const float norrath_ctors_ant_lantern_view_480e64 = 0x1.9999980000000p-3f;
