// Observed .lit4 word at 0x480cc8. The bits are 0xbe999999. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480cc8;
const float norrath_ctors_ant_lantern_view_480cc8 = -0x1.3333320000000p-2f;
