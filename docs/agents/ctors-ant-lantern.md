# Constructors from Ant through Lantern

Package: the 32 `norrath_construct_*` imports named in the task, from
`norrath_construct_Ant` through `norrath_construct_Lantern`. Branch
`agents/ctors-ant-lantern`. `norrath_construct_Vehicle` and
`norrath_vehicle_reset` stay imports. Virtual-table slots stay imports.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Addresses below are that file's symbol values. No game bytes or disassembly
listings are committed.

Each function is compiled with the reviewed EE g++ `authored_cpp` flags and
has a host contract under `host/` that returns non-zero when the stores, call
order, or result are wrong. Host files are not EE inputs. `snapshot.json`
still rejects unlisted files under `experimental/sources/`; this package does
not edit that file or `experimental/build-plan.json`.

Compiled bodies are larger than the image extents. These are behavior
reconstructions. Nothing here has run on the EE or as gameplay.

## Functions

| source | symbol | image | compiled bytes | sha256 |
|---|---|---|---|---|
| `ctors-ant-lantern-constructors/auto_chest_lp.cpp` | `norrath_construct_AutoChestLP` | `__11AutoChestLPPcG6Point3sPPc` `0x225250`, 144 bytes | 260 | `be93be3ab39e599d3b32a615ae8a30bf90c16db16df1c749feb7e8f497fd8001` |
| `ctors-ant-lantern-constructors/auto_container_lp.cpp` | `norrath_construct_AutoContainerLP` | `__15AutoContainerLPPcG6Point3sPPc` `0x225518`, 144 bytes | 260 | `19b4b1be540573960671aa15ee944640e5a3b0d414f7c43ad13ce1f4b2ad2a51` |
| `ctors-ant-lantern-constructors/boat.cpp` | `norrath_construct_Boat` | `__4BoatG6Point3sPPc` `0x2bfa20`, 208 bytes | 396 | `36b6d96cb284647a20d58e912d796d8f0c98781d3e00f81c87cc6e521323ab76` |
| `ctors-ant-lantern-constructors/lamp.cpp` | `norrath_construct_Lamp` | `__4LampG6Point3` `0x2309e8`, 312 bytes | 628 | `e699562cf2b5bb3d1918610f405d29f70330a50bae094e4b1bc1758e6a98f3b1` |
| `ctors-ant-lantern-constructors/lantern.cpp` | `norrath_construct_Lantern` | `__7LanternG6Point3` `0x230890`, 344 bytes | 744 | `a19c0f92cf3c8633b7a42cf8c72ceed6ec7c38de5f099aa9295df8725874d9c8` |
| `ctors-ant-lantern-constructors/generator.cpp` | `norrath_construct_Generator` | `__9GeneratorG6Point3sPPc` `0x1f19c0`, 400 bytes | 764 | `7f5ef2c76055c9f88e97c130a144f6d0e52c25c8e0fbcc8ef89e3d801a651208` |
| `ctors-ant-lantern-constructors/ice.cpp` | `norrath_construct_Ice` | `__3IceG6Point3sPcN43` `0x1e61f0`, 316 bytes | 688 | `02e1e3cc3b8a05bae91577c5d9f9f20779c84923358bd7cd94fbea499713e4af` |
| `ctors-ant-lantern-constructors/fire_fly.cpp` | `norrath_construct_FireFly` | `__7FireFlyG6Point3sPPc` `0x2118e8`, 484 bytes | 912 | `4fdda530636f1af3b261899994e176027e7cf27457982fcc5bdbc4071e553a84` |
| `ctors-ant-lantern-constructors/chest.cpp` | `norrath_construct_Chest` | `__5ChestG6Point3sPPci` `0x218790`, 596 bytes | 880 | `68fb4079e22d8db10461fc3457f9ab1ff8c5b3f49689b022f7100a33cd46ecba` |
| `ctors-ant-lantern-constructors/gnome.cpp` | `norrath_construct_Gnome` | `__5GnomeG6Point3sPPcPc` `0x2011f8`, 472 bytes | 920 | `605178b3f8b63ab60387a138aac482b55578cbc21aa516741cd336e5a5a915e1` |

`detail.hpp` sha256 `2dafa89b1e715cd398cd16b202ca40af455066f2be1150e5d050b2c86d3c334f`.

## Build-plan entries needed

Profile `authored_cpp`. Header closure is
`sources/ctors-ant-lantern-constructors/detail.hpp`. Do not add `host/`.

