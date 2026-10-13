/* Observed sceScfGetTimeZone at 0x002d11a8, 76 bytes. IsT10K (0x002d1068) selects the
 * source. When it is nonzero the result is the sign-extended halfword at offset 0 of
 * the 8-byte object gConfig (0x00444850). Otherwise GetOsdConfigParam fills one word
 * and the result is that word shifted right arithmetically by 21, except that a zero in
 * bits 13..15 of the word gives 540 instead.
 * Inferred: the value is a signed minutes offset and 540 is a default (nine hours),
 * matching the base subtracted by sceScfGetLocalTimefromRTC.
 * Unknown: what writes gConfig, and what bits 13..15 of the OSD word encode.
 */
extern "C" {
int IsT10K();
void GetOsdConfigParam(unsigned* destination);
extern unsigned char gConfig[8];

int sceScfGetTimeZone() {
    if (IsT10K()) return *reinterpret_cast<const short*>(gConfig);
    int word;
    GetOsdConfigParam(reinterpret_cast<unsigned*>(&word));
    if (((word >> 13) & 7) == 0) return 540;
    return word >> 21;
}
}
