# Startup and runtime support (b)

Branch `agents/w9-startup-crt-b`. Private reference: `work/private/SLUS_205.65`; addresses
below are symbol values of that file. No game bytes, disassembly or decompiler output are
committed. Each function was compiled with the reviewed EE g++ `authored_cpp` flags. Host
checks under each directory are not EE inputs. A function counts as done only when it has its
own commit, a host check that fails if the stores or arithmetic are wrong, and (where noted)
a comparison with the original in a private interpreter run kept outside git (it runs the
original and the authored object from identical memory with identical callee stubs and
compares call logs and every changed byte). Nothing ran on the EE or as gameplay.

## Imports closed

Run-time support: `__builtin_new`, `__terminate`, `__malloc_lock`, `__malloc_unlock`,
`__malloc_update_mallinfo`.

Math: `__ieee754_powf`, `__ieee754_rem_pio2f`, `__kernel_tanf`.

Type-info accessors (18): `__tf3Orc`, `__tf4Boat`, `__tf6Candle`, `__tf6PhProp`, `__tf7AntTrap`,
`__tf7Blocker`, `__tf7Candle2`, `__tf7EggSack`, `__tf9DoorSwing`, `__tf10DoorSecret`,
`__tf10GameObject`, `__tf11AutoChestLP`, `__tf11FloorSwitch`, `__tf11LavaVehicle`,
`__tf11PushTrigger`, `__tf14__si_type_info`, `__tf15AutoContainerLP`, `__tf16__user_type_info`,
plus the four inline-only nodes in `w9-startup-crt-b-ti-inline-bases`.

Constructors: `__4GoldG6Point3i`, `__6PhPropG6Point3PcN22liPQ24item11descriptionf`,
`__6PlayerG6Point3sii`, `__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii`, `__7VISetup`,
`__8GutChunkG6Point3iif`, `__9VIStation`.

That is 33 of the 38 names in the task scope (18 accessors, 7 constructors, 3 math, 5
run-time); the four inline nodes are extra.

## Imports not closed, and why

- `__divdi3`, `__moddi3`: the repo already carries an excluded, unintegrated draft
  (`game-software-arithmetic-source-v2`) whose status line says original frame/table/fault
  state is unfinished. I did not write a second version. Decision belongs to whoever owns
  that draft or the integrator.