```json
{
  "header_closure": ["sources/ctors-ant-lantern-constructors/detail.hpp"],
  "kind": "independently_authored_source",
  "object_name": "auto_chest_lp.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/auto_chest_lp.cpp",
  "sha256": "be93be3ab39e599d3b32a615ae8a30bf90c16db16df1c749feb7e8f497fd8001",
  "emitted_functions": [{"name": "norrath_construct_AutoChestLP", "compiled_body_bytes": 260}]
}
```

```json
{
  "header_closure": ["sources/ctors-ant-lantern-constructors/detail.hpp"],
  "kind": "independently_authored_source",
  "object_name": "auto_container_lp.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/auto_container_lp.cpp",
  "sha256": "19b4b1be540573960671aa15ee944640e5a3b0d414f7c43ad13ce1f4b2ad2a51",
  "emitted_functions": [{"name": "norrath_construct_AutoContainerLP", "compiled_body_bytes": 260}]
}
```

```json
{
  "header_closure": ["sources/ctors-ant-lantern-constructors/detail.hpp"],
  "kind": "independently_authored_source",
  "object_name": "boat.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/boat.cpp",
  "sha256": "36b6d96cb284647a20d58e912d796d8f0c98781d3e00f81c87cc6e521323ab76",
  "emitted_functions": [{"name": "norrath_construct_Boat", "compiled_body_bytes": 396}]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "lamp.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/lamp.cpp",
  "sha256": "e699562cf2b5bb3d1918610f405d29f70330a50bae094e4b1bc1758e6a98f3b1",
  "emitted_functions": [
    {
      "name": "norrath_construct_Lamp",
      "compiled_body_bytes": 628
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "lantern.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/lantern.cpp",
  "sha256": "a19c0f92cf3c8633b7a42cf8c72ceed6ec7c38de5f099aa9295df8725874d9c8",
  "emitted_functions": [
    {
      "name": "norrath_construct_Lantern",
      "compiled_body_bytes": 744
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "generator.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/generator.cpp",
  "sha256": "7f5ef2c76055c9f88e97c130a144f6d0e52c25c8e0fbcc8ef89e3d801a651208",
  "emitted_functions": [
    {
      "name": "norrath_construct_Generator",
      "compiled_body_bytes": 764
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "ice.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/ice.cpp",
  "sha256": "02e1e3cc3b8a05bae91577c5d9f9f20779c84923358bd7cd94fbea499713e4af",
  "emitted_functions": [
    {
      "name": "norrath_construct_Ice",
      "compiled_body_bytes": 688
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "fire_fly.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/fire_fly.cpp",
  "sha256": "4fdda530636f1af3b261899994e176027e7cf27457982fcc5bdbc4071e553a84",
  "emitted_functions": [
    {
      "name": "norrath_construct_FireFly",
      "compiled_body_bytes": 912
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "chest.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/chest.cpp",
  "sha256": "68fb4079e22d8db10461fc3457f9ab1ff8c5b3f49689b022f7100a33cd46ecba",
  "emitted_functions": [
    {
      "name": "norrath_construct_Chest",
      "compiled_body_bytes": 880
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "gnome.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/gnome.cpp",
  "sha256": "605178b3f8b63ab60387a138aac482b55578cbc21aa516741cd336e5a5a915e1",
  "emitted_functions": [
    {
      "name": "norrath_construct_Gnome",
      "compiled_body_bytes": 920
    }
  ]
}
```

## What was reconstructed

Observed `__11AutoChestLPPcG6Point3sPPc` at `0x225250`, 144 bytes. The 12 point
bytes are copied and the short is sign-extended before `__9AutoChest`. The
vtable word is `0x44ea78` (`_vt$11AutoChestLP`) at object+72, stored before
`blockAlloc__Fi` of 388. A zero allocation does not call
`__11AddParticleG6Point3PPc` and V0 stays 0. A nonzero allocation constructs
AddParticle on that block with the same point copy and the argument list; V0
is AddParticle's V0. The allocation word is then stored at object+248.

Observed `__15AutoContainerLPPcG6Point3sPPc` at `0x225518`, 144 bytes. Same calls and
the same 388-byte allocation. The vtable word is `0x44e940`
(`_vt$15AutoContainerLP`). The parent is `__13AutoContainer`. The allocation
word is stored at object+328.

