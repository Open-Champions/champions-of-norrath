// Observed: CombineDeadkey__10VIKeyboardUc at 0x002cbbe0, 696 bytes. Arguments: the keyboard and an 8-bit character
// code. The word at offset 528 of the keyboard (the pending dead key) selects one of five tables; any other value
// returns 0. The code c is used as-is (low 8 bits). Pending 94, 96, 180, 168 and 126 each select a range of
// c - 65 (53, 53, 57, 57 and 47 entries respectively, so c from 65 to 117, 117, 121, 121 and 111); outside the
// range, or for a c with no entry, the result is 0. The entries are the Latin-1 accented forms: 94 gives
// circumflex vowels (A E I O U a e i o u), 96 grave vowels (same set), 180 acute vowels and Y/y, 168
// diaeresis vowels, Y -> 0xdf and y -> 0xff, and 126 tilde A N O a n o. The case table in the source
// lists the exact numeric pairs read from the original's five jump tables. The pending word itself is not
// modified here.
// Inferred: this is the dead-key composition step of the keyboard reader: the pending key is the previous
// dead-key press (^ ` acute diaeresis ~) and c the next letter. The result for diaeresis + Y being 0xdf (a
// sharp s, not a Y with diaeresis) is as observed; it is probably an error in the original table, kept as is.
// Unknown: where the pending word is set and cleared, and whether other pending values occur.
extern "C" {
unsigned char CombineDeadkey__10VIKeyboardUc(void* keyboard, unsigned char c) {
    const int pending = *reinterpret_cast<const int*>(static_cast<const char*>(keyboard) + 528);
    switch (pending) {
    case 94:
      switch (c) {
      case 65: return 194;  // 0x41
      case 69: return 202;  // 0x45
      case 73: return 206;  // 0x49
      case 79: return 212;  // 0x4f
      case 85: return 219;  // 0x55
      case 97: return 226;  // 0x61
      case 101: return 234;  // 0x65
      case 105: return 238;  // 0x69
      case 111: return 244;  // 0x6f
      case 117: return 251;  // 0x75
      default: return 0;
      }
    case 96:
      switch (c) {
      case 65: return 192;  // 0x41
      case 69: return 200;  // 0x45
      case 73: return 204;  // 0x49
      case 79: return 210;  // 0x4f
      case 85: return 217;  // 0x55
      case 97: return 224;  // 0x61
      case 101: return 232;  // 0x65
      case 105: return 236;  // 0x69
      case 111: return 242;  // 0x6f
      case 117: return 249;  // 0x75
      default: return 0;
      }
    case 180:
      switch (c) {
      case 65: return 193;  // 0x41
      case 69: return 201;  // 0x45
      case 73: return 205;  // 0x49
      case 79: return 211;  // 0x4f
      case 85: return 218;  // 0x55
      case 89: return 221;  // 0x59
      case 97: return 225;  // 0x61
      case 101: return 233;  // 0x65
      case 105: return 237;  // 0x69
      case 111: return 243;  // 0x6f
      case 117: return 250;  // 0x75
      case 121: return 253;  // 0x79
      default: return 0;
      }
    case 168:
      switch (c) {
      case 65: return 196;  // 0x41
      case 69: return 203;  // 0x45
      case 73: return 207;  // 0x49
      case 79: return 214;  // 0x4f
      case 85: return 220;  // 0x55
      case 89: return 223;  // 0x59
      case 97: return 228;  // 0x61
      case 101: return 235;  // 0x65
      case 105: return 239;  // 0x69
      case 111: return 246;  // 0x6f
      case 117: return 252;  // 0x75
      case 121: return 255;  // 0x79
      default: return 0;
      }
    case 126:
      switch (c) {
      case 65: return 195;  // 0x41
      case 78: return 209;  // 0x4e
      case 79: return 213;  // 0x4f
      case 97: return 227;  // 0x61
      case 110: return 241;  // 0x6e
      case 111: return 245;  // 0x6f
      default: return 0;
      }
    default:
        return 0;
    }
}
}
