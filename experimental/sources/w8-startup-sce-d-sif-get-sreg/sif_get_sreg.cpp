/* Observed sceSifGetSreg at 0x002d7160, 24 bytes. Returns word `index` of the
 * local array soft_reg (0x008debc0). No bound check. The array length is not
 * established by this function.
 */
extern "C" unsigned soft_reg[];

extern "C" unsigned sceSifGetSreg(int index) {
    return soft_reg[index];
}
