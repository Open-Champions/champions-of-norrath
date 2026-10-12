# Spell, skill and actor static initializers (1/2)

Package `w8-spell-static-init-a`, branch `agents/w8-spell-static-init-a`. New source is under
`experimental/sources/w8-spell-static-init-a-*`; one directory per initializer plus
`w8-spell-static-init-a-common` (shared header, host recorder). Host checks sit beside the
sources and stay out of the EE plan.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that
file's symbol values. No game bytes or listings are committed. Nothing here ran on the EE.

## What the 31 symbols are

Observed: each `_GLOBAL_$I$...` symbol is 32 bytes and has the same shape. It calls the
unit-local `__static_initialization_and_destruction_0` (a local text symbol, one per original
translation unit) with arguments (1, 0xffff) and returns. Observed: that local routine tests
that pair and then makes a straight run of calls to `__11ParticleDefiiiiiiiifffiiiPFPv_v`, one
per static particle definition record of the unit, and makes no other call. The first record
argument is the record address; then come eight integers (the eighth on the stack), three
single floats, three more integers and a null callback (stack, EABI argument order). Records
are 80 bytes apart in data and the observed addresses are 16-byte aligned.

Inferred: the constructor-call name suggests a particle definition class; the record layout is
unknown here, so records are opaque 80-byte, 16-aligned storage. Inferred from the branch
structure (no other calls): the (0, 0xffff) destruction path does nothing, because these
records have no destructor work. The host contract asserts that paths other than (1, 0xffff)
construct nothing.

Observed: the records are bss for the unit-local ones (`b`) and initialized-data symbols
(`D`, zero-filled in the file) for six in the Switch unit: `fireTrailParticle`,
`trackfireTrailParticle`, `saveParticle`, `saveParticle2`, `teleportParticle1`,
`teleportParticle2`. Those six are authored as global zero-initialized objects (they will land in
bss, not data); every other record is file-local, matching the original binding. Record names are
the original symbol names, taken from the symbol table. Four names repeat across units
(`sparkParticle`, `convertP`, `magic`, `miss`, also `heatParticle`) and are separate
file-local objects.

## Functions

Every row: wrapper size 32 in the original. The compiled body includes the inlined local routine,
so it is larger than the wrapper alone. Source column paths are relative to
`experimental/sources/`.

