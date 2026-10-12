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
| `ctors-ant-lantern-constructors/demon.cpp` | `norrath_construct_Demon` | `__5DemonG6Point3sPPc` `0x1e9bc0`, 740 bytes | 1068 | `fed73d8ac1415bc0263573d0001c815f8879217ddffb4b374014780d765d7319` |
| `ctors-ant-lantern-constructors/cyclops.cpp` | `norrath_construct_Cyclops` | `__7CyclopsG6Point3sPPci` `0x202678`, 912 bytes | 1508 | `d43a3d3d210f1a9bb734209343f92db5f60b9dcc86c92f6d94586609296151a1` |
| `ctors-ant-lantern-constructors/fire_beetle.cpp` | `norrath_construct_FireBeetle` | `__10FireBeetleG6Point3sPPci` `0x20ff48`, 1008 bytes | 1652 | `2c2b5af6cc978ec2b8280576a0cb391a4300899fa02606f5703de2ef2b4359de` |
| `ctors-ant-lantern-constructors/auto_chest.cpp` | `norrath_construct_AutoChest` | `__9AutoChestPcG6Point3sPPc` `0x21eee8`, 704 bytes | 1052 | `a921d51b7df7006ca1833c20c21241f11d0904850229977bcfb89a5c8bc83d44` |
| `ctors-ant-lantern-constructors/auto_prop.cpp` | `norrath_construct_AutoProp` | `__8AutoPropPcG6Point3sPPc` `0x220278`, 1328 bytes | 2260 | `49f65e85cf20e80c14afcd665cb421f501a359c50e5d7604e30acafbed63ec82` |
| `ctors-ant-lantern-constructors/auto_prop_lp.cpp` | `norrath_construct_AutoPropLP` | `__10AutoPropLPPcG6Point3sPPc` `0x223bd8`, 1828 bytes | 2124 | `94ab3b0b8551697103022a847caaeb4d4a8f040e64a6c25819877991957a0a1f` |
| `ctors-ant-lantern-constructors/auto_container.cpp` | `norrath_construct_AutoContainer` | `__13AutoContainerPcG6Point3sPPc` `0x221118`, 1876 bytes | 1700 | `755c4131287e74d14af4234c18ff395089d39d6c766725fa7ced2f6b496b69d6` |
| `ctors-ant-lantern-constructors/ant_queen.cpp` | `norrath_construct_AntQueen` | `__8AntQueenG6Point3sPPc` `0x206c08`, 672 bytes | 1240 | `b8708886b3f2f535c4ed83fb72181ecaca7f6f72faaedfc82a11b852c57860a8` |
| `ctors-ant-lantern-constructors/cthulu.cpp` | `norrath_construct_Cthulu` | `__6CthuluG6Point3sPPc` `0x280a88`, 796 bytes | 1392 | `4f5add1f2419bce139cdcdb794be130faf8c805a53c199e6fce2f1afc35a7cd3` |
| `ctors-ant-lantern-constructors/arenabeast.cpp` | `norrath_construct_Arenabeast` | `__10ArenabeastG6Point3sPPc` `0x235478`, 888 bytes | 1380 | `79410540ac8ec05661f14a93f2b2c294247d6ecce2ebd036b61a7faa1ff7ae6a` |
| `ctors-ant-lantern-constructors/ghoul.cpp` | `norrath_construct_Ghoul` | `__5GhoulG6Point3siPPc` `0x1f62e8`, 852 bytes | 1196 | `dcd8561f3b62d89461db09859d46d1a971662e89c52437ffced89262f3b93cc2` |
| `ctors-ant-lantern-constructors/badger.cpp` | `norrath_construct_Badger` | `__6BadgerG6Point3sPPci` `0x1eef40`, 924 bytes | 1356 | `82b21131bc106be592822d587beaa6a121ae85ddefdc3b83303dfd000b9b01e1` |
| `ctors-ant-lantern-constructors/black_widow.cpp` | `norrath_construct_BlackWidow` | `__10BlackWidowG6Point3sPPcPc` `0x209c20`, 896 bytes | 1364 | `7d5294ccabdedfe07ba739b9baeafe41bbd2fd9d84dfa6aebba9d933609c8bd0` |
| `ctors-ant-lantern-constructors/froglock.cpp` | `norrath_construct_Froglock` | `__8FroglockG6Point3sPPcPc` `0x1ff820`, 880 bytes | 1272 | `59e3d75582c155db40d7bb5f6b60381fa182c16df76324ff787d87fcc6af687d` |

