// Observed getVifAndTex__FPcT0PP12_modelHeaderPP8_texture at 0x002809c0, extent 168.
// (name1, name2, modelOut, textureOut) -> int. Two 128-byte stack texts (the image
// does not state a size) are built with sprintf from the argument pair: "<name1>_<name2>.vif"
// and "<name1>_<name2>.tex". Each is looked up with lumpFindResource__FPcT0 in the lump
// named by npcLumpName (read for each call); the first result is stored to
// *modelOut, the second to *textureOut. Returns 1 when *modelOut is nonzero,
// else 0; the texture result does not affect the return value.
// Inferred: the stored pointers are the model header and texture of one named part.
extern "C" {
extern char* npcLumpName;
int sprintf(char*, const char*, ...);
void* lumpFindResource__FPcT0(char*, char*);

int getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(char* name1, char* name2, void** modelOut, void** textureOut) {
    char vifName[128];
    char texName[128];
    sprintf(vifName, "%s_%s.vif", name1, name2);
    sprintf(texName, "%s_%s.tex", name1, name2);
    *modelOut = lumpFindResource__FPcT0(npcLumpName, vifName);
    *textureOut = lumpFindResource__FPcT0(npcLumpName, texName);
    return *modelOut != 0;
}
}
