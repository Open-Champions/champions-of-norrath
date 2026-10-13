// Observed gsm_frame_decode at 0x002eb768, 36 bytes. The twin of gsm_frame_encode: it ends with a
// jump to gsm_decode at 0x002eb7e0, with the word at object + 24 (not +20) as gsm_decode's first
// argument and its own first two arguments as the second and third. Whatever gsm_decode returns is
// returned.
// The only observed caller is lgFrameDecode, which calls it as fn(frame buffer, output position,
// object) and ignores the result. Inferred only: the word at +24 is a pointer to decoder state.
// Nothing here was run on an EE.
extern "C" {
int gsm_decode(unsigned state, void* frame, void* out);

int gsm_frame_decode(void* frame, void* out, const unsigned char* object) {
    return gsm_decode(*reinterpret_cast<const unsigned*>(object + 24), frame, out);
}
}
