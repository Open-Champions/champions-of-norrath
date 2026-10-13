// Observed gsm_frame_encode at 0x002eb740, 36 bytes (extent from the symbol table). A forwarding
// adapter that ends with a jump (not a call) to gsm_encode at 0x002ec2c0. Its third argument is the
// codec object: the word at object + 20 becomes gsm_encode's first argument, and this function's
// first two arguments become gsm_encode's second and third. Whatever gsm_encode returns is returned.
// The only observed use is as the per-frame callback that lgCodecGSMEncode hands to lgFrameEncode
// (called there as fn(frame buffer, output position, object)).
// Inferred only: the word at +20 is a pointer to codec state, the first argument is the frame of
// samples and the second the output position. Nothing here was run on an EE.
extern "C" {
int gsm_encode(unsigned state, void* frame, void* out);

int gsm_frame_encode(void* frame, void* out, const unsigned char* object) {
    return gsm_encode(*reinterpret_cast<const unsigned*>(object + 20), frame, out);
}
}
