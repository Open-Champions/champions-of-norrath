// Observed exitToMenuMode at 0x004842ce, 1 byte, zero in the image.
// The ELF binding is local. This definition is global because the pause-menu
// source imports the name.
extern "C" {
unsigned char exitToMenuMode = 0;
}
