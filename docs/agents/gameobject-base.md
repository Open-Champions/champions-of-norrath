# GameObject base methods

Package notes for `agents/gameobject-base`. The worktree starts at
`origin/agents/object-infra` so the GameObject vtable is present. Addresses
are from the private hash-pinned `SLUS_205.65` symbol table and the reviewed
EE decoder. They are local evidence, not values reproduced by the relocatable
build. Host checks sit beside each source and are not plan inputs.

Direct `jal` counts in the executable `.text` and `.srs` sections: `memcpy`
320, the destructor 121, the type-info function 118. The other fourteen
methods have no direct `jal`. They are reached through the vtable.

## memcpy

Observed `memcpy` at `0x301ffc`, 176 bytes. A count below 32, or either
address not 16-byte aligned, is a forward byte copy. The aligned path captures
16 bytes, stores them, then captures the next 16. Eight-byte steps follow,
then bytes. The function returns the original destination. Count zero writes
nothing. The reviewed compile emits one 540-byte body. The standard name is
`memcpy`, with an `unsigned` count, matching the heap package's declaration.

## Destructor

Observed `_$_10GameObject` at `0x141658`, 680 bytes. The authored name is
`norrath_vt_dtor_10GameObject`. It stores the GameObject vtable at object+72,
then selects successive `name=` arguments. Each selection folds the existing
tag-b view with ctype bit 0, compares four bytes, and requires `=` at byte 4.
The first scan only tests that a value exists. The second scan passes the
bytes after `=` to `objectRemoveString`. Then `listDestroy` runs on object+8,
object+4, and the object. Flag bit 0 selects `blockFree`. The reviewed compile
emits one 736-byte body.

## Type info

Observed `__tf10GameObject` at `0x312dd0`, 60 bytes. The authored name is
`norrath_vt_tf_10GameObject`. It returns the eight-byte object whose observed
symbol is `__ti10GameObject` at `0x486dc8`. When that object's first word is
zero, it calls `__rtti_user` with the object and the bytes `10GameObject` at
`0x461e98` through the NUL. A nonzero first word skips the call. The reviewed
compile emits one 72-byte body. The same unit defines
`norrath_ti_10GameObject` as eight zero bytes.

## Clone

Observed `clone__10GameObject` at `0x113ce0`, 244 bytes. The authored name is
`norrath_vt_clone_10GameObject`. It allocates 76 bytes. A null allocation
returns null and does not write the vtable. Otherwise it stores the GameObject
vtable at the new object+72 before three calls to `__8ListHeadRC8ListHead`.
That callee zeros the destination word. The clone then copies source bytes
12..41, 44..57, and 60..71. Bytes 42, 43, 58, and 59 stay as the allocator
left them. The reviewed compile emits one 512-byte body.

## Leaves

These are the remaining vtable slots. Empty leaves are `jr ra` / `nop` and
write nothing. The four that set `v0` return zero. The two point leaves copy
12 bytes from the incoming point onto the stack and discard that copy.

