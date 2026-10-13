// Observed: the file-local table iconName.1132 at 0x004271b8, 36 bytes in .data: nine
// pointers to NUL-terminated strings, in this order: "ab", "critical", "damage",
// "eletric", "flaming", "ice", "weight", "gp", "blunt". The spelling "eletric" is
// what the image holds. invInitSharedInterfaceTextures__Fv builds the nine tiny icon
// texture names "tiny%s.tex" from them.
// Naming: the image symbol carries the compiler's local suffix .1132; the authored
// consumer imports the plain name iconName, so that is the name defined here.
// Unknown: what each name stands for beyond its use as a texture-name part.
extern "C" {
const char* iconName[9] = {"ab", "critical", "damage", "eletric", "flaming", "ice", "weight", "gp", "blunt"};
}
