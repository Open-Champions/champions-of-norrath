// Observed cutCameraPos at 0x00790f00. The next symbol (cutSceneName) starts 16 bytes
// later. The authored cut-run-frame caller treats the first 12 bytes as a Point3 and passes
// the address to worldSetViewMatricies. Whether bytes 12..15 are padding or a fourth
// component is unknown. The 16-byte extent is the symbol gap, not a measured size.
extern "C" {
unsigned char cutCameraPos[16];
}
