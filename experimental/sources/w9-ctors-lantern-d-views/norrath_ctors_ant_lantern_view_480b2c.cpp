// Observed .lit4 word at 0x480b2c. The bits are 0x40a66666. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480b2c;
const float norrath_ctors_ant_lantern_view_480b2c = 0x1.4ccccc0000000p+2f;
