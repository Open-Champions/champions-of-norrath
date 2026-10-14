# w12-actor-spell-a: spell, projectile, prop and misc actor classes (A)

Branch `agents/w12-actor-spell-a`. Every function below is its own commit. Sources are under
`experimental/sources/w12-actor-spell-a-*`; each source starts with a comment stating what was observed in the
private image, what is inferred and what is unknown. Nothing from the image (bytes, disassembly) is committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and sizes below are that file's symbol
values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the
`authored_cpp` flags; it differs from the image size because these are behavior reconstructions, not byte matches.

## What was done

Twenty-nine units close all 29 assigned imports (28 functions and the `_vt$10MummySnake` table; 29 plan inputs). Types of unit:

- 11 type-information functions (`__tf...`): each returns its class's node (`__ti...`), whose first word gates a
  chain of `__rtti_si` / `__rtti_user` registrations (some bases inline, some through `__tf10GameObject` or `__tf5Spell`).
- 8 destructors (`_$_...`): table store(s), member destructors/list teardown, then `blockFree__FPv` when bit 0 of the flags is set.
- 1 data table: `_vt$10MummySnake` (18 slots, 144 bytes).
- 9 functions: the GutChunk, WebTrap, PhProp, StaticFire, ItemProp, FireBombletJr, Critter and Skeleton constructors
  and `TurnToFaceEdge__14WoodElfSoldier`.

All callees use their real image symbol names (via `asm` labels where the C++ name differs); no alias symbols were
introduced, and existing `norrath_*`-named constructors (for example `norrath_construct_Critter`) were not reused
because their callees are invented names that do not exist in the image.

## Units

