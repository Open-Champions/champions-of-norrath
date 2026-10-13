// Observed attachmentLightPos at 0x006e1d50, .bss, no file bytes; the next symbol, decompressStack,
// is at 0x006e1d80, so at most 48 bytes are available. The authored modelSetRelightAttachment writes
// three floats there (a Point3) and modelSetupAttachment passes its address as the point argument
// of lightSetupForCharacter, so 12 bytes are established as a Point3. The other 36 bytes before
// decompressStack are not shown to belong to it. Unknown: what else is placed there.
struct Point3 {
    float x, y, z;
};
extern "C" {
Point3 attachmentLightPos;
}
