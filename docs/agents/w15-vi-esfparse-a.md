# w15-vi-esfparse-a: ESF resource parser handlers

Package: the 35 `VIESFParse` handlers that the wave-14 object parsers (`ParseCSpriteObj`, `ParseHSpriteObj`, `ParseSurfaceObj`,
`ParseWorldObj` and the others) call. One verified function per commit, branch `agents/w15-vi-esfparse-a`. Every source is
under `experimental/sources/w15-vi-esfparse-a-*/`, one directory per function: the function source (with an
observed / inferred / unknown comment header) and a native `host_contract.cpp`.

Evidence for each function (the same recipe every time): it compiles with the reviewed EE g++ `authored_cpp` flags; the native
host contract passes; and a private paired run on the R5900 interpreter executes the original function from the verified image
and the authored object side by side from identical memory with identical stubbed callees (a seeded result and out-parameter
per call index), then compares the full call log (every callee argument, floats and pointed-to bytes included) and every
changed byte outside the stack. All pairs have zero mismatches; instruction coverage is full apart from nops and annulled
delay slots. Deliberate code mutations of each source (one operator, constant, offset, field or call order at a time) all fail
the comparison, except values the original also overwrites before use (see "unknown"). The harness and scenarios stay private and are
not in the repository.

## Imports closed

`ParseSurface__10VIESFParse`, `ParseHSpriteAnim__10VIESFParse`, `ParseHSpriteSpriteArray__10VIESFParse`, `ParseCSpriteSpriteArray__10VIESFParse`, `ParseGroupSpriteSpriteArray__10VIESFParse`, `ParseSoundArray__10VIESFParse`, `ParseLODSpriteSpriteArray__10VIESFParse`, `ParseHSpriteAnimArray__10VIESFParse`, `ParseRefMap__10VIESFParse`, `ParseSound__10VIESFParse`, `ParseHSpriteTriggers__10VIESFParseR9VIHSprite`, `ParseCSpriteContSound__10VIESFParseR9VICSprite`, `ParseCSpriteSkinList__10VIESFParseR9VICSprite`, `ParseCSpriteNodeIDList__10VIESFParseR9VICSprite`, `ParseCSpriteASlotList__10VIESFParseR9VICSprite`, `ParseCSpriteTSlotList__10VIESFParseR9VICSprite`, `ParseCSpritePartDefinitions__10VIESFParse`, `ParseGroupSpriteMembers__10VIESFParseR13VIGroupSprite`, `ParseLODSpriteLevels__10VIESFParseR11VILODSprite`, `ParseWorldBase__10VIESFParseR7VIWorld`, `ParseResourceTable__10VIESFParseRt7VIArray1Z14VIResourceElem`, `ParseHSpriteAttachments__10VIESFParseR9VIHSprite`, `Convert8__10VIESFParseR12VIPixelStorePUcT2i`, `Convert4__10VIESFParseR12VIPixelStorePUcT2i`, `Convert32__10VIESFParseR12VIPixelStorePUcT2i`, `Convert24__10VIESFParseR12VIPixelStorePUcT2i`, `Convert16__10VIESFParseR12VIPixelStorePUcT2i`, `ParseCSpritePartEmitters__10VIESFParseR9VICSprite`, `ParseCSpritePlayList__10VIESFParseR9VICSprite`, `ParseHSpriteHierarchy__10VIESFParseR9VIHSprite`, `ParseWorldZones__10VIESFParseR7VIWorld`, `ParseMaterial__10VIESFParseP10VIMaterial`, `ParseCollBuffer__10VIESFParse`, `ParseFloraPrimBuffer__10VIESFParse`, `ParsePrimBuffer__10VIESFParse`

A private partial build with a plan containing these 35 entries (see "Build-plan entries needed") no longer lists any of them in
`unresolved-imports.json`.

## Imports opened