| Authored name | Observed symbol | Address | Bytes | Compiled body |
|---|---|---|---|---|
| `norrath_vt_msg_hurt_10GameObject` | `msg_hurt__10GameObjectfP10GameObjecti` | `0x312e48` | 8 | 8 |
| `norrath_vt_msg_trigger_10GameObject` | `msg_trigger__10GameObjecti` | `0x312e50` | 8 | 8 |
| `norrath_vt_msg_run_10GameObject` | `msg_run__10GameObject` | `0x312e58` | 8 | 8 |
| `norrath_vt_msg_draw_10GameObject` | `msg_draw__10GameObject` | `0x312e60` | 8 | 8 |
| `norrath_vt_msg_collision_10GameObject` | `msg_collision__10GameObjectP10GameObject` | `0x312e68` | 8 | 8 |
| `norrath_vt_msg_route_10GameObject` | `msg_route__10GameObjecti` | `0x312e70` | 8 | 8 |
| `norrath_vt_msg_load_10GameObject` | `msg_load__10GameObjectPc` | `0x312e78` | 8 | 8, returns 0 |
| `norrath_vt_msg_save_10GameObject` | `msg_save__10GameObjectPc` | `0x312e80` | 8 | 8, returns 0 |
| `norrath_vt_msg_collision_world_10GameObject` | `msg_collisionWorld__10GameObjectG6Point3` | `0x312e88` | 36 | 52 |
| `norrath_vt_msg_move_collision_10GameObject` | `msg_moveCollision__10GameObjectPP10GameObject` | `0x312eb0` | 8 | 8 |
| `norrath_vt_msg_alert_10GameObject` | `msg_alert__10GameObjectifG6Point3P10GameObject` | `0x312eb8` | 36 | 52 |
| `norrath_vt_msg_get_poke_name_10GameObject` | `msg_getPokeName__10GameObjectPUsi` | `0x312ee0` | 8 | 8, returns 0 |
| `norrath_vt_msg_poke_10GameObject` | `msg_poke__10GameObjecti` | `0x312ee8` | 8 | 8 |
| `norrath_vt_get_model_10GameObject` | `getModel__10GameObject` | `0x3130e0` | 8 | 8, returns 0 |

`msg_alert` takes an int, a float, the point, and an object pointer. The point
is the pointer actually read. `msg_collisionWorld` reads its point pointer.
Neither function writes the object.

## Imports closed

- `memcpy`
- `norrath_vt_tf_10GameObject`
- `norrath_vt_dtor_10GameObject`
- `norrath_vt_msg_hurt_10GameObject`
- `norrath_vt_msg_trigger_10GameObject`
- `norrath_vt_msg_run_10GameObject`
- `norrath_vt_msg_draw_10GameObject`
- `norrath_vt_msg_collision_10GameObject`
- `norrath_vt_msg_route_10GameObject`
- `norrath_vt_msg_load_10GameObject`
- `norrath_vt_msg_save_10GameObject`
- `norrath_vt_msg_collision_world_10GameObject`
- `norrath_vt_msg_move_collision_10GameObject`
- `norrath_vt_msg_alert_10GameObject`
- `norrath_vt_msg_get_poke_name_10GameObject`
- `norrath_vt_msg_poke_10GameObject`
- `norrath_vt_clone_10GameObject`
- `norrath_vt_get_model_10GameObject`

## Imports opened

- `norrath_object_remove_string` for observed `objectRemoveString__FPcP10GameObject`
- `norrath_list_head_copy_construct` for observed `__8ListHeadRC8ListHead`
- `norrath_rtti_user` for observed `__rtti_user`

The destructor also uses the already-unresolved
`norrath_factory_ctype_minimum_view` and the existing
`norrath_factory_strncmp`, `norrath_list_destroy`, `norrath_block_free`,
`norrath_constructor_tag_b_view`, and `norrath_game_object_vtable_view`.
Clone uses the existing `norrath_block_alloc` and the vtable. `eRand`,
`eRandf`, and `eRandv` are GameObject methods and are not this package.

## Build-plan entries needed

Append these authored inputs. Do not add any `host_check.cpp`. Profile
`authored_cpp`. Header closure is empty. The orders below assume this
branch's 298-entry plan, then the four object-infra units at 298-301. The
integrator renumbers. `emitted_functions` is the one function named here,
with the compiled body size, except the type-info unit also defines the
eight-byte `norrath_ti_10GameObject` object and no second function.

