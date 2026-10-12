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

`detail.hpp` sha256 `e3fc83d133e1bf2462ff3072766329ad81533b60d3696f530a6e80095cd3140d`.

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

## Imports closed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`

## Imports opened

`__11AddParticleG6Point3PPc` — original AddParticle constructor. Not defined here.
`norrath_construct_AutoChest` — original `__9AutoChestPcG6Point3sPPc` at `0x21eee8`. Defined later in this package once reconstructed; unresolved until that unit is linked.
`norrath_construct_AutoContainer` — original `__13AutoContainerPcG6Point3sPPc` at `0x221118`. Same status.
`norrath_ctors_ant_lantern_view_44ea78` — address of `_vt$11AutoChestLP` at `0x44ea78`. Slots are not defined.
`norrath_ctors_ant_lantern_view_44e940` — address of `_vt$15AutoContainerLP` at `0x44e940`. Slots are not defined.
`norrath_ctors_ant_lantern_view_453328` — address of `_vt$4Boat` at `0x453328`. Slots are not defined.
`norrath_world_check_for_water` — original `worldCheckForWater__FP12_worldHeaderG6Point3fPf` at `0x17db78`. Already pending from other packages; still not defined here.

Already defined, so not opened: `norrath_block_alloc`, `norrath_construct_Vehicle`, `norrath_vehicle_reset`, `norrath_actors_b_view_4844a0`.

## Authored but never executed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat` (host contracts and EE compilation only; not run as gameplay).

## Unknown

The AddParticle body, the bytes of `_vt$11AutoChestLP`, `_vt$15AutoContainerLP`, and `_vt$4Boat`, and whether any caller uses these constructors' V0. The remaining constructors in this package are not reconstructed yet.
