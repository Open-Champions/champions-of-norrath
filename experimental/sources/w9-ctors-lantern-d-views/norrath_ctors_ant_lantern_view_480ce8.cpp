// Observed .lit4 word at 0x480ce8. The bits are 0x3f4ccccc. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480ce8;
const float norrath_ctors_ant_lantern_view_480ce8 = 0x1.9999980000000p-1f;
