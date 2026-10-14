# w14-esfparse: ESF resource object parsers

Branch `agents/w14-esfparse`. One commit per function; each source directory is `experimental/sources/w14-esfparse-<name>/` and
holds the plan input (`parse_*.cpp`) plus a native `host_contract.cpp` (not a plan input). Addresses and sizes are from the
pinned private `SLUS_205.65` symbol table. Nothing here ran on the EE or as gameplay; the compiled sizes are the reviewed EE
`authored_cpp` objects and are not claims of byte identity (the compiler orders and schedules differently from the image, e.g.
the image's unaligned `ldl/ldr` structure copies and branch-likely delay slots).

## How each function was checked

* Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags and links in a private partial build of the shared
  tip plus these 24 units (`--check-plan` passes: 6,259 selected inputs).
* Every unit has a native host contract (`c++ -std=c++17 -Wall -Wextra -Werror`, exit 0) with stubbed callees that records the
  exact call order and arguments, the values stored into the output records, and every early-return branch.
* Every unit was compared with the image in a private paired run on an R5900 interpreter kept outside git: the original function
  and the freshly compiled authored function run from identical memory snapshots, with identical Python stubs for every callee
  (seeded results by call index, including output parameters written through pointers), and the call logs (arguments, the
  contents of structures passed by pointer, float arguments), the return value and every changed byte outside the stack must be
  equal. Scenarios were scripted per function (the tag sequence, `ObjectVersion`, result pools, failure injection at each call);
  instruction coverage of the original was measured and all uncovered words are `nop`s, delay-slot filler or unreachable code.
  A halted run counts as a failure.
* Between 15 and 70 deliberately wrong variants per unit (constants, offsets, comparison senses, call order, stored fields,
  version gates) were run through the same comparison; all of them fail (a few first attempts died only on compile errors or
  matched the header comment text instead of the code and were redone precisely).
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
* Private partial build (shared tip plus these 24 units, 6,259 inputs, one link, no retries): `partial_build_complete`, 4,945
  function bodies, zero COMMON symbols, 1,036 unresolved imports. Against the wave-14 baseline list (957 names) exactly the 24
  names of this package disappear and exactly 103 new names appear (the opened imports below); no other change.

## Units

| Function | Image address | Image bytes | Compiled bytes | Directory |
|---|---|---|---|---|
| `ParseSurfaceArrayObj__10VIESFParse` | 0x0117abe0 | 124 | 156 | `w14-esfparse-surfacearray` |
| `ParseWorldObj__10VIESFParsei` | 0x011760d8 | 168 | 224 | `w14-esfparse-worldobj` |
| `ParsePointSpriteObj__10VIESFParse` | 0x011785d0 | 276 | 312 | `w14-esfparse-pointsprite` |
| `ParseActorObj__10VIESFParse` | 0x01176ac8 | 408 | 368 | `w14-esfparse-actorobj` |
| `ParseResourceDirObj__10VIESFParseiR13VIResourceDir` | 0x01176c60 | 228 | 264 | `w14-esfparse-resourcedirobj` |
| `ParseStreamAudioSpriteObj__10VIESFParse` | 0x011786e8 | 440 | 412 | `w14-esfparse-streamaudiosprite` |
| `ParsePointLightObj__10VIESFParse` | 0x01177e78 | 492 | 448 | `w14-esfparse-pointlight` |
| `ParseAdpcmObj__10VIESFParse` | 0x011736a0 | 420 | 448 | `w14-esfparse-adpcmobj` |
| `ParseXmObj__10VIESFParse` | 0x01173848 | 472 | 480 | `w14-esfparse-xmobj` |
| `ParseSoundSpriteObj__10VIESFParse` | 0x01178068 | 580 | 560 | `w14-esfparse-soundsprite` |
| `ParseParticleDefinitionObj__10VIESFParse` | 0x01178f08 | 480 | 528 | `w14-esfparse-particledefinitionobj` |
| `ParseGroupSpriteObj__10VIESFParse` | 0x01177af0 | 536 | 536 | `w14-esfparse-groupsprite` |
| `ParseLODSpriteObj__10VIESFParse` | 0x011772e8 | 648 | 652 | `w14-esfparse-lodsprite` |
| `ParseFloraSpriteObj__10VIESFParse` | 0x011788a0 | 688 | 648 | `w14-esfparse-floraspriteobj` |
| `ParseParticleSpriteObj__10VIESFParse` | 0x011782b0 | 796 | 716 | `w14-esfparse-particlespriteobj` |
| `ParseEffectVolumeSpriteObj__10VIESFParse` | 0x01178b50 | 948 | 956 | `w14-esfparse-effectvolumespriteobj` |
| `ParseMaterialPalObj__10VIESFParse` | 0x01171a28 | 644 | 744 | `w14-esfparse-materialpalobj` |
| `ParseSurfaceObj__10VIESFParse` | 0x01171640 | 996 | 1076 | `w14-esfparse-surfaceobj` |
| `ParseFontObj__10VIESFParse` | 0x01176e48 | 1180 | 1180 | `w14-esfparse-fontobj` |
| `ParseSimpleSpriteObj__10VIESFParse` | 0x01172128 | 1032 | 976 | `w14-esfparse-simplespriteobj` |
| `ParseHSpriteObj__10VIESFParse` | 0x01172530 | 1016 | 992 | `w14-esfparse-hspriteobj` |
| `ParseHSpriteAnimObj__10VIESFParse` | 0x01172f60 | 1856 | 1220 | `w14-esfparse-hspriteanimobj` |
| `ParseCSpriteObj__10VIESFParse` | 0x01173bc8 | 1676 | 1648 | `w14-esfparse-cspriteobj` |
| `ParseSpellEffectObj__10VIESFParse` | 0x01179568 | 1140 | 1128 | `w14-esfparse-spelleffectobj` |

## Observed, inferred, unknown (shared)

* Observed: all 24 handlers take the ESF parser record by pointer and use the same word offsets: +0 creation argument passed to
  the scene/raster create calls, +4 progress-callback word and +8 its argument (called as `callback(arg)` by the surface, ADPCM,
  XM, font and animation parsers), +12 raster pointer (a plain flag word for the handlers that do not use it as a raster), +16
  sound device, +20 collision system, +24 scene, +32 dictionary, +36 object file, +40 a scratch area that receives palettes.
  The dispatcher reading the 16-bit tag lives in `ParseObject` (already authored); these handlers read their own header with
  `ReadBegin`, most do not examine the tag, and end with `ReadEnd`.
* Observed: the dictionary resource types used with `Find`/`Add` are 1 surface, 3 material palette, 4 simple sprite, 5
  hierarchical sprite, 6 hierarchical sprite animation, 9 character sprite, 12 font, 14 sound (ADPCM and XM), 15 LOD sprite, 16
  point light, 17 sound sprite, 18 particle definition, 19 particle sprite, 20 group sprite, 21 spell effect, 23 point sprite, 24
  stream audio sprite, 25 flora sprite, 26 effect volume sprite. Sprite creation types: 1 simple, 2 point, 4 hierarchical, 5
  character, 7 LOD, 8 point light, 9 sound, 10 particle, 11 group, 12 stream audio, 13 flora, 14 effect volume. Object tags
  handled inside: 0x1000 surface, 0x1001 surface array, 0x1100/0x1101/0x1111 material palette entry/list/object, 0x1110 palette
  header, 0xc230/0xc320/0xc330 nested resources and parameter blocks, 0xb010/0xb020 ADPCM header and data, 0xb040/0xb060 XM.
* Observed: a common prologue appears in the sprite parsers (id, six-float box, dictionary reuse, create, register, fill the
  sprite's box at +8..+28 and a sphere at +32). The ADPCM and XM parsers read their data blocks and drop them; for id 0 their
  original return is an unset stack slot (this code returns 0). The font parser returns 0 on success (the handle only on a
  dictionary hit). The spell effect parser returns the last event lookup's handle when there was one. Point-light and
  sound-sprite parsers write an inline box around a unit-style sphere with the x lane computed as a negation (preserving the sign
  of a zero radius).
* Inferred: the names of resource types and tags above (they are labels derived from the handler names and their callees,
  not from symbols); field meanings; that `ParseWorldObj`'s flag chooses between "base only" and "base plus zones".
* Unknown: the layouts of VIScene, VIRaster, VIDictionary and the sprite classes (only the stored ranges are known, so the
  sources use byte/word offsets), the meaning of the second ReadBegin output in most handlers, the roles of several floats and
  flag words, the enumerators behind format/depth/playback arguments, and the bodies of every callee.

## Imports closed

`ParseActorObj__10VIESFParse`, `ParseAdpcmObj__10VIESFParse`, `ParseCSpriteObj__10VIESFParse`, `ParseEffectVolumeSpriteObj__10VIESFParse`, `ParseFloraSpriteObj__10VIESFParse`, `ParseFontObj__10VIESFParse`, `ParseGroupSpriteObj__10VIESFParse`, `ParseHSpriteAnimObj__10VIESFParse`, `ParseHSpriteObj__10VIESFParse`, `ParseLODSpriteObj__10VIESFParse`, `ParseMaterialPalObj__10VIESFParse`, `ParseParticleDefinitionObj__10VIESFParse`, `ParseParticleSpriteObj__10VIESFParse`, `ParsePointLightObj__10VIESFParse`, `ParsePointSpriteObj__10VIESFParse`, `ParseResourceDirObj__10VIESFParseiR13VIResourceDir`, `ParseSimpleSpriteObj__10VIESFParse`, `ParseSoundSpriteObj__10VIESFParse`, `ParseSpellEffectObj__10VIESFParse`, `ParseStreamAudioSpriteObj__10VIESFParse`, `ParseSurfaceArrayObj__10VIESFParse`, `ParseSurfaceObj__10VIESFParse`, `ParseWorldObj__10VIESFParsei`, `ParseXmObj__10VIESFParse`

## Imports opened

The units declare 118 callee symbols other than the 24 above. Six resolve to functions the shared tip already defines
(`ParseObject__10VIESFParse`, `CreateSprite__7VIScene12VISpriteTypei`, `Sprite__7VIScenei`, `GetMaterialPalEntry__8VIRasterii`,
`__builtin_vec_new`, `__builtin_vec_delete`). The other 112 are unresolved imports; 9 of them were already imports in the
wave-14 baseline (`Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition`, `Find__12VIDictionaryUiR14VIResourceTypeRi`, `Init__8VISphereRC6VIBBox`, `LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc`, `ParticleDefinition__16VIParticleSystemi`, `ReadBegin__9VIObjFileRUsT1`, `ReadEnd__9VIObjFile`, `Surface__8VIRasteri`, `UnlockMipLevel__9VISurfacei`) and 103 were not. The 103 names newly opened (they are other packages' or later waves' work; none is
stubbed here):

`AddPlay__9VIHSpriteiR7VIScene`, `Add__12VIDictionaryR10VIResourceUi14VIResourceTypei`, `Animation__7VIScenei`, `AttachSurface__14VIEffectVolumeii`, `BeginChar__7VIRFontUsRiPPUc`, `CalcLODOffDist__9VICSpriteR7VIScene`, `Convert16__10VIESFParseR12VIPixelStorePUcT2i`, `Convert24__10VIESFParseR12VIPixelStorePUcT2i`, `Convert32__10VIESFParseR12VIPixelStorePUcT2i`, `Convert4__10VIESFParseR12VIPixelStorePUcT2i`, `Convert8__10VIESFParseR12VIPixelStorePUcT2i`, `CreateActor__7VISceneRC7VIVect3T1fi`, `CreateAnimation__7VIScenei`, `CreateFont__8VIRasteri`, `CreateMaterialPal__8VIRasterii`, `CreateParticleDefinition__16VIParticleSystemi`, `CreateParticleEmitters__9VICSpriteR7VIScene`, `CreateSpellEffect__19VISpellEffectSystemi`, `CreateSurface__8VIRasteri`, `Create__13VISpellEffectP7VIScene`, `Create__16VISpellEventListiP7VIScene`, `EndChar__7VIRFont`, `Font__8VIRasteri`, `InitTextSlots__9VICSpriteR8VIRaster`, `Init__13VIHSpriteAnim19VIHSpriteAnimFormatiiiff14VIPlaybackType`, `Init__13VIPointSprite11VIPointType`, `Init__19VIStreamAudioSpriteRC6VIBBox`, `Init__20VIEffectVolumeSpriteRC6VIBBox`, `Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi`, `Init__9VICSprite17VICSpriteSkelType13VICSpriteRace12VICSpriteSexfi`, `Init__9VIHSprite`, `Init__9VISurfaceR8VIRasterii10VIBitDepthi`, `IsPalettized__7VIRFont`, `LockPalette__9VISurfaceRiR12VIPixelStorePPUc`, `Lock__7VIRFontR8VIRaster`, `MaterialPal__8VIRasteri`, `NumSubObjects__9VIObjFile`, `ObjectSize__9VIObjFile`, `ObjectVersion__9VIObjFile`, `Palette__7VIRFontRiR12VIPixelStorePPUc`, `ParseCSpriteASlotList__10VIESFParseR9VICSprite`, `ParseCSpriteContSound__10VIESFParseR9VICSprite`, `ParseCSpriteNodeIDList__10VIESFParseR9VICSprite`, `ParseCSpritePartDefinitions__10VIESFParse`, `ParseCSpritePartEmitters__10VIESFParseR9VICSprite`, `ParseCSpritePlayList__10VIESFParseR9VICSprite`, `ParseCSpriteSkinList__10VIESFParseR9VICSprite`, `ParseCSpriteSpriteArray__10VIESFParse`, `ParseCSpriteTSlotList__10VIESFParseR9VICSprite`, `ParseCollBuffer__10VIESFParse`, `ParseFloraPrimBuffer__10VIESFParse`, `ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite`, `ParseGroupSpriteSpriteArray__10VIESFParse`, `ParseHSpriteAnimArray__10VIESFParse`, `ParseHSpriteAnim__10VIESFParse`, `ParseHSpriteAttachments__10VIESFParseR9VIHSprite`, `ParseHSpriteHierarchy__10VIESFParseR9VIHSprite`, `ParseHSpriteSpriteArray__10VIESFParse`, `ParseHSpriteTriggers__10VIESFParseR9VIHSprite`, `ParseLODSpriteLevels__10VIESFParseR11VILODSprite`, `ParseLODSpriteSpriteArray__10VIESFParse`, `ParseMaterial__10VIESFParseP10VIMaterial`, `ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition`, `ParsePrimBuffer__10VIESFParse`, `ParseRefMap__10VIESFParse`, `ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem`, `ParseSoundArray__10VIESFParse`, `ParseSound__10VIESFParse`, `ParseSurface__10VIESFParse`, `ParseWorldBase__10VIESFParseR7VIWorld`, `ParseWorldZones__10VIESFParseR7VIWorld`, `PixelStore__7VIRFont`, `ReadString__9VIObjFilePcUs`, `Read__9VIObjFilePUci`, `Read__9VIObjFileRUc`, `Read__9VIObjFileRUi`, `Read__9VIObjFileRUs`, `Read__9VIObjFileRf`, `Read__9VIObjFileRi`, `Read__9VIObjFileRs`, `SetAnimPriorities__9VICSprite`, `SetAnimSoundChannels__9VICSprite`, `SetDefaultEntries__9VICSprite`, `SetDefaults__9VICSpriteR7VIScene`, `SetFlags__14VIEffectVolumeUi`, `SetKeyframe__13VIHSpriteAnimiif`, `SetNodeFrame__13VIHSpriteAnimiiR14VIHSpriteFrame`, `SetNodeFrame__13VIHSpriteAnimiiR18VIHSpritePackFrame`, `SetNodeRefID__13VIHSpriteAnimii`, `SetType__14VIEffectVolume18VIEffectVolumeTypef`, `ShareAnimation__7VIScenei`, `ShareCollBuffer__9VICollidei`, `ShareMaterialPal__8VIRasteri`, `ShareParticleDefinition__16VIParticleSystemi`, `SharePrimBuffer__8VIRasteri`, `ShareRefMap__7VIScenei`, `ShareSound__13VISoundDevicei`, `ShareSprite__7VIScenei`, `Share__9VICSpriteR7VISceneP8VIRaster`, `SpellEffect__19VISpellEffectSystemi`, `StaticBounds__C17VIParticleEmitter`, `UnlockPalette__9VISurface`, `Unlock__7VIRFontR8VIRaster`

Non-leaf callees whose bodies are
sizeable and not authored here: the `ParseHSprite*`, `ParseCSprite*`, `ParseGroupSprite*`, `ParseLODSprite*`, `ParseWorld*`,
`ParseResourceTable`, `ParseSurface`, `ParsePrimBuffer`, `ParseCollBuffer`, `ParseFloraPrimBuffer`, `ParseRefMap`,
`ParseSound*` and `Convert4/8/16/24/32` families.

## Build-plan entries needed

`python3 docs/agents/w14-esfparse-plan-entries.py --toolchain /path/to/ee` prints the 24 JSON plan entries (one per directory,
in directory-name order, `profile` `authored_cpp`, no header closure, object names `w14esf_*.o`, each emitting exactly one
function) and `--private-root DIR` writes a private plan and snapshot copy that contain them (nothing is written inside the
repository). The shared `build-plan.json` and `snapshot.json` are not edited here; the lead integrator merges the printed
entries. `input_order` values assume this branch's tip.

## Authored but never executed

Nothing ran on the EE. Every function ran only on the private R5900 interpreter (against stubbed callees) and, natively, in
host contracts. Not covered: real callee behaviour (all stubbed), hardware timing, the VU/GS state those callees touch, the real
object file bytes (no ESF file was parsed end to end), and the two version-gated paths whose gating variable comes from the
file (`ObjectVersion`) beyond the values 0..8 used in the tests. The 24 handlers have never been called by a running game.

## Unknown / not done

* The callees listed above are not authored here, so no ESF file can be parsed end to end from this branch alone.
* Not attempted: `ParseObject`'s dispatcher (already authored), the helper parsers named above, and the data tables
  (`_vt` and similar) that other code may need.
* Field names, resource-type names and the layout of the parser record beyond the word offsets above remain unconfirmed.