| unit | symbol | image | compiled bytes | source sha256 |
|---|---|---|---|---|
| `w12-actor-spell-a-ctor-critter` | `__7CritterPcG6Point3sPPc` | `0x001efeb0`, 1944 bytes | 1996 | `0fb19715776ec731bc53ae2c28373e07c395b1ec1491181e49eaafa16f16d75c` |
| `w12-actor-spell-a-ctor-firebomblet-jr` | `__13FireBombletJrP11FireBomblet` | `0x0028aca8`, 1040 bytes | 956 | `e582bba16c7fa27b6a836b6a1b15a79e50272f97a8b63bca96b8cd4466c99dab` |
| `w12-actor-spell-a-ctor-gutchunk` | `__8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif` | `0x001e5190`, 160 bytes | 136 | `260b6a135dca50fc5c974df8f8fd7cde8b52c121099645b126a9cfc8c4db8255` |
| `w12-actor-spell-a-ctor-itemprop-matrix` | `__8ItemPropG6Point3PQ24item11descriptionP8Matrix34` | `0x0014bbc8`, 796 bytes | 656 | `f2420afe3f98769e5a1dc87db5db1a5f8c5be69275c8ed5e90c4069e25d2e5e6` |
| `w12-actor-spell-a-ctor-phprop-texture` | `__6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf` | `0x00149250`, 488 bytes | 496 | `15ac3610ceb01dcf40156934c7d0d35ee799366a0a94dbcb784ab05e8884d7d9` |
| `w12-actor-spell-a-ctor-skeleton` | `__8SkeletonG6Point3sPPcPci` | `0x00243868`, 2640 bytes | 2588 | `d7c26aa80ee755a3ac8e6108e52aea92acb74b1289a863bbebcc96105017bdf5` |
| `w12-actor-spell-a-ctor-staticfire-box` | `__10StaticFireG6Point3fif` | `0x0028da68`, 664 bytes | 560 | `d8365fdcf9c7bb7eb169a399004f0c70a6fb29183ce5eb576e6b287853afa46a` |
| `w12-actor-spell-a-ctor-webtrap-points` | `__7WebTrapG6Point3T1` | `0x0020e758`, 400 bytes | 336 | `683510e3a8800e1312bd42c7b87b604d48abfaec558a1cb69ae5a254234e64c0` |
| `w12-actor-spell-a-dtor-13looseicechunk` | `_$_13LooseIceChunk` | `0x0030fcc0`, 100 bytes | 88 | `38a2cc4e2fd786eecbdd3e2b01ef77b3b77232ed4e2166a8f9e2d13cff837709` |
| `w12-actor-spell-a-dtor-15glowingcylinder` | `_$_15GlowingCylinder` | `0x00310668`, 100 bytes | 88 | `c759ae6e81bd83f8593d1d1adbea3b6260024ee1833afaa0b75c7b377a13cba6` |
| `w12-actor-spell-a-dtor-17vehiclebodysphere` | `_$_17VehicleBodySphere` | `0x00312c28`, 100 bytes | 88 | `198155d820c2e87f3f8a7822d1bb9ce96d94a522e5a1c4399701961413ecb7b5` |
| `w12-actor-spell-a-dtor-19spellhornomeneffect` | `_$_19SpellHornOmenEffect` | `0x002ca358`, 116 bytes | 116 | `e934b7035ada7f3d5eab82e90566f1b2c2e8047d6b63391ec710dfcf1f1bf57b` |
| `w12-actor-spell-a-dtor-22spellplanarsteeleffect` | `_$_22SpellPlanarSteelEffect` | `0x002c9988`, 144 bytes | 136 | `250b84993a4805fd491562f3a62b2b42f726a0ad5d0fbca78feff3b958d0d67f` |
| `w12-actor-spell-a-dtor-5phice` | `_$_5PhIce` | `0x0030ddd0`, 100 bytes | 88 | `124af7982cf1856cb07ed17d5a5cd09c2f831a9f4bf8cbb5b234f50ba05fdc23` |
| `w12-actor-spell-a-dtor-7oniball` | `_$_7OniBall` | `0x00310d18`, 132 bytes | 136 | `be46e11a44058125f066b75a8083b525ce63f7f5b8571f94ccd9f90fee7ac944` |
| `w12-actor-spell-a-dtor-7weather` | `_$_7Weather` | `0x00312710`, 100 bytes | 88 | `2dec545fe9141c3717a0d8cffea17488641d75bc1f720f17decd9cc8f463193f` |
| `w12-actor-spell-a-tf-11spellcharge` | `__tf11SpellCharge` | `0x00316b18`, 112 bytes | 164 | `e03c69fe5553c465347fe7af9ff4f10e7f643c1547a07e606558a6a5a8fec6c5` |
| `w12-actor-spell-a-tf-13looseicechunk` | `__tf13LooseIceChunk` | `0x0030fd28`, 176 bytes | 208 | `a9249ed9a69640d5ae5fb11f27866cb4363d3161ae554eab1d592190b3c4e3db` |
| `w12-actor-spell-a-tf-15glowingcylinder` | `__tf15GlowingCylinder` | `0x003106d0`, 76 bytes | 92 | `93a218adf6f79ba5fb4c1d3cf76f3ed12337690e8683808dafcb03a50e068f36` |
| `w12-actor-spell-a-tf-15spellholyshield` | `__tf15SpellHolyShield` | `0x003190d0`, 112 bytes | 164 | `3620b14a9cbe3cb043e3acc4e8bdb01f97908ebcb0f46b38edf7f5b48c77179a` |
| `w12-actor-spell-a-tf-17vehiclebodysphere` | `__tf17VehicleBodySphere` | `0x00312c90`, 76 bytes | 92 | `98ab1a5f7e81442b779714392817dad51e2cdc736f7a94e572514dbe89b3ce9a` |
| `w12-actor-spell-a-tf-19spellhornomeneffect` | `__tf19SpellHornOmenEffect` | `0x0031a900`, 76 bytes | 92 | `8647531d55a4fcf5287f912d4de29cb2cac73a434b9f4c7a884dc77bfd4b7de1` |
| `w12-actor-spell-a-tf-22spellplanarsteeleffect` | `__tf22SpellPlanarSteelEffect` | `0x0031a890`, 112 bytes | 164 | `e66052c59b9234836df4ec91c49e002b0e29a247f29bec29901cba58e4827abe` |
| `w12-actor-spell-a-tf-5phice` | `__tf5PhIce` | `0x0030de38`, 176 bytes | 208 | `c518cf7acbeff2506196ce7079e7447eda47ef254e28b125e557c2d2c9dee1c3` |
| `w12-actor-spell-a-tf-5spell` | `__tf5Spell` | `0x00314950`, 60 bytes | 72 | `471509dfe2adaa2f46c94503e7b8b7c43492934b096fb7da6b5ca63e081a6346` |
| `w12-actor-spell-a-tf-7oniball` | `__tf7OniBall` | `0x00310da0`, 132 bytes | 184 | `4f3f8de71dc0caa5edca30579c3d1ce930395a789e8b34b7197985919e721fdb` |
| `w12-actor-spell-a-tf-7weather` | `__tf7Weather` | `0x00312778`, 76 bytes | 92 | `caa100b4b54c554052c511af73eefa23a80c37d3cdb6a36894eafd2c2b12d61b` |
| `w12-actor-spell-a-vtable-mummy-snake` | `_vt$10MummySnake` | `0x0044f888`, 144 bytes | data | `0b28263753d7d1703e19b75aeae6114f0ef04e9b146bc141287ac32e5fc3e5d5` |
| `w12-actor-spell-a-wes-turn-to-face-edge` | `TurnToFaceEdge__14WoodElfSoldier` | `0x00249780`, 788 bytes | 552 | `7eba9cd83b82fec5e4cdbbacb6acb104b23a932f5ba6a8c7bb6be0cbeeb6d5d3` |

