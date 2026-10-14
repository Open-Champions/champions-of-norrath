// Observed .lit4 word at 0x480b4c. The bits are 0x3f199999. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480b4c;
const float norrath_ctors_ant_lantern_view_480b4c = 0x1.3333320000000p-1f;
