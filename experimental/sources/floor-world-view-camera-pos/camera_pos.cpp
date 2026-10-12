// Observed worldCameraPos at 0x0037ee80, 12 bytes in .data. The file bytes
// are zero. worldViewCenter begins at the next address. floorDraw reads the
// third word.
extern "C" {
unsigned char worldCameraPos[12];
}