These are the callee symbols the units declare that are neither defined in the plan of this branch nor closed here. They
belong to other packages or later waves; nothing here is stubbed. Already unresolved before this package (11):
`Add__12VIDictionaryR10VIResourceUi14VIResourceTypei`, `FindTyped__12VIDictionaryUi14VIResourceTypeRi`, `Init__t7VIArray1Z20VICSpritePartEmitteri`, `NumSubObjects__9VIObjFile`, `ObjectVersion__9VIObjFile`, `Read__9VIObjFileRUc`, `Read__9VIObjFileRUi`, `Read__9VIObjFileRf`, `Read__9VIObjFileRi`, `Read__9VIObjFileRs`, `ShareSprite__7VIScenei`.

Newly opened (72): `AddRoot__9VIHSprite`, `Add__9VIHSpritei`, `AttachSkin__9VIHSpriteiR7VIScene`, `Attach__9VIHSpriteiiR7VIScene`, `BeginMaterial__12VIPrimBufferi`, `BeginPrimGroup__12VICollBuffer`, `Begin__12VICollBufferi`, `Begin__12VIPrimBufferi`, `CalcBVolumes__12VICollBufferiP7VIVect3`, `CalcBVolumes__12VIPrimBufferiP7VIVect3`, `CollBuffer__9VICollidei`, `Color__12VIPrimBufferR9VIColor32`, `CreateCollBuffer__9VICollidei`, `CreatePrimBuffer__8VIRasteri`, `CreateRefMap__7VIScenei`, `EndMaterial__12VIPrimBuffer`, `EndPrimGroup__12VICollBuffer`, `End__12VICollBuffer`, `End__12VIPrimBuffer`, `FloraType__12VICollBufferSc`, `Init__12VICollBuffer16VICollBufferTypeiii`, `Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii`, `Init__t5VIMap2ZiZii`, `Init__t7VIArray1Z14VIResourceElemi`, `Init__t7VIArray1Z16VIHSpriteTriggeri`, `Init__t8VIVector1Z10VIMatrix44i`, `Init__t8VIVector1Z13VIHSpriteNodei`, `Insert__t5VIMap2ZiZiRCiT1`, `Lock__12VICollBuffer`, `Lock__12VIPrimBuffer`, `Normal__12VIPrimBufferR7VIVect3`, `Normal__12VIPrimBufferR9VIVect3S8`, `ParsePrimBufferObjV0__10VIESFParse`, `ParseSprite__10VIESFParse`, `ParseSubSprite__10VIESFParse`, `ParseWorldBaseHeader__10VIESFParseR7VIWorld`, `ParseWorldRegions__10VIESFParseR7VIWorld`, `ParseWorldTree__10VIESFParseR11VIWorldTree`, `ParseWorldZoneProxies__10VIESFParseR7VIWorld`, `ParseZoneBaseObj__10VIESFParse`, `ParseZoneResource__10VIESFParse`, `ParseZoneRoomActorsObj__10VIESFParse`, `ParseZoneRoomStaticLightingsObj__10VIESFParse`, `PrimBuffer__8VIRasteri`, `ReadBegin__9VIObjFileUlRUsT2`, `Read__9VIObjFileRSc`, `Read__9VIObjFileRUl`, `RefMap__7VIScenei`, `RemoveAllActors__7VIScene`, `ReplaceProxyRooms__7VISceneR6VIZone`, `ReplaceStaticProxyActors__7VISceneR6VIZone`, `SetEmissiveColor__10VIMaterialRC9VIColor32`, `SetLayerUVRate__10VIMaterialiRC7VIVect2`, `SetLayerUVSet__10VIMaterialii`, `SetLayerUVTransform__10VIMaterialiRC10VIMatrix33`, `SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode`, `SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3`, `SetNodeFixScale__9VIHSpriteiRC7VIVect3`, `SetPacking__12VICollBufferi`, `SetPacking__12VIPrimBufferiii`, `SetTessellate__10VIMateriali`, `UV0__12VIPrimBufferR10VIVect2S16`, `UV0__12VIPrimBufferR7VIVect2`, `Unlock__12VICollBuffer`, `Unlock__12VIPrimBuffer`, `UpdateAllActors__7VIScene`, `VertexGroup__12VICollBufferSc`, `VertexGroup__12VIPrimBuffers`, `Vertex__12VICollBufferR10VIVect3S16`, `Vertex__12VICollBufferR7VIVect3`, `Vertex__12VIPrimBufferR10VIVect3S16`, `Vertex__12VIPrimBufferR7VIVect3`.

