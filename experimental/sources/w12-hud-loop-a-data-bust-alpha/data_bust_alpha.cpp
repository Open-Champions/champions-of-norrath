// Observed bustAlpha at 0x00484b08, 4 bytes, section .sdata/.data (local d in the image).
// A local initialised word of 4 bytes, zero at start. drawBust__Fv (0x001c6da8) loads and stores it with single-precision float instructions; drawBustOn is the byte after it.
// Inferred: A float in the range 0 to 1 used as the bust panel fade; 0.0 at start. Published here under its image name as an exported symbol so that callers resolve.
// Unknown: Whether any other code reads it (the scan found no other gp-relative reference).
extern "C" {
float bustAlpha = 0.0f;
}
