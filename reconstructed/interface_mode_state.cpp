// Isolated storage adapter. Original interfaceMode is a local four-byte object;
// its complete declaration and state machine have not been reconstructed.
typedef char unsigned_width[sizeof(unsigned) == 4 ? 1 : -1];
extern "C" {
unsigned norrath_interface_mode = 0u;
}
