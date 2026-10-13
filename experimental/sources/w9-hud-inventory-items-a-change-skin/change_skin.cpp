// Observed changeSkin__Fii at 0x0027eaf0, 396 bytes, in the pinned SLUS_205.65.
// (index, delta). Records of the targets table at 0x00437038 are 636 bytes; the
// first word of a record is a name string pointer and the signed byte at +24 is a count.
// Adjusting: delta 0 sets skinSetting[index] to 0; delta > 0 adds delta when the setting
// (signed) is below count-1; delta < 0 adds delta when the setting is above 0. The result
// is 1 only when a delta was added, else 0.
// Then, on every path, two resources are refreshed for the object targetObjs[index]
// (a pointer to a game object, a word array at 0x008d5d40): sprintf("%s_bodyv1.vif",
// name) then lumpFindResource(npcLumpName, text) stored at object+1056; sprintf(
// "%s_bodyv%d.tex", name, skin+1) then the lookup stored at object+1060.
// The text buffer is on the stack; its size is not named by the image.
// Unknown: how the two stored resource pointers are used by the object.
extern "C" {
struct SkinTarget { const char* name; unsigned char head[20]; signed char count; unsigned char tail[611]; };
extern SkinTarget targets[];
extern signed char skinSetting[];
extern unsigned char* targetObjs[];
extern const char* npcLumpName;
int sprintf(char*, const char*, ...);
void* lumpFindResource__FPcT0(const char*, char*);

int changeSkin__Fii(int index, int delta) {
    char text[96];
    int changed = 0;
    if (delta == 0) {
        skinSetting[index] = 0;
    } else if (delta > 0) {
        if (skinSetting[index] < targets[index].count - 1) {
            skinSetting[index] = static_cast<signed char>(skinSetting[index] + delta);
            changed = 1;
        }
    }
    if (delta < 0) {
        if (skinSetting[index] > 0) {
            skinSetting[index] = static_cast<signed char>(skinSetting[index] + delta);
            changed = 1;
        }
    }
    sprintf(text, "%s_bodyv1.vif", targets[index].name);
    *reinterpret_cast<void**>(targetObjs[index] + 1056) = lumpFindResource__FPcT0(npcLumpName, text);
    sprintf(text, "%s_bodyv%d.tex", targets[index].name, skinSetting[index] + 1);
    *reinterpret_cast<void**>(targetObjs[index] + 1060) = lumpFindResource__FPcT0(npcLumpName, text);
    return changed;
}
}