```json
[
  {
    "source": "sources/gameobject-base-memcpy/memcpy.cpp",
    "object_name": "memcpy.o",
    "sha256": "a9d29928ea949771cb881bb4b63020f116986aeae924b34d7e2161a74e85b6d3",
    "input_order": 302,
    "emitted_functions": [{"name": "memcpy", "compiled_body_bytes": 540}]
  },
  {
    "source": "sources/gameobject-base-dtor/dtor.cpp",
    "object_name": "dtor.o",
    "sha256": "20d60d176a4242074d92a21ecc8107f97f5ee2b92ea892cca9fd75b45b9515b1",
    "input_order": 303,
    "emitted_functions": [{"name": "norrath_vt_dtor_10GameObject", "compiled_body_bytes": 736}]
  },
  {
    "source": "sources/gameobject-base-typeinfo/typeinfo.cpp",
    "object_name": "typeinfo.o",
    "sha256": "af9043d4ebb40c72ecd7201799340ad9e8406705d4d223c3c88dac4b18113281",
    "input_order": 304,
    "emitted_functions": [{"name": "norrath_vt_tf_10GameObject", "compiled_body_bytes": 72}]
  },
  {
    "source": "sources/gameobject-base-clone/clone.cpp",
    "object_name": "clone.o",
    "sha256": "b0f21741a46bfc006550ba9a8698e4a36df50dbc8db2e29413667c23a5dc4fc8",
    "input_order": 305,
    "emitted_functions": [{"name": "norrath_vt_clone_10GameObject", "compiled_body_bytes": 512}]
  },
  {
    "source": "sources/gameobject-base-msg-hurt/msg_hurt.cpp",
    "object_name": "msg_hurt.o",
    "sha256": "895512e07aedda46c98262cb130a7acb729502367481ebbeecc81ccd1e42a2a2",
    "input_order": 306,
    "emitted_functions": [{"name": "norrath_vt_msg_hurt_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-trigger/msg_trigger.cpp",
    "object_name": "msg_trigger.o",
    "sha256": "35ec3cdc6f28dd03e5c509217a03d8f521dbf0ee892edec3a5d4979e178f2385",
    "input_order": 307,
    "emitted_functions": [{"name": "norrath_vt_msg_trigger_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-run/msg_run.cpp",
    "object_name": "msg_run.o",
    "sha256": "02e7d1cfeaf513512bf9a832b4bb2cf90d4e21564c3c8be1a3dd65da1f493be0",
    "input_order": 308,
    "emitted_functions": [{"name": "norrath_vt_msg_run_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-draw/msg_draw.cpp",
    "object_name": "msg_draw.o",
    "sha256": "9d476645cfaa5713f0fdda027653a926f03ae2c1e0c0c43c86931aa39263daed",
    "input_order": 309,
    "emitted_functions": [{"name": "norrath_vt_msg_draw_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-collision/msg_collision.cpp",
    "object_name": "msg_collision.o",
    "sha256": "1fb22676569be547c04201439c01cde2037db0f35ecb74d5f332d759ad02a01c",
    "input_order": 310,
    "emitted_functions": [{"name": "norrath_vt_msg_collision_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-route/msg_route.cpp",
    "object_name": "msg_route.o",
    "sha256": "e9605665f4fc0d2db1c2905ed45cee6fccb3b8409bb9e6e52d4915f7ecbf06f7",
    "input_order": 311,
    "emitted_functions": [{"name": "norrath_vt_msg_route_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-load/msg_load.cpp",
    "object_name": "msg_load.o",
    "sha256": "5ea5fa9db592c32a8eef675af2f90aeabe592f27ef16a89124c112147c5af3c9",
    "input_order": 312,
    "emitted_functions": [{"name": "norrath_vt_msg_load_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-save/msg_save.cpp",
    "object_name": "msg_save.o",
    "sha256": "ab4971c46e2cdeac9df6e97626e1f8a59b44c58c60dacf1cabc6d21ca1a9688d",
    "input_order": 313,
    "emitted_functions": [{"name": "norrath_vt_msg_save_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-collision-world/msg_collision_world.cpp",
    "object_name": "msg_collision_world.o",
    "sha256": "96f62477630309697fab15ac49be77fbb5200995b8c1904fab622366823cb33a",
    "input_order": 314,
    "emitted_functions": [{"name": "norrath_vt_msg_collision_world_10GameObject", "compiled_body_bytes": 52}]
  },
  {
    "source": "sources/gameobject-base-msg-move-collision/msg_move_collision.cpp",
    "object_name": "msg_move_collision.o",
    "sha256": "01f451e498af4d63eb7673b646e0991e2fabe9dcf3317010de8e9fba9a924040",
    "input_order": 315,
    "emitted_functions": [{"name": "norrath_vt_msg_move_collision_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-alert/msg_alert.cpp",
    "object_name": "msg_alert.o",
    "sha256": "408d7b0d72677f3b2fd2833780dcd60abf4514f87570afdea92e7083ee4d29d1",
    "input_order": 316,
    "emitted_functions": [{"name": "norrath_vt_msg_alert_10GameObject", "compiled_body_bytes": 52}]
  },
  {
    "source": "sources/gameobject-base-msg-get-poke-name/msg_get_poke_name.cpp",
    "object_name": "msg_get_poke_name.o",
    "sha256": "9b8b5c03d7e56aa8ecf3d1234a31aa3f35d0de661ae17b76c2ec4e8767fabed7",
    "input_order": 317,
    "emitted_functions": [{"name": "norrath_vt_msg_get_poke_name_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-msg-poke/msg_poke.cpp",
    "object_name": "msg_poke.o",
    "sha256": "42e5e17e42b718b137ef31f8ab9123f6f9ba751c3ed524cb2c967badee2883e6",
    "input_order": 318,
    "emitted_functions": [{"name": "norrath_vt_msg_poke_10GameObject", "compiled_body_bytes": 8}]
  },
  {
    "source": "sources/gameobject-base-get-model/get_model.cpp",
    "object_name": "get_model.o",
    "sha256": "3e884a654cb6a8b5e4d9a26d89ccf4eaf4277b435edcca556f17b51983be32f0",
    "input_order": 319,
    "emitted_functions": [{"name": "norrath_vt_get_model_10GameObject", "compiled_body_bytes": 8}]
  }
]
```

