extern "C" {
extern const unsigned norrath_rtti_entities_a_view_480bb8;
extern const unsigned norrath_rtti_entities_a_view_480cb4;
extern const unsigned norrath_rtti_entities_a_view_480cb8;
extern const unsigned norrath_rtti_entities_a_view_480cbc;
extern const unsigned norrath_rtti_entities_a_view_480cc0;
extern const unsigned norrath_rtti_entities_a_view_4810b4;
extern const unsigned norrath_rtti_entities_a_view_4810b8;
extern const unsigned norrath_rtti_entities_a_view_481710;
extern const unsigned norrath_rtti_entities_a_view_4818ac[19];
extern const unsigned norrath_rtti_entities_a_view_482424;
extern const unsigned norrath_rtti_entities_a_view_482428;
extern const unsigned norrath_rtti_entities_a_view_48242c;
extern unsigned char particleBloodDrop[80], particleEctoDrop[80], particleSlimeDrop[80];
extern unsigned char particleWeaponBlood[80], particleWeaponEcto[80], particleWeaponSlime[80];
extern unsigned char snowParticle1[80], sparkParticle[80], trailParticleFire[80], trailParticleSmoke[80];
extern int norrath_w11_actor_c_view_4852ac;
extern const char* norrath_w11_actor_c_view_4852b0;
extern unsigned char norrath_w11_actor_c_view_7d7110[80];
extern unsigned char norrath_w11_actor_c_view_7d7540[80], norrath_w11_actor_c_view_7d7590[80];
extern unsigned char norrath_w11_actor_c_view_7d7630[80], norrath_w11_actor_c_view_7d7680[80];
extern unsigned char sDustCard[2880], sFallEffect[2880], sFogCard[384];
}

int main() {
    const unsigned expected[12] = {
        0x3e4ccccc, 0x3db851eb, 0x3f199999, 0x3f68ba2f, 0x45081000,
        0x3e4ccccc, 0x3e4ccccc, 0x3f051eb8, 0x3f999999, 0x3fb33333,
        0x3f999999, 0x3e4ccccc,
    };
    const unsigned actual[12] = {
        norrath_rtti_entities_a_view_480bb8, norrath_rtti_entities_a_view_480cb4,
        norrath_rtti_entities_a_view_480cb8, norrath_rtti_entities_a_view_480cbc,
        norrath_rtti_entities_a_view_480cc0, norrath_rtti_entities_a_view_4810b4,
        norrath_rtti_entities_a_view_4810b8, norrath_rtti_entities_a_view_481710,
        norrath_rtti_entities_a_view_482424, norrath_rtti_entities_a_view_482428,
        norrath_rtti_entities_a_view_48242c, norrath_rtti_entities_a_view_4818ac[8],
    };
    for (int i = 0; i < 12; ++i) if (actual[i] != expected[i]) return i + 1;
    const unsigned table[19] = {
        0x3f333333u, 0x3f4cccccu, 0x3f333333u, 0x3fb33333u, 0x3fccccccu,
        0x3fccccccu, 0x3f333333u, 0x3fb33333u, 0x3e4cccccu, 0x3f4cccccu,
        0x3f333333u, 0x3f4cccccu, 0x3f333333u, 0x3fccccccu, 0x3e4cccccu,
        0x3e4cccccu, 0x3e4cccccu, 0x3e4cccccu, 0x3e4cccccu,
    };
    for (int i = 0; i < 19; ++i) if (norrath_rtti_entities_a_view_4818ac[i] != table[i]) return 20 + i;
    if (norrath_w11_actor_c_view_4852ac != 0 || !norrath_w11_actor_c_view_4852b0) return 40;
    const char* skeleton = "Skeleton";
    for (int i = 0; i != 9; ++i) if (norrath_w11_actor_c_view_4852b0[i] != skeleton[i]) return 41;
    unsigned char* const records[] = {
        particleBloodDrop, particleEctoDrop, particleSlimeDrop,
        particleWeaponBlood, particleWeaponEcto, particleWeaponSlime,
        snowParticle1, sparkParticle, trailParticleFire, trailParticleSmoke,
        norrath_w11_actor_c_view_7d7110, norrath_w11_actor_c_view_7d7540,
        norrath_w11_actor_c_view_7d7590, norrath_w11_actor_c_view_7d7630,
        norrath_w11_actor_c_view_7d7680,
    };
    for (unsigned i = 0; i < sizeof(records) / sizeof(records[0]); ++i)
        if (records[i][79] != 0) return 50;
    if (sDustCard[2879] || sFallEffect[2879] || sFogCard[383]) return 51;
    return 0;
}
