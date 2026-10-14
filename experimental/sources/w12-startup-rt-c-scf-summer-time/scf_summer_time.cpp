/* Observed sceScfGetSummerTime at 0x002d1260, 100 bytes. IsT10K (0x002d1068) selects the
 * source. When it is nonzero the result is the unsigned byte at offset 6 of gConfig
 * (0x00444850, 8 bytes). Otherwise GetOsdConfigParam fills one word; when bits 13..15 of
 * it are zero the result is 0. Else GetOsdConfigParam2(buffer, 1, 1) fills a byte and the
 * result is bit 4 of that byte.
 * Inferred: the result is a daylight-saving flag (it is multiplied by 60 minutes by
 * sceScfGetLocalTimefromRTC).
 * Unknown: the meaning of the last two GetOsdConfigParam2 arguments (passed through as
 * 1 and 1), and what writes gConfig.
 */
extern "C" {
int IsT10K();
void GetOsdConfigParam(unsigned* destination);
void GetOsdConfigParam2(unsigned char* destination, int argument1, int argument2);
extern unsigned char gConfig[8];

int sceScfGetSummerTime() {
    if (IsT10K()) return gConfig[6];
    unsigned word;
    GetOsdConfigParam(&word);
    if (((word >> 13) & 7u) == 0u) return 0;
    unsigned char value[4];
    GetOsdConfigParam2(value, 1, 1);
    return (value[0] >> 4) & 1;
}
}
