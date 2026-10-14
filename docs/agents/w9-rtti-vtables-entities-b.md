# RTTI, vtables and entity class methods (b)

Branch `agents/w9-rtti-vtables-entities-b`. Closes 35 unresolved imports that the actor view
tables (`actor-view-tables-views`, `w8-core-loop-d-vtables`, `vehicle-actors-views`) point at:
the `msg_*` virtual methods of AutoProp (LP variant), AutoChest (and LP), AutoContainer (and
LP), Candle, Candle2, DoorSecret, DoorSwing, FloorSwitch, AntTrap, GothTrap, HateHole and
Shooter. Everything was checked against the private `SLUS_205.65` only as local reference.

## How each function was checked

- Read the image's function privately (never stored in the repository), then wrote the
  behaviour in my own words and my own code. Names are the image's own mangled symbols;
  field names are byte offsets only.
- Compiled with the reviewed `authored_cpp` EE profile.
- A private paired run (an R5900 interpreter kept outside git) executed the original
  function and the authored one from identical memory snapshots with identical stubbed
  callees. A function passes when the callee call logs (argument registers, pointed-to
  words, float registers) and every changed byte outside the stack are equal. Scenarios
  were randomised or enumerated over every branch. Instruction coverage of the original
  was measured for the three largest bodies: `msg_run__10AutoPropLP` 158/159 words,
  `msg_draw__10AutoPropLP` 239/244 and `msg_poke__9DoorSwingi` 235/238; the uncovered words
  are alignment nops. Mutation checks (changing a constant, a comparison or a threshold in
  a private copy) were detected for `msg_run__9DoorSwing`, `msg_run__6Candle` and
  `msg_run__11FloorSwitch`, except a one-ulp change of a multiplier that cannot show up
  in the result.
- Every function also has a committed `host_check.cpp` that fails when its stated
  behaviour is wrong (stubbed callees, 32-bit object words mapped to host addresses with
  the existing `NORRATH_W8AVB_HOST` mechanism where a word is dereferenced).
- The three functions already authored by `w8-actor-virtuals-c` (`msg_run__8GothTrap`,
  `msg_run__8HateHole`, `msg_save__7ShooterPc`) were re-run through the paired comparison
  here and match; they only lack a build-plan entry.

## Evidence

- Observed: `msg_*` slots are called as `function(object + adjustment, ...)` through the
  table at object+72; the slot offsets used here are 136 and 144 (see the sources).
- Observed: struct-by-value arguments (`Point3`) are passed by hidden pointer to a copy;
  the sources declare them by value so the compiler makes the same copies.
- Observed: the world pointer is the word `world` (0x4844a0), the frame counter is
  `frameNm` (0x4844a4), the level name buffer is `levelName` (string tested at +81).
  The two candle particle definitions are the 80-byte objects `candleParticle` (0x7d7ce0)
  and `candle2Particle` (0x7d7d30); neither is defined by an authored unit yet.
- Inferred: the `AutoPropLP` entry loop shares one update between its run and draw methods
  (`w9-rtti-vtables-entities-b-autoproplp-common/entries.hpp`); the two methods differ only
  after it.
- Unknown: the meaning of nearly every object field (all listed in the file headers), what the
  table slots at 136 and 144 do for the entries of AutoPropLP/AutoChestLP/AutoContainerLP,
  what the `world` door table fields are beyond the offsets read, and what the particle
  definition bytes contain.

## Imports closed

All 35 names in the package scope are no longer unresolved in a private build that adds the
units below to the current plan (2351 inputs, 1483 unresolved imports, down from 1511).

