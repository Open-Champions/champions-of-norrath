/* Observed sceGsSwapDBuffDc at 0x002d2538, 92 bytes. The arguments are the
 * double-buffer record that sceGsSetDefDBuffDc fills and a buffer number of
 * which only bit 0 is used. With that bit n, it calls sceGsPutDispEnv(record
 * + 40 * n) and then sceGsPutDrawEnv on the packet header at offset 448 when n
 * is 1 and at offset 80 when n is 0. Nothing is returned; the original leaves
 * the result of the last call in the result register.
 * Inferred: the 40-byte records at 0 and 40 are the two display environments
 * and the 16-byte headers at 80 and 448 start the two packed drawing packets,
 * following sceGsSetDefDBuffDc.
 */
extern "C" {
void sceGsPutDispEnv(unsigned char* env);
void sceGsPutDrawEnv(unsigned char* packet);

void sceGsSwapDBuffDc(unsigned char* db, int id) {
    const int n = id & 1;
    sceGsPutDispEnv(db + n * 40);
    if (n != 0)
        sceGsPutDrawEnv(db + 448);
    else
        sceGsPutDrawEnv(db + 80);
}
}