- `__extendsfdf2`: observed: the image has no function by this name (the nearby soft-float
  pieces are `__pack_d`, `__unpack_d`, `__make_dp`, `dptofp`, ...). The reference to it comes
  from the current compiler lowering a float-to-double conversion in
  `async_change_players` (the callers' unit), not from an original call. It cannot be closed
  by authoring an original function; the caller or the build needs an explicit decision.
- `__malloc_current_mallinfo`: a 40-byte zero-initialised word block at `0x004488b8`.
  `memory-malloc-state` already defines it under the name
  `norrath_player_malloc_current_info_view`; the plan has no symbol alias, so the import stays
  until the integrator maps the name. `update-mallinfo` (this package) uses the view name.
- `__EH_FRAME_BEGIN__` (`0x00453970`): observed: it is the start of the exception-frame
  section, filled by the compiler and linker from every object's frame records. It is not an
  authorable function or table; it needs link-script support, not source.

## Evidence notes

- Observed `__ieee754_powf`: `(x-x)/(x-x)` is a real divide in the image. The EE float model
  in the reviewed compiler folds `d / d` to 1, so `zero_over_zero` hides the equal operands
  with empty `asm` statements; without that the invalid-operation paths returned 1.0 in the
  paired run. The private interpreter returns `0xff800000` for 0/0; the host check accepts any NaN
  for those recorded vectors.
- Observed `__6PlayerG6Point3sii`: the image sign-extends its short argument itself, so the
  parameter is taken as `int` and narrowed explicitly. The four light halves it halves
  afterwards are the constants it just wrote (188, 188, 188, -64); the rounding never
  matters but is kept.
- Observed `__7VISetup`, `__9VIStation`: the image clears one range twice, once with scalar
  float stores and once with quad-word vector stores of a zeroed register. The result is the
  same bytes, so one clearing is authored. `__9VIStation` ends in a tail call to `printf`
  with one string; `__7VISetup` ends in a tail call to `__10VIWndLegal`.
- Constructor return values: the originals leave unrelated values in the result register
  (GutChunk 2, Sparks and PhProp a callee result). They are authored as `void`; Gold returns
  the run-list result because that is its last call.
- Mutation checks in the private runs: for each constructor I changed single constants and
  offsets and confirmed the paired run reports a mismatch. Not detected: the last
  one-bit changes of a few `powf` polynomial constants (`0x35bfbe8c`, `0x3338aa3b`), which
  change results only for inputs outside the 7,936 cases; the host vectors are a subset.
- Not covered by any paired run: `__builtin_new` throw path (needs exception tables, see its
  header), the malloc lock functions' kernel calls (stubbed), `__terminate`.

## Imports opened

Names that are original-image symbols (they exist in the reference symbol table): 
`__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`,
`__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap`, `__4PropG6Point3PcN22li`,
`__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc`, `initStuff__6PhPropi`,
`animInitState__FP14AnimationState`, `animAdd__FP14AnimationStateP15AnimationHeaderi`,
`get__11ResourceMap`, `lightAdd__FP5Light`, `lightDeactivate__FP5Light`,
`lumpFindResource__FPcT0`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`objectAddToRunList__FP10GameObject`, `resetPlayerAnimations__Fi`,
`enumerateAlivePlayers__Fv`, `playerRecomputeAbilityModifiers__Fv`,
`worldFindHeight__FP12_worldHeaderG6Point3f`, `sprintf`, `printf`, `scePrintf`, `memset`,
`fabsf`, `scalbnf`, `__ieee754_sqrtf`, `__kernel_rem_pio2f`, `GetThreadId`, `WaitSema`,
`SignalSema`, `__rtti_si`, `__rtti_user`, `__eh_alloc`, `__cp_push_exception`, `__throw`,
`__wrap_malloc`, the base accessors (`__tf8Creature`, `__tf7Vehicle`, `__tf8AutoProp`,
`__tf10AutoPropLP`, `__tf9type_info`, `__tf9bad_alloc`), the 17 `VI*` sub-constructors
(`__8VIRaster`, `__9VICollide`, `__12VIDictionary`, `__13VISoundDevice`, `__7VIScene`, `__6VIFont`,
`__4VIUI`, `__7VIFader`, `__5VIPad`, `__10VIKeyboard`, `__15VIWindowManager`,
`__20VIWndGenericRenderer`, `__11VIWndSplash`, `__12VIWndPatcher`, `__9VIWndDnas`,
`__9VIWndEula`, `__10VIWndLegal`), and the data symbols `player`, `racePrefix`,
`typeToResource`, `selectLumpName`, `levelFlags`, `nmPlayers`, `thereIsACutPlayer`,
`coreForceSmallCharacters`, `world`, `frameNm`, `goldAnm`, `fxResourceMap`, the vtables
`_vt$4Gold`, `_vt$6PhProp`, `_vt$6Player`, `_vt$6Sparks`, `_vt$8GutChunk`, `_vt$9bad_alloc`,
the type_info nodes `__ti7Vehicle`, `__ti8Creature`, `__ti8AutoProp`, `__ti10GameObject`,
`__ti10AutoPropLP`, `__ti9WorldPart`, `__ti9type_info`, and `__new_handler`,
`__terminate_func`, `__sce_sema_id`.

Names I invented for image bytes that have no symbol (strings and loose words); each is an
`extern` view that needs a definition at the matching content, none of which I authored:
`norrath_w9_startup_crt_b_view_<address>` for addresses 11c3ae8, 462260, 4644c0, 464ab8,
464ac8, 464ad8, 464ae8, 464af8, 464b10, 464b20, 464b30, 464b48, 464b58, 464b68, 464b78,
464b88, 464b98, 4708d0, 483b70, 485380 (strings) and 4801f8 (six floats, used as an array),
4816dc (one float). From the earlier part of this package the memory views
`norrath_player_malloc_bins_view`, `norrath_player_malloc_current_info_view`,
`norrath_player_malloc_lock_count_view`, `norrath_player_malloc_lock_owner_view` are
imported and already defined by `memory-malloc-state`.

## Build-plan entries needed

Add after the existing entries (profile `authored_cpp`; no header closure; `input_order`
left to the integrator). `host_check.cpp` / `*_host_test.cpp` files are not inputs.

| function | source | compiled bytes |
|---|---|---|
| `__builtin_new` | `sources/w9-startup-crt-b-builtin-new/builtin_new.cpp` | 160 |
| `__4GoldG6Point3i` | `sources/w9-startup-crt-b-ctor-gold/gold.cpp` | 180 |
| `__8GutChunkG6Point3iif` | `sources/w9-startup-crt-b-ctor-gutchunk/gutchunk.cpp` | 208 |
| `__6PhPropG6Point3PcN22liPQ24item11descriptionf` | `sources/w9-startup-crt-b-ctor-phprop/phprop.cpp` | 536 |
| `__6PlayerG6Point3sii` | `sources/w9-startup-crt-b-ctor-player/player.cpp` | 1828 |
| `__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii` | `sources/w9-startup-crt-b-ctor-sparks/sparks.cpp` | 412 |
| `__7VISetup` | `sources/w9-startup-crt-b-ctor-visetup/visetup.cpp` | 248 |
| `__9VIStation` | `sources/w9-startup-crt-b-ctor-vistation/vistation.cpp` | 228 |
| `__ieee754_powf` | `sources/w9-startup-crt-b-ieee754-powf/powf.cpp` | 2068 |
| `__kernel_tanf` | `sources/w9-startup-crt-b-kernel-tanf/kernel_tanf.cpp` | 760 |
| `__malloc_lock` | `sources/w9-startup-crt-b-malloc-lock/malloc_lock.cpp` | 96 |
| `__malloc_unlock` | `sources/w9-startup-crt-b-malloc-unlock/malloc_unlock.cpp` | 56 |
| `__ieee754_rem_pio2f` | `sources/w9-startup-crt-b-rem-pio2f/rem_pio2f.cpp` | 912 |
| `__terminate` | `sources/w9-startup-crt-b-terminate/terminate.cpp` | 24 |
| `__tf10DoorSecret` | `sources/w9-startup-crt-b-tf-10doorsecret/typeinfo.cpp` | 184 |
| `__tf10GameObject` | `sources/w9-startup-crt-b-tf-10gameobject/typeinfo.cpp` | 72 |
| `__tf11AutoChestLP` | `sources/w9-startup-crt-b-tf-11autochestlp/typeinfo.cpp` | 208 |
| `__tf11FloorSwitch` | `sources/w9-startup-crt-b-tf-11floorswitch/typeinfo.cpp` | 184 |
| `__tf11LavaVehicle` | `sources/w9-startup-crt-b-tf-11lavavehicle/typeinfo.cpp` | 92 |
| `__tf11PushTrigger` | `sources/w9-startup-crt-b-tf-11pushtrigger/typeinfo.cpp` | 92 |
| `__tf14__si_type_info` | `sources/w9-startup-crt-b-tf-14si-type-info/typeinfo.cpp` | 92 |
| `__tf15AutoContainerLP` | `sources/w9-startup-crt-b-tf-15autocontainerlp/typeinfo.cpp` | 208 |
| `__tf16__user_type_info` | `sources/w9-startup-crt-b-tf-16user-type-info/typeinfo.cpp` | 92 |
| `__tf3Orc` | `sources/w9-startup-crt-b-tf-3orc/typeinfo.cpp` | 92 |
| `__tf4Boat` | `sources/w9-startup-crt-b-tf-4boat/typeinfo.cpp` | 92 |
| `__tf6Candle` | `sources/w9-startup-crt-b-tf-6candle/typeinfo.cpp` | 92 |
| `__tf6PhProp` | `sources/w9-startup-crt-b-tf-6phprop/typeinfo.cpp` | 184 |
| `__tf7AntTrap` | `sources/w9-startup-crt-b-tf-7anttrap/typeinfo.cpp` | 92 |
| `__tf7Blocker` | `sources/w9-startup-crt-b-tf-7blocker/typeinfo.cpp` | 92 |
| `__tf7Candle2` | `sources/w9-startup-crt-b-tf-7candle2/typeinfo.cpp` | 92 |
| `__tf7EggSack` | `sources/w9-startup-crt-b-tf-7eggsack/typeinfo.cpp` | 92 |
| `__tf9DoorSwing` | `sources/w9-startup-crt-b-tf-9doorswing/typeinfo.cpp` | 184 |
| `__malloc_update_mallinfo` | `sources/w9-startup-crt-b-update-mallinfo/update_mallinfo.cpp` | 152 |

```json
[
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "builtin_new.o",
    "profile": "authored_cpp",
    "sha256": "1332da0c9445c207059fb10bc1a4aedee5e66e2a305373a2b0df0d0ad53e907e",
    "source": "sources/w9-startup-crt-b-builtin-new/builtin_new.cpp",
    "emitted_functions": [
      {
        "name": "__builtin_new",
        "compiled_body_bytes": 160
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "gold.o",
    "profile": "authored_cpp",
    "sha256": "2607103d644e9e5a38791bd3535a1fbdbaba394ae2b5f866b9fd4dcf79bceca6",
    "source": "sources/w9-startup-crt-b-ctor-gold/gold.cpp",
    "emitted_functions": [
      {
        "name": "__4GoldG6Point3i",
        "compiled_body_bytes": 180
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "gutchunk.o",
    "profile": "authored_cpp",
    "sha256": "dc624594689cc3d7fa82b50fa1a3376901f62affe78ddcbd281ad309f376aa20",
    "source": "sources/w9-startup-crt-b-ctor-gutchunk/gutchunk.cpp",
    "emitted_functions": [
      {
        "name": "__8GutChunkG6Point3iif",
        "compiled_body_bytes": 208
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "phprop.o",
    "profile": "authored_cpp",
    "sha256": "4d96c7abe08fc72975f80284ec09664a2f69219b57b7126bf22e2016d009f6a2",
    "source": "sources/w9-startup-crt-b-ctor-phprop/phprop.cpp",
    "emitted_functions": [
      {
        "name": "__6PhPropG6Point3PcN22liPQ24item11descriptionf",
        "compiled_body_bytes": 536
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "player.o",
    "profile": "authored_cpp",
    "sha256": "57577ade7ab0fff06232a93d9d933008e725cfd31c150b506ca9040c896051fb",
    "source": "sources/w9-startup-crt-b-ctor-player/player.cpp",
    "emitted_functions": [
      {
        "name": "__6PlayerG6Point3sii",
        "compiled_body_bytes": 1828
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sparks.o",
    "profile": "authored_cpp",
    "sha256": "3b9a079d58352e9949abede2b674a82f991fe74c9bffa374b9c8c549bc8f237c",
    "source": "sources/w9-startup-crt-b-ctor-sparks/sparks.cpp",
    "emitted_functions": [
      {
        "name": "__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii",
        "compiled_body_bytes": 412
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "visetup.o",
    "profile": "authored_cpp",
    "sha256": "c3e26b8977ba420879673a34745864f2f7bdfb92d29586247d59cf6359f4a16f",
    "source": "sources/w9-startup-crt-b-ctor-visetup/visetup.cpp",
    "emitted_functions": [
      {
        "name": "__7VISetup",
        "compiled_body_bytes": 248
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "vistation.o",
    "profile": "authored_cpp",
    "sha256": "791b6992d2d8ebbb67257fd67ac14ee1ddac9839bf0e4b5d2b606dfd5e2f7871",
    "source": "sources/w9-startup-crt-b-ctor-vistation/vistation.cpp",
    "emitted_functions": [
      {
        "name": "__9VIStation",
        "compiled_body_bytes": 228
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "powf.o",
    "profile": "authored_cpp",
    "sha256": "33633046eea212c8b4890367c2ef2ec1dc15f4113b5ce1960362b2f9f2617926",
    "source": "sources/w9-startup-crt-b-ieee754-powf/powf.cpp",
    "emitted_functions": [
      {
        "name": "__ieee754_powf",
        "compiled_body_bytes": 2068
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "kernel_tanf.o",
    "profile": "authored_cpp",
    "sha256": "b03f8c39163bb5025cce6e877c2244e8ff1236b76b803bd8d6023081b91d909f",
    "source": "sources/w9-startup-crt-b-kernel-tanf/kernel_tanf.cpp",
    "emitted_functions": [
      {
        "name": "__kernel_tanf",
        "compiled_body_bytes": 760
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "malloc_lock.o",
    "profile": "authored_cpp",
    "sha256": "be558d1e61b3b96661752fc35803ef77ef46e937ddad47330d4a88c7628c1b4c",
    "source": "sources/w9-startup-crt-b-malloc-lock/malloc_lock.cpp",
    "emitted_functions": [
      {
        "name": "__malloc_lock",
        "compiled_body_bytes": 96
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "malloc_unlock.o",
    "profile": "authored_cpp",
    "sha256": "22e3ec5f80c2ab06d41ce3e94fc23e5c7c1ac7d6fe1a940be7a691daea536281",
    "source": "sources/w9-startup-crt-b-malloc-unlock/malloc_unlock.cpp",
    "emitted_functions": [
      {
        "name": "__malloc_unlock",
        "compiled_body_bytes": 56
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "rem_pio2f.o",
    "profile": "authored_cpp",
    "sha256": "aa89b5a53f72aa4cc6f14527cd1fd80e631e229a92d335e51929b4c40641b970",
    "source": "sources/w9-startup-crt-b-rem-pio2f/rem_pio2f.cpp",
    "emitted_functions": [
      {
        "name": "__ieee754_rem_pio2f",
        "compiled_body_bytes": 912
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "terminate.o",
    "profile": "authored_cpp",
    "sha256": "08973bb9bda3ad6a6b1bf95f91e3dbe0b023ef6186ed9fadae5aaf79112ea84e",
    "source": "sources/w9-startup-crt-b-terminate/terminate.cpp",
    "emitted_functions": [
      {
        "name": "__terminate",
        "compiled_body_bytes": 24
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "bd0a5d3aba9b78aa0b6cbdbb488b38180aaf35491d8ee3dd3ad9c704f1b6fbe4",
    "source": "sources/w9-startup-crt-b-tf-10doorsecret/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf10DoorSecret",
        "compiled_body_bytes": 184
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "0fa6c3e76f622b056b714f7a71f239701e4b879fad8c521129a0f1b59b5a5220",
    "source": "sources/w9-startup-crt-b-tf-10gameobject/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf10GameObject",
        "compiled_body_bytes": 72
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "e63eef12f67e61e0f2408f7d51f530e9031f9bfd86667713cc83a57c3fec19a9",
    "source": "sources/w9-startup-crt-b-tf-11autochestlp/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf11AutoChestLP",
        "compiled_body_bytes": 208
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "affbda83b67ee473e2e6270ec4c5e4a27922e59d284893ee4c332343d7b17fa3",
    "source": "sources/w9-startup-crt-b-tf-11floorswitch/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf11FloorSwitch",
        "compiled_body_bytes": 184
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "c57ec5463d20ea0e91db06b94a5d08950b10ca5be7e1112f6ae10eb21eee6f54",
    "source": "sources/w9-startup-crt-b-tf-11lavavehicle/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf11LavaVehicle",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "57ef00578fc7ea1f81005179c28c2ff0355a1097c05bde59018cacda834ac1c5",
    "source": "sources/w9-startup-crt-b-tf-11pushtrigger/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf11PushTrigger",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "46cc42e2390fef83529db76b71856cb247d3a7bf8f526b0c0e19775b79c6383e",
    "source": "sources/w9-startup-crt-b-tf-14si-type-info/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf14__si_type_info",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "8b50c25c9bac5cdbdda4fafa813b6cba46aef27090cf3345811b8848ad886d6a",
    "source": "sources/w9-startup-crt-b-tf-15autocontainerlp/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf15AutoContainerLP",
        "compiled_body_bytes": 208
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "96817de2bd9a99c5fe6f74ac872c4b6a8e4bd881050681e7db10516d9f09cdb2",
    "source": "sources/w9-startup-crt-b-tf-16user-type-info/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf16__user_type_info",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "a449c0aae3f461b6a45bb65f4f6f468bfd9baa6321e7a3531f9dcd15ae912eb1",
    "source": "sources/w9-startup-crt-b-tf-3orc/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf3Orc",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "e5966262486bdfe3bb0e40685b61c45962cbb3a3e9e104efecb9b819511435f2",
    "source": "sources/w9-startup-crt-b-tf-4boat/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf4Boat",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "86888100b87c8b906e655cd76dad91d5b28e7662ca76607897792fbe2839142b",
    "source": "sources/w9-startup-crt-b-tf-6candle/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf6Candle",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "b4e4acf4b556afe9cdf39da6f74552b428cdef8e25258c0194f4725ae5b24a70",
    "source": "sources/w9-startup-crt-b-tf-6phprop/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf6PhProp",
        "compiled_body_bytes": 184
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "b988b8bd2620019d624ac4fede0a438a96fb59c08593245e71f13902a3523e62",
    "source": "sources/w9-startup-crt-b-tf-7anttrap/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf7AntTrap",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "ddc12f3badd13b7e0150a84e0c394ac778f8df4f7cf1d48192210e1f25be92f2",
    "source": "sources/w9-startup-crt-b-tf-7blocker/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf7Blocker",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "4de4bc3dcd64b9102a8556b0a23d6a5fc33b0b2cc1e64486f2aea2c0f58157c4",
    "source": "sources/w9-startup-crt-b-tf-7candle2/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf7Candle2",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "a80246a53b492cc7d46976c47f4b26cdc73d1011f38db668d95a0641c2c74b54",
    "source": "sources/w9-startup-crt-b-tf-7eggsack/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf7EggSack",
        "compiled_body_bytes": 92
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "3be94e0dbebc38283230d593e673254d36ce3b5b92144f627da03787e136efc9",
    "source": "sources/w9-startup-crt-b-tf-9doorswing/typeinfo.cpp",
    "emitted_functions": [
      {
        "name": "__tf9DoorSwing",
        "compiled_body_bytes": 184
      }
    ]
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "inline_bases.o",
    "profile": "authored_cpp",
    "sha256": "f600195aa597e72c455ced2b8bf7f3714ad34b1b8ff33aea27f1a57c1aeb5c59",
    "source": "sources/w9-startup-crt-b-ti-inline-bases/inline_bases.cpp",
    "emitted_functions": []
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "update_mallinfo.o",
    "profile": "authored_cpp",
    "sha256": "9ea0cd3786abc0a251d75b5565e906ebd6501de4c391f1be0827f10c2ec1c62b",
    "source": "sources/w9-startup-crt-b-update-mallinfo/update_mallinfo.cpp",
    "emitted_functions": [
      {
        "name": "__malloc_update_mallinfo",
        "compiled_body_bytes": 152
      }
    ]
  }
]
```

The four inline-only type_info nodes have no function; their object only defines data
(`__ti4Prop`, `__ti9AutoChest`, `__ti10AutoObject`, `__ti13AutoContainer`, 12 bytes each).
The source `w9-startup-crt-b-ti-inline-bases/inline_bases.cpp` still needs an entry with an
empty `emitted_functions` list (listed above).

## Authored but never executed on the EE

All of it. What did run, on the host or in the private interpreter:

- Host checks (x86/arm64 host build) for every unit.
- Private interpreter comparisons against the original (call logs, memory effects):
  `__ieee754_powf` (7,936 cases), `__kernel_tanf` and `__ieee754_rem_pio2f` (recorded
  vectors, see their directories), Gold (5), GutChunk (5), Sparks (5), PhProp (48), VISetup
  (3), VIStation (3), Player (400 scenarios; original covered except padding no-ops). The
  VU0 macro instructions the VI constructors use were given a two-instruction shim in my
  private copy of the interpreter (`vsub.xyzw` of `vf0 - vf0`, `sqc2`).

## Unknown

- Meaning of nearly every field written by the constructors (offsets only, no names); the
  strings at the `view` addresses (their text is not quoted here or in the sources).
- The two integer arguments of Gold, GutChunk, Sparks and PhProp.
- Whether `__EH_FRAME_BEGIN__` should come from link-time section ordering or an explicit
  empty record; owner: integrator.
- The initial contents of `__new_handler` and `__terminate_func`.
- The first 1,552 bytes of the VIStation object (untouched by this constructor).
- The creature/player-side callees are unverified here (`__8Creature...`, `__4Prop...`,
  `initStuff__6PhPropi`, ...): the paired runs stub them.
