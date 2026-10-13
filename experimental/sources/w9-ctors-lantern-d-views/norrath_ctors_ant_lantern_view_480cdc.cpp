// Observed .lit4 word at 0x480cdc. The bits are 0x40866666. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480cdc;
const float norrath_ctors_ant_lantern_view_480cdc = 0x1.0ccccc0000000p+2f;