- `msg_draw__10AutoPropLP`
- `msg_draw__11AutoChestLP`
- `msg_draw__15AutoContainerLP`
- `msg_draw__6Candle`
- `msg_draw__7Candle2`
- `msg_getPokeName__10DoorSecretPUsi`
- `msg_getPokeName__13AutoContainerPUsi`
- `msg_getPokeName__9AutoChestPUsi`
- `msg_getPokeName__9DoorSwingPUsi`
- `msg_hurt__13AutoContainerfP10GameObjecti`
- `msg_load__10DoorSecretPc`
- `msg_load__11FloorSwitchPc`
- `msg_load__13AutoContainerPc`
- `msg_load__9AutoChestPc`
- `msg_load__9DoorSwingPc`
- `msg_poke__10DoorSecreti`
- `msg_poke__11AutoChestLPi`
- `msg_poke__13AutoContaineri`
- `msg_poke__9DoorSwingi`
- `msg_presave__9DoorSwing`
- `msg_run__10AutoPropLP`
- `msg_run__10DoorSecret`
- `msg_run__11AutoChestLP`
- `msg_run__11FloorSwitch`
- `msg_run__15AutoContainerLP`
- `msg_run__6Candle`
- `msg_run__7AntTrap`
- `msg_run__7Candle2`
- `msg_run__8GothTrap`
- `msg_run__8HateHole`
- `msg_run__9DoorSwing`
- `msg_save__10DoorSecretPc`
- `msg_save__11FloorSwitchPc`
- `msg_save__13AutoContainerPc`
- `msg_save__7ShooterPc`

Authored by this package (one commit each, 32 units plus one shared header):

- `w9-rtti-vtables-entities-b-draw-10autoproplp` — msg_draw__10AutoPropLP (760 bytes)
- `w9-rtti-vtables-entities-b-draw-11autochestlp` — msg_draw__11AutoChestLP (104 bytes)
- `w9-rtti-vtables-entities-b-draw-15autocontainerlp` — msg_draw__15AutoContainerLP (104 bytes)
- `w9-rtti-vtables-entities-b-draw-6candle` — msg_draw__6Candle (64 bytes)
- `w9-rtti-vtables-entities-b-draw-7candle2` — msg_draw__7Candle2 (64 bytes)
- `w9-rtti-vtables-entities-b-gpn-10doorsecret` — msg_getPokeName__10DoorSecretPUsi (60 bytes)
- `w9-rtti-vtables-entities-b-gpn-13autocontainer` — msg_getPokeName__13AutoContainerPUsi (84 bytes)
- `w9-rtti-vtables-entities-b-gpn-9autochest` — msg_getPokeName__9AutoChestPUsi (204 bytes)
- `w9-rtti-vtables-entities-b-gpn-9doorswing` — msg_getPokeName__9DoorSwingPUsi (108 bytes)
- `w9-rtti-vtables-entities-b-hurt-13autocontainer` — msg_hurt__13AutoContainerfP10GameObjecti (100 bytes)
- `w9-rtti-vtables-entities-b-load-10doorsecret` — msg_load__10DoorSecretPc (148 bytes)
- `w9-rtti-vtables-entities-b-load-11floorswitch` — msg_load__11FloorSwitchPc (16 bytes)
- `w9-rtti-vtables-entities-b-load-13autocontainer` — msg_load__13AutoContainerPc (84 bytes)
- `w9-rtti-vtables-entities-b-load-9autochest` — msg_load__9AutoChestPc (172 bytes)
- `w9-rtti-vtables-entities-b-load-9doorswing` — msg_load__9DoorSwingPc (204 bytes)
- `w9-rtti-vtables-entities-b-poke-10doorsecret` — msg_poke__10DoorSecreti (264 bytes)
- `w9-rtti-vtables-entities-b-poke-11autochestlp` — msg_poke__11AutoChestLPi (72 bytes)
- `w9-rtti-vtables-entities-b-poke-13autocontainer` — msg_poke__13AutoContaineri (104 bytes)
- `w9-rtti-vtables-entities-b-poke-9doorswing` — msg_poke__9DoorSwingi (908 bytes)
- `w9-rtti-vtables-entities-b-presave-9doorswing` — msg_presave__9DoorSwing (8 bytes)
- `w9-rtti-vtables-entities-b-run-10autoproplp` — msg_run__10AutoPropLP (584 bytes)
- `w9-rtti-vtables-entities-b-run-10doorsecret` — msg_run__10DoorSecret (460 bytes)
- `w9-rtti-vtables-entities-b-run-11autochestlp` — msg_run__11AutoChestLP (96 bytes)
- `w9-rtti-vtables-entities-b-run-11floorswitch` — _ZL10slide_doorPh, msg_run__11FloorSwitch (108 bytes, 732 bytes)
- `w9-rtti-vtables-entities-b-run-15autocontainerlp` — msg_run__15AutoContainerLP (96 bytes)
- `w9-rtti-vtables-entities-b-run-6candle` — msg_run__6Candle (776 bytes)
- `w9-rtti-vtables-entities-b-run-7anttrap` — msg_run__7AntTrap (524 bytes)
- `w9-rtti-vtables-entities-b-run-7candle2` — msg_run__7Candle2 (584 bytes)
- `w9-rtti-vtables-entities-b-run-9doorswing` — msg_run__9DoorSwing (332 bytes)
- `w9-rtti-vtables-entities-b-save-10doorsecret` — msg_save__10DoorSecretPc (16 bytes)
- `w9-rtti-vtables-entities-b-save-11floorswitch` — msg_save__11FloorSwitchPc (16 bytes)
- `w9-rtti-vtables-entities-b-save-13autocontainer` — msg_save__13AutoContainerPc (16 bytes)