A private copy of this branch's plan, kept outside git, included the four
object-infra units and these eighteen. The driver reported 318 authored
inputs, one link, 439 function bodies, and 632 unresolved imports. Every name
under Imports closed was absent. The three names under Imports opened were
present. `memcpy` is defined; this branch's earlier unresolved list did not
contain it, because the heap caller is not in this plan. `runtime_verified`
and `game_executable` were false.

## Authored but never executed

The eighteen functions and the eight-byte typeinfo object. They were compiled
with the reviewed EE toolchain and relocatable-linked. Host checks passed for
the copy lengths and the aligned overlap order, the destructor's folded name
scan and flag bit, the type-info gate, clone's gaps and null allocation, and
the leaves' unchanged inputs. No PS2 or game execution. The vtable slots are
not called.

## Unknown

- Whether `objectRemoveString` changes the argument list. The destructor calls
  it and then searches for the next occurrence, which is what the 680-byte
  function does.
- The eight-byte list-head callee zeros one word and ignores its source. Its
  body is not in this package.
- `__rtti_user` writes the name pointer and the user-type-info vtable. This
  package only calls it.
- Bytes after the NUL at `0x461e98` are a different literal and are not part
  of the class name.
- `__ti10GameObject` is `.sbss` in the executable. The reviewed `-G0` profile
  puts the eight zero bytes in ordinary BSS.
- Empty leaves leave `v0` unchanged. The authored functions are void, so a
  caller that reads `v0` after them is outside what these leaves establish.
- The point leaves' stack copy is discarded. A caller that depends on that
  slot after the return is not established.
- Overlap other than a forward copy, an aligned destination 16 bytes ahead of
  the source, and a one-byte-ahead byte copy. `memcpy` does not implement
  `memmove`.
- Interrupts, timing, and faults from a null point pointer or a short
  argument string. The original reads those bytes.
- `eRand__10GameObject`, `eRandf__10GameObject`, and `eRandv__10GameObject`
  were not started.