| source | symbol | wrapper address / bytes | local routine address / bytes | records | compiled bytes | sha256 |
|---|---|---|---|---|---|---|
| `w8-spell-static-init-a-holy-strikes/holy_strikes_static_init.cpp` | `_GLOBAL_$I$__10HolyStrikesiG6Point3P10GameObjectP5Skill` | `0x2b2e50` / 32 | `0x2b2b90` / 328 | 3 | 308 | `a00b4e2d22fa70ac7a4fdd07874ca819726600eed380ff6fefd60b949b0a0c75` |
| `w8-spell-static-init-a-poison-rain/poison_rain_static_init.cpp` | `_GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader` | `0x2bf270` / 32 | `0x2bf0b0` / 232 | 2 | 204 | `cac021a7fcebbe614c2803905229894d5dbb94f9d241aab5c098db5b7f830fc4` |
| `w8-spell-static-init-a-static-fire/static_fire_static_init.cpp` | `_GLOBAL_$I$__10StaticFireG6Point3fif` | `0x28f5e8` / 32 | `0x28f348` / 328 | 3 | 296 | `33d4fef7b29647ab5fc0ba389b21e8f41682a34effe2268e7b84e329197dae19` |
| `w8-spell-static-init-a-disease-bolts/disease_bolts_static_init.cpp` | `_GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill` | `0x2938a8` / 32 | `0x2934d0` / 344 | 3 | 308 | `eb53ee8d36903f5d1038a635944c78ceaf9b9b54cd8c79f95ea3346f8f86ba77` |
| `w8-spell-static-init-a-spell-blessed/spell_blessed_static_init.cpp` | `_GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill` | `0x2a7048` / 32 | `0x2a6c30` / 240 | 2 | 196 | `3d3c6e4a3adab1ef701321bed6f276971407dbbdea9cf1e6b00934a3c245486c` |
| `w8-spell-static-init-a-spell-cyclone/spell_cyclone_static_init.cpp` | `_GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill` | `0x29a9e0` / 32 | `0x29a618` / 248 | 2 | 220 | `8fb36f53d5aed3c35de3d1d87239f6b1ea9ebd3c99c233baec1a0e96643bc263` |
| `w8-spell-static-init-a-hammer-of-wrath/hammer_of_wrath_static_init.cpp` | `_GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill` | `0x2b5878` / 32 | `0x2b54d0` / 320 | 3 | 308 | `ecd545a2cf523384b1420be0a8febf6fee35fdad9f36ae8ae78cb204b36ebeab` |
| `w8-spell-static-init-a-spell-cone-of-fire/spell_cone_of_fire_static_init.cpp` | `_GLOBAL_$I$__15SpellConeOfFireiP8CreatureP5Skill` | `0x2b6b38` / 32 | `0x2b6898` / 328 | 3 | 296 | `419218ee7c45ed3d92199e492b52896e1601a30ad304557c38e0a84ac9bcd393` |
| `w8-spell-static-init-a-spell-wizard-beam/spell_wizard_beam_static_init.cpp` | `_GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill` | `0x2b94b0` / 32 | `0x2b9220` / 328 | 3 | 296 | `ee22a038f465e8a70b94f518a9523be829327992358788fabbb54e0f37a319c7` |
| `w8-spell-static-init-a-spell-cold-weapons/spell_cold_weapons_static_init.cpp` | `_GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill` | `0x2ac4d0` / 32 | `0x2ac240` / 240 | 2 | 212 | `d18a58e8e8ed9c92976fc2edd01ec9a42fd9c8583cc08a078e5a13aab93cc3c7` |
| `w8-spell-static-init-a-spell-cone-of-frost/spell_cone_of_frost_static_init.cpp` | `_GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill` | `0x2aff88` / 32 | `0x2afb58` / 328 | 3 | 300 | `619f6fefeea4e43d952e350e7a90c01473cb1b80c4ddbdd90f19c736cedf8da7` |
| `w8-spell-static-init-a-spell-fire-weapons/spell_fire_weapons_static_init.cpp` | `_GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill` | `0x2ab528` / 32 | `0x2ab1b8` / 328 | 3 | 296 | `921ea27cb96ae3a3b446638dee00342a81b23c902f36cd86ffaedcbbeec694b3` |
| `w8-spell-static-init-a-spell-convert-enemy/spell_convert_enemy_static_init.cpp` | `_GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill` | `0x298ce0` / 32 | `0x2989f0` / 136 | 1 | 120 | `29c6017a2a610ea6e931753fc33bc7e6ff24a6caa462f4b74a1c06a5756c1e76` |
| `w8-spell-static-init-a-spell-undead-shield/spell_undead_shield_static_init.cpp` | `_GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill` | `0x2b1440` / 32 | `0x2b0fc8` / 240 | 2 | 200 | `7c74304a724c5571da01fbee21b92998f8434528f967e579c7bc6215eb5180ff` |
| `w8-spell-static-init-a-spell-disease-shield/spell_disease_shield_static_init.cpp` | `_GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill` | `0x296910` / 32 | `0x296668` / 248 | 2 | 204 | `c286476215868347e13032911ad9f5d61d273b7640017627ff8289a4b3d95bcb` |
| `w8-spell-static-init-a-blinding-light-effect/blinding_light_effect_static_init.cpp` | `_GLOBAL_$I$__19BlindingLightEffectG6Point3iP8CreatureP5Skill` | `0x2ba1b8` / 32 | `0x2b9ee0` / 328 | 3 | 308 | `cb37aed24787323bab9602088a84c125516d8611441b56ff833b0218babbca27` |
| `w8-spell-static-init-a-cold-arrow-projectile/cold_arrow_projectile_static_init.cpp` | `_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill` | `0x29fe00` / 32 | `0x29f9f8` / 328 | 3 | 308 | `353241d056f28463492182eaea22f484a6924e179e450fa041f735b46965f8bd` |
| `w8-spell-static-init-a-spell-planar-steel-effect/spell_planar_steel_effect_static_init.cpp` | `_GLOBAL_$I$__22SpellPlanarSteelEffectiP8CreatureP5Skill` | `0x2c9a18` / 32 | `0x2c97e0` / 424 | 4 | 384 | `20b1d26a49585b21ed41bbebc84d0c0b2972b897f6b039eb6e620fc929816205` |
| `w8-spell-static-init-a-npc/npc_static_init.cpp` | `_GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc` | `0x233280` / 32 | `0x233110` / 136 | 1 | 116 | `5e0acb871498e7fc6e44c4ddf935297bee32acdc4f5a8ec9ab6dcc1194a0d512` |
| `w8-spell-static-init-a-torch/torch_static_init.cpp` | `_GLOBAL_$I$__5TorchG6Point3PPci` | `0x2313a8` / 32 | `0x230d48` / 704 | 7 | 676 | `3fbd2c56903b07610e9350eff796f30cd18257d5274580b16b6f88cf39b539e0` |
| `w8-spell-static-init-a-switch/switch_static_init.cpp` | `_GLOBAL_$I$__6SwitchG6Point3PPc` | `0x21def8` / 32 | `0x21b430` / 2192 | 23 | 2044 | `79c696496177ebf2c456309fb01610d70cff51f29050c307813f094fac9056ba` |
| `w8-spell-static-init-a-fire-bomb/fire_bomb_static_init.cpp` | `_GLOBAL_$I$__8FireBombG6Point3sPPc` | `0x28b838` / 32 | `0x28b718` / 136 | 1 | 116 | `2be45cbeba3606966843cba97cefef639a0ccfa9820763785067f23997cbfb2e` |
| `w8-spell-static-init-a-holy-bolt/holy_bolt_static_init.cpp` | `_GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill` | `0x2bbd58` / 32 | `0x2bba08` / 528 | 5 | 484 | `0df8d3be9fef966802e5a5e51283c3d5e8df82ae0563b05f062dd19621969271` |
| `w8-spell-static-init-a-jump-gate/jump_gate_static_init.cpp` | `_GLOBAL_$I$__8JumpGateG6Point3sPPc` | `0x2848f8` / 32 | `0x2842f8` / 344 | 3 | 324 | `e3e8b37a9d757e6347e7cdb7032a9ff022c2afe9f6711f5e61e371dc1bedb69b` |
| `w8-spell-static-init-a-spell-harm-touch/spell_harm_touch_static_init.cpp` | `_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2` | `0x29b218` / 32 | `0x29adb0` / 248 | 2 | 220 | `55f52ada8df5edba32a9a99555a7b4c3349547cb8fbe736009f2473910ec7ee2` |
| `w8-spell-static-init-a-skill-convert-undead/skill_convert_undead_static_init.cpp` | `_GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature` | `0x299738` / 32 | `0x299448` / 136 | 1 | 120 | `8252600040a13c4c6364da54e3aa729b0473b8eb890c338615649e8cea7b8f9c` |
| `w8-spell-static-init-a-skill-darkness/skill_darkness_static_init.cpp` | `_GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s` | `0x298340` / 32 | `0x2980d0` / 128 | 1 | 112 | `55f9a8dd7dd9c625d6ee06cda723f77f59da3e06a773e64ca5b6bc6a310be281` |
| `w8-spell-static-init-a-skill-entangle/skill_entangle_static_init.cpp` | `_GLOBAL_$I$cast__13SkillEntangleiP8CreatureG6Point3s` | `0x2c70b8` / 32 | `0x2c6c80` / 336 | 3 | 328 | `d1a7c6a8c8e401e204a558b4ea94ab29c49fe167cbf5dd870b0869cb573a85b4` |
| `w8-spell-static-init-a-skill-multi-fire/skill_multi_fire_static_init.cpp` | `_GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s` | `0x2a1e58` / 32 | `0x2a1b30` / 128 | 1 | 116 | `70f5ee509d3e9f8c8292b4b5c2be8c55bb4064b06f3efd754362bc880ffb8aeb` |
| `w8-spell-static-init-a-skill-cleric-root/skill_cleric_root_static_init.cpp` | `_GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s` | `0x2c6440` / 32 | `0x2c6008` / 336 | 3 | 328 | `9b06a1cc8d2beff6024ef7c1b91c7114b61c2ef1b9a062bee710622e06d534a8` |
| `w8-spell-static-init-a-skill-repulse-undead/skill_repulse_undead_static_init.cpp` | `_GLOBAL_$I$cast__18SkillRepulseUndeadiP8CreatureG6Point3s` | `0x2ba768` / 32 | `0x2ba4b8` / 328 | 3 | 308 | `de2945eb4a36fe66858673379585b9637a23a531557d0bbb8b01762d473a8368` |

