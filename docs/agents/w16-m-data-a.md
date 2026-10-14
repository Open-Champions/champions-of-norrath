# w16-m-data-a: plain data and type-info tables (part 1)

Branch `agents/w16-m-data-a`. Private reference: `work/private/SLUS_205.65` in the main checkout, read only with the
reviewed EE `nm`, `objdump` and `objdump -d`. No image bytes, disassembly, dump output or extracted string table is
committed; the notes below give symbol-table addresses, sizes and types, and describe behaviour in our own words.

## Result

Of the 32 scope names, this package authors storage or a getter for 22, leaves one (`__EH_FRAME_BEGIN__`) to an existing
authored unit that needs one plan entry, and leaves nine open. The nine are six linker-provided section symbols, the
`UITextures` table and two VU1 microcode blobs; each is explained below.

Closed by authored units in this branch (22; each becomes closed in a build once the entries below are added):

- Storage: `_13VISoundDevice$sBgmVersion`, `_13VIEESurfCache$Key`, `__ti9exception`, `__ti10VIResource`.
- Type-info getters: `__tf9exception`, `__tf10VIResource`, `__tf8HolyBolt`, `__tf8LostSoul`, `__tf10FrostShard`,
  `__tf10HolyStrike`, `__tf12GlowingShaft`, `__tf12SpellHealing`, `__tf13HammerOfWrath`, `__tf13SpellEntangle`,
  `__tf15SpellClericRoot`, `__tf16SpellConeOfFrost`, `__tf16VIRealmInterface`, `__tf17SpellMinorHealing`,
  `__tf18DarknessProjectile`, `__tf18FrostStormIceShard`, `__tf19BlindingLightEffect`, `__tf19VIFlatFileDirectory`.

Closed by one plan entry for an existing unit (1): `__EH_FRAME_BEGIN__`, from `w15-startup-a-link-eh-frame-begin`.
That unit was compiled here and emits the symbol in `.eh_frame` with size zero. No copy was authored in this package.

Still open (9):

- `_bssend`, `_end`, `_fbss`, `_gp`, `_srsctorsend`, `_srsctorsstart`: linker or start-up section symbols. The authored
  build is `ld -r` with no link script, so these cannot be closed by source without inventing layout. See Unknown.
- `UITextures`: a 251-record table of 36-byte entries. It is game data, so this package does not transcribe it.
  Its owner is `w15-fe-ui-b`, which also left it open.
- `VIDMABillboardMicrocode`, `VIDMARasterMicrocode`: VU1 program chains embedded in the image. Copying the bytes is
  copying proprietary code, so they stay open, as `w14-raster` and `w15-vi-raster-b` decided.

## Observed, inferred and unknown

Observed (private image, symbol table and bytes):

- `_13VISoundDevice$sBgmVersion` at `0x011b0248` and `_13VIEESurfCache$Key` at `0x011b0254`: each a 4-byte word,
  zero in the image, inside the `.srs` region.
- `__ti9exception` at `0x00486de8`: 8 bytes, bss. `__ti10VIResource` at `0x01228970`: 8 bytes, zero, `.srs` region.
- The 18 getters: each returns its class's node. A nonzero first word returns the node at once. A zero word triggers
  a registration path. Its shape is one of four: a root `__rtti_user` call; one base getter then `__rtti_si`; an
  inline two-level base registration then `__rtti_si`; or a three-level chain through the `__ti10Projectile` node.
  Class names and base names were read from the image's `.rodata` and checked against the implementations.
- Image sizes of the getters, from the symbol table: 64 (`__tf9exception`), 76 (single-base), 80 (`__tf10FrostShard`,
  `__tf10HolyStrike`, `__tf16VIRealmInterface`), 112 (`SpellHealing`, `SpellConeOfFrost`, `SpellMinorHealing`),
  116 (`VIFlatFileDirectory`), 132 (`HolyBolt`, `LostSoul`).

Inferred:

- Each getter is the lazy-init form that the sibling `w15-m-rtti-a` units also use. Each node is a zeroed block that the
  first call fills in via `__rtti_si` or `__rtti_user`.
- `_13VISoundDevice$sBgmVersion` and `_13VIEESurfCache$Key` start at zero. Nothing in the image contradicts this.

Unknown:

- The node fields beyond the first word, and what `__rtti_user` and `__rtti_si` write there. Those routines belong to
  other packages.
- The EE compiled body sizes differ from the image's sizes. Compiled: 72 for the two root getters, 92 for the
  single-base getters, 164 for the two-level getters and 184 for `HolyBolt` and `LostSoul`. Image: 64, 76, 112 and
  132 respectively. These are behavioural reconstructions, not byte matches.

## Checks performed

- Each unit compiled with the reviewed EE g++ and the plan's `authored_cpp` flags; `nm -S` on each object shows the
  expected symbol, storage class and size.
- Each unit has a host check built with host `c++ -std=c++17 -Wall -Wextra`. Each passes. Each was mutated once
  (removing a registration call, or a wrong initial value) and the mutant fails, confirming the check can detect a wrong
  implementation.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Scratch-plan check: `tools/build_partial.py --check-plan` on a copy of the shared plan with these entries appended
  reports `Input coverage mismatch`. The validator requires a successor `snapshot.json`, which is a shared file, so
  this package did not produce one. The integrator must regenerate the snapshot and plan.
- Full partial build, started from a private copy of the shared plan at `work/w16-m-data-a-build`. See the build
  line below for its state.

Build state: complete. The partial build from a private copy of the shared plan (plan SHA-256 7e9ac625...) compiled 7,327 sources, made one link, and reported 5,834 functions and 841 unresolved imports, with no game executable. All 32 scope names are still unresolved in it, because this package's entries are not in that plan. The 841-name list matches the newest earlier build, plan16-build.

## Imports opened

Names these units reference that no authored unit in the shared plan defines (27 names; see the list in
"Unknown" for the classes whose node storage is not authored here):

- Base getters: `__tf10GameObject`, `__tf10Projectile`, `__tf5Spell`, `__tfQ212RealmService15RealmGatewayAPI`.
- Base nodes: `__ti5Spell` (8 bytes), `__ti10GameObject` (8 bytes), `__ti10Projectile` (12 bytes), `__ti10VIFlatFile`
  (8 bytes), `__tiQ212RealmService15RealmGatewayAPI` (8 bytes).
- Class nodes, zeroed storage like the authored `__ti9exception`: `__ti8HolyBolt`, `__ti8LostSoul`, `__ti10FrostShard`,
  `__ti10HolyStrike`, `__ti12GlowingShaft`, `__ti12SpellHealing`, `__ti13HammerOfWrath`, `__ti13SpellEntangle`,
  `__ti15SpellClericRoot`, `__ti16SpellConeOfFrost`, `__ti16VIRealmInterface`, `__ti17SpellMinorHealing`,
  `__ti18DarknessProjectile`, `__ti18FrostStormIceShard`, `__ti19BlindingLightEffect`, `__ti19VIFlatFileDirectory`
  (12 bytes each, `.bss` in the image).

Also referenced but already defined by planned units: `__rtti_si` (`w8-runtime-libc-a-rtti-si`) and `__rtti_user`
(`w10-startup-a-rtti-user`).

## Build-plan entries needed

