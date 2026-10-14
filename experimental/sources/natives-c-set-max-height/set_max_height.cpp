// Observed SetMaxHeight__6Camera at 0x0026fb78, 8 bytes. The float
// argument is stored at camera byte 164 and the function returns.
extern "C" void SetMaxHeight__6Cameraf(void* camera, float height) {
    auto* bytes = static_cast<unsigned char*>(camera);
    auto* source = reinterpret_cast<const unsigned char*>(&height);
    bytes[164] = source[0];
    bytes[165] = source[1];
    bytes[166] = source[2];
    bytes[167] = source[3];
}
