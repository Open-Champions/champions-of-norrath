/* Observed data, pinned SLUS_205.65. Five two-word tables of the options window, each 8 bytes, all zero in the
 * image before the window first draws (initial bytes read from the image):
 *   VIWndOptions$sSoundOutputText    0x011e8bf8
 *   VIWndOptions$sCompactKeyboardText 0x011e8c00
 *   VIWndOptions$sOnOffText          0x011e8c08
 *   VIWndOptions$sCameraTypeText     0x011e8c10
 *   VIWndOptions$sCameraAxisText     0x011e8c18
 * Observed from the window's draw routine (OnDrawSelf__12VIWndOptions): the first time it runs it stores a
 * 16-bit string pointer from the string table into each of the two words of every table (strings 781/804,
 * 802/755, 552/551, 805/68 and 784/779), and later reads the words as the choice labels of the matching option.
 * Inferred: each table holds the two labels of a two-way choice (sound output, compact keyboard, on/off,
 * camera type, camera axis); the word order follows the option value.
 * Unknown: whether the image lays the five out as members of one object; the names above are the image's own
 * symbol names and say nothing about their wording.
 */
extern "C" {
unsigned sSoundOutputText[2] __asm__("VIWndOptions$sSoundOutputText") = {0, 0};
unsigned sCompactKeyboardText[2] __asm__("VIWndOptions$sCompactKeyboardText") = {0, 0};
unsigned sOnOffText[2] __asm__("VIWndOptions$sOnOffText") = {0, 0};
unsigned sCameraTypeText[2] __asm__("VIWndOptions$sCameraTypeText") = {0, 0};
unsigned sCameraAxisText[2] __asm__("VIWndOptions$sCameraAxisText") = {0, 0};
}
