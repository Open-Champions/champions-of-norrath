// Observed initFile__FPcPvi at 0x1641a0 (a local symbol), extent 408. Called by the
// lump initializer for each row of a lump. Arguments: the row name, the row data and
// the row size. The routine picks an initializer by the text after the last '.' of
// the name (strrchr), compared in this order with strcmp:
//   "anm"   animInitAnimation(data); returns 0
//   "vif"   modelInitModel(data); returns 0
//   "tex"   textureInitTexture(data); returns 0
//   "fnt"   fontInitFont(data); returns 0
//   "world" r = worldInitWorld(data); returns size - r when r is nonzero, else 0
//   "vag"   soundAddSound(name, data, noNewVags); returns size - 64
//   "uni"   textAddText(data); returns 0
//   "etex"  compared, but nothing is called; returns 0
// A name with no '.' returns 0 without any comparison; an unknown extension returns 0.
// The third argument of soundAddSound is the whole word at noNewVags (0x483f90), not
// a normalized boolean.
// Inferred: the result is a byte count the caller subtracts from its running shift
// (a size minus what the initializer says it still needs).
// Unknown: what worldInitWorld's result means and what the "etex" files need.
extern "C" {
char* strrchr(const char* text, int character);
int strcmp(const char* left, const char* right);
void animInitAnimation__FP15AnimationHeader(void* data);
void modelInitModel__FPv(void* data);
void textureInitTexture__FP8_texture(void* data);
void fontInitFont__FP4Font(void* data);
int worldInitWorld__FP12_worldHeader(void* data);
void soundAddSound__FPcP10_vagHeaderb(char* name, void* data, int no_new_vags);
void textAddText__FPv(void* data);
extern int noNewVags;
}

extern "C" int initFile__FPcPvi(char* name, void* data, int size) {
    int result = 0;
    const char* dot = strrchr(name, '.');
    if (dot == nullptr) return 0;
    const char* extension = dot + 1;
    if (strcmp(extension, "anm") == 0) {
        animInitAnimation__FP15AnimationHeader(data);
        return 0;
    }
    if (strcmp(extension, "vif") == 0) {
        modelInitModel__FPv(data);
        return 0;
    }
    if (strcmp(extension, "tex") == 0) {
        textureInitTexture__FP8_texture(data);
        return 0;
    }
    if (strcmp(extension, "fnt") == 0) {
        fontInitFont__FP4Font(data);
        return 0;
    }
    if (strcmp(extension, "world") == 0) {
        const int needed = worldInitWorld__FP12_worldHeader(data);
        if (needed != 0) result = size - needed;
        return result;
    }
    if (strcmp(extension, "vag") == 0) {
        soundAddSound__FPcP10_vagHeaderb(name, data, noNewVags);
        return size - 64;
    }
    if (strcmp(extension, "uni") == 0) {
        textAddText__FPv(data);
        return 0;
    }
    strcmp(extension, "etex");
    return result;
}