Already authored by `w8-actor-virtuals-c`, re-verified here, not yet in the build plan:
`msg_run__8GothTrap`, `msg_run__8HateHole`, `msg_save__7ShooterPc`.

## Imports opened

New unresolved names that these units reference (not defined by any authored unit in the
current plan):

- `candle2Particle` (80-byte particle definition, 0x7d7d30; `candleParticle` at 0x7d7ce0 is
  already provided elsewhere)
- `findTagInt__10GameObjectPci`
- `msg_poke__9AutoChesti`
- `msg_run__9AutoChest`
- `msg_run__13AutoContainer`
- `objectAddToMediumRunList__FP10GameObject`
- `worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2`

These belong to other packages' scope (object infrastructure, world, AutoChest/AutoContainer
class methods, static initialisers) and are left unresolved on purpose.

## Build-plan entries needed

`experimental/build-plan.json` and `snapshot.json` are not edited here. Generate the exact
entries, and optionally a private plan for a check build, with

    python3 docs/agents/w9-rtti-vtables-entities-b-plan-entries.py \\
        --toolchain /path/to/reviewed/ee [--private-root DIR] [--skip-w8c]

It prints one JSON entry per unit (35 lines: the 32 units here, and with the default flags the
three `w8-actor-virtuals-c` units listed above). The entries recorded when the notes were written (the
sources are not changed by the notes commit; source hashes, header closures, emitted function names and body
sizes) are:

```
{"emitted_functions": [{"compiled_body_bytes": 760, "name": "msg_draw__10AutoPropLP"}], "header_closure": ["sources/w9-rtti-vtables-entities-b-autoproplp-common/entries.hpp", "sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_draw_10autoproplp.o", "profile": "authored_cpp", "sha256": "0f1cc16bd6c9a1ec4c1c3d35580e5029eb4703b529515487ceec23ae10b75119", "source": "sources/w9-rtti-vtables-entities-b-draw-10autoproplp/draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "msg_draw__11AutoChestLP"}], "header_closure": ["sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_draw_11autochestlp.o", "profile": "authored_cpp", "sha256": "6b652f881c454814fa1a48a8717746e96f56774087d60a2539c70d985f7468c4", "source": "sources/w9-rtti-vtables-entities-b-draw-11autochestlp/draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "msg_draw__15AutoContainerLP"}], "header_closure": ["sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_draw_15autocontainerlp.o", "profile": "authored_cpp", "sha256": "9ba9848788d11de8c6e11333b5bdecc43b52a6af68b86c093fde4fc65bb3d222", "source": "sources/w9-rtti-vtables-entities-b-draw-15autocontainerlp/draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "msg_draw__6Candle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_draw_6candle.o", "profile": "authored_cpp", "sha256": "be56976123f107d2b092d64440207ef3005a9420897e36a8bbedcf431b7a0eff", "source": "sources/w9-rtti-vtables-entities-b-draw-6candle/draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "msg_draw__7Candle2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_draw_7candle2.o", "profile": "authored_cpp", "sha256": "a6c0cfca649c6dfee83ca5819ee8b85cee193c5c5fe96fceb4bc94e6c6bf011c", "source": "sources/w9-rtti-vtables-entities-b-draw-7candle2/draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 60, "name": "msg_getPokeName__10DoorSecretPUsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_gpn_10doorsecret.o", "profile": "authored_cpp", "sha256": "ffa9be0a5bc382c4c579267e7ca75b8d0973839f772e56dafe56d1264e2c49cd", "source": "sources/w9-rtti-vtables-entities-b-gpn-10doorsecret/msg_get_poke_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "msg_getPokeName__13AutoContainerPUsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_gpn_13autocontainer.o", "profile": "authored_cpp", "sha256": "fd2248d49386488d6b0679594195a94a9775e936d78a382eae6a096f168dad8c", "source": "sources/w9-rtti-vtables-entities-b-gpn-13autocontainer/msg_get_poke_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "msg_getPokeName__9AutoChestPUsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_gpn_9autochest.o", "profile": "authored_cpp", "sha256": "e18a72d7df4065dd51ac3487ae671047deeaae46775dd4f81eb53829ed4e01f9", "source": "sources/w9-rtti-vtables-entities-b-gpn-9autochest/msg_get_poke_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "msg_getPokeName__9DoorSwingPUsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_gpn_9doorswing.o", "profile": "authored_cpp", "sha256": "ee0a1a1b131b5b96268a0858702013ae5883caa6c1b2f86b2d17f38c504854c0", "source": "sources/w9-rtti-vtables-entities-b-gpn-9doorswing/msg_get_poke_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "msg_hurt__13AutoContainerfP10GameObjecti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_hurt_13autocontainer.o", "profile": "authored_cpp", "sha256": "9c318d43f77175107b95a82665367fa0da6e5acb3c04948d413bb03016aa5138", "source": "sources/w9-rtti-vtables-entities-b-hurt-13autocontainer/msg_hurt.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "msg_load__10DoorSecretPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_load_10doorsecret.o", "profile": "authored_cpp", "sha256": "1cbf8fc43b9fff0b223596a67aab1a9037582ac71bbca0aff7c3bd84d44cc4a3", "source": "sources/w9-rtti-vtables-entities-b-load-10doorsecret/load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "msg_load__11FloorSwitchPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_load_11floorswitch.o", "profile": "authored_cpp", "sha256": "a0c0566bf152d3e722094f55f61c24dccd390fd188781291604dc97f18c742d1", "source": "sources/w9-rtti-vtables-entities-b-load-11floorswitch/load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "msg_load__13AutoContainerPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_load_13autocontainer.o", "profile": "authored_cpp", "sha256": "71ba8eca4288cae3477522cab245f0fe004a8159fdfa0a65d5f5263c23c0a135", "source": "sources/w9-rtti-vtables-entities-b-load-13autocontainer/load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 172, "name": "msg_load__9AutoChestPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_load_9autochest.o", "profile": "authored_cpp", "sha256": "8a8a8d2dd2977bd540deea86a1c591a23365a6ef8ff3b01914f43b1fba0d3926", "source": "sources/w9-rtti-vtables-entities-b-load-9autochest/load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "msg_load__9DoorSwingPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_load_9doorswing.o", "profile": "authored_cpp", "sha256": "f699b2cd4a812ccf4d19687cd664c12993040e910138762a192026234d432394", "source": "sources/w9-rtti-vtables-entities-b-load-9doorswing/load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 264, "name": "msg_poke__10DoorSecreti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_poke_10doorsecret.o", "profile": "authored_cpp", "sha256": "d4a5cd60af9dd966247f94e329843ddb723957ed1a41be266689a2973f23446d", "source": "sources/w9-rtti-vtables-entities-b-poke-10doorsecret/msg_poke.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "msg_poke__11AutoChestLPi"}], "header_closure": ["sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_poke_11autochestlp.o", "profile": "authored_cpp", "sha256": "467ad6f539731065a3eedd46d6010cf218a2962e98ed1276745692480e793976", "source": "sources/w9-rtti-vtables-entities-b-poke-11autochestlp/msg_poke.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "msg_poke__13AutoContaineri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_poke_13autocontainer.o", "profile": "authored_cpp", "sha256": "a4b8fc8c144338081e9d96c6584860ca56f561a883606ecd19fd21198c38a858", "source": "sources/w9-rtti-vtables-entities-b-poke-13autocontainer/msg_poke.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 908, "name": "msg_poke__9DoorSwingi"}], "header_closure": ["sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_poke_9doorswing.o", "profile": "authored_cpp", "sha256": "4ce0559e38dea480484cc16ce0d547f29b04ade5f602f5908379e429795fc31b", "source": "sources/w9-rtti-vtables-entities-b-poke-9doorswing/msg_poke.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_presave__9DoorSwing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_presave_9doorswing.o", "profile": "authored_cpp", "sha256": "fa5e53ba28d840407eb2d18fa84aa4618c1b799b44a43fdfa5c3f081f9cbda6c", "source": "sources/w9-rtti-vtables-entities-b-presave-9doorswing/presave.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 584, "name": "msg_run__10AutoPropLP"}], "header_closure": ["sources/w9-rtti-vtables-entities-b-autoproplp-common/entries.hpp", "sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_run_10autoproplp.o", "profile": "authored_cpp", "sha256": "faa5c4474162217ece685e3993cbcf323bbf7b23ba73706f26957e202f6a27e3", "source": "sources/w9-rtti-vtables-entities-b-run-10autoproplp/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 460, "name": "msg_run__10DoorSecret"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_run_10doorsecret.o", "profile": "authored_cpp", "sha256": "ab401979674dd0b67d91a9fab24edc0b27ace6c1b142f089d15708c5afe7deda", "source": "sources/w9-rtti-vtables-entities-b-run-10doorsecret/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "msg_run__11AutoChestLP"}], "header_closure": ["sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_run_11autochestlp.o", "profile": "authored_cpp", "sha256": "ded009716586e8b170036b7535c9c538968199ae7a61297ceb4c4645f110e9b3", "source": "sources/w9-rtti-vtables-entities-b-run-11autochestlp/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "_ZL10slide_doorPh"}, {"compiled_body_bytes": 732, "name": "msg_run__11FloorSwitch"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_run_11floorswitch.o", "profile": "authored_cpp", "sha256": "79ea7da7ef6146da343a53a0420cf318694b0d32720ba0cd8a6d3241a578723e", "source": "sources/w9-rtti-vtables-entities-b-run-11floorswitch/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "msg_run__15AutoContainerLP"}], "header_closure": ["sources/w8-actor-virtuals-b-common/virtual_call.hpp"], "kind": "independently_authored_source", "object_name": "w9rveb_run_15autocontainerlp.o", "profile": "authored_cpp", "sha256": "9f56a50299aad18eaf4e73335473b849b63509ec6e0e050fe070ba2edcb00a03", "source": "sources/w9-rtti-vtables-entities-b-run-15autocontainerlp/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 776, "name": "msg_run__6Candle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_run_6candle.o", "profile": "authored_cpp", "sha256": "c2d3f55aff674a30cd65864042d89ad85080efea9cfdea9fa400c92595fe72ef", "source": "sources/w9-rtti-vtables-entities-b-run-6candle/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 524, "name": "msg_run__7AntTrap"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_run_7anttrap.o", "profile": "authored_cpp", "sha256": "779cb70f502670330ee877895b74d00b0e492afbe340f302ff71f0ad6389de94", "source": "sources/w9-rtti-vtables-entities-b-run-7anttrap/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 584, "name": "msg_run__7Candle2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_run_7candle2.o", "profile": "authored_cpp", "sha256": "942ff4bf635ef7b590e0afd08c3fcbe7cded422f7eca0a45896bc6c7312b900b", "source": "sources/w9-rtti-vtables-entities-b-run-7candle2/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 332, "name": "msg_run__9DoorSwing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_run_9doorswing.o", "profile": "authored_cpp", "sha256": "44f3d3d9715411112eeb019091d6a7b24fba82c555da7cf0741989bebc4772c1", "source": "sources/w9-rtti-vtables-entities-b-run-9doorswing/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "msg_save__10DoorSecretPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_save_10doorsecret.o", "profile": "authored_cpp", "sha256": "5b3269e315379a11fe42540a733a6074b0dbf6538ddc2453aa44d31e2d699c69", "source": "sources/w9-rtti-vtables-entities-b-save-10doorsecret/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "msg_save__11FloorSwitchPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_save_11floorswitch.o", "profile": "authored_cpp", "sha256": "d776254128c163a356ae7bd9382f19c55f8e5c8546e1c61d3f7b9b2e455ff1b6", "source": "sources/w9-rtti-vtables-entities-b-save-11floorswitch/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "msg_save__13AutoContainerPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9rveb_save_13autocontainer.o", "profile": "authored_cpp", "sha256": "99362353c3d383308a7ccd28b3d1e138dbe9f9dea9bbb0a8f2b8e7cf136e5c81", "source": "sources/w9-rtti-vtables-entities-b-save-13autocontainer/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_run__8GothTrap"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_goth_trap_run.o", "profile": "authored_cpp", "sha256": "d98953d34a8591c1c5a24134ff53192a8922845718be497cd0d935abc1f8c557", "source": "sources/w8-actor-virtuals-c-goth-trap-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_run__8HateHole"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_hate_hole_run.o", "profile": "authored_cpp", "sha256": "22e13b51cfb992512ab876e1f063a77016cbf77b905de82b46fad8419973b64c", "source": "sources/w8-actor-virtuals-c-hate-hole-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "msg_save__7ShooterPc"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_shooter_save.o", "profile": "authored_cpp", "sha256": "0fb439ebd88d54429f015a43fd93a57898ef4833f2a525f109a384f60e0c0da4", "source": "sources/w8-actor-virtuals-c-shooter-save/save.cpp"}
```

