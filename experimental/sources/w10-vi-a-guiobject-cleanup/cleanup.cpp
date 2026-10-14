// Observed: CleanUp__11VIGUIObject at 0x01031918 is 16 bytes and takes no
// argument. It stores -1 in the word at 0x0118e818 (symbol
// _11VIGUIObject$m_alphaFillMaterial) and returns.
// Inferred: that word is a static material index, -1 meaning none.
// Unknown: the code that creates the material, and the word's initial value,
// which belongs to its defining unit and is left an import here.
extern "C" {
extern int _11VIGUIObject$m_alphaFillMaterial;

void CleanUp__11VIGUIObject() {
    _11VIGUIObject$m_alphaFillMaterial = -1;
}
}
