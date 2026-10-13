// Observed .lit4 word at 0x480bb4. The bits are 0x3e999999. Ant-through-lantern constructors load it as a single float
// (some copy the raw word into an object field). Neighboring literals are separate words.
extern "C" const float norrath_ctors_ant_lantern_view_480bb4;
const float norrath_ctors_ant_lantern_view_480bb4 = 0x1.3333320000000p-2f;