Header files included by these units (all hashed through the entries' `header_closure`):
`sources/w9-rtti-vtables-entities-b-autoproplp-common/entries.hpp`,
`sources/w8-actor-virtuals-b-common/virtual_call.hpp`.

## Authored but never executed

Everything. The functions ran only in the private R5900 interpreter against the image with
stubbed callees (object lists, tag lookups, script start, sound, world door calls, particle
and model calls, the random generator and the text system were all stand-ins) and in
native host checks with stubbed callees. No callee behind a stub is exercised by these
bodies on real data, and no game state was ever running. The interpreter rounds to nearest
where the EE truncates; both sides of every comparison ran in the same interpreter, so
the comparison is not affected, but the numbers were not checked on hardware.

## Unknown

- What the fields of AutoProp, AutoChest, AutoContainer, Candle, Candle2, DoorSecret,
  DoorSwing, FloorSwitch and AntTrap mean beyond the offsets and tests recorded in the file
  headers.
- What the entry objects of AutoPropLP/AutoChestLP/AutoContainerLP are, and what their table
  slots at byte offsets 136 and 144 do.
- The layout of the world door table (28-byte entries at world+84, 60-byte records at
  world+36) beyond the fields read by `msg_poke__9DoorSwingi`.
- The `scratch` float array and the matrix argument of `modelGetShadowPos` (passed as a
  4-float buffer and null), the unused return values of the model and world calls, and the
  size of the text buffers (`msg_getPokeName__9AutoChestPUsi` uses 400 bytes as the image's
  frame does).
- The retail edition of the executable; only the one file named in the task was used.