Library functions the units also call and the shared plan already resolves: `powf`, `memcpy`, `__10VIMatrix33`,
`_$_10VIMatrix33`, `Init__10VIMateriali` and the `SetLayer*` material setters used by `ParseMaterial` that the plan defines.

## Build-plan entries needed

`python3 docs/agents/w15-vi-esfparse-a-plan-entries.py --toolchain /path/to/ee` prints the 35 JSON plan entries (one per
directory in directory-name order, `profile` `authored_cpp`, no header closure, object names `w15esfa_*.o`, each emitting
exactly one function). With `--private-root DIR` it writes a private plan and snapshot copy that contain them (nothing is
written inside the repository); that private plan builds with `tools/build_partial.py` and closes the 35 imports. The shared
`build-plan.json` and `snapshot.json` were not edited; the lead integrator merges the printed entries (`input_order` values assume
this branch's tip).

## Observed layout evidence (addresses are in the verified image `SLUS_205.65`)

Parser record words used (word index in the `VIESFParse` record): +0 creation argument, +4/+8 callback pointer and its
argument (`ParseMaterial`), +12 raster, +16 sound device, +20 collision system, +24 scene (world), +28 current zone / object
(`ParseWorldZones` stores the zone address there; `ParseCollBuffer` and `ParsePrimBuffer` read a count at +84 and a table pointer
at +88 of it), +32 dictionary, +36 object file. Tags seen: 0x1200 prim buffer, 0x1230 flora prim buffer, 0x2400 hsprite
hierarchy, 0x2500 attachments, 0x2910 play list, 0x2960 part emitters, 0x4200 collision buffer, 0x8200 world base, 0x9000
resource table, and the zone tags 0x3200, 0x3000, 0x3100, 0x3290, 0x3270, 0x32b0.
Resource types seen in dictionary lookups: 1 (material texture), 6, 11 (primitive buffers), 14 (sounds), 18 (emitters).
`Ul` ReadBegin keys and `Read...RUl` take an unsigned long that the original loads as a doubleword, so the EE `long` is
8 bytes; the reviewed flags build with a 4-byte long, so the 64-bit key is declared `unsigned long long` in `ParseWorldZones`.
`ParseWorldBase` never reads its world argument (it works on the scene word at +24 plus 4 / 56).
The float constant 0x3c010204 (about 1/127.5) scales signed-byte normals in `ParsePrimBuffer`; the constant 0.8 replaces a
read value for object version 0 in `ParseMaterial` (built as 0x3f4ccccc).

## Authored but never executed

Nothing ran on the EE. Every function ran only on the private R5900 interpreter against stubbed callees (and, natively, in the host
contracts). Not covered: real callee behaviour (all stubbed), hardware timing, the real object file bytes (no ESF file was parsed
end to end), `powf` accuracy (hooked), and anything past the callee boundary. None of the 35 has run in a game. The
`ParseMaterial` callback (word +4) was only exercised with a recording stub.

## Unknown / not done

* The callees listed under "Imports opened" are not authored here, so no ESF file can be parsed end to end from this branch alone;
  `ParsePrimBufferObjV0`, `ParseZone*`, `ParseWorldBaseHeader`, `ParseWorldZoneProxies`, `ParseWorldTree`, `ParseWorldRegions`
  and the `VICollBuffer` / `VIPrimBuffer` methods are the largest ones.
* Field and enumerator names are not established: the sprite and zone record fields, buffer kinds (0/2/4/6/7/8 for primitive
  buffers, 0-3 for collision buffers), the material fill / wrap / blend modes, the packing exponents, the meaning of the
  callback at parse+4, and the second `ReadBegin` output (called "version" in the sources because it gates optional reads; that
  is a guess from its use).
* Some zero-initialisations of locals in the originals are dead stores; the authored sources keep a few of them (colour alpha,
  UV rate, vertex vectors). They are unobservable in the paired runs, so those two or three mutations of initial values survive by
  design.
* Not attempted (outside the listed scope): the `Obj`-level parsers (already authored in wave 14), `ParseSubSprite`, `ParseSprite`,
  `ParseParticleDefinition` and the data tables.
