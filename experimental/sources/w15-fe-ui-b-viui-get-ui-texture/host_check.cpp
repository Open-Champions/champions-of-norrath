#include "get_ui_texture.cpp"

// UITextures is an import in the source; the host supplies a 251 * 36 byte stand-in.
extern "C" { unsigned char UITextures[251 * 36]; }

int main() {
    if (GetUITexture__F9VIUITexId(0) != UITextures) return 1;
    if (GetUITexture__F9VIUITexId(1) != UITextures + 36) return 2;
    if (GetUITexture__F9VIUITexId(250) != UITextures + 250 * 36) return 3;
    if (GetUITexture__F9VIUITexId(251) != UITextures) return 4;
    if (GetUITexture__F9VIUITexId(0xffffffffu) != UITextures) return 5;
    if (GetUITexture__F9VIUITexId(1000) != UITextures) return 6;
    return 0;
}
