// Observed file-local object extents and actor references are recorded in
// docs/agents/w11-actor-a.md, docs/agents/w11-actor-c.md,
// docs/agents/w11-vi-render-a.md, and the relevant w8/w9 static-init sources.
// The particle records are 80 bytes. Their defining initializers are not
// reproduced here; zero storage alone does not prove the post-construction
// value or which same-named file-local object an original caller used.
extern "C" {
unsigned char particleBloodDrop[80];
unsigned char particleEctoDrop[80];
unsigned char particleSlimeDrop[80];
unsigned char particleWeaponBlood[80];
unsigned char particleWeaponEcto[80];
unsigned char particleWeaponSlime[80];
unsigned char snowParticle1[80];
unsigned char sparkParticle[80];
unsigned char trailParticleFire[80];
unsigned char trailParticleSmoke[80];

int norrath_w11_actor_c_view_4852ac;
const char* norrath_w11_actor_c_view_4852b0 = "Skeleton";
unsigned char norrath_w11_actor_c_view_7d7110[80];
unsigned char norrath_w11_actor_c_view_7d7540[80];
unsigned char norrath_w11_actor_c_view_7d7590[80];
unsigned char norrath_w11_actor_c_view_7d7630[80];
unsigned char norrath_w11_actor_c_view_7d7680[80];

// Observed extents: 60*48, 60*48, and 8*48 bytes respectively. Their
// authored initializer is GlobalInit__14VIEffectVolumeP7VIScene.
unsigned char sDustCard[60 * 48];
unsigned char sFallEffect[60 * 48];
unsigned char sFogCard[8 * 48];
}
