// Observed RenderShadows__7VIScene at 0x01144ac0, 108 bytes. The count word at offset 6792 of
// the scene is re-read after every call. For each index i below it, the word at offset
// 6280 + 4 * i is passed to RenderShadow__7VIScenei(scene, word). The result is zero.
// Unknown: what the words are (queue entries) and the capacity of the table.
extern "C" {
struct VIScene;
int RenderShadow__7VIScenei(VIScene* scene, int entry);

int RenderShadows__7VIScene(VIScene* scene) {
    char* base = reinterpret_cast<char*>(scene);
    for (int i = 0; i < *reinterpret_cast<int*>(base + 6792); ++i)
        RenderShadow__7VIScenei(scene, *reinterpret_cast<int*>(base + 6280 + 4 * i));
    return 0;
}
}
