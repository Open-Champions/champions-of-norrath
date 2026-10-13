// Observed .lit4 word at 0x480cd8. The bits are 0x40933333. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480cd8;
const float norrath_ctors_ant_lantern_view_480cd8 = 0x1.2666660000000p+2f;
