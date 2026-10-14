// Observed lightGetAmbient__FPs at 0x0013e6c0, 44 bytes, no frame. It widens four consecutive bytes of the global
// scene record (0x004f4060, offsets 48 to 51) to four halfwords written to the caller's buffer, in order, and
// returns nothing. The bytes are read as unsigned.
// Inferred: the four bytes are the ambient light channels (the name says ambient); the fourth is probably alpha.
// Unknown: the layout of `scene` beyond these four bytes and whether it is one object or a view of a larger record.
extern "C" {
extern unsigned char scene[];

void lightGetAmbient__FPs(short* out) {
    out[0] = scene[48];
    out[1] = scene[49];
    out[2] = scene[50];
    out[3] = scene[51];
}
}
