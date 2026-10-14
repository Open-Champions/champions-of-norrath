/* Observed CheckAddress at 0x002d2c48, 40 bytes. When the top nibble is 7,
 * the low 28 bits are kept and bit 31 is set. Every other address is
 * returned unchanged.
 */
extern "C" unsigned CheckAddress(unsigned address) {
    if ((address >> 28) == 7u)
        address = (address & 0x0fffffffu) | 0x80000000u;
    return address;
}