Append these entries to `experimental/build-plan.json` `inputs`, in any order the integrator chooses; `input_order` is
assigned by the integrator. The entries carry the exact source hashes and emitted sizes measured here. Each
`compiled_body_bytes` is the EE compiled size, not the image size.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_sound_bgm_version.o",
  "profile": "authored_cpp",
  "sha256": "b80e8235153f71b5cc5d2da3bee0003756664d432b35cc3ce043b5026a50c554",
  "source": "sources/w16-m-data-a-sound-bgm-version/bgm_version.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_10frostshard.o",
  "profile": "authored_cpp",
  "sha256": "3ad59b4fb14a1cf020fc5d6d7bb5c98257b388c5660110f253d2f63a05169887",
  "source": "sources/w16-m-data-a-tf-10frostshard/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10FrostShard",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_10holystrike.o",
  "profile": "authored_cpp",
  "sha256": "11c4ced846b4101c841d0369bd2271f19911c0d00ea2a5a9460e8b6e01da5266",
  "source": "sources/w16-m-data-a-tf-10holystrike/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10HolyStrike",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_10viresource.o",
  "profile": "authored_cpp",
  "sha256": "998ea7a9c0074442f2cb2e0d516bd7b98ef31c636fe8d430e9ff97f5452d3970",
  "source": "sources/w16-m-data-a-tf-10viresource/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10VIResource",
    "compiled_body_bytes": 72
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_12glowingshaft.o",
  "profile": "authored_cpp",
  "sha256": "1cd6adc7c1b1a8704711923af9eecf6f222347f546dcee9d0bcc47a693cceb00",
  "source": "sources/w16-m-data-a-tf-12glowingshaft/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf12GlowingShaft",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_12spellhealing.o",
  "profile": "authored_cpp",
  "sha256": "662fd902f4c5903d860bb896211b508bbeaef20bbfeb94cd83a333e6297b9305",
  "source": "sources/w16-m-data-a-tf-12spellhealing/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf12SpellHealing",
    "compiled_body_bytes": 164
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_13hammerofwrath.o",
  "profile": "authored_cpp",
  "sha256": "7fe854c21dc69a0eb664cf53f02007b3f43e9fca72b8becfba209b3624950867",
  "source": "sources/w16-m-data-a-tf-13hammerofwrath/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13HammerOfWrath",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_13spellentangle.o",
  "profile": "authored_cpp",
  "sha256": "da3daa30ee8c5b45e63a936ae48b25dc4834f61c9e5b29fde71384076f0db62b",
  "source": "sources/w16-m-data-a-tf-13spellentangle/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13SpellEntangle",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_15spellclericroot.o",
  "profile": "authored_cpp",
  "sha256": "9e3d0937231d7186847f0df24cf85cc3eff68251edc90ac8254ba9e6e00e21cf",
  "source": "sources/w16-m-data-a-tf-15spellclericroot/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf15SpellClericRoot",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_16spellconeoffrost.o",
  "profile": "authored_cpp",
  "sha256": "39bfa75a7ee20dbd65a459cdddea5c49d617e37bc8db9940fa7914f48358fd3c",
  "source": "sources/w16-m-data-a-tf-16spellconeoffrost/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf16SpellConeOfFrost",
    "compiled_body_bytes": 164
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_16virealminterface.o",
  "profile": "authored_cpp",
  "sha256": "e5042f96980ee79d347309f0b15fc8eac11cabf39d61efe4f733a6c90db176f1",
  "source": "sources/w16-m-data-a-tf-16virealminterface/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf16VIRealmInterface",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_17spellminorhealing.o",
  "profile": "authored_cpp",
  "sha256": "d32f8029f2106d058c361cc822972e6572629d9daf0d23d36ac837477c73eefa",
  "source": "sources/w16-m-data-a-tf-17spellminorhealing/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf17SpellMinorHealing",
    "compiled_body_bytes": 164
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_18darknessprojectile.o",
  "profile": "authored_cpp",
  "sha256": "c0d8b34c1fc03ec94818bd7452b6a8b42387a64a2995c9dc0aa093a8598cf1fe",
  "source": "sources/w16-m-data-a-tf-18darknessprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf18DarknessProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_18froststormiceshard.o",
  "profile": "authored_cpp",
  "sha256": "9b04e3e31bc28a5aa463dc0d636095d2c7728d3df3a9fdea3a044dcadb9e8649",
  "source": "sources/w16-m-data-a-tf-18froststormiceshard/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf18FrostStormIceShard",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_19blindinglighteffect.o",
  "profile": "authored_cpp",
  "sha256": "7100b561abb6b654abe78539b09e01e1ea5294d1296c589d0a59e678dbc6c287",
  "source": "sources/w16-m-data-a-tf-19blindinglighteffect/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf19BlindingLightEffect",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_19viflatfiledirectory.o",
  "profile": "authored_cpp",
  "sha256": "3c0e9dd56a129c74c0aaf2489eea3db729693636ecbd17f3c7ea303c36dbe6ae",
  "source": "sources/w16-m-data-a-tf-19viflatfiledirectory/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf19VIFlatFileDirectory",
    "compiled_body_bytes": 164
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_8holybolt.o",
  "profile": "authored_cpp",
  "sha256": "e5e91f0565222b435a59f4ffa2447581f6e94499346950d97e91402b9e6c4c99",
  "source": "sources/w16-m-data-a-tf-8holybolt/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf8HolyBolt",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_8lostsoul.o",
  "profile": "authored_cpp",
  "sha256": "a6e1a04d29ec7fe29130891ef8ca19919c2092315d74ee11c1c0b3da54a4914f",
  "source": "sources/w16-m-data-a-tf-8lostsoul/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf8LostSoul",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_tf_9exception.o",
  "profile": "authored_cpp",
  "sha256": "7a3e24b516ee8245cce7cd6b20d0c8032f0755ad9e5cdcf74f958212781a3839",
  "source": "sources/w16-m-data-a-tf-9exception/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9exception",
    "compiled_body_bytes": 72
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_ti_10viresource.o",
  "profile": "authored_cpp",
  "sha256": "6dd38d48851e906e1e79aef23daabe8e810dc83cf819cb40bbbdeee25dbec93d",
  "source": "sources/w16-m-data-a-ti-10viresource/typeinfo_node.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_ti_9exception.o",
  "profile": "authored_cpp",
  "sha256": "279f2a65a8f348f957b415e6017f5e8b108c77067daaa2f0fc006caa0fda404a",
  "source": "sources/w16-m-data-a-ti-9exception/typeinfo_node.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_m_data_a_vi_eesurfcache_key.o",
  "profile": "authored_cpp",
  "sha256": "61673bfeff4dbb851c30dc79097b84c17ed5af9eff4702b93979f4928ce9bbe8",
  "source": "sources/w16-m-data-a-vi-eesurfcache-key/surface_cache_key.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_startup_a_link_eh_frame_begin.o",
  "profile": "authored_cpp",
  "sha256": "9b7a8de5e09db9fcc05fc3e0006a2fbad0785f03d1a4305d061aa23d28a4e026",
  "source": "sources/w15-startup-a-link-eh-frame-begin/eh_frame_begin.cpp",
  "emitted_functions": []
 }
]
```

The shared `snapshot.json` must also include these inputs. This package did not change it.

## Authored but never executed

Nothing from this package ran on the EE or in the game. All host checks run on the build host, with the EE-only
behaviour (calling convention, real `__rtti_si`/`__rtti_user`, lazy-init order under the real runtime) not exercised. No
paired run against the original was done. The EE objects were compiled and inspected, not linked or loaded.

## Unknown

- Whether the original's `__rtti_si` and `__rtti_user` see identical arguments for every class. The arguments were read
  from the getters' calling sequences, not from the callee.
- The `.srs` region's ownership. Two words in it are `T` symbols in the image, which suggests the SRS module's data
  layout is not the same as the main image's. Their storage as plain `.bss` in the partial build is an assumption.
- `_fbss`: `items-player-pic-init` defines `_fbss` as an `unsigned char*` variable, while the image symbol is a
  `.sbss` start label at `0x00486800` used as a data base. These two models conflict. This package does not resolve
  that conflict.
- `_gp` is an absolute symbol with value `0x004879f0` in the image, set by the linker. `_end`, `_bssend` and the
  `.srsctors` bounds are section boundaries. A relocatable build cannot define them correctly without a link script.
- `UITextures`: record meanings are only partly established by `w15-fe-ui-b`. Its contents are not authored here.
- Microcode: owned by a VU1 microcode author. Not in this package.
