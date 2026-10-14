# Scene, world, collide, zone and vector math (1 of 2)

Package notes for `agents/w15-vi-scene-a`. Addresses are from the private
hash-pinned `SLUS_205.65` symbol table and are local evidence only. Each of the
22 functions below is its own commit, lives in
`experimental/sources/w15-vi-scene-a-<name>/` with a `host_check.cpp` that
returns non-zero when the behaviour it checks is wrong, and was compiled with
the reviewed `authored_cpp` profile. None of the compiled bodies is claimed as
an original-byte match. The per-source header comments carry the
observed / inferred / unknown split; this file summarises it.

Checks run when this file was written: every `host_check.cpp` built with the
host compiler and exited 0; `python3 -m unittest discover -s tools -p
'test_*.py'` ran 214 tests, OK. No partial build was run by this pass.

## Functions authored

| Symbol | Address | Bytes |
|---|---|---|
| `Transpose__10VIMatrix44RC10VIMatrix44` | 0x0109b180 | 88 |
| `__10VIZoneRoom` | 0x0111ef38 | 116 |
| `SetLevelSprite__11VILODSpriteiiR7VIScene` | 0x01076840 | 124 |
| `FilterActor__13VIActorFilterR7VIScenei` | 0x0105ec28 | 16 |
| `FilterActor__13VISceneFilterR7VIScenei` | 0x01138350 | 16 |
| `QueryEmitters__6VIZoneR23VISceneEmitterQueryVars` | 0x01119c38 | 68 |
| `RenderByVolume__6VIZoneiR15VISceneRendVars` | 0x011155b0 | 628 |
| `GetLightUISprite__7VIScene` | 0x01144d10 | 8 |
| `GetSoundUISprite__7VIScene` | 0x01144d28 | 8 |
| `GetPointUISprite__7VIScene` | 0x01144d58 | 8 |
| `GetStreamAudioUISprite__7VIScene` | 0x01144d70 | 8 |
| `GetNextColorBuffer__7VIScene` | 0x01145158 | 112 |
| `PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri` | 0x01145b10 | 120 |
| `RasterUISprite__7VISceneR8VICameraiUi` | 0x01144c60 | 152 |
| `CreateActor__7VISceneRC7VIVect3T1fi` | 0x0113ed08 | 500 |
| `Animation__7VIScenei` | 0x01144e00 | 28 |
| `CreateAnimation__7VIScenei` | 0x01144d78 | 132 |
| `GetSpriteType__7VIScenei` | 0x01144b88 | 32 |
| `ReleaseAnimation__7VIScenei` | 0x01144e50 | 172 |
| `ReleaseRefMap__7VIScenei` | 0x0113ce58 | 180 |
| `ReleaseSprite__7VIScenei` | 0x0113cd48 | 272 |
| `ReleaseStaticLighting__7VIScenei` | 0x0113cf10 | 216 |

## Scene layout observed through these functions

Offsets into the scene object, as used by the authored code: sprite pool at
+996, animation pool at +1020, ref-map pool at +1044, static-lighting pool at
+1068, current static-lighting index at +1092 with a cursor counter beside it
at +1096, and four built-in interface sprite indices at +1128 (sound), +1132
(light), +1140 (point) and +1144 (stream audio). The roles of the pools are
inferred from which functions add to and erase from them. Reference counts at
record +4 and a dictionary-registered marker at record +0 are inferred from
the release functions.

## Build-plan entries needed

Not edited in the shared plan. `docs/agents/w15-vi-scene-a-plan-entries.py
--toolchain <ee> [--private-root DIR]` prints the 22 entries (kind
`independently_authored_source`, profile `authored_cpp`, object names
`w15vsa_*.o`, empty header closure) and can write a private plan copy outside
git. The integrator merges them.

## Imports closed

All 22 names above, which are the 22 symbols of this package's list.

## Imports opened

Referenced but not authored here (other packages or later work):
`Add__t6VIList1Z7VIActor`,
`Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim`,
`Clear__10VIZoneRoom`,
`Erase__t6VIPool1ZP13VIHSpriteAnimi`, `Erase__t6VIPool1ZP16VIStaticLightingi`,
`Erase__t6VIPool1ZP8VIRefMapi`, `Erase__t6VIPool1ZP8VISpritei`,
`QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars`,
`ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary`,
`Remove__12VIDictionaryR10VIResource`, `RenderLeaf__6VIZoneiR15VISceneRendVars`,
`ShareSprite__7VIScenei`, `Sprite__7VIScenei`, `UpdateActor__7VIScenei`,
`__13VIHSpriteAnim`, `__t7VIArray1Zi`, `__builtin_new`, `__builtin_delete`.
This list was read from the source declarations; a partial build would confirm
it against `unresolved-imports.json`, and was not run.

## Authored but never executed

Everything. The only execution is the host-side contract per function,
against synthetic memory; no function ran inside the original or an emulated
game image here, and no paired run against the original bytes was done.

## Unknown

Field names and sizes of the scene, zone, room, sprite, animation, ref-map and
static-lighting records beyond the offsets above; the meaning of the sprite
type tag values; what the flag bits in `QueryEmitters` and `RenderByVolume`
node values mean beyond the traversal shown; when the four interface sprite
indices are assigned; whether `Transpose` is ever called with the same matrix
as source and destination; the template arguments and destructor flag meanings
in the release functions.
