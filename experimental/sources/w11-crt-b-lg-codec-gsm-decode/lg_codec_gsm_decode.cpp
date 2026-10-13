// Observed lgCodecGSMDecode at 0x002eb6e8, 84 bytes. A thin adapter over lgFrameDecode. Its first
// argument is a codec object; the other four arguments (the 2nd to 5th) are passed through
// unchanged as the first four arguments of lgFrameDecode. The remaining seven arguments are
//   object + 352, 33, object + 388, 160, 33, the address of gsm_frame_decode, object.
// The first argument's type and the meaning of the four forwarded words are not established
// from this function. Nothing was executed on the EE.
// Inferred only: the names suggest a GSM 06.10 frame codec; the image carries no field names.
extern "C" {
void gsm_frame_decode();
int lgFrameDecode(int a, int b, int c, int d, void* e, int f, void* g, int h, int i, void (*fn)(), void* object);

int lgCodecGSMDecode(unsigned char* object, int a, int b, int c, int d) {
    return lgFrameDecode(a, b, c, d, object + 352, 33, object + 388, 160, 33, gsm_frame_decode, object);
}
}