`w8-spell-static-init-a-common/particle_def.hpp` sha256
`186ebf892409fd56aa2d4f942d45ffa80c08177d7c6b4353934bc5408bc4bff8`.

## Verification

- Each unit compiled with the reviewed EE g++ and the `authored_cpp` flags (one unit at a
  time, no partial build). The object defines the wrapper and leaves only the particle
  constructor undefined.
- Each `host_check.cpp` includes its source, stubs the constructor, calls the wrapper twice,
  and fails if the construction count, order, record identity/alignment, any of the 15
  arguments (floats compared by bit pattern) or the null callback differs from the expected
  rows, or if a non-(1, 0xffff) call to the local routine constructs anything. Compile with
  `clang++ -std=c++17 -O2`; do not add `host_check.cpp` to the EE plan.
- `python3 docs/agents/w8-spell-static-init-a-check.py --original <SLUS_205.65> --toolchain
  <ee>` rebuilds and runs every host check and independently replays the original wrapper and
  local routine (constants, stack stores, gp-relative float loads, record addresses). It
  compares each row with the host output and the record symbol names/bindings with the source.
  All 31 pass. A deliberate one-integer change in a source made it fail. It prints only
  pass/fail lines.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).

## Imports closed

`_GLOBAL_$I$__10HolyStrikesiG6Point3P10GameObjectP5Skill`
`_GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`
`_GLOBAL_$I$__10StaticFireG6Point3fif`
`_GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill`
`_GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill`
`_GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill`
`_GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill`
`_GLOBAL_$I$__15SpellConeOfFireiP8CreatureP5Skill`
`_GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill`
`_GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill`
`_GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill`
`_GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill`
`_GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill`
`_GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill`
`_GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill`
`_GLOBAL_$I$__19BlindingLightEffectG6Point3iP8CreatureP5Skill`
`_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`
`_GLOBAL_$I$__22SpellPlanarSteelEffectiP8CreatureP5Skill`
`_GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc`
`_GLOBAL_$I$__5TorchG6Point3PPci`
`_GLOBAL_$I$__6SwitchG6Point3PPc`
`_GLOBAL_$I$__8FireBombG6Point3sPPc`
`_GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill`
`_GLOBAL_$I$__8JumpGateG6Point3sPPc`
`_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2`
`_GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature`
`_GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s`
`_GLOBAL_$I$cast__13SkillEntangleiP8CreatureG6Point3s`
`_GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s`
`_GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s`
`_GLOBAL_$I$cast__18SkillRepulseUndeadiP8CreatureG6Point3s`