## Checks performed

Every unit has a `host_check.cpp` (not an EE input; it includes the source and replaces callees with recording stubs;
it was built with `clang++ -std=c++17` and exits non-zero on a wrong call order, argument, store or return). The
expected logs of the larger units (FireBombletJr, TurnToFaceEdge, Critter, Skeleton, StaticFire) are rebuilt in the
check by a separately written model, not copied from the source.

Every function except the table was also compared with the original in a private paired-run harness (an R5900
interpreter that runs the original function from the image and the freshly compiled authored function from identical
memory snapshots, with every callee replaced by the same Python stub). Calls with their arguments (registers, stack
slots, float registers), object snapshots at each call, the return where meaningful, and every changed byte outside the
stack were compared; `HALT` never occurred. The harness lives outside git (`priv-w12-actor-spell-a/`).

| function | scenarios | original words covered | notes |
|---|---|---|---|
| the 11 `__tf` functions | every combination of node states x 4 word patterns | all | |
| the 8 destructors | 9 flag values x 2 fills | all but 1 padding nop in the five that end in a tail jump | |
| GutChunk (model/animation ctor) | 10 | 40 of 40 | PhProp call arguments incl. stack slots and float register |
| WebTrap (two positions) | 8 | 100 of 100 | |
| PhProp (model and texture) | 12 | 122 of 122 | |
| StaticFire (box search) | 16 | 163 of 166 | uncovered: the null-entry typeid throw, unreachable |
| ItemProp (description and matrix) | 10 | 199 of 199 | incl. a callee changing halfword +320 |
| FireBombletJr | 40 | 260 of 260 | random sync-generator states |
| TurnToFaceEdge | 44 | 196 of 197 | every combination of the four ground probes + near-threshold heights |
| Critter | 48 | 474 of 486 | uncovered words are nops |
| Skeleton | 100 | 649 of 660 | uncovered words are nops |

Each paired script was also run against deliberate mutations of the source (offsets, constants, strings, branch
conditions, roll arithmetic); all but the exceptions listed under Unknown were caught, and the mutated files were
restored before commit. The scripts are not committed (private tooling).

