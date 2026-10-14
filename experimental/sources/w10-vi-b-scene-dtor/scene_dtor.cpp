// Observed _$_7VIScene at 0x0113aae0, 324 bytes. It calls Clear__7VIScene
// with the object, then runs these destructors, each with flag 2, in order,
// on members at the given byte offsets: VISpellEffectSystem 7024,
// VIQueryTriList 6128, VIAtmosphere 1600, VIParticleSystem 1352, VIArray<VIVect3>
// 1340 and 1332, VIArray<VIPlane> 1324, VIArray<VIVect3> 1312,
// VIArray<VIClipEntry> 1300, VIMatrix44 1232 and 1168,
// VIVector<VISceneDisplayElem> 1112, VIArray<int> 1104,
// VIPool<VIStaticLighting*> 1068, VIPool<VIRefMap*> 1044,
// VIPool<VIHSpriteAnim*> 1020, VIPool<VISprite*> 996,
// VIList<VIScene::VISceneOccup...> 984 (the image's mangled name is cut at
// that point), VIList<VIActor> 972 and VIWorld at 4. If bit 0 of the second
// argument is set it finally tail-calls __builtin_delete with the object. The
// member at 10876 (VIRadialFloraSystem) is not destroyed here.
extern "C" {
struct VIScene;
struct Member;

void Clear__7VIScene(VIScene* scene);
void _$_19VISpellEffectSystem(Member* member, int flags);
void _$_14VIQueryTriList(Member* member, int flags);
void _$_12VIAtmosphere(Member* member, int flags);
void _$_16VIParticleSystem(Member* member, int flags);
void _$_t7VIArray1Z7VIVect3(Member* member, int flags);
void _$_t7VIArray1Z7VIPlane(Member* member, int flags);
void _$_t7VIArray1Z11VIClipEntry(Member* member, int flags);
void _$_10VIMatrix44(Member* member, int flags);
void _$_t8VIVector1Z18VISceneDisplayElem(Member* member, int flags);
void _$_t7VIArray1Zi(Member* member, int flags);
void _$_t6VIPool1ZP16VIStaticLighting(Member* member, int flags);
void _$_t6VIPool1ZP8VIRefMap(Member* member, int flags);
void _$_t6VIPool1ZP13VIHSpriteAnim(Member* member, int flags);
void _$_t6VIPool1ZP8VISprite(Member* member, int flags);
void _$_t6VIList1ZQ27VIScene12VISceneOccup(Member* member, int flags);
void _$_t6VIList1Z7VIActor(Member* member, int flags);
void _$_7VIWorld(Member* member, int flags);
void __builtin_delete(void* pointer);

void _$_7VIScene(VIScene* scene, int flags) {
    char* base = reinterpret_cast<char*>(scene);
    Clear__7VIScene(scene);
    _$_19VISpellEffectSystem(reinterpret_cast<Member*>(base + 7024), 2);
    _$_14VIQueryTriList(reinterpret_cast<Member*>(base + 6128), 2);
    _$_12VIAtmosphere(reinterpret_cast<Member*>(base + 1600), 2);
    _$_16VIParticleSystem(reinterpret_cast<Member*>(base + 1352), 2);
    _$_t7VIArray1Z7VIVect3(reinterpret_cast<Member*>(base + 1340), 2);
    _$_t7VIArray1Z7VIVect3(reinterpret_cast<Member*>(base + 1332), 2);
    _$_t7VIArray1Z7VIPlane(reinterpret_cast<Member*>(base + 1324), 2);
    _$_t7VIArray1Z7VIVect3(reinterpret_cast<Member*>(base + 1312), 2);
    _$_t7VIArray1Z11VIClipEntry(reinterpret_cast<Member*>(base + 1300), 2);
    _$_10VIMatrix44(reinterpret_cast<Member*>(base + 1232), 2);
    _$_10VIMatrix44(reinterpret_cast<Member*>(base + 1168), 2);
    _$_t8VIVector1Z18VISceneDisplayElem(reinterpret_cast<Member*>(base + 1112), 2);
    _$_t7VIArray1Zi(reinterpret_cast<Member*>(base + 1104), 2);
    _$_t6VIPool1ZP16VIStaticLighting(reinterpret_cast<Member*>(base + 1068), 2);
    _$_t6VIPool1ZP8VIRefMap(reinterpret_cast<Member*>(base + 1044), 2);
    _$_t6VIPool1ZP13VIHSpriteAnim(reinterpret_cast<Member*>(base + 1020), 2);
    _$_t6VIPool1ZP8VISprite(reinterpret_cast<Member*>(base + 996), 2);
    _$_t6VIList1ZQ27VIScene12VISceneOccup(reinterpret_cast<Member*>(base + 984), 2);
    _$_t6VIList1Z7VIActor(reinterpret_cast<Member*>(base + 972), 2);
    _$_7VIWorld(reinterpret_cast<Member*>(base + 4), 2);
    if (flags & 1) __builtin_delete(scene);
}
}
