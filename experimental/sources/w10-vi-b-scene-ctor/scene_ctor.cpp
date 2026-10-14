// Observed __7VIScene at 0x0113a890, 588 bytes. Offsets are bytes from the
// object. The constructor takes only the object and returns nothing. In order
// it runs the constructors of the VIWorld at 4; VIList<VIActor> 972;
// VIList<VIScene::VISceneOccup...> 984 (the image's mangled name is cut
// there); VIPool<VISprite*> 996; VIPool<VIHSpriteAnim*> 1020;
// VIPool<VIRefMap*> 1044; VIPool<VIStaticLighting*> 1068; VIArray<int> 1104;
// VIVector<VISceneDisplayElem> 1112. It then writes float zero to the 64 bytes
// at 1168 and the 64 bytes at 1232 (each written twice in the original, the
// second time with 16-byte vector stores), and runs the constructors of
// VIArray<VIClipEntry> 1300; VIArray<VIVect3> 1312, 1332 and 1340;
// VIArray<VIPlane> 1324; VIParticleSystem 1352; VIAtmosphere 1600;
// VIQueryTriList 6128; VISpellEffectSystem 7024. Then it writes float zero to
// the 16 bytes at 8164, to 24 elements of 108 bytes from 8180 (all 108 bytes
// of each), to 16 bytes at 10792 and 12 bytes at 10776, runs the
// VIRadialFloraSystem constructor at 10876, stores 1 at 1348, zero at 19356,
// and calls Clear__7VIScene(object). What the zeroed elements are is not
// established here.
extern "C" {
struct VIScene;
struct Member;

void __7VIWorld(Member* member);
void __t6VIList1Z7VIActor(Member* member);
void __t6VIList1ZQ27VIScene12VISceneOccup(Member* member);
void __t6VIPool1ZP8VISprite(Member* member);
void __t6VIPool1ZP13VIHSpriteAnim(Member* member);
void __t6VIPool1ZP8VIRefMap(Member* member);
void __t6VIPool1ZP16VIStaticLighting(Member* member);
void __t7VIArray1Zi(Member* member);
void __t8VIVector1Z18VISceneDisplayElem(Member* member);
void __t7VIArray1Z11VIClipEntry(Member* member);
void __t7VIArray1Z7VIVect3(Member* member);
void __t7VIArray1Z7VIPlane(Member* member);
void __16VIParticleSystem(Member* member);
void __12VIAtmosphere(Member* member);
void __14VIQueryTriList(Member* member);
void __19VISpellEffectSystem(Member* member);
void __19VIRadialFloraSystem(Member* member);
void Clear__7VIScene(VIScene* scene);

static void zero_floats(char* at, int count) {
    float* words = reinterpret_cast<float*>(at);
    for (int i = 0; i < count; ++i) words[i] = 0.0f;
}

void __7VIScene(VIScene* scene) {
    char* base = reinterpret_cast<char*>(scene);
    __7VIWorld(reinterpret_cast<Member*>(base + 4));
    __t6VIList1Z7VIActor(reinterpret_cast<Member*>(base + 972));
    __t6VIList1ZQ27VIScene12VISceneOccup(reinterpret_cast<Member*>(base + 984));
    __t6VIPool1ZP8VISprite(reinterpret_cast<Member*>(base + 996));
    __t6VIPool1ZP13VIHSpriteAnim(reinterpret_cast<Member*>(base + 1020));
    __t6VIPool1ZP8VIRefMap(reinterpret_cast<Member*>(base + 1044));
    __t6VIPool1ZP16VIStaticLighting(reinterpret_cast<Member*>(base + 1068));
    __t7VIArray1Zi(reinterpret_cast<Member*>(base + 1104));
    __t8VIVector1Z18VISceneDisplayElem(reinterpret_cast<Member*>(base + 1112));
    zero_floats(base + 1168, 16);
    zero_floats(base + 1232, 16);
    __t7VIArray1Z11VIClipEntry(reinterpret_cast<Member*>(base + 1300));
    __t7VIArray1Z7VIVect3(reinterpret_cast<Member*>(base + 1312));
    __t7VIArray1Z7VIPlane(reinterpret_cast<Member*>(base + 1324));
    __t7VIArray1Z7VIVect3(reinterpret_cast<Member*>(base + 1332));
    __t7VIArray1Z7VIVect3(reinterpret_cast<Member*>(base + 1340));
    __16VIParticleSystem(reinterpret_cast<Member*>(base + 1352));
    __12VIAtmosphere(reinterpret_cast<Member*>(base + 1600));
    __14VIQueryTriList(reinterpret_cast<Member*>(base + 6128));
    __19VISpellEffectSystem(reinterpret_cast<Member*>(base + 7024));
    zero_floats(base + 8164, 4);
    for (int i = 0; i < 24; ++i) zero_floats(base + 8180 + 108 * i, 27);
    zero_floats(base + 10792, 4);
    zero_floats(base + 10776, 3);
    __19VIRadialFloraSystem(reinterpret_cast<Member*>(base + 10876));
    *reinterpret_cast<int*>(base + 1348) = 1;
    *reinterpret_cast<int*>(base + 19356) = 0;
    Clear__7VIScene(scene);
}
}