A private partial build (shared selection plus these 29 units, plan copy kept outside git) produced 5,142 compiled
inputs, 3,937 function bodies, no COMMON symbols and **809 unresolved imports**; none of the 28 assigned names nor
`_vt$10MummySnake` is among them.
`python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

Plan helper: `python3 docs/agents/w12-actor-spell-a-plan-entries.py --toolchain /Users/case/norrath-agents/repo/work/toolchains/ee [--private-root DIR]`
prints the entries below (and optionally writes a private plan/snapshot copy outside git).

## Observed behavior worth knowing (details are in the source comments)

- `LooseIceChunk` and `PhIce` destructors store `_vt$4Prop`, not their own table, before calling the GameObject destructor.
- `__tf5Spell` calls `__rtti_user` (a root with no base); the Spell effect classes either inline that call
  (`SpellCharge`, `SpellHolyShield`, `SpellPlanarSteelEffect`) or call `__tf5Spell` (`SpellHornOmenEffect`).
- `TurnToFaceEdge__14WoodElfSoldier`: the third and fourth ground probes are added to the previous probe point, not to
  the object position (the first two are relative to the position). This is the observed behavior and may be unintended.
- `FireBombletJr`: the float at +84 is both the z slot of the vector at +76 and a separately computed value; the later
  store wins.
- `ItemProp` (matrix overload): the branches testing halfword +320 are only reachable if a callee changes it, since it is
  stored as 1 shortly before.
- `Critter`: the original keeps three 16-byte stack slots for the name copy, lump name and resource name and overruns them
  for names of 12 or more characters; the authored version uses larger buffers.

## Imports closed

`__7CritterPcG6Point3sPPc`, `__13FireBombletJrP11FireBomblet`, `__8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif`, `__8ItemPropG6Point3PQ24item11descriptionP8Matrix34`, `__6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf`, `__8SkeletonG6Point3sPPcPci`, `__10StaticFireG6Point3fif`, `__7WebTrapG6Point3T1`, `_$_13LooseIceChunk`, `_$_15GlowingCylinder`, `_$_17VehicleBodySphere`, `_$_19SpellHornOmenEffect`, `_$_22SpellPlanarSteelEffect`, `_$_5PhIce`, `_$_7OniBall`, `_$_7Weather`, `__tf11SpellCharge`, `__tf13LooseIceChunk`, `__tf15GlowingCylinder`, `__tf15SpellHolyShield`, `__tf17VehicleBodySphere`, `__tf19SpellHornOmenEffect`, `__tf22SpellPlanarSteelEffect`, `__tf5PhIce`, `__tf5Spell`, `__tf7OniBall`, `__tf7Weather`, `TurnToFaceEdge__14WoodElfSoldier`, `_vt$10MummySnake`.

## Imports opened

New callee/data names introduced by these sources that no other source in the tree already uses (all are real image
symbols; their definitions belong to other packages): `__4PropG6Point3P12_modelHeaderP8_textureli` (Prop constructor,
model-and-texture overload, called by the PhProp unit), `_vt$13FireBombletJr` (FireBombletJr table),
`_$_10MummySnake`, `__tf10MummySnake`, `msg_run__10MummySnake` (slots of the MummySnake table).

Already imports of other packages and still unresolved in the check build, now also used here: `_ctype_`, `_vt$5Spell`,
`_vt$8Skeleton`, `bombResMap`, `init__10StaticFire`, `initType__8Skeletonb`, `skeletonResourceMap`.

## Build-plan entries needed

None of the 29 inputs is in `experimental/build-plan.json` (not edited by this package). Profile `authored_cpp`; header
closures include other packages' shared headers (`w11-ctor-a-common`, `w10-actor-m-common`) which the plan already lists.
Host checks are not plan inputs. Entries (also printed by the plan helper):

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 1996,
      "name": "__7CritterPcG6Point3sPPc"
    }
  ],
  "header_closure": [
    "sources/w11-ctor-a-common/ctor.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_critter.o",
  "profile": "authored_cpp",
  "sha256": "0fb19715776ec731bc53ae2c28373e07c395b1ec1491181e49eaafa16f16d75c",
  "source": "sources/w12-actor-spell-a-ctor-critter/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 956,
      "name": "__13FireBombletJrP11FireBomblet"
    }
  ],
  "header_closure": [
    "sources/w10-actor-m-common/access.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_firebomblet_jr.o",
  "profile": "authored_cpp",
  "sha256": "e582bba16c7fa27b6a836b6a1b15a79e50272f97a8b63bca96b8cd4466c99dab",
  "source": "sources/w12-actor-spell-a-ctor-firebomblet-jr/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 136,
      "name": "__8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif"
    }
  ],
  "header_closure": [
    "sources/w11-ctor-a-common/ctor.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_gutchunk.o",
  "profile": "authored_cpp",
  "sha256": "260b6a135dca50fc5c974df8f8fd7cde8b52c121099645b126a9cfc8c4db8255",
  "source": "sources/w12-actor-spell-a-ctor-gutchunk/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 656,
      "name": "__8ItemPropG6Point3PQ24item11descriptionP8Matrix34"
    }
  ],
  "header_closure": [
    "sources/w11-ctor-a-common/phprop.hpp",
    "sources/w11-ctor-a-common/ctor.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_itemprop_matrix.o",
  "profile": "authored_cpp",
  "sha256": "f2420afe3f98769e5a1dc87db5db1a5f8c5be69275c8ed5e90c4069e25d2e5e6",
  "source": "sources/w12-actor-spell-a-ctor-itemprop-matrix/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 496,
      "name": "__6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf"
    }
  ],
  "header_closure": [
    "sources/w11-ctor-a-common/phprop.hpp",
    "sources/w11-ctor-a-common/ctor.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_phprop_texture.o",
  "profile": "authored_cpp",
  "sha256": "15ac3610ceb01dcf40156934c7d0d35ee799366a0a94dbcb784ab05e8884d7d9",
  "source": "sources/w12-actor-spell-a-ctor-phprop-texture/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 2588,
      "name": "__8SkeletonG6Point3sPPcPci"
    }
  ],
  "header_closure": [
    "sources/w10-actor-m-common/access.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_skeleton.o",
  "profile": "authored_cpp",
  "sha256": "d7c26aa80ee755a3ac8e6108e52aea92acb74b1289a863bbebcc96105017bdf5",
  "source": "sources/w12-actor-spell-a-ctor-skeleton/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 560,
      "name": "__10StaticFireG6Point3fif"
    }
  ],
  "header_closure": [
    "sources/w10-actor-m-common/access.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_staticfire_box.o",
  "profile": "authored_cpp",
  "sha256": "d8365fdcf9c7bb7eb169a399004f0c70a6fb29183ce5eb576e6b287853afa46a",
  "source": "sources/w12-actor-spell-a-ctor-staticfire-box/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 336,
      "name": "__7WebTrapG6Point3T1"
    }
  ],
  "header_closure": [
    "sources/w11-ctor-a-common/ctor.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_ctor_webtrap_points.o",
  "profile": "authored_cpp",
  "sha256": "683510e3a8800e1312bd42c7b87b604d48abfaec558a1cb69ae5a254234e64c0",
  "source": "sources/w12-actor-spell-a-ctor-webtrap-points/ctor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 88,
      "name": "_$_13LooseIceChunk"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_13looseicechunk.o",
  "profile": "authored_cpp",
  "sha256": "38a2cc4e2fd786eecbdd3e2b01ef77b3b77232ed4e2166a8f9e2d13cff837709",
  "source": "sources/w12-actor-spell-a-dtor-13looseicechunk/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 88,
      "name": "_$_15GlowingCylinder"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_15glowingcylinder.o",
  "profile": "authored_cpp",
  "sha256": "c759ae6e81bd83f8593d1d1adbea3b6260024ee1833afaa0b75c7b377a13cba6",
  "source": "sources/w12-actor-spell-a-dtor-15glowingcylinder/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 88,
      "name": "_$_17VehicleBodySphere"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_17vehiclebodysphere.o",
  "profile": "authored_cpp",
  "sha256": "198155d820c2e87f3f8a7822d1bb9ce96d94a522e5a1c4399701961413ecb7b5",
  "source": "sources/w12-actor-spell-a-dtor-17vehiclebodysphere/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 116,
      "name": "_$_19SpellHornOmenEffect"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_19spellhornomeneffect.o",
  "profile": "authored_cpp",
  "sha256": "e934b7035ada7f3d5eab82e90566f1b2c2e8047d6b63391ec710dfcf1f1bf57b",
  "source": "sources/w12-actor-spell-a-dtor-19spellhornomeneffect/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 136,
      "name": "_$_22SpellPlanarSteelEffect"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_22spellplanarsteeleffect.o",
  "profile": "authored_cpp",
  "sha256": "250b84993a4805fd491562f3a62b2b42f726a0ad5d0fbca78feff3b958d0d67f",
  "source": "sources/w12-actor-spell-a-dtor-22spellplanarsteeleffect/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 88,
      "name": "_$_5PhIce"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_5phice.o",
  "profile": "authored_cpp",
  "sha256": "124af7982cf1856cb07ed17d5a5cd09c2f831a9f4bf8cbb5b234f50ba05fdc23",
  "source": "sources/w12-actor-spell-a-dtor-5phice/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 136,
      "name": "_$_7OniBall"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_7oniball.o",
  "profile": "authored_cpp",
  "sha256": "be46e11a44058125f066b75a8083b525ce63f7f5b8571f94ccd9f90fee7ac944",
  "source": "sources/w12-actor-spell-a-dtor-7oniball/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 88,
      "name": "_$_7Weather"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_dtor_7weather.o",
  "profile": "authored_cpp",
  "sha256": "2dec545fe9141c3717a0d8cffea17488641d75bc1f720f17decd9cc8f463193f",
  "source": "sources/w12-actor-spell-a-dtor-7weather/dtor.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 164,
      "name": "__tf11SpellCharge"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_11spellcharge.o",
  "profile": "authored_cpp",
  "sha256": "e03c69fe5553c465347fe7af9ff4f10e7f643c1547a07e606558a6a5a8fec6c5",
  "source": "sources/w12-actor-spell-a-tf-11spellcharge/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 208,
      "name": "__tf13LooseIceChunk"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_13looseicechunk.o",
  "profile": "authored_cpp",
  "sha256": "a9249ed9a69640d5ae5fb11f27866cb4363d3161ae554eab1d592190b3c4e3db",
  "source": "sources/w12-actor-spell-a-tf-13looseicechunk/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 92,
      "name": "__tf15GlowingCylinder"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_15glowingcylinder.o",
  "profile": "authored_cpp",
  "sha256": "93a218adf6f79ba5fb4c1d3cf76f3ed12337690e8683808dafcb03a50e068f36",
  "source": "sources/w12-actor-spell-a-tf-15glowingcylinder/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 164,
      "name": "__tf15SpellHolyShield"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_15spellholyshield.o",
  "profile": "authored_cpp",
  "sha256": "3620b14a9cbe3cb043e3acc4e8bdb01f97908ebcb0f46b38edf7f5b48c77179a",
  "source": "sources/w12-actor-spell-a-tf-15spellholyshield/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 92,
      "name": "__tf17VehicleBodySphere"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_17vehiclebodysphere.o",
  "profile": "authored_cpp",
  "sha256": "98ab1a5f7e81442b779714392817dad51e2cdc736f7a94e572514dbe89b3ce9a",
  "source": "sources/w12-actor-spell-a-tf-17vehiclebodysphere/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 92,
      "name": "__tf19SpellHornOmenEffect"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_19spellhornomeneffect.o",
  "profile": "authored_cpp",
  "sha256": "8647531d55a4fcf5287f912d4de29cb2cac73a434b9f4c7a884dc77bfd4b7de1",
  "source": "sources/w12-actor-spell-a-tf-19spellhornomeneffect/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 164,
      "name": "__tf22SpellPlanarSteelEffect"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_22spellplanarsteeleffect.o",
  "profile": "authored_cpp",
  "sha256": "e66052c59b9234836df4ec91c49e002b0e29a247f29bec29901cba58e4827abe",
  "source": "sources/w12-actor-spell-a-tf-22spellplanarsteeleffect/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 208,
      "name": "__tf5PhIce"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_5phice.o",
  "profile": "authored_cpp",
  "sha256": "c518cf7acbeff2506196ce7079e7447eda47ef254e28b125e557c2d2c9dee1c3",
  "source": "sources/w12-actor-spell-a-tf-5phice/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 72,
      "name": "__tf5Spell"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_5spell.o",
  "profile": "authored_cpp",
  "sha256": "471509dfe2adaa2f46c94503e7b8b7c43492934b096fb7da6b5ca63e081a6346",
  "source": "sources/w12-actor-spell-a-tf-5spell/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 184,
      "name": "__tf7OniBall"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_7oniball.o",
  "profile": "authored_cpp",
  "sha256": "4f3f8de71dc0caa5edca30579c3d1ce930395a789e8b34b7197985919e721fdb",
  "source": "sources/w12-actor-spell-a-tf-7oniball/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 92,
      "name": "__tf7Weather"
    }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_tf_7weather.o",
  "profile": "authored_cpp",
  "sha256": "caa100b4b54c554052c511af73eefa23a80c37d3cdb6a36894eafd2c2b12d61b",
  "source": "sources/w12-actor-spell-a-tf-7weather/typeinfo.cpp"
}
```

