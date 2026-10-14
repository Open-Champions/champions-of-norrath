// Observed getMoveDir__FiiiP12screenStruct at 0x001b25b8, 312 bytes. Arguments: a pad index, two
// counts (columns and rows, inferred from the comparisons below) and a screen record; the result
// is the int at screen + 20, which this function may set first.
// Let bits be the word at netLocalPad + 88 * pad + 8. Four cases are tried in this order and only
// the first whose buttons are held counts:
//  - bits & 0x4000 or bits & 0x200000: when screen[20] is zero and screen[24] + 2 <= the third
//    argument, screen[20] becomes 1;
//  - bits & 0x1000 or bits & 0x100000: when screen[20] is zero and screen[24] > 0, screen[20]
//    becomes -1;
//  - bits & 0x2000 or bits & 0x800000: when screen[20] is zero and screen[28] + 1 < the second
//    argument, screen[20] becomes 4;
//  - bits & 0x8000 or bits & 0x400000: when screen[20] is zero and screen[28] > 0, screen[20]
//    becomes 2.
// With none of the buttons held, or with screen[20] already nonzero, nothing is stored. The result
// is screen[20] read after the stores.
// Inferred: screen[20] is a latched move direction (1 and -1 along the rows, 4 and 2 along the
// columns), screen[24] and screen[28] the current row and column; the four button pairs look like
// the d-pad and the left stick directions.
// Unknown: the meaning of the direction values, and why the row limit is the third argument - 2.
extern "C" unsigned char netLocalPad[];

extern "C" int getMoveDir__FiiiP12screenStruct(int pad, int columns, int rows, void* screen) {
    int* fields = static_cast<int*>(screen);
    const unsigned bits = *reinterpret_cast<const unsigned*>(netLocalPad + pad * 88 + 8);
    if ((bits & 0x4000) != 0 || (bits & 0x200000) != 0) {
        if (fields[5] == 0 && !(rows - 2 < fields[6])) fields[5] = 1;
    } else if ((bits & 0x1000) != 0 || (bits & 0x100000) != 0) {
        if (fields[5] == 0 && fields[6] > 0) fields[5] = -1;
    } else if ((bits & 0x2000) != 0 || (bits & 0x800000) != 0) {
        if (fields[5] == 0 && fields[7] < columns - 1) fields[5] = 4;
    } else if ((bits & 0x8000) != 0 || (bits & 0x400000) != 0) {
        if (fields[5] == 0 && fields[7] > 0) fields[5] = 2;
    }
    return fields[5];
}
