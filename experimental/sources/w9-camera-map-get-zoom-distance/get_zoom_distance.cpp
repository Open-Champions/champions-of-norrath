// Observed GetZoomDistance__6Camera at 0x0026fc50, 8 bytes.
// Returns the float at camera+140. Nothing is stored. Nothing here was
// executed on the EE.
typedef float __attribute__((may_alias)) AliasedFloat;

extern "C" float GetZoomDistance__6Camera(const unsigned char* camera) {
    return *reinterpret_cast<const AliasedFloat*>(camera + 140);
}
