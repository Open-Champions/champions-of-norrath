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

`detail.hpp` sha256 `e03f840656717881a5a7e9ad8c0f469141446e8b0c76852bf8ea37684e32b736`.

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

## Imports closed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`, `norrath_construct_Lamp`, `norrath_construct_Lantern`, `norrath_construct_Generator`, `norrath_construct_Ice`

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
Already defined, so not opened: `norrath_block_alloc`, `norrath_construct_Vehicle`, `norrath_vehicle_reset`, `norrath_actors_b_view_4844a0`.

## Authored but never executed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`, `norrath_construct_Lamp`, `norrath_construct_Lantern`, `norrath_construct_Generator`, `norrath_construct_Ice` (host contracts and EE compilation only; not run as gameplay).

## Unknown

The AddParticle body, the bytes of `_vt$11AutoChestLP`, `_vt$15AutoContainerLP`, and `_vt$4Boat`, and whether any caller uses these constructors' V0. The remaining constructors in this package are not reconstructed yet.