Observed `__4BoatG6Point3sPPc` at `0x2bfa20`, 208 bytes. It calls
`__7VehicleG6Point3sPPci` with flag 1, stores `_vt$4Boat` (`0x453328`) at
object+72, calls `worldCheckForWater` with the `world` pointer at `0x4844a0`,
a snapshot of object+16, immediate single 32, and output at object+24, copies
those 12 bytes to object+1632, clears bit `0x00100000`, and returns
`Reset__7Vehiclei(object, 0)`.

Observed `__4LampG6Point3` at `0x2309e8`, 312 bytes. Same shape as the Candle constructor: GameObject with two null arguments, an embedded light at object+76 that is filled before `lightAdd`, then flag, byte and half stores and the run-list call. The light tint word comes from global `0x481990`. The vtable word is `0x44a810` (`_vt$4Lamp`). V0 is the run-list result.

Observed `__7LanternG6Point3` at `0x230890`, 344 bytes. Same light setup as Lamp (vtable `0x44a8a0`, tint global `0x48198c`), but the flag bits set are `0x10001`, the 12 bytes at object+16 are copied to object+136 after `lightAdd`, and bytes 148 and 149 are cleared. V0 is the run-list result.

Observed `__9GeneratorG6Point3sPPc` at `0x1f19c0`, 400 bytes. Calls the Creature constructor (null map, scale 8.0), stores type bytes and words including the vtable `0x44bbd0` (`_vt$9Generator`) and the global at `0x4844a4`, then `setParameters` with five 1.0 values and 1. Three tag lookups (a string at `0x46ded0`, integers at `0x46dee0` and `0x46def0`) fill object+960 and bits 1 and 2 of object+980. Then it joins the slow run list and compares the stored string with the text at `0x485230`; equality sets bit 2 again. Inferred, not confirmed: the three tags name a generator and two trigger flags. V0 is the comparison result when nonzero, otherwise the updated flag word.

Observed `__3IceG6Point3sPcN43` at `0x1e61f0`, 316 bytes. The five trailing arguments are all char pointers. After GameObject (two nulls), byte 40 is 120 and the vtable is `0x44cc28` (`_vt$3Ice`). `scriptGetGameState` on the text at `0x46d070`: if its 64-bit result is nonzero the constructor calls virtual slot 3 of its own table (short adjustment at vtable+8, function at vtable+12, flag 3) and stops; otherwise it stores the arguments at 76/80/84/88/92, two `lumpFindResource` results at 96/100, 200.0 at 28 and 32, and the run-list V0 is returned.

Observed `__7FireFlyG6Point3sPPc` at `0x2118e8`, 484 bytes. Calls the FireBeetle constructor with flag 0, stores vtable `0x44bdb0` (`_vt$7FireFly`), fills and registers a light at object+1064 (tint singles from globals `0x481540`/`0x481544`), calls `setParameters`, sets several singles and flag bits, initialises the animation state at object+288 with the header at (object+36)->+68, randomises its time, joins the slow run list and returns `finishConstruction`'s V0.

Observed `__5ChestG6Point3sPPci` at `0x218790`, 596 bytes. GameObject gets the pointer-table entry at `0x42c8a0` indexed by the int argument and the argument list. Vtable `0x44cb98` (`_vt$5Chest`), byte 88 from `eRand < 2000`, overridden by two level-text comparisons (text at `0x34b6b1`, a field inside `levelName`), an optional float tag (`0x485438`) that scales the roll, an int tag (`0x485440`), and bits of `levelFlags` (`0x4844c0`). Half 76 is `(int)((eRand%12+12) * scale * gameTreasureRamp)`, mapped to a class (0/1/2/4, or 8 and 76 cleared when byte 88 is set) that goes to `modelGetActiveChunks`; animation state at object+92 is then initialised. V0 after the last call is not established; this returns 0. Whether R5900 float rounding changes the truncated amount is unknown.

Observed `__5GnomeG6Point3sPPcPc` at `0x2011f8`, 472 bytes. The last argument is a name string. Creature gets the table at `0x42ba00`, scale 10.0. Vtable `0x44b9f0` (`_vt$5Gnome`). Word 964 is `eRand % 3` and is replaced by 0, 1 or 2 when the name matches (`strticmp`) the texts at `0x46f310`, `0x46f320`, `0x46f330` in that order. `initType__5Gnome` runs, then the eight-byte `modelGetActiveChunks` result is stored at +552, the animation state at +288 is set up and the object joins the slow run list. V0 is `finishConstruction`'s. Inferred only: the three texts are variant names.

## Imports closed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`, `norrath_construct_Lamp`, `norrath_construct_Lantern`, `norrath_construct_Generator`, `norrath_construct_Ice`, `norrath_construct_FireFly`, `norrath_construct_Chest`, `norrath_construct_Gnome`