```json
{
  "emitted_functions": [],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w12asa_vtable_mummy_snake.o",
  "profile": "authored_cpp",
  "sha256": "0b28263753d7d1703e19b75aeae6114f0ef04e9b146bc141287ac32e5fc3e5d5",
  "source": "sources/w12-actor-spell-a-vtable-mummy-snake/vt_mummy_snake.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 552,
      "name": "TurnToFaceEdge__14WoodElfSoldier"
    }
  ],
  "header_closure": [
    "sources/w10-actor-m-common/access.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w12asa_wes_turn_to_face_edge.o",
  "profile": "authored_cpp",
  "sha256": "7eba9cd83b82fec5e4cdbbacb6acb104b23a932f5ba6a8c7bb6be0cbeeb6d5d3",
  "source": "sources/w12-actor-spell-a-wes-turn-to-face-edge/turn.cpp"
}
```

## Authored but never executed

Everything. No function ran on an EE, on hardware or inside the game. The sources were compiled for the EE and exercised
only (a) on the host against recording stubs and (b) in the private R5900 interpreter next to the original function with
stubbed callees. The stubs replace the real callees, so nothing here shows that a callee behaves as assumed; virtual
dispatch (`TurnToFaceEdge`, the StaticFire box search type tests) used fabricated tables.

## Unknown

- The meaning of nearly every object field named only by offset (the sources say which offsets they touch and nothing more).
- Whether the additive third/fourth probes of `TurnToFaceEdge` are intended, what the angles 0/16384/32767/-16384 and the
  radius 10.0 mean to `worldFindStandHeight`, and the exact slot name at table byte 176 (matched to the Creature `turnToFace`
  only by position and signature).
- One-ulp changes of the five Skeleton variant thresholds (and the FireBombletJr 0.707 threshold) cannot be told apart by any
  integer roll or by the scenarios used, so the exact bit patterns are taken from the image, not validated by behavior.
- `Critter` with names of 12 or more characters (the original overruns stack slots; not reproduced).
- The return register of the constructors (finishConstruction's leftover); the sources declare `void`.
- Whether the null-entry `__throw_bad_typeid` call in the StaticFire box search is reachable (the list ends at a null word).
- Retail edition and any behavior outside the single private image above.
