#include "cleanup.cpp"

extern "C" int _11VIGUIObject$m_alphaFillMaterial = 12;

int main() {
    CleanUp__11VIGUIObject();
    if (_11VIGUIObject$m_alphaFillMaterial != -1) return 1;
    CleanUp__11VIGUIObject();
    if (_11VIGUIObject$m_alphaFillMaterial != -1) return 2;
    return 0;
}
