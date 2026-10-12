// Observed: two file-local words used by menuRun__FP4MenuP3padPi and
// menuDraw__FiiP4MenuUiUiP4FontP8_texturei. The image has no symbol at
// either address, so these names only record the address.
// 0x004844e8 is a 4-byte float whose image value is 1.0f; menuRun advances
// it by a constant per call up to 1.0f, resets it to 0 when the cursor moves,
// and menuDraw feeds it to the highlight easing and color blend.
// 0x00484514 is a 4-byte word whose image value is 116; menuDraw converts it
// to float and scales it by commThresh to place a marker on the kind 8 bar.
// A separate global `t` exists at 0x00484c80; it is not this object.
// Inferred: the first is a cursor-move tween time and the second a bar width
// in pixels. Unknown: whether either was a function-local static.
extern "C" {
float norrath_menu_local_t_004844e8 = 1.0f;
int norrath_menu_word_00484514 = 116;
}
static_assert(sizeof(float) == 4 && sizeof(int) == 4, "both words are four bytes");
