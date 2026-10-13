# w11-ctor-b: skill constructors (2 of 3)

Branch `agents/w11-ctor-b`. Package: the 28 `Skill*` constructor imports named in the task, from
`__15SkillClericRoot` through `__15SkillHolyStrike`. One function per commit.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0` (the hash recorded by the
sibling package notes; not re-verified in this session). Addresses are symbol values in that file.
No game bytes or disassembly are committed.

## Observed
- Each constructor is 44 or 48 bytes, makes no calls, reads nothing and returns nothing. It takes one
  argument (the object address) and performs five stores: the address of a lowercase name text at +0,
  a word at +4, a word at +8, a halfword at +12, and the address of the class's `_vt$` table at +96.
- The existing `w10-item-a-skills` unit already calls all 28 as `void f(void*)` on 104-byte objects, so
  the signature used here matches its callers.
- Values (+4, +8, halfword +12): Cyclone 1, 0, 30; ConvertEnemy and ConvertUndead 0, 0, 600;
  HarmTouch 0, 7, 300; Dodge 0, 3, 30; ConeOfFire and ConeOfFrost 0, 1, 30; ColdArrow, FireArrow and
  ExplodeArrow 0, 8, 30; all other classes 0, 0, 30.
- Name texts are the lowercase class names without the `Skill` prefix (for example `clericroot`,
  `coneoffrost`, `hammerofwrath`), each followed by a `<name>_rankdesc` text nearby in the image.

## Inferred
- The name pointers refer to string constants in the image's read-only data. These sources keep their own
  copy of the text, so the stored address differs from the image's; only the text is reproduced.

## Unknown
- What the numbers at +4, +8 and the halfword at +12 mean. The meaning of the +8 values 1, 3, 7, 8 is not
  established, and the 600/300 halfwords are not explained.
- Whether the name texts are shared with other tables (so a separately placed copy could differ in
  behaviour if something compares name pointers rather than text).
- The contents of the `_vt$` tables; they remain imports (see below).

## Checks performed
- Each source compiles with the reviewed EE g++ `authored_cpp` flags (single-unit compiles; no full partial
  build was run).
- Each has `host_check.cpp` (not an EE input): it fills a 112-byte buffer with a sentinel, calls the
  constructor, and compares every byte against an independently written expectation, plus the name text.
  Changing one stored value (Dodge +8 from 3 to 4) makes both the host check and the paired run fail.
- Private paired run (R5900 interpreter harness kept outside git): original and authored function run from
  identical memory with four different fills of the object; every byte of the first 128 was compared except
  the name-address word, whose pointed-to text was compared instead (and matched the original's text);
  call logs were equal (empty). 28 functions, 112 runs, 0 mismatches, no HALT. Instruction coverage is
  the whole function as there are no branches.
- `python3 -m unittest discover -s tools -p 'test_*.py'`214 tests, OK.

## Functions

| source | symbol | image | compiled text bytes | sha256 |
|---|---|---|---|---|
| `sources/w11-ctor-b-skill-cleric-root/skill_cleric_root.cpp` | `__15SkillClericRoot` | `0x2c6158`, 44 bytes | 48 | `2a8b0f2b55212c97cec0f6d71d46678dd54ab5fb53a71ea9768f4e13d813c21c` |
| `sources/w11-ctor-b-skill-cold-arrow/skill_cold_arrow.cpp` | `__14SkillColdArrow` | `0x29fb40`, 48 bytes | 48 | `5a36ddf4978fc411f5bfbd63085d472c1de50f95b6b477f13f9fa947828d37e4` |
| `sources/w11-ctor-b-skill-cold-weapons/skill_cold_weapons.cpp` | `__16SkillColdWeapons` | `0x2ac330`, 44 bytes | 48 | `4957855f0ef8525eee4a51400d220f1cbc916b04f76152f3dbfd061a28f024ef` |
| `sources/w11-ctor-b-skill-cone-of-fire/skill_cone_of_fire.cpp` | `__15SkillConeOfFire` | `0x2b69e0`, 48 bytes | 48 | `eea83ff8cf770f5517df3cabbf7c1f4ee1b947bab97f5e82d70090319f52f6d6` |
| `sources/w11-ctor-b-skill-cone-of-frost/skill_cone_of_frost.cpp` | `__16SkillConeOfFrost` | `0x2afca0`, 48 bytes | 48 | `d8f94e71c0a002008356f9ad8ebf9040a6fec6b26ae01889cedbe5727d28415f` |
| `sources/w11-ctor-b-skill-convert-enemy/skill_convert_enemy.cpp` | `__17SkillConvertEnemy` | `0x298a78`, 44 bytes | 48 | `5933c4a2114c04dd5fb3799a000ed8548dc434bdfd19b285610c523ce80155c4` |
| `sources/w11-ctor-b-skill-convert-undead/skill_convert_undead.cpp` | `__18SkillConvertUndead` | `0x2994d0`, 44 bytes | 48 | `2ead48393a5b1b2eb5cb099889f9f6fece6a548a2067d932455b8f23bc1b63ba` |
| `sources/w11-ctor-b-skill-critical-hit/skill_critical_hit.cpp` | `__16SkillCriticalHit` | `0x2adb70`, 44 bytes | 48 | `bedf6fc00a5d72ca055d15da32723afed3fbd379ce2de7b8ad289a54ad2b0c20` |
| `sources/w11-ctor-b-skill-cyclone/skill_cyclone.cpp` | `__12SkillCyclone` | `0x29a710`, 48 bytes | 48 | `c5b12ccf8967cf38eb083fedea90bcbba47c6adeaa45158b1a690f6c95b168ff` |
| `sources/w11-ctor-b-skill-darkness/skill_darkness.cpp` | `__13SkillDarkness` | `0x298150`, 44 bytes | 48 | `1a084ac0407deff697f643f50a2ea0d677aa1a7a1306567a1cf4f53b74782e34` |
| `sources/w11-ctor-b-skill-disease-bolt/skill_disease_bolt.cpp` | `__16SkillDiseaseBolt` | `0x293738`, 44 bytes | 48 | `d279e8ffcdecaa67e4b1243227786aaa67b797722d4586383d0241b08df03a4d` |
| `sources/w11-ctor-b-skill-disease-shield/skill_disease_shield.cpp` | `__18SkillDiseaseShield` | `0x296760`, 44 bytes | 48 | `6ffdd437e0568d21c25fc993438dc7f27041589f5d16b6f854ae4396cae1474f` |
| `sources/w11-ctor-b-skill-disease-trail/skill_disease_trail.cpp` | `__17SkillDiseaseTrail` | `0x295428`, 44 bytes | 48 | `57c6aa013d64c958d16bb03729b9c5fec70e04126ddee7f939dbca787e8b89d1` |
| `sources/w11-ctor-b-skill-dismiss-undead/skill_dismiss_undead.cpp` | `__18SkillDismissUndead` | `0x2bbc18`, 44 bytes | 48 | `81362b9b7f48a5b14901ba418290469cd65203c6af97d4bee2e86bfc02c5d797` |
| `sources/w11-ctor-b-skill-dodge/skill_dodge.cpp` | `__10SkillDodge` | `0x2ae6b8`, 48 bytes | 48 | `2cede65d3f8e115e06dfcc5746ab7d6b02748707f4e186c2b8648298f5768b4a` |
| `sources/w11-ctor-b-skill-entangle/skill_entangle.cpp` | `__13SkillEntangle` | `0x2c6dd0`, 44 bytes | 48 | `9b397edc71f921a2e701c7499d174ead8e206d20065c57662b668773c154e907` |
| `sources/w11-ctor-b-skill-explode-arrow/skill_explode_arrow.cpp` | `__17SkillExplodeArrow` | `0x2a2648`, 48 bytes | 48 | `d6ac90ffe2345457468447637431e77d78a80a7ec5a38c3bbf88ed5cffb04ffe` |
| `sources/w11-ctor-b-skill-fire-arrow/skill_fire_arrow.cpp` | `__14SkillFireArrow` | `0x29e2c8`, 48 bytes | 48 | `3e8a27a12d285eee4a22db5ab5bbd86634b131b11807566c9755cb1f2901da8d` |
| `sources/w11-ctor-b-skill-fire-flares/skill_fire_flares.cpp` | `__15SkillFireFlares` | `0x2a4858`, 44 bytes | 48 | `bebef43b67511be955c81cf39364d89fa93da34b4acb089d4d3d8ca24790d7e2` |
| `sources/w11-ctor-b-skill-fire-storm/skill_fire_storm.cpp` | `__14SkillFireStorm` | `0x29d190`, 44 bytes | 48 | `c084a0ff7db145694f5aee8a9e41ecb1057ee0d0159bd08f9151a8ac022fd9aa` |
| `sources/w11-ctor-b-skill-fire-weapons/skill_fire_weapons.cpp` | `__16SkillFireWeapons` | `0x2ab390`, 44 bytes | 48 | `94d2e5c218891f19b616db1f0aa747209121d85188df5ac0902aa1fcd3b2e1c1` |
| `sources/w11-ctor-b-skill-frost-storm/skill_frost_storm.cpp` | `__15SkillFrostStorm` | `0x2b34e0`, 44 bytes | 48 | `954ba0168cc49f22aa23da24f0ad8f2b677bf75ce4c36807aed13be7d9f2b9ee` |
| `sources/w11-ctor-b-skill-ground-pound/skill_ground_pound.cpp` | `__16SkillGroundPound` | `0x290888`, 44 bytes | 48 | `7230bac365cc92e7d8345beb62e9394d343aa5b6a899056cb2705d2867c7a446` |
| `sources/w11-ctor-b-skill-hammer-of-wrath/skill_hammer_of_wrath.cpp` | `__18SkillHammerOfWrath` | `0x2b5790`, 44 bytes | 48 | `7bb6ab1319f24a355af966c393dad0d8925af469d13c1eda3e4fe27afb5e1962` |
| `sources/w11-ctor-b-skill-harm-touch/skill_harm_touch.cpp` | `__14SkillHarmTouch` | `0x29aea8`, 48 bytes | 48 | `8e16200fb43bf556d24057070202c48c36e7a1b665536640440a0a03dc39983f` |
| `sources/w11-ctor-b-skill-healing/skill_healing.cpp` | `__12SkillHealing` | `0x2af208`, 44 bytes | 48 | `1d406ae267bf728d17b8ac9e2ca82df852c75ca4d7de15d2fc51caa1d54285e4` |
| `sources/w11-ctor-b-skill-holy-shield/skill_holy_shield.cpp` | `__15SkillHolyShield` | `0x2ae440`, 44 bytes | 48 | `6109c7804ae9f6b6f5842aab44bfab377e428fcc9592c280b5618290ec96f67b` |
| `sources/w11-ctor-b-skill-holy-strike/skill_holy_strike.cpp` | `__15SkillHolyStrike` | `0x2b2cd8`, 44 bytes | 48 | `7f53def9be04037b3da6f5a8e4d2f154d1703f5449f095b4ec1dc1003aaac5be` |

## Imports closed
__15SkillClericRoot, __14SkillColdArrow, __16SkillColdWeapons, __15SkillConeOfFire, __16SkillConeOfFrost, __17SkillConvertEnemy, __18SkillConvertUndead, __16SkillCriticalHit, __12SkillCyclone, __13SkillDarkness, __16SkillDiseaseBolt, __18SkillDiseaseShield, __17SkillDiseaseTrail, __18SkillDismissUndead, __10SkillDodge, __13SkillEntangle, __17SkillExplodeArrow, __14SkillFireArrow, __15SkillFireFlares, __14SkillFireStorm, __16SkillFireWeapons, __15SkillFrostStorm, __16SkillGroundPound, __18SkillHammerOfWrath, __14SkillHarmTouch, __12SkillHealing, __15SkillHolyShield, __15SkillHolyStrike.

## Imports opened
The 28 class table symbols `_vt$<n>Skill<Name>` (for example `_vt$10SkillDodge`), one per constructor. They are
data owned by whichever package reconstructs the skill tables; their contents are not authored here.

## Build-plan entries needed
Append these 28 entries (profile `authored_cpp`, no header closure, one emitted function each) to
`experimental/build-plan.json`:

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_cleric_root.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-cleric-root/skill_cleric_root.cpp",
  "sha256": "2a8b0f2b55212c97cec0f6d71d46678dd54ab5fb53a71ea9768f4e13d813c21c",
  "emitted_functions": [
   {
    "name": "__15SkillClericRoot",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_cold_arrow.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-cold-arrow/skill_cold_arrow.cpp",
  "sha256": "5a36ddf4978fc411f5bfbd63085d472c1de50f95b6b477f13f9fa947828d37e4",
  "emitted_functions": [
   {
    "name": "__14SkillColdArrow",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_cold_weapons.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-cold-weapons/skill_cold_weapons.cpp",
  "sha256": "4957855f0ef8525eee4a51400d220f1cbc916b04f76152f3dbfd061a28f024ef",
  "emitted_functions": [
   {
    "name": "__16SkillColdWeapons",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_cone_of_fire.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-cone-of-fire/skill_cone_of_fire.cpp",
  "sha256": "eea83ff8cf770f5517df3cabbf7c1f4ee1b947bab97f5e82d70090319f52f6d6",
  "emitted_functions": [
   {
    "name": "__15SkillConeOfFire",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_cone_of_frost.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-cone-of-frost/skill_cone_of_frost.cpp",
  "sha256": "d8f94e71c0a002008356f9ad8ebf9040a6fec6b26ae01889cedbe5727d28415f",
  "emitted_functions": [
   {
    "name": "__16SkillConeOfFrost",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_convert_enemy.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-convert-enemy/skill_convert_enemy.cpp",
  "sha256": "5933c4a2114c04dd5fb3799a000ed8548dc434bdfd19b285610c523ce80155c4",
  "emitted_functions": [
   {
    "name": "__17SkillConvertEnemy",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_convert_undead.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-convert-undead/skill_convert_undead.cpp",
  "sha256": "2ead48393a5b1b2eb5cb099889f9f6fece6a548a2067d932455b8f23bc1b63ba",
  "emitted_functions": [
   {
    "name": "__18SkillConvertUndead",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_critical_hit.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-critical-hit/skill_critical_hit.cpp",
  "sha256": "bedf6fc00a5d72ca055d15da32723afed3fbd379ce2de7b8ad289a54ad2b0c20",
  "emitted_functions": [
   {
    "name": "__16SkillCriticalHit",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_cyclone.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-cyclone/skill_cyclone.cpp",
  "sha256": "c5b12ccf8967cf38eb083fedea90bcbba47c6adeaa45158b1a690f6c95b168ff",
  "emitted_functions": [
   {
    "name": "__12SkillCyclone",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_darkness.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-darkness/skill_darkness.cpp",
  "sha256": "1a084ac0407deff697f643f50a2ea0d677aa1a7a1306567a1cf4f53b74782e34",
  "emitted_functions": [
   {
    "name": "__13SkillDarkness",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_disease_bolt.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-disease-bolt/skill_disease_bolt.cpp",
  "sha256": "d279e8ffcdecaa67e4b1243227786aaa67b797722d4586383d0241b08df03a4d",
  "emitted_functions": [
   {
    "name": "__16SkillDiseaseBolt",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_disease_shield.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-disease-shield/skill_disease_shield.cpp",
  "sha256": "6ffdd437e0568d21c25fc993438dc7f27041589f5d16b6f854ae4396cae1474f",
  "emitted_functions": [
   {
    "name": "__18SkillDiseaseShield",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_disease_trail.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-disease-trail/skill_disease_trail.cpp",
  "sha256": "57c6aa013d64c958d16bb03729b9c5fec70e04126ddee7f939dbca787e8b89d1",
  "emitted_functions": [
   {
    "name": "__17SkillDiseaseTrail",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_dismiss_undead.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-dismiss-undead/skill_dismiss_undead.cpp",
  "sha256": "81362b9b7f48a5b14901ba418290469cd65203c6af97d4bee2e86bfc02c5d797",
  "emitted_functions": [
   {
    "name": "__18SkillDismissUndead",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_dodge.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-dodge/skill_dodge.cpp",
  "sha256": "2cede65d3f8e115e06dfcc5746ab7d6b02748707f4e186c2b8648298f5768b4a",
  "emitted_functions": [
   {
    "name": "__10SkillDodge",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_entangle.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-entangle/skill_entangle.cpp",
  "sha256": "9b397edc71f921a2e701c7499d174ead8e206d20065c57662b668773c154e907",
  "emitted_functions": [
   {
    "name": "__13SkillEntangle",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_explode_arrow.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-explode-arrow/skill_explode_arrow.cpp",
  "sha256": "d6ac90ffe2345457468447637431e77d78a80a7ec5a38c3bbf88ed5cffb04ffe",
  "emitted_functions": [
   {
    "name": "__17SkillExplodeArrow",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_fire_arrow.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-fire-arrow/skill_fire_arrow.cpp",
  "sha256": "3e8a27a12d285eee4a22db5ab5bbd86634b131b11807566c9755cb1f2901da8d",
  "emitted_functions": [
   {
    "name": "__14SkillFireArrow",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_fire_flares.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-fire-flares/skill_fire_flares.cpp",
  "sha256": "bebef43b67511be955c81cf39364d89fa93da34b4acb089d4d3d8ca24790d7e2",
  "emitted_functions": [
   {
    "name": "__15SkillFireFlares",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_fire_storm.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-fire-storm/skill_fire_storm.cpp",
  "sha256": "c084a0ff7db145694f5aee8a9e41ecb1057ee0d0159bd08f9151a8ac022fd9aa",
  "emitted_functions": [
   {
    "name": "__14SkillFireStorm",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_fire_weapons.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-fire-weapons/skill_fire_weapons.cpp",
  "sha256": "94d2e5c218891f19b616db1f0aa747209121d85188df5ac0902aa1fcd3b2e1c1",
  "emitted_functions": [
   {
    "name": "__16SkillFireWeapons",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_frost_storm.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-frost-storm/skill_frost_storm.cpp",
  "sha256": "954ba0168cc49f22aa23da24f0ad8f2b677bf75ce4c36807aed13be7d9f2b9ee",
  "emitted_functions": [
   {
    "name": "__15SkillFrostStorm",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_ground_pound.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-ground-pound/skill_ground_pound.cpp",
  "sha256": "7230bac365cc92e7d8345beb62e9394d343aa5b6a899056cb2705d2867c7a446",
  "emitted_functions": [
   {
    "name": "__16SkillGroundPound",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_hammer_of_wrath.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-hammer-of-wrath/skill_hammer_of_wrath.cpp",
  "sha256": "7bb6ab1319f24a355af966c393dad0d8925af469d13c1eda3e4fe27afb5e1962",
  "emitted_functions": [
   {
    "name": "__18SkillHammerOfWrath",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_harm_touch.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-harm-touch/skill_harm_touch.cpp",
  "sha256": "8e16200fb43bf556d24057070202c48c36e7a1b665536640440a0a03dc39983f",
  "emitted_functions": [
   {
    "name": "__14SkillHarmTouch",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_healing.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-healing/skill_healing.cpp",
  "sha256": "1d406ae267bf728d17b8ac9e2ca82df852c75ca4d7de15d2fc51caa1d54285e4",
  "emitted_functions": [
   {
    "name": "__12SkillHealing",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_holy_shield.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-holy-shield/skill_holy_shield.cpp",
  "sha256": "6109c7804ae9f6b6f5842aab44bfab377e428fcc9592c280b5618290ec96f67b",
  "emitted_functions": [
   {
    "name": "__15SkillHolyShield",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "skill_holy_strike.o",
  "profile": "authored_cpp",
  "source": "sources/w11-ctor-b-skill-holy-strike/skill_holy_strike.cpp",
  "sha256": "7f53def9be04037b3da6f5a8e4d2f154d1703f5449f095b4ec1dc1003aaac5be",
  "emitted_functions": [
   {
    "name": "__15SkillHolyStrike",
    "compiled_body_bytes": 48
   }
  ]
 }
]
```

## Authored but never executed
All 28 constructors. They ran only inside the private interpreter against the original image's data and on
the host; none has run on the EE or in the game, and nothing was linked into an executable.

## Unknown
See the Unknown section above.