## Imports opened

`__11AddParticleG6Point3PPc` — original AddParticle constructor. Not defined here.
`norrath_construct_AutoChest` — original `__9AutoChestPcG6Point3sPPc` at `0x21eee8`. Defined later in this package once reconstructed; unresolved until that unit is linked.
`norrath_construct_AutoContainer` — original `__13AutoContainerPcG6Point3sPPc` at `0x221118`. Same status.
`norrath_ctors_ant_lantern_view_44ea78` — address of `_vt$11AutoChestLP` at `0x44ea78`. Slots are not defined.
`norrath_ctors_ant_lantern_view_44e940` — address of `_vt$15AutoContainerLP` at `0x44e940`. Slots are not defined.
`norrath_ctors_ant_lantern_view_453328` — address of `_vt$4Boat` at `0x453328`. Slots are not defined.
`norrath_world_check_for_water` — original `worldCheckForWater__FP12_worldHeaderG6Point3fPf` at `0x17db78`. Already pending from other packages; still not defined here.

`norrath_game_object_construct`, `norrath_light_add`, `norrath_actor_add_run_list` — original GameObject constructor, `lightAdd__FP5Light`, `objectAddToRunList__FP10GameObject`; also used by actors-a.
`norrath_ctors_ant_lantern_view_44a810`, `norrath_ctors_ant_lantern_view_481990` — `_vt$4Lamp` and the 4-byte tint global at `0x481990`.
`norrath_ctors_ant_lantern_view_44a8a0` (`_vt$7Lantern`) and `norrath_ctors_ant_lantern_view_48198c` (tint global) — slots/bytes not defined here.
`norrath_creature_construct`, `norrath_actor_set_parameters`, `norrath_actor_find_tag_string`, `norrath_game_object_find_tag_int`, `norrath_actor_add_slow_run_list`, `norrath_factory_strcasecmp` — existing names, also used by actors-b and others; `norrath_actor_view_4844a4` exists in other packages.
`norrath_ctors_ant_lantern_view_44bbd0` (`_vt$9Generator`), `norrath_ctors_ant_lantern_view_46ded0`, `_46dee0`, `_46def0` (tag text) and `_485230` (comparison text) — address views only.
`norrath_script_get_game_state`, `norrath_lump_find_resource` — existing names; `norrath_ctors_ant_lantern_view_44cc28` (`_vt$3Ice`) and `_46d070` (state text) — address views only.
`norrath_construct_FireBeetle` — original \`__10FireBeetleG6Point3sPPci\` at `0x20ff48`; defined later in this package. `norrath_actor_animation_initialize`, `norrath_actor_animation_randomize_time`, `norrath_actor_animation_add`, `norrath_actor_finish_construction`, `norrath_light_add`, `norrath_actor_set_parameters` — existing names from other packages.
`norrath_ctors_ant_lantern_view_44bdb0` (`_vt$7FireFly`), `_481540` and `_481544` (single-precision globals) — address views only.
`norrath_actor_random`, `norrath_player_find_tag_float`, `norrath_player_treasure_ramp`, `norrath_actor_model_active_chunks`, `strcmp` — existing names from other packages / libc. Address views: `norrath_ctors_ant_lantern_view_42c8a0`, `_44cb98`, `_34b6b1`, `_485398`, `_485420`, `_485438`, `_485440`, `_4844c0`.
`strticmp__FPCcT0` (original at `0x155ab8`) and `initType__5Gnome` (original at `0x2023e8`) — original names, not defined here. Address views `norrath_ctors_ant_lantern_view_42ba00`, `_44b9f0`, `_480b10`, `_480b14`, `_46f310`, `_46f320`, `_46f330`.
Already defined, so not opened: `norrath_block_alloc`, `norrath_construct_Vehicle`, `norrath_vehicle_reset`, `norrath_actors_b_view_4844a0`.

## Authored but never executed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`, `norrath_construct_Lamp`, `norrath_construct_Lantern`, `norrath_construct_Generator`, `norrath_construct_Ice`, `norrath_construct_FireFly`, `norrath_construct_Chest`, `norrath_construct_Gnome` (host contracts and EE compilation only; not run as gameplay).

## Unknown

The AddParticle body, the bytes of `_vt$11AutoChestLP`, `_vt$15AutoContainerLP`, and `_vt$4Boat`, and whether any caller uses these constructors' V0. The remaining constructors in this package are not reconstructed yet.