`detail.hpp` sha256 `c9a61b66fce6e74093079d1a1ca52e736427ace4112dabcb2f80cbb754a3ec62`.

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

```json
{
  "header_closure": [
    "sources/ctors-ant-lantern-constructors/detail.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "demon.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/demon.cpp",
  "sha256": "fed73d8ac1415bc0263573d0001c815f8879217ddffb4b374014780d765d7319",
  "emitted_functions": [
    {
      "name": "norrath_construct_Demon",
      "compiled_body_bytes": 1068
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
  "object_name": "cyclops.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/cyclops.cpp",
  "sha256": "d43a3d3d210f1a9bb734209343f92db5f60b9dcc86c92f6d94586609296151a1",
  "emitted_functions": [
    {
      "name": "norrath_construct_Cyclops",
      "compiled_body_bytes": 1508
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
  "object_name": "fire_beetle.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/fire_beetle.cpp",
  "sha256": "2c2b5af6cc978ec2b8280576a0cb391a4300899fa02606f5703de2ef2b4359de",
  "emitted_functions": [
    {
      "name": "norrath_construct_FireBeetle",
      "compiled_body_bytes": 1652
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
  "object_name": "auto_chest.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/auto_chest.cpp",
  "sha256": "a921d51b7df7006ca1833c20c21241f11d0904850229977bcfb89a5c8bc83d44",
  "emitted_functions": [
    {
      "name": "norrath_construct_AutoChest",
      "compiled_body_bytes": 1052
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
  "object_name": "auto_prop.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/auto_prop.cpp",
  "sha256": "49f65e85cf20e80c14afcd665cb421f501a359c50e5d7604e30acafbed63ec82",
  "emitted_functions": [
    {
      "name": "norrath_construct_AutoProp",
      "compiled_body_bytes": 2260
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
  "object_name": "auto_prop_lp.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/auto_prop_lp.cpp",
  "sha256": "94ab3b0b8551697103022a847caaeb4d4a8f040e64a6c25819877991957a0a1f",
  "emitted_functions": [
    {
      "name": "norrath_construct_AutoPropLP",
      "compiled_body_bytes": 2124
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
  "object_name": "auto_container.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/auto_container.cpp",
  "sha256": "755c4131287e74d14af4234c18ff395089d39d6c766725fa7ced2f6b496b69d6",
  "emitted_functions": [
    {
      "name": "norrath_construct_AutoContainer",
      "compiled_body_bytes": 1700
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
  "object_name": "ant_queen.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/ant_queen.cpp",
  "sha256": "b8708886b3f2f535c4ed83fb72181ecaca7f6f72faaedfc82a11b852c57860a8",
  "emitted_functions": [
    {
      "name": "norrath_construct_AntQueen",
      "compiled_body_bytes": 1240
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
  "object_name": "cthulu.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/cthulu.cpp",
  "sha256": "4f5add1f2419bce139cdcdb794be130faf8c805a53c199e6fce2f1afc35a7cd3",
  "emitted_functions": [
    {
      "name": "norrath_construct_Cthulu",
      "compiled_body_bytes": 1392
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
  "object_name": "arenabeast.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/arenabeast.cpp",
  "sha256": "79410540ac8ec05661f14a93f2b2c294247d6ecce2ebd036b61a7faa1ff7ae6a",
  "emitted_functions": [
    {
      "name": "norrath_construct_Arenabeast",
      "compiled_body_bytes": 1380
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
  "object_name": "ghoul.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/ghoul.cpp",
  "sha256": "dcd8561f3b62d89461db09859d46d1a971662e89c52437ffced89262f3b93cc2",
  "emitted_functions": [
    {
      "name": "norrath_construct_Ghoul",
      "compiled_body_bytes": 1196
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
  "object_name": "badger.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/badger.cpp",
  "sha256": "82b21131bc106be592822d587beaa6a121ae85ddefdc3b83303dfd000b9b01e1",
  "emitted_functions": [
    {
      "name": "norrath_construct_Badger",
      "compiled_body_bytes": 1356
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
  "object_name": "black_widow.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/black_widow.cpp",
  "sha256": "7d5294ccabdedfe07ba739b9baeafe41bbd2fd9d84dfa6aebba9d933609c8bd0",
  "emitted_functions": [
    {
      "name": "norrath_construct_BlackWidow",
      "compiled_body_bytes": 1364
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
  "object_name": "froglock.o",
  "profile": "authored_cpp",
  "source": "sources/ctors-ant-lantern-constructors/froglock.cpp",
  "sha256": "59e3d75582c155db40d7bb5f6b60381fa182c16df76324ff787d87fcc6af687d",
  "emitted_functions": [
    {
      "name": "norrath_construct_Froglock",
      "compiled_body_bytes": 1272
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

Observed `__5DemonG6Point3sPPc` at `0x1e9bc0`, 740 bytes. Creature gets the table at `0x42af70` and scale 10.0; vtable `0x44dd68` (`_vt$5Demon`). It fills four halves with 0xffff, two bytes with 0xff and a 16-byte zero run in the object range 1048..1093, sets flag bits in the 64-bit word at +160, several singles and clears. A random roll scaled by 2^-16 under 0.5 triggers a second roll stored as (r&3)+1 at +208. `initAttackVariant__5Demon`, `setParameters`, animation setup with the header at model+68, the slow run list, five `setAnimEventFlags` calls (indexes 3, 4, 5, 14, 15), and `finishConstruction`'s V0.

Observed `__7CyclopsG6Point3sPPci` at `0x202678`, 912 bytes. `eRand` is called before Creature; the Creature scale is `(eRand % 14) + 4` as a single and the map is the 216-byte record at `0x42bb08 + 216 * kind`. Vtable `0x44bf90` (`_vt$7Cyclops`). After `setParameters` a second roll builds the word at +244 from bits 1, 2, 4, 8 and 0x10. When bit 0x10 is clear the original replaces the whole word with 0x20 (the generated code ORs 0x20 into a register that holds zero); the reconstruction reproduces that. Then `modelGetActiveChunks` (eight-byte result stored at +552), animation setup with the header at model+108, slow run list, eight `setAnimEventFlags` calls (indexes 3, 4, 5, 6, 8, 9, 16, 18) and `finishConstruction`'s V0.

Observed `__10FireBeetleG6Point3sPPci` at `0x20ff48`, 1008 bytes. `eRand` first, scale `(eRand % 6) + 8`, map the 152-byte record at `0x42c308 + 152 * kind`, vtable `0x44bea0` (`_vt$10FireBeetle`). Four 0xffff entry heads at 1028..1052, a marker pair and a zero run at 1008..1025, `setParameters` with the single at `0x4814e4`, flag bits, singles, a factor from a second roll stored at +224 and multiplied into +28, animation setup with the header at model+84. A third roll under the single at `0x4814f0` calls a virtual function on the object whose pointer is stored at `0x34b474` (table pointer at its offset 96, adjustment and function at that table+96/+100) with 0 and the truncated single from creature view `0x484cac`, and records the result and a second value in the first free 8-byte entry. Then the slow run list, four `setAnimEventFlags` calls (3, 4, 5, 6), bit 1 into +244, an eight-byte chunk result at +552 and `finishConstruction`'s V0. The entry search can only hit the first entry because all four heads are freshly 0xffff; the 21 marker branch is reproduced but unreachable from here. What the 0x34b474 object is, is unknown.

Observed `__9AutoChestPcG6Point3sPPc` at `0x21eee8`, 704 bytes. Calls AutoObject (`__10AutoObjectPcG6Point3sPPc` at `0x21e548`) with the name, a point snapshot, the sign-extended short and the argument list; stores flag bits `0x00100018` and vtable `0x44ece8` (`_vt$9AutoChest`). A mode tag (`0x4854c8`) matched against three texts sets byte 240 to 0/1/2; a text compared with the name sets word 236; the roll and class logic is the same as Chest (tags at `0x4854f0`/`0x4854f8`, levelFlags, treasure ramp) with an extra clear when a further roll is below 10000 and a name text at `0x46fd38` that clears byte 228. The name at +76 plus the suffix at `0x485500` is looked up in the lump named at +109 and stored at +244; the eight-byte chunk result is stored at +200. V0 after the last call is not established; this returns 0.

Observed `__8AutoPropPcG6Point3sPPc` at `0x220278`, 1328 bytes. AutoObject first, then vtable `0x44ec40` (`_vt$8AutoProp`), a block of integer-tag tests that set bits in the word at +284 (and one that sets `0x10000` in +12), a count tag at `0x485538` that selects either one lump lookup (+296) or a counted series of lookups at an 8-byte stride from +296 using two format texts, a ten-character string tag whose '1' characters set bit 0 of the words at +300+8i, two alternative "door" and "switch" tag paths (the door path queries the world at `0x4844a0` and stores a door index at +396 and a height single at +400), animation add with flag 6 plus set-to-end or flag 1 plus randomise, name copies with two suffixes at +216/+248, the eight-byte chunk result at +200, `setPassableState`, an optional `setState`, the slow run list, and two name comparisons that either edit the resource header at +296 in place or set byte 40 to 0xa8. The tag texts at these addresses read as property names; that is inferred, not confirmed. V0 is not established; this returns 0.

Observed `__10AutoPropLPPcG6Point3sPPc` at `0x223bd8`, 1828 bytes. AutoProp first; vtable `0x44e9d0` (`_vt$10AutoPropLP`). Four optional 388-byte particle blocks (selected by `hasLightOrParticleNumber(arguments, n)`; a failed allocation is stored as null and AddParticle with the extra integer is skipped) are stored at +408..+420 and copied to +424..+436. For each of four indexes it builds two tables of ten (first, second) word pairs at object+456+80i and object+776+80i from a string tag (characters '1' give (0,-1), others (-1,0)) and two families of counted integer tags, with a fallback of (0,-1) when nothing was set. Finally a name equal (`strticmp`) to either of two texts stores 30 at +380 of each live block. The helpers are always-inline so the object emits one function. V0 is not established; this returns 0.

Observed `__13AutoContainerPcG6Point3sPPc` at `0x221118`, 1876 bytes. AutoObject first, vtable `0x44ebb0` (`_vt$13AutoContainer`), two name-plus-suffix lump lookups (+216, +220), three integer tags that set bits in +292, a mode tag matched against four texts, and a fixed sequence of 29 `strticmp` name comparisons that store 0, 1 or 2 into byte 288 (the texts form a 16-byte-spaced run from `0x471030` and an 8-byte-spaced run from `0x485580`; the call order and values are in a small constant table in the source). It then sets flag bits, writes -1 to the eight bytes at +200, records the low byte of a `worldCheckForWater` call with a null output, and, when a string tag and an integer tag at `0x471150` both exist, builds two Blockers 50 units ahead of and 40 units behind the object along (icos, isin) of the type plus 0x4000. Each live block gets 0x4000 in its +12 and 36.0 at +28; allocation failures are stored as null. The stack vector the original also fills is never read and is not reproduced. V0 is not established; this returns 0.

Observed `__8AntQueenG6Point3sPPc` at `0x206c08`, 672 bytes. `eRand` first; Creature scale `(eRand % 14) + 4`, map table `0x42bf58`, vtable `0x44c440` (`_vt$8AntQueen`). Stores include 140.0, 45.0, bit patterns in the 64-bit word at +160 (low `0x20001000`, high `0x80`), 180 at +972 and a second roll mapped into +976. `setParameters` gets 48.0, two globals, 4.0 and 10.5; the eight-byte chunk result goes to +552; animation header from model+124; slow run list; five `setAnimEventFlags` calls (3, 4, 5, 6, 19). V0 is `finishConstruction`'s.

Observed `__6CthuluG6Point3sPPc` at `0x280a88`, 796 bytes. Creature with table `0x438a38` and scale 10.0, vtable `0x450160` (`_vt$6Cthulu`). A roll scaled by 2^-16 and a global single is widened (`fptodp`), added to a double constant stored at `0x476ac8` (`dpadd`) and narrowed (`dptofp`); the result is stored at +224, multiplied into +28 and (times -15.0) stored at +228. Other rolls fill +960, +964 and +972. `setParameters`, animation header from model+76, slow run list, seven `setAnimEventFlags` calls (3, 4, 5, 6, 11, 18, 14 with a2 = 4), and `finishConstruction`'s V0.

Observed `__10ArenabeastG6Point3sPPc` at `0x235478`, 888 bytes. Creature with table `0x42d200` and scale 10.0, vtable `0x44f6a8` (`_vt$10Arenabeast`), bytes 85 and 80 at +40/+41, flag bits in the 64-bit word at +160, singles, `setParameters` (60.0, two globals, 2.0, 10.5), animation header from model+140, slow run list, eleven `setAnimEventFlags` calls (5, 6, 7, 8, 10, 11, 12, 9 with a2 = 17, 3 and 4 and 13 with a2 = 16) and a final roll stored at +964 as +808 plus (roll % 300) plus 300 before `finishConstruction`, whose V0 is returned.

Observed `__5GhoulG6Point3siPPc` at `0x1f62e8`, 852 bytes. Three `eRand` rolls give the Creature scale (r1&7)+(r2&7)+(r3&7)+18. The int argument selects the map table (`0x42b630` when nonzero, `0x42b548` when zero). Vtable `0x44bae0` (`_vt$5Ghoul`), a few singles, `setParameters` with a global and four 1.0 values, animation header from model+172 plus randomised time, slow run list, eight `setAnimEventFlags` calls (3, 4, 5, 5, 17, 18, 19, 20), the eight-byte chunk result at +552 and `finishConstruction`'s V0.

Observed `__6BadgerG6Point3sPPci` at `0x1eef40`, 924 bytes. Creature with the 176-byte record at `0x42b1d0 + 176 * kind` as map and scale 10.0, vtable `0x44c350` (`_vt$6Badger`), the kind's low byte stored at +964. When that byte (as signed) equals 1 a set of singles and one `setParameters` call are used; otherwise byte 40 becomes 45, +228 becomes -15.0, and the level text at `0x34b6b1` is compared with a text at `0x46dbe0` to choose between two `setParameters` calls. Then the eight-byte chunk result at +552, animation header from model+132, slow run list, seven `setAnimEventFlags` calls (4, 5, 6, 7, 10, 11, 12) and `finishConstruction`'s V0.

Observed `__10BlackWidowG6Point3sPPcPc` at `0x209c20`, 896 bytes. The last argument is a name string. Two `eRand` rolls give the Creature scale (r1&7)+(r2&7)+32, map `0x42c188`. Vtable `0x44c260` (`_vt$10BlackWidow`), a `Web` (`__3Webii`, 35 and 180) constructed at object+976, flag bits and singles, `setParameters` with 1.0, a global and three 1.0 values. The name is compared (`strticmp`) with a text at `0x46ffe0`: on a match the header at model+148 is added with flag 4 and extra flag bits and half 572 = 16 are set; otherwise the header at model+116 is added with flag 1 and its time randomised. Then the eight-byte chunk result at +552, slow run list, eight `setAnimEventFlags` calls (16, 4, 6, 8, 9, 10, 11, 18) and `finishConstruction`'s V0.

Observed `__8FroglockG6Point3sPPcPc` at `0x1ff820`, 880 bytes. The last argument is a name string. Creature with table `0x42b958` and scale 10.0, vtable `0x44bcc0` (`_vt$8Froglock`), the same four-halves / marker-bytes / zero-run block as Demon at 1048..1093, flag bits, singles and clears, +960 = 17. Two `strticmp` name comparisons (texts at `0x46f120` and `0x46f130`) optionally change +960 to 16 and, behind a roll scaled by 2^-16 against a global threshold, store a small value at +208. Then `initAttackVariant__8Froglock`, `setParameters`, the eight-byte chunk result at +552, animation header from model+60, slow run list, five `setAnimEventFlags` calls (12, 3, 13, 4, 5) and `finishConstruction`'s V0. Inferred only: the two texts read as weapon-variant names.

## Imports closed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`, `norrath_construct_Lamp`, `norrath_construct_Lantern`, `norrath_construct_Generator`, `norrath_construct_Ice`, `norrath_construct_FireFly`, `norrath_construct_Chest`, `norrath_construct_Gnome`, `norrath_construct_Demon`, `norrath_construct_Cyclops`, `norrath_construct_FireBeetle`, `norrath_construct_AutoChest`, `norrath_construct_AutoProp`, `norrath_construct_AutoPropLP`, `norrath_construct_AutoContainer`, `norrath_construct_AntQueen`, `norrath_construct_Cthulu`, `norrath_construct_Arenabeast`, `norrath_construct_Ghoul`, `norrath_construct_Badger`, `norrath_construct_BlackWidow`, `norrath_construct_Froglock`

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
`initAttackVariant__5Demon` (original at `0x1e9ea8`), `setAnimEventFlags__8Creatureififififi` (original at `0x1128a8`; argument order int, float, int, float, int, float, int, float, int is read from the call registers) — original names, not defined here. Address views `norrath_ctors_ant_lantern_view_42af70`, `_44dd68`, `_480ba8`, `_480bac`, `_480bb0`, `_480bb4`.
Address views `norrath_ctors_ant_lantern_view_42bb08`, `_44bf90`, `_481160` through `_48117c` (eight singles). `setAnimEventFlags__8Creatureififififi` as for Demon.
Address views `norrath_ctors_ant_lantern_view_42c308`, `_44bea0`, `_4814e4` through `_4814f8` (six singles), `_34b474` (an object pointer word). Reuses `norrath_creature_view_484cac` (already defined by data-views).
`norrath_construct_AutoObject` — original `__10AutoObjectPcG6Point3sPPc` at `0x21e548`; not in this package, unresolved. `strcpy`, `strcat` — libc names. `norrath_factory_strcasecmp` replaces the original `strcasecmp` call. Address views `norrath_ctors_ant_lantern_view_44ece8`, `_46fd38`, `_4854c8` through `_4854f8` and `_485500`.
`norrath_factory_sprintf`, `norrath_world_find_closest_door`, `norrath_world_set_door_collision`, `norrath_world_get_door_height`, `norrath_animation_set_to_end`, `norrath_actor_animation_add` — existing names. `setPassableState__8AutoPropb` (original at `0x221040`) and `setState__8AutoPropi` (original at `0x220f30`) — original names, not defined here. Address views `norrath_ctors_ant_lantern_view_44ec40`, `_470df8`, `_470e08`, `_470e18`, `_470e28`, `_470e38`, `_470e48`, `_470e58`, `_470e68`, `_470e78`, `_470e88`, `_470e98`, `_470ea8`, `_470eb8`, `_485480`, `_485488`, `_485530`, `_485538`, `_485540`, `_485548`, `_481704`, `_481708`.
`hasLightOrParticleNumber__FPPci` (original at `0x21df18`) and `__11AddParticleG6Point3PPci` (original at `0x222bb0`; a different overload from the AddParticle that AutoChestLP and AutoContainerLP call) — original names, not defined here. `strticmp__FPCcT0` as for Gnome. Address views `norrath_ctors_ant_lantern_view_44e9d0`, `_4711f0`, `_471208`, `_471218`, `_471230`, `_471248`, `_471260`, `_471278`, `_485760`, `_485768`.
`norrath_construct_Blocker` (existing, actors-a), `norrath_world_check_for_water` (already pending), `icos__Fi` and `isin__Fi` (original names, already pending from other packages), `strticmp__FPCcT0`. Address views `norrath_ctors_ant_lantern_view_44ebb0`, `_470ff0`, `_471000`, `_471010`, `_471020`, `_471030` (start of a 18-slot, 16-byte-spaced text run), `_471150`, `_485560`, `_485568`, `_485570`, `_485578`, `_485580` (start of an 11-slot, 8-byte-spaced text run). The run spacing is read from the compare immediates, not from the executable layout.
Address views `norrath_ctors_ant_lantern_view_42bf58`, `_44c440`, `_4812d8` through `_4812ec` (six singles).
`dpadd` (original at `0x2fb5c0`) and `dptofp` (original at `0x2fbc20`) — software double routines used by the original; the reconstruction calls them and `norrath_entity_fptodp` (existing, original `fptodp`). The double at `0x476ac8` is read as two words. Address views `norrath_ctors_ant_lantern_view_438a38`, `_450160`, `_476ac8`, `_482328` through `_482340` (seven singles).
Address views `norrath_ctors_ant_lantern_view_42d200`, `_44f6a8`, `_481ab4` through `_481ac8` (six singles).
Address views `norrath_ctors_ant_lantern_view_42b630`, `_42b548`, `_44bae0`, `_480e58` through `_480e70` (seven singles).
Address views `norrath_ctors_ant_lantern_view_42b1d0`, `_44c350`, `_46dbe0`, `_480cc8` through `_480cec` (ten singles); `_34b6b1` as for Chest.
`__3Webii` (original at `0x20fac8`) — original name, not defined here. Address views `norrath_ctors_ant_lantern_view_42c188`, `_44c260`, `_46ffe0`, `_4813bc` through `_4813d4` (seven singles).
`initAttackVariant__8Froglock` (original at `0x1ffb90`) — original name, not defined here. Address views `norrath_ctors_ant_lantern_view_42b958`, `_44bcc0`, `_46f120`, `_46f130`, `_48109c` through `_4810b0` (six singles).
Already defined, so not opened: `norrath_block_alloc`, `norrath_construct_Vehicle`, `norrath_vehicle_reset`, `norrath_actors_b_view_4844a0`.

## Authored but never executed

`norrath_construct_AutoChestLP`, `norrath_construct_AutoContainerLP`, `norrath_construct_Boat`, `norrath_construct_Lamp`, `norrath_construct_Lantern`, `norrath_construct_Generator`, `norrath_construct_Ice`, `norrath_construct_FireFly`, `norrath_construct_Chest`, `norrath_construct_Gnome`, `norrath_construct_Demon`, `norrath_construct_Cyclops`, `norrath_construct_FireBeetle`, `norrath_construct_AutoChest`, `norrath_construct_AutoProp`, `norrath_construct_AutoPropLP`, `norrath_construct_AutoContainer`, `norrath_construct_AntQueen`, `norrath_construct_Cthulu`, `norrath_construct_Arenabeast`, `norrath_construct_Ghoul`, `norrath_construct_Badger`, `norrath_construct_BlackWidow`, `norrath_construct_Froglock` (host contracts and EE compilation only; not run as gameplay).

## Unknown

The AddParticle body, the bytes of `_vt$11AutoChestLP`, `_vt$15AutoContainerLP`, and `_vt$4Boat`, and whether any caller uses these constructors' V0. The remaining constructors in this package are not reconstructed yet.
