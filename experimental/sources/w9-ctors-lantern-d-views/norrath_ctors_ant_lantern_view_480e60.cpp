// Observed .lit4 word at 0x480e60. The bits are 0x3fa66666. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480e60;
const float norrath_ctors_ant_lantern_view_480e60 = 0x1.4ccccc0000000p+0f;
