/* Observed data, pinned SLUS_205.65 (initial bytes read from the image).
 *   sTranslucency   0x01192bc4, 4 bytes: 00 00 80 3f, the float 1.0.
 *   sCompactKeyboard 0x01192bc8, 4 bytes: 01 00 00 00, the word 1.
 * Observed from the text-entry window's draw routine (OnDrawSelf__9VIWndEdit): it stores the compact-keyboard
 * result (0 or 1) into sCompactKeyboard each frame, and moves a fade value at +360 of the window towards
 * sTranslucency; the key handler (OnKeyDown__9VIWndEditi) reads sCompactKeyboard.
 * Inferred: sTranslucency is a ceiling for the window fade; sCompactKeyboard mirrors whether the small keyboard
 * layout is in use.
 * Unknown: what else writes either word, and why the initial compact value is 1.
 */
extern "C" {
float sTranslucency = 1.0f;
int sCompactKeyboard = 1;
}