(31 names. The list in the task statement is the same set.) Closed by symbol definition; a full
partial build was not run for this package, to keep the machine free.

## Imports opened

None new. `__11ParticleDefiiiiiiiifffiiiPFPv_v` is referenced by all 31 and was already on the
unresolved list (the particle package owns it; it is declared in the shared header with an
opaque first argument).

## Build-plan entries needed

Profile `authored_cpp`; header closure is the shared header. Do not add `host_check.cpp`,
`host_record.hpp` or the check script. The shared files (`build-plan.json`, `snapshot.json`)
were not edited.

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "holy_strikes_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-holy-strikes/holy_strikes_static_init.cpp",
  "sha256": "a00b4e2d22fa70ac7a4fdd07874ca819726600eed380ff6fefd60b949b0a0c75",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__10HolyStrikesiG6Point3P10GameObjectP5Skill",
      "compiled_body_bytes": 308
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "poison_rain_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-poison-rain/poison_rain_static_init.cpp",
  "sha256": "cac021a7fcebbe614c2803905229894d5dbb94f9d241aab5c098db5b7f830fc4",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader",
      "compiled_body_bytes": 204
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "static_fire_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-static-fire/static_fire_static_init.cpp",
  "sha256": "33d4fef7b29647ab5fc0ba389b21e8f41682a34effe2268e7b84e329197dae19",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__10StaticFireG6Point3fif",
      "compiled_body_bytes": 296
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "disease_bolts_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-disease-bolts/disease_bolts_static_init.cpp",
  "sha256": "eb53ee8d36903f5d1038a635944c78ceaf9b9b54cd8c79f95ea3346f8f86ba77",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__11DiseaseBoltsiG6Point3P10GameObjectP5Skill",
      "compiled_body_bytes": 308
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_blessed_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-blessed/spell_blessed_static_init.cpp",
  "sha256": "3d3c6e4a3adab1ef701321bed6f276971407dbbdea9cf1e6b00934a3c245486c",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__12SpellBlessediP8CreatureP5Skill",
      "compiled_body_bytes": 196
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_cyclone_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-cyclone/spell_cyclone_static_init.cpp",
  "sha256": "8fb36f53d5aed3c35de3d1d87239f6b1ea9ebd3c99c233baec1a0e96643bc263",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__12SpellCycloneiP8CreatureP5Skill",
      "compiled_body_bytes": 220
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "hammer_of_wrath_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-hammer-of-wrath/hammer_of_wrath_static_init.cpp",
  "sha256": "ecd545a2cf523384b1420be0a8febf6fee35fdad9f36ae8ae78cb204b36ebeab",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__13HammerOfWrathG6Point3sP10GameObjectiP5Skill",
      "compiled_body_bytes": 308
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_cone_of_fire_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-cone-of-fire/spell_cone_of_fire_static_init.cpp",
  "sha256": "419218ee7c45ed3d92199e492b52896e1601a30ad304557c38e0a84ac9bcd393",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__15SpellConeOfFireiP8CreatureP5Skill",
      "compiled_body_bytes": 296
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_wizard_beam_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-wizard-beam/spell_wizard_beam_static_init.cpp",
  "sha256": "ee22a038f465e8a70b94f518a9523be829327992358788fabbb54e0f37a319c7",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__15SpellWizardBeamiP8CreatureP5Skill",
      "compiled_body_bytes": 296
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_cold_weapons_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-cold-weapons/spell_cold_weapons_static_init.cpp",
  "sha256": "d18a58e8e8ed9c92976fc2edd01ec9a42fd9c8583cc08a078e5a13aab93cc3c7",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__16SpellColdWeaponsiP8CreatureP5Skill",
      "compiled_body_bytes": 212
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_cone_of_frost_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-cone-of-frost/spell_cone_of_frost_static_init.cpp",
  "sha256": "619f6fefeea4e43d952e350e7a90c01473cb1b80c4ddbdd90f19c736cedf8da7",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__16SpellConeOfFrostiP8CreatureP5Skill",
      "compiled_body_bytes": 300
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_fire_weapons_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-fire-weapons/spell_fire_weapons_static_init.cpp",
  "sha256": "921ea27cb96ae3a3b446638dee00342a81b23c902f36cd86ffaedcbbeec694b3",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__16SpellFireWeaponsiP8CreatureP5Skill",
      "compiled_body_bytes": 296
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_convert_enemy_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-convert-enemy/spell_convert_enemy_static_init.cpp",
  "sha256": "29c6017a2a610ea6e931753fc33bc7e6ff24a6caa462f4b74a1c06a5756c1e76",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__17SpellConvertEnemyiP8CreatureP5Skill",
      "compiled_body_bytes": 120
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_undead_shield_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-undead-shield/spell_undead_shield_static_init.cpp",
  "sha256": "7c74304a724c5571da01fbee21b92998f8434528f967e579c7bc6215eb5180ff",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__17SpellUndeadShieldiP8CreatureP5Skill",
      "compiled_body_bytes": 200
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_disease_shield_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-disease-shield/spell_disease_shield_static_init.cpp",
  "sha256": "c286476215868347e13032911ad9f5d61d273b7640017627ff8289a4b3d95bcb",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__18SpellDiseaseShieldiP8CreatureP5Skill",
      "compiled_body_bytes": 204
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "blinding_light_effect_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-blinding-light-effect/blinding_light_effect_static_init.cpp",
  "sha256": "cb37aed24787323bab9602088a84c125516d8611441b56ff833b0218babbca27",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__19BlindingLightEffectG6Point3iP8CreatureP5Skill",
      "compiled_body_bytes": 308
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "cold_arrow_projectile_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-cold-arrow-projectile/cold_arrow_projectile_static_init.cpp",
  "sha256": "353241d056f28463492182eaea22f484a6924e179e450fa041f735b46965f8bd",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill",
      "compiled_body_bytes": 308
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_planar_steel_effect_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-planar-steel-effect/spell_planar_steel_effect_static_init.cpp",
  "sha256": "20b1d26a49585b21ed41bbebc84d0c0b2972b897f6b039eb6e620fc929816205",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__22SpellPlanarSteelEffectiP8CreatureP5Skill",
      "compiled_body_bytes": 384
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "npc_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-npc/npc_static_init.cpp",
  "sha256": "5e0acb871498e7fc6e44c4ddf935297bee32acdc4f5a8ec9ab6dcc1194a0d512",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__3NPCG6Point3sPcN33ifPPc",
      "compiled_body_bytes": 116
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "torch_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-torch/torch_static_init.cpp",
  "sha256": "3fbd2c56903b07610e9350eff796f30cd18257d5274580b16b6f88cf39b539e0",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__5TorchG6Point3PPci",
      "compiled_body_bytes": 676
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "switch_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-switch/switch_static_init.cpp",
  "sha256": "79c696496177ebf2c456309fb01610d70cff51f29050c307813f094fac9056ba",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__6SwitchG6Point3PPc",
      "compiled_body_bytes": 2044
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "fire_bomb_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-fire-bomb/fire_bomb_static_init.cpp",
  "sha256": "2be45cbeba3606966843cba97cefef639a0ccfa9820763785067f23997cbfb2e",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__8FireBombG6Point3sPPc",
      "compiled_body_bytes": 116
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "holy_bolt_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-holy-bolt/holy_bolt_static_init.cpp",
  "sha256": "0df8d3be9fef966802e5a5e51283c3d5e8df82ae0563b05f062dd19621969271",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__8HolyBoltG6Point3iiP8CreatureP5Skill",
      "compiled_body_bytes": 484
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "jump_gate_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-jump-gate/jump_gate_static_init.cpp",
  "sha256": "e3e8b37a9d757e6347e7cdb7032a9ff022c2afe9f6711f5e61e371dc1bedb69b",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$__8JumpGateG6Point3sPPc",
      "compiled_body_bytes": 324
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "spell_harm_touch_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-spell-harm-touch/spell_harm_touch_static_init.cpp",
  "sha256": "55f52ada8df5edba32a9a99555a7b4c3349547cb8fbe736009f2473910ec7ee2",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2",
      "compiled_body_bytes": 220
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "skill_convert_undead_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-skill-convert-undead/skill_convert_undead_static_init.cpp",
  "sha256": "8252600040a13c4c6364da54e3aa729b0473b8eb890c338615649e8cea7b8f9c",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$canCast__18SkillConvertUndeadP8Creature",
      "compiled_body_bytes": 120
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "skill_darkness_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-skill-darkness/skill_darkness_static_init.cpp",
  "sha256": "55f9a8dd7dd9c625d6ee06cda723f77f59da3e06a773e64ca5b6bc6a310be281",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$cast__13SkillDarknessiP8CreatureG6Point3s",
      "compiled_body_bytes": 112
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "skill_entangle_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-skill-entangle/skill_entangle_static_init.cpp",
  "sha256": "d1a7c6a8c8e401e204a558b4ea94ab29c49fe167cbf5dd870b0869cb573a85b4",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$cast__13SkillEntangleiP8CreatureG6Point3s",
      "compiled_body_bytes": 328
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "skill_multi_fire_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-skill-multi-fire/skill_multi_fire_static_init.cpp",
  "sha256": "70f5ee509d3e9f8c8292b4b5c2be8c55bb4064b06f3efd754362bc880ffb8aeb",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$cast__14SkillMultiFireiP8CreatureG6Point3s",
      "compiled_body_bytes": 116
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "skill_cleric_root_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-skill-cleric-root/skill_cleric_root_static_init.cpp",
  "sha256": "9b06a1cc8d2beff6024ef7c1b91c7114b61c2ef1b9a062bee710622e06d534a8",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$cast__15SkillClericRootiP8CreatureG6Point3s",
      "compiled_body_bytes": 328
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w8-spell-static-init-a-common/particle_def.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "skill_repulse_undead_static_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-spell-static-init-a-skill-repulse-undead/skill_repulse_undead_static_init.cpp",
  "sha256": "de2945eb4a36fe66858673379585b9637a23a531557d0bbb8b01762d473a8368",
  "emitted_functions": [
    {
      "name": "_GLOBAL_$I$cast__18SkillRepulseUndeadiP8CreatureG6Point3s",
      "compiled_body_bytes": 308
    }
  ]
}
```

## Authored but never executed

All 31 wrappers and local routines. Only host stubs ran them; nothing ran on the EE.

## Unknown

- The layout and meaning of the 80-byte particle definition record; the constructor body.
- Whether the original `D` records were placed in data for a reason (a compile option or an
  initializer) that bss placement here does not reproduce.
- The remaining functions of each original translation unit still need an owner; code that
  refers to these records (for example `fireTrailParticle`) cannot reach file-local ones from
  another authored file, so a later merge of unit contents will need the records moved
  into the owning unit.
- Which particle fields the callbacks null argument corresponds to (it is always null here).
