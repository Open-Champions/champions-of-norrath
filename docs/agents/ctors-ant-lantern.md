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

`detail.hpp` sha256 `a135670598cdfdc9351eeb1aafd277e007ddc4b94ceee3f04cf4f9a7d89839dd`.

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

## What was reconstructed

Observed `__11AutoChestLPPcG6Point3sPPc` at `0x225250`, 144 bytes. The 12 point
bytes are copied and the short is sign-extended before `__9AutoChest`. The
vtable word is `0x44ea78` (`_vt$11AutoChestLP`) at object+72, stored before
`blockAlloc__Fi` of 388. A zero allocation does not call
`__11AddParticleG6Point3PPc` and V0 stays 0. A nonzero allocation constructs
AddParticle on that block with the same point copy and the argument list; V0
is AddParticle's V0. The allocation word is then stored at object+248.

## Imports closed

`norrath_construct_AutoChestLP`

## Imports opened

`__11AddParticleG6Point3PPc` — original AddParticle constructor. Not defined here.
`norrath_construct_AutoChest` — original `__9AutoChestPcG6Point3sPPc` at `0x21eee8`. Defined later in this package once reconstructed; unresolved until that unit is linked.
`norrath_ctors_ant_lantern_view_44ea78` — address of `_vt$11AutoChestLP` at `0x44ea78`. Slots are not defined.

Already defined, so not opened: `norrath_block_alloc`.

## Authored but never executed

`norrath_construct_AutoChestLP` (host contract and EE compilation only; not run as gameplay).

## Unknown

The AddParticle body, the bytes of `_vt$11AutoChestLP`, and whether any caller uses this constructor's V0. The remaining constructors in this package are not reconstructed yet.
