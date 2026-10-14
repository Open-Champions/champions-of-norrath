# Object lists, memory lists and math helpers (w8-object-list-math)

Branch `agents/w8-object-list-math`. New source is under `experimental/sources/w8-object-list-math-*`; the shared headers are in `w8-object-list-math-common`. Reference executable: the private `SLUS_205.65` named in the docs, read locally only. No game bytes, listings or decompiler output are committed; addresses and extents below are that file's symbol table, and the reviewed EE `objdump` was used to read each function.

Nothing here has run on the EE. Every function compiles with the reviewed `authored_cpp` profile. Every function has a `host_check.cpp` that compiles with the host compiler and exits non-zero when a store, call order, bound, result or branch is wrong; host checks include host headers and are not build-plan inputs. EE objects are not original-byte matches and no match is claimed.

## What was checked

- Private plan (copy of `experimental/`, `snapshot.json` and `build-plan.json` regenerated outside git by a script that appends the 42 units below): `--check-plan` passed, then the partial build ran once: 1,449 compilations, one link, 1,152 function bodies (1,107 before), 1,154 unresolved imports (1,195 before). No duplicate definitions.
- All 45 names in the package are absent from the new unresolved list (list below). Three imports are new.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Mutation spot checks: a wrong link in `next__16ListSafeIterator`, a `<=` for the fit test in `memAlloc__FiPc`, and a `<=` for the split test in `memAlloc__FiPc` each made the host check fail.
- Float code was compared by reading the EE assembly for the same operation order and instruction kinds (`sqrt.s`, `rsqrt.s`, `max.s`, `c.le.s`, `div.s`, `cvt.w.s`). Host float checks build with `-ffp-contract=off` and use tolerances where the host square root differs from the EE instruction.

## Conventions used

- Importer-facing names are the mangled originals, written as `extern "C"` definitions. Where a callee is not in this package, the already-authored provider is called instead of opening a new import: `norrath_list_remove` for `listRemove__FP10__listNode`, `norrath_native_atan2f` for libm `atan2f`, `norrath_animation_state_deallocate` for `__builtin_delete`, `norrath_mem_dump_heap` for `memDumpHeap__Fv`, `scePrintf`, `memcpy`, `strcmp`, `strncmp`, `sprintf`, `strlen`, `strcpy`, and `norrath_factory_ctype_minimum_view` for the character-class table. The `sqrt.s` instruction is used inline, as in other authored sources, so no `sqrtf` import is added.
- Data owned by other packages is referenced by its original name when an authored package already defines it under that name (`freeNodes`, `offTheGrid`, `objectGrid`, `ghostObjectGrid`, `alwaysDrawList`, `stringHash`, `objectHash`, `objectStrings`, `objectIgnoreFlags`) or when it is an existing import (`currentRunningObject`, `lastNmBlocksFree`). Otherwise the existing authored view is used: `norrath_create_by_name_function_word`, `norrath_user_filter_function_word`, and the `norrath_heap_*` words for `startNode`, `movingNode`, `rawMin`, `MEM_FreeHeap` and `mark`.
- A list head is one word (the first node). Evidence: the medium and slow run-list head tables step by four bytes (15 and 60 heads, extents 64 and 240 up to the next symbol). `runList` and `deleteList` each occupy an eight-byte slot whose second word nothing in this package touches.
- Block header (memory): 28 bytes, magic 0xABCD at +0, state halfword at +2 (0 free, 1 allocated, 2 absorbed), size at +4, name at +8, next at +12, previous at +16, tag at +20, caller at +24. Sizes in the source are written with `sizeof(HeapBlock)` so the host (larger header) checks the same logic; the EE build asserts 28.

## Functions and data

| source directory (`experimental/sources/w8-object-list-math-…`) | emitted function | image address | image extent | EE body bytes |
|---|---|---|---|---|
| `activation-check` | `objectActivationCheck__8Creaturei` | 0x10a428 | 228 | 212 |
| `add-to-delete-list` | `objectAddToDeleteList__FP10GameObject` | 0x1437e8 | 68 | 80 |
| `create-by-name-forward` | `createByName__FPcG6Point3sPPc` | 0x14c410 | 15600 | 52 |
| `do-my-actions` | `objectDoMyActions__FP10GameObjecti` | 0x142310 | 392 | 360 |
| `find-animation` | `objectFindAnimation__FP10GameObjectPc` | 0x152cc8 | 356 | 344 |
| `find-object-by-class` | `_ZN12_GLOBAL__N_1L12collect_fromEP8ListHeadPKcPP17GameObjectVirtualiRi (local helper)` | - | - | 292 |
| `find-object-by-class` | `objectFindObjectByClass__FPcPP10GameObjecti` | 0x1407d8 | 676 | 252 |
| `global-dtor-lastkey` | `_GLOBAL_$D$lastkey` | 0x2cb480 | 32 | 12 |
| `global-dtor-lastkey` | `_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii (local helper)` | - | - | 76 |
| `global-init-addpmlist` | `_GLOBAL_$I$addpmlist__FPc` | 0x26b3b8 | 32 | 12 |
| `global-init-addpmlist` | `_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii (local helper)` | - | - | 8 |
| `global-init-lastkey` | `_GLOBAL_$I$lastkey` | 0x2cb460 | 32 | 12 |
| `global-init-lastkey` | `_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii (local helper)` | - | - | 76 |
| `global-init-object-lists` | `_GLOBAL_$D$objectIgnoreFlags` | 0x1440b8 | 32 | 12 |
| `global-init-object-lists` | `_GLOBAL_$I$objectIgnoreFlags` | 0x144098 | 32 | 12 |
| `global-init-object-lists` | `_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii (local helper)` | - | - | 536 |
| `iatan2` | `iatan2__Fff` | 0x15a198 | 52 | 60 |
| `icos` | `icos__Fi` | 0x15a070 | 144 | 172 |
| `isin` | `isin__Fi` | 0x15a100 | 148 | 180 |
| `line-initialize` | `initializeMe__4Line` | 0x279470 | 1124 | 736 |
| `line-to-y` | `lineToY__Fi` | 0x1d3990 | 20 | 20 |
| `list-oneway-link` | `listOnewayLink__FP8ListHeadT0Pv` | 0x194390 | 140 | 120 |
| `mat-ident33` | `matIdent__FP8Matrix33` | 0x159090 | 48 | 48 |
| `mat-make-trans-yaw` | `matMakeTransYaw__FP8Matrix34G6Point3s` | 0x157668 | 356 | 376 |
| `mat-rot33` | `matRot__FP8Matrix33is` | 0x156610 | 520 | 328 |
| `mat-to-quaternion` | `matToQuaternion__FP4QuatP8Matrix34` | 0x157e20 | 584 | 628 |
| `mat-transform33` | `matTransform__FP6Point3P8Matrix33T0` | 0x159880 | 160 | 128 |
| `mat-transform34-vec` | `matTransform34Vec__FP6Point3P8Matrix34T0` | 0x159b98 | 160 | 128 |
| `mem-alloc` | `memAlloc__FiPc` | 0x144368 | 492 | 524 |
| `mem-check-heap-real` | `memCheckHeapReal__FPCci` | 0x144b60 | 8 | 8 |
| `mem-free` | `memFree__FPv` | 0x144558 | 232 | 240 |
| `mem-realloc` | `memRealloc__FPvi` | 0x144278 | 240 | 236 |
| `object-clear` | `_ZN12_GLOBAL__N_1L10delete_allEP8ListHeadi (local helper)` | - | - | 116 |
| `object-clear` | `objectClear__Fv` | 0x140b68 | 680 | 312 |
| `quat-to-mat33` | `quatToMat__FP8Matrix33P4Quat` | 0x15a2b0 | 208 | 200 |
| `remove-from-grid` | `objectRemoveFromGrid__FP10GameObject` | 0x143558 | 80 | 68 |
| `remove-from-run-list` | `objectRemoveFromRunList__FP10GameObject` | 0x143878 | 40 | 28 |
| `safe-iterator-construct` | `__16ListSafeIteratorP8ListHeadi` | 0x194660 | 160 | 108 |
| `safe-iterator-destroy` | `_$_16ListSafeIterator` | 0x194700 | 80 | 64 |
| `safe-iterator-next` | `next__16ListSafeIterator` | 0x194750 | 124 | 148 |
| `save-add-to-string-pool` | `objectSaveAddToStringPool__FPCc` | 0x143c38 | 212 | 316 |
| `save-get-from-string-pool` | `objectSaveGetFromStringPool__Fi` | 0x143d10 | 24 | 32 |
| `set-create-by-name-function` | `objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject` | 0x143d28 | 8 | 12 |
| `set-filter-function` | `objectSetFilterFunction__FPFPc_i` | 0x143d30 | 8 | 12 |
| `trigger-inside` | `objectInside__7TriggerP10GameObject` | 0x1d1ae0 | 80 | 112 |

Data units (no function bodies): `run-list-heads` defines `runList` (0x486878, 8), `deleteList` (0x486880, 8), `mediumRunList` (0x4fbf90, 64) and `slowRunList` (0x4fbfd0, 240), all `.bss`; `string-pool-state` defines `poolStart` (0x48689c), `poolEnd` (0x4868a0) and `poolOffset` (0x4868a4), four bytes each; `next-index` defines `nextIndex` (0x486b38, 4); `object-cycle-words` defines `objectKillingEveryone` (0x483ac4), `slowListCycle` (0x483ad0) and `mediumListCycle` (0x483ad4), zero in the file image, which were not imports but are written by `objectClear__Fv`.

One commit per function except where noted: `global-init-object-lists` holds both `_GLOBAL_$I$objectIgnoreFlags` and `_GLOBAL_$D$objectIgnoreFlags` because they share one local helper in the original; the three `global-*` units for `addpmlist` and `lastkey` each carry their own copy of the small local helper. `create-by-name-forward` is **not** a reconstruction (see below).

## Observed

- `listOnewayLink__FP8ListHeadT0Pv` takes two nodes from the free-node stack (`freeNodes`, 0x486a6c), clears each halfword at +16, pushes the first on the first head and the second on the second head, cross-links their partner words, puts the value on the first and zero on the second, and returns the first. There is no empty-stack check.
- `objectAddToDeleteList__FP10GameObject` passes the **object address itself** as the second head (the run-list callers pass object+4). The link therefore rewrites the object's first word. Kept as observed.
- `objectRemoveFromGrid__FP10GameObject` unlinks the grid node's partner (the node at +8 of the node at object+8) from its chain without testing the previous pointer and pushes it on `offTheGrid`.
- The safe iterator is itself a list node (24 bytes, mask at +20) placed directly after the node it stands for; callers read the current node from its +4 word and the object from that node's +12. `next__16ListSafeIterator` unlinks the marker through its previous node without a null test, so advancing an exhausted marker faults.
- `icos__Fi` and `isin__Fi` share one body (angle folded to a signed halfword, constants 0x38490fda, 0x3e2aaa57, 0x3c086b24, 0x39458017); `isin` subtracts 16384 first. The host checks cover every halfword against libm within 4.1e-7. `matMakeTransYaw__FP8Matrix34G6Point3s` has the same arithmetic inlined, and the host check compares its sine and cosine with these two routines bit for bit for all 65,536 angles.
- `quatToMat__FP8Matrix33P4Quat` and `matToQuaternion__FP4QuatP8Matrix34` are inverses of each other under the element placement written in their comments; a 20,000-sample round trip agrees to 3e-7.
- `matRot__FP8Matrix33is` rotates two rows (axis 0: rows 1 and 2; axis 1: rows 0 and 2 with the sine sign swapped; axis 2: rows 0 and 1), evaluates `icos` then `isin` even for an axis that selects nothing.
- Heap routines: see the header comments of `mem-alloc`, `mem-free` and `mem-realloc`. Every allocation the first-fit scan returns has a payload on a 256-byte boundary (host check asserts this). The failure path stores zero to address 0 (a deliberate fault) after printing; the source uses inline `sw $0, 0($0)` on the EE.
- `objectDoMyActions__FP10GameObjecti` rescans the tag array for each action by count, so a script that removes an earlier "action=" tag causes the next one to be skipped (host check covers it). It lower-cases its writable "action" prefix string in place each call.
- `objectFindObjectByClass__FPcPP10GameObjecti` stops when the count equals limit minus one (an equality, not a bound).
- `objectActivationCheck__8Creaturei`, `objectInside__7TriggerP10GameObject` and `objectFindAnimation__FP10GameObjectPc`: field offsets, tag strings (`followFoundScript`, `followLostScript`, `%sname`, `name`, `%s.anm`, `%s_%s.anm`) and call order are in each header comment.
- `_GLOBAL_$I$objectIgnoreFlags` / `_GLOBAL_$D$objectIgnoreFlags` construct (clear) or destroy (`listDestroy__FP8ListHead`, in reverse) every list head of the object module; destruction order is checked against 28,879 expected calls.

## Inferred

- Names for fields (`run_node`, `grid_node`, `side`, `blend`, `corner`, and so on) are mine, chosen from use, not recovered debug names. The matrix/line/quaternion element roles are inferred from the arithmetic; handedness is unknown.
- Virtual-table entries are eight bytes (`short delta`, a gap, a function); the word at table index 1 is a function returning a pointer to a type record whose first word is the class name; the entry at table+8 called with `(object + delta, 3)` is the deleting destructor.
- The 64-byte point of `Line` (position, direction, side, blend, segment length, distance, corner, slot) from the arithmetic in `initializeMe__4Line` and the constructor notes of `light-methods-line-construct`.
- `createByName__FPcG6Point3sPPc` and `norrath_game_create_by_name` are one function (the authored source cites selection points inside 0x14c410 and has the same four-argument interface).

## Imports closed

`_$_16ListSafeIterator`, `_GLOBAL_$D$lastkey`, `_GLOBAL_$D$objectIgnoreFlags`, `_GLOBAL_$I$addpmlist__FPc`, `_GLOBAL_$I$lastkey`, `_GLOBAL_$I$objectIgnoreFlags`, `__16ListSafeIteratorP8ListHeadi`, `createByName__FPcG6Point3sPPc` (forwarder only), `deleteList`, `iatan2__Fff`, `icos__Fi`, `initializeMe__4Line`, `isin__Fi`, `lineToY__Fi`, `listOnewayLink__FP8ListHeadT0Pv`, `matIdent__FP8Matrix33`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `matRot__FP8Matrix33is`, `matToQuaternion__FP4QuatP8Matrix34`, `matTransform34Vec__FP6Point3P8Matrix34T0`, `matTransform__FP6Point3P8Matrix33T0`, `mediumRunList`, `memAlloc__FiPc`, `memCheckHeapReal__FPCci`, `memFree__FPv`, `memRealloc__FPvi`, `nextIndex`, `next__16ListSafeIterator`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`, `objectClear__Fv`, `objectDoMyActions__FP10GameObjecti`, `objectFindAnimation__FP10GameObjectPc`, `objectFindObjectByClass__FPcPP10GameObjecti`, `objectInside__7TriggerP10GameObject`, `objectRemoveFromGrid__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `objectSaveAddToStringPool__FPCc`, `objectSaveGetFromStringPool__Fi`, `objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`, `objectSetFilterFunction__FPFPc_i`, `quatToMat__FP8Matrix33P4Quat`, `runList`, `slowRunList`.

## Imports opened

- `__10VIKeyboard` and `_$_10VIKeyboard` (the keyboard class constructor and destructor, 0x2cbef0 and 0x2cbf00), called by `_GLOBAL_$I$lastkey` / `_GLOBAL_$D$lastkey`. The constructor is a 12-byte function that stores -1 at +4; the destructor takes the delete flag.
- `objectActiveProcessing__FP10GameObjectii` (0x142158, 436 bytes), called by `objectActivationCheck__8Creaturei`.

Not opened: `lumpFindResource__FPcT0`, `findTagString__10GameObjectPci`, `scriptStart__FPc`, `animResetFramestates__FP14AnimationState`, `__tf3NPC`, `__eq__C9type_infoRC9type_info` and `__throw_bad_typeid` were already defined or already imports.

## Decisions for the integrator

- `createByName__FPcG6Point3sPPc`: the unit only forwards its four arguments to `norrath_game_create_by_name`. Repeating the roughly 100 branches would open as many constructor imports under different names. It earns no reconstruction credit. If the two should be one symbol, drop the forwarder and rename the authored function, or keep both.
- Several words have two authored names: `slowListCycle` / `mediumListCycle` here against `norrath_medium_list_cycle_word` (vehicle-actors) and the unresolved `norrath_actor_view_483ad0` (object-infra slow run list); `norrath_create_by_name_function_word` / `norrath_user_filter_function_word` / `norrath_heap_*` words against their original names, which nobody defines. The package uses the existing views for the last two groups and defines the first group under the original names. The earlier `objectAddToDeleteList` (`norrath_object_add_to_delete_list`), safe-iterator (`norrath_list_safe_iterator_*`), trig (`norrath_integer_*`), heap (`norrath_mem_*`), run-list and several other authored functions duplicate bodies published here under the original names; the plan should choose one source of each pair (this branch does not edit them).
- `listDestroy__FP8ListHead` and the grid/hash names are used by their original names; they are defined by other packages.

## Build-plan entries needed

Append these inputs after the current last input and assign fresh `input_order` values. `default_authored_inputs` increases by 42. Add the `files` entries below to `files` in the plan and snapshot, and add a snapshot input record per unit (component = the source directory name). No `private_object_sha256` is claimed: there is no integrated private object for these units. `profile` is `authored_cpp`; body sizes are from the reviewed EE `g++` profile, not from the original executable. Host check files are not inputs.

```json
{"emitted_functions":[{"compiled_body_bytes":212,"name":"objectActivationCheck__8Creaturei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"activation_check.o","profile":"authored_cpp","sha256":"72b0ce651c48aea07144d0a78ce9fb73b7740fee278dac4588b4655969eb70c9","source":"sources/w8-object-list-math-activation-check/activation_check.cpp"}
{"emitted_functions":[{"compiled_body_bytes":80,"name":"objectAddToDeleteList__FP10GameObject"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"add_to_delete_list.o","profile":"authored_cpp","sha256":"37851166ca52700b16883de9d90e222432bdc513094c2d1c1bebf4a1042d5982","source":"sources/w8-object-list-math-add-to-delete-list/add_to_delete_list.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"createByName__FPcG6Point3sPPc"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"create_by_name_forward.o","profile":"authored_cpp","sha256":"2f17c1b70aed79a905068f06c363f3e74c0752cb0d9ad5dc13540b1b0b0827c1","source":"sources/w8-object-list-math-create-by-name-forward/create_by_name_forward.cpp"}
{"emitted_functions":[{"compiled_body_bytes":360,"name":"objectDoMyActions__FP10GameObjecti"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"do_my_actions.o","profile":"authored_cpp","sha256":"81c23d5d60b723111846d3c38cf57d14f74e2e84e27bce85c44af298e05977af","source":"sources/w8-object-list-math-do-my-actions/do_my_actions.cpp"}
{"emitted_functions":[{"compiled_body_bytes":344,"name":"objectFindAnimation__FP10GameObjectPc"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"find_animation.o","profile":"authored_cpp","sha256":"6a21e7b2ea92b99c65848dda6727442a3ca70c8084714c8bdc6a25b160a9ef0a","source":"sources/w8-object-list-math-find-animation/find_animation.cpp"}
{"emitted_functions":[{"compiled_body_bytes":292,"name":"_ZN12_GLOBAL__N_1L12collect_fromEP8ListHeadPKcPP17GameObjectVirtualiRi"},{"compiled_body_bytes":252,"name":"objectFindObjectByClass__FPcPP10GameObjecti"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp","sources/w8-object-list-math-common/safe_iterator.hpp"],"kind":"independently_authored_source","object_name":"find_object_by_class.o","profile":"authored_cpp","sha256":"758683aba90fbc64729cc70880c271ad91b6465b082a68a9257de93a816daddc","source":"sources/w8-object-list-math-find-object-by-class/find_object_by_class.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"_GLOBAL_$D$lastkey"},{"compiled_body_bytes":76,"name":"_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"global_dtor_lastkey.o","profile":"authored_cpp","sha256":"55a1e80fe4304f19ea548866ba5cc7a76369708986355d5c7e92759a573fe7ef","source":"sources/w8-object-list-math-global-dtor-lastkey/global_dtor_lastkey.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"_GLOBAL_$I$addpmlist__FPc"},{"compiled_body_bytes":8,"name":"_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"global_init_addpmlist.o","profile":"authored_cpp","sha256":"fc772ac42e7e66b2410a9f59e7ccbe241c90adb724cb01b5f52e310b5259c019","source":"sources/w8-object-list-math-global-init-addpmlist/global_init_addpmlist.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"_GLOBAL_$I$lastkey"},{"compiled_body_bytes":76,"name":"_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"global_init_lastkey.o","profile":"authored_cpp","sha256":"2b278e895762b5831327cb9d54f9504d4bf79219367bac7c052031c7aed11673","source":"sources/w8-object-list-math-global-init-lastkey/global_init_lastkey.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"_GLOBAL_$D$objectIgnoreFlags"},{"compiled_body_bytes":12,"name":"_GLOBAL_$I$objectIgnoreFlags"},{"compiled_body_bytes":536,"name":"_ZN12_GLOBAL__N_1L39static_initialization_and_destruction_0Eii"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp"],"kind":"independently_authored_source","object_name":"global_init_object_lists.o","profile":"authored_cpp","sha256":"7e8adda477b04df769b1728d9ff9c853e58966890fa2284262bb1270dc5af86a","source":"sources/w8-object-list-math-global-init-object-lists/global_init_object_lists.cpp"}
{"emitted_functions":[{"compiled_body_bytes":60,"name":"iatan2__Fff"}],"header_closure":[],"kind":"independently_authored_source","object_name":"iatan2.o","profile":"authored_cpp","sha256":"c81d9bb048f7828e728a30bfd61fa8c73b504b85c694d47efcbbdb86074db185","source":"sources/w8-object-list-math-iatan2/iatan2.cpp"}
{"emitted_functions":[{"compiled_body_bytes":172,"name":"icos__Fi"}],"header_closure":["sources/w8-object-list-math-common/trig_core.hpp"],"kind":"independently_authored_source","object_name":"icos.o","profile":"authored_cpp","sha256":"95920516dd8fe7ef0ededf4603e875d08b1441713395a996dfc6ab217bba44cd","source":"sources/w8-object-list-math-icos/icos.cpp"}
{"emitted_functions":[{"compiled_body_bytes":180,"name":"isin__Fi"}],"header_closure":["sources/w8-object-list-math-common/trig_core.hpp"],"kind":"independently_authored_source","object_name":"isin.o","profile":"authored_cpp","sha256":"0a6fea253baaef63b0a7631e19b19c0b3eda739f661f171d9b60ecb0589c83c9","source":"sources/w8-object-list-math-isin/isin.cpp"}
{"emitted_functions":[{"compiled_body_bytes":736,"name":"initializeMe__4Line"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"line_initialize.o","profile":"authored_cpp","sha256":"c6a377ec7fe986eb8775ce1ed305ded01b24bb63b3835f35d1cb815f9b05211d","source":"sources/w8-object-list-math-line-initialize/line_initialize.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"lineToY__Fi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"line_to_y.o","profile":"authored_cpp","sha256":"e2832e220192ae837cac7d8be83a4e4055b5ef75ddabb2c8d9f6542ba44de96c","source":"sources/w8-object-list-math-line-to-y/line_to_y.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"listOnewayLink__FP8ListHeadT0Pv"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp"],"kind":"independently_authored_source","object_name":"list_oneway_link.o","profile":"authored_cpp","sha256":"f855fcaa30ab0d12f87a26130f5009b9181dcd8818c55bc0cf0ee0e7bc25ec19","source":"sources/w8-object-list-math-list-oneway-link/list_oneway_link.cpp"}
{"emitted_functions":[{"compiled_body_bytes":48,"name":"matIdent__FP8Matrix33"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mat_ident33.o","profile":"authored_cpp","sha256":"5d01abb4f7ded14fb53ac2ba80d4d0232c4690c90d22171f96889b1f608813e0","source":"sources/w8-object-list-math-mat-ident33/mat_ident33.cpp"}
{"emitted_functions":[{"compiled_body_bytes":376,"name":"matMakeTransYaw__FP8Matrix34G6Point3s"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/matrix_types.hpp","sources/w8-object-list-math-common/object_types.hpp","sources/w8-object-list-math-common/trig_core.hpp"],"kind":"independently_authored_source","object_name":"mat_make_trans_yaw.o","profile":"authored_cpp","sha256":"f25b324ce549d5e3adeb2def192f6a0f2d6f3c198c030022b18ec1b9f794ecab","source":"sources/w8-object-list-math-mat-make-trans-yaw/mat_make_trans_yaw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":328,"name":"matRot__FP8Matrix33is"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/matrix_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"mat_rot33.o","profile":"authored_cpp","sha256":"9782780078f5b0343bb928ec4ad56b9aefda204f5b8e694d58ac8f6d070ec588","source":"sources/w8-object-list-math-mat-rot33/mat_rot33.cpp"}
{"emitted_functions":[{"compiled_body_bytes":628,"name":"matToQuaternion__FP4QuatP8Matrix34"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/matrix_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"mat_to_quaternion.o","profile":"authored_cpp","sha256":"a33dc4fce951000a51da3355fe166fbf23aec3851ea2e13e97a095d47cdd9984","source":"sources/w8-object-list-math-mat-to-quaternion/mat_to_quaternion.cpp"}
{"emitted_functions":[{"compiled_body_bytes":128,"name":"matTransform__FP6Point3P8Matrix33T0"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/matrix_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"mat_transform33.o","profile":"authored_cpp","sha256":"1bb735235a3ac64620f6e8df805086dbf31215d997a4ef1cb0a28d8e020070e3","source":"sources/w8-object-list-math-mat-transform33/mat_transform33.cpp"}
{"emitted_functions":[{"compiled_body_bytes":128,"name":"matTransform34Vec__FP6Point3P8Matrix34T0"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/matrix_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"mat_transform34_vec.o","profile":"authored_cpp","sha256":"401563c6a0a3b740498225e3ea766f6556bbaa726e789e389c57b2a8ed3daebe","source":"sources/w8-object-list-math-mat-transform34-vec/mat_transform34_vec.cpp"}
{"emitted_functions":[{"compiled_body_bytes":524,"name":"memAlloc__FiPc"}],"header_closure":["sources/w8-object-list-math-common/heap_block.hpp"],"kind":"independently_authored_source","object_name":"mem_alloc.o","profile":"authored_cpp","sha256":"ffb0e3f90009565d7c6f574818fddba661584bc5959bb55774ecfebe33f10adc","source":"sources/w8-object-list-math-mem-alloc/mem_alloc.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"memCheckHeapReal__FPCci"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mem_check_heap_real.o","profile":"authored_cpp","sha256":"1bd16b825e7164dfa54adaf083deb43d5a7849332fd937e6356c2cf11c2ac27e","source":"sources/w8-object-list-math-mem-check-heap-real/mem_check_heap_real.cpp"}
{"emitted_functions":[{"compiled_body_bytes":240,"name":"memFree__FPv"}],"header_closure":["sources/w8-object-list-math-common/heap_block.hpp"],"kind":"independently_authored_source","object_name":"mem_free.o","profile":"authored_cpp","sha256":"7420503b52f8721f0fbc8d47f202a3541fdfeef414327ef5dcc4d2e5609a557f","source":"sources/w8-object-list-math-mem-free/mem_free.cpp"}
{"emitted_functions":[{"compiled_body_bytes":236,"name":"memRealloc__FPvi"}],"header_closure":["sources/w8-object-list-math-common/heap_block.hpp"],"kind":"independently_authored_source","object_name":"mem_realloc.o","profile":"authored_cpp","sha256":"aca2044a92eab622c7501f3f1424eaa05754928e0c5ae5fe3f7e00777acbec49","source":"sources/w8-object-list-math-mem-realloc/mem_realloc.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"next_index.o","profile":"authored_cpp","sha256":"2e1c5c89a24d05bac6eef1ba3d472f06604df66de02f01ac869958cac98d65a5","source":"sources/w8-object-list-math-next-index/next_index.cpp"}
{"emitted_functions":[{"compiled_body_bytes":116,"name":"_ZN12_GLOBAL__N_1L10delete_allEP8ListHeadi"},{"compiled_body_bytes":312,"name":"objectClear__Fv"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp","sources/w8-object-list-math-common/safe_iterator.hpp"],"kind":"independently_authored_source","object_name":"object_clear.o","profile":"authored_cpp","sha256":"ce2da6e60bd3a1735739e91a2bd4b08eb9e0e4fc64c2768f13f630ffd38abbc0","source":"sources/w8-object-list-math-object-clear/object_clear.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"object_cycle_words.o","profile":"authored_cpp","sha256":"d281ae66948772f5e608764fc8a53724a5688c84739049792cfd3cf808685428","source":"sources/w8-object-list-math-object-cycle-words/object_cycle_words.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"quatToMat__FP8Matrix33P4Quat"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/matrix_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"quat_to_mat33.o","profile":"authored_cpp","sha256":"4f3d0ac82a01a35038c5bea7adb2edf576b48b68447cc08ca01a76e76e6ad3db","source":"sources/w8-object-list-math-quat-to-mat33/quat_to_mat33.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"objectRemoveFromGrid__FP10GameObject"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"remove_from_grid.o","profile":"authored_cpp","sha256":"3624a1bbde590b050b37a38bda12c7062b19d7963610f836ad80a98d6da5b324","source":"sources/w8-object-list-math-remove-from-grid/remove_from_grid.cpp"}
{"emitted_functions":[{"compiled_body_bytes":28,"name":"objectRemoveFromRunList__FP10GameObject"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"remove_from_run_list.o","profile":"authored_cpp","sha256":"edd2c6fae0a270f799b426b5e533803323092a1d212213ea102ceed2cf7b0f5a","source":"sources/w8-object-list-math-remove-from-run-list/remove_from_run_list.cpp"}
{"emitted_functions":[],"header_closure":["sources/w8-object-list-math-common/list_types.hpp"],"kind":"independently_authored_source","object_name":"run_list_heads.o","profile":"authored_cpp","sha256":"9ddaa5253a8500655101a864acf28af4268efe9197ebe8f64a16b5ad64021caf","source":"sources/w8-object-list-math-run-list-heads/run_list_heads.cpp"}
{"emitted_functions":[{"compiled_body_bytes":108,"name":"__16ListSafeIteratorP8ListHeadi"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp","sources/w8-object-list-math-common/safe_iterator.hpp"],"kind":"independently_authored_source","object_name":"safe_iterator_construct.o","profile":"authored_cpp","sha256":"9a56ba3abcb40511430f21dc5518525c1fa16404d76be0187f38a7d3a60a9939","source":"sources/w8-object-list-math-safe-iterator-construct/safe_iterator_construct.cpp"}
{"emitted_functions":[{"compiled_body_bytes":64,"name":"_$_16ListSafeIterator"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp","sources/w8-object-list-math-common/safe_iterator.hpp"],"kind":"independently_authored_source","object_name":"safe_iterator_destroy.o","profile":"authored_cpp","sha256":"c8942f80e369609a4d0e689334dd19c43436d70683d34cfcfd7ad46589ce42f5","source":"sources/w8-object-list-math-safe-iterator-destroy/safe_iterator_destroy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":148,"name":"next__16ListSafeIterator"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp","sources/w8-object-list-math-common/safe_iterator.hpp"],"kind":"independently_authored_source","object_name":"safe_iterator_next.o","profile":"authored_cpp","sha256":"35cd26880c8224412a08df64fa711a416b98c6d230819e33a5dfb0b989bbeb35","source":"sources/w8-object-list-math-safe-iterator-next/safe_iterator_next.cpp"}
{"emitted_functions":[{"compiled_body_bytes":316,"name":"objectSaveAddToStringPool__FPCc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"save_add_to_string_pool.o","profile":"authored_cpp","sha256":"bd38695809016a415191522a2fb023603218516b18aa27ce5a9ccddbe99742ba","source":"sources/w8-object-list-math-save-add-to-string-pool/save_add_to_string_pool.cpp"}
{"emitted_functions":[{"compiled_body_bytes":32,"name":"objectSaveGetFromStringPool__Fi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"save_get_from_string_pool.o","profile":"authored_cpp","sha256":"0ac3ce79c8b044ee98aebf1855f8534cc11954d621413ac7756c4538d851e229","source":"sources/w8-object-list-math-save-get-from-string-pool/save_get_from_string_pool.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject"}],"header_closure":["sources/w8-object-list-math-common/list_types.hpp","sources/w8-object-list-math-common/object_types.hpp"],"kind":"independently_authored_source","object_name":"set_create_by_name_function.o","profile":"authored_cpp","sha256":"430c9006087e04668dc44804f5c3eb170951beeb4f746eefb44bb9dfd8574d2d","source":"sources/w8-object-list-math-set-create-by-name-function/set_create_by_name_function.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"objectSetFilterFunction__FPFPc_i"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_filter_function.o","profile":"authored_cpp","sha256":"7df005cd9fba4ac08d67616aba3b8adc1377bdff797f27bb3dab39f85b2264b3","source":"sources/w8-object-list-math-set-filter-function/set_filter_function.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"string_pool_state.o","profile":"authored_cpp","sha256":"ae7edf6742b597435c98b80af264b21d3264cbcb4d2b1c531a8a25bed92efe62","source":"sources/w8-object-list-math-string-pool-state/string_pool_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"objectInside__7TriggerP10GameObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"trigger_inside.o","profile":"authored_cpp","sha256":"db1ef793607df4c4cae9a1b28a6e8ee38509affbb88d954dca3b8a98e9d4147d","source":"sources/w8-object-list-math-trigger-inside/trigger_inside.cpp"}
```

Files to add (sources and quoted-include headers, with their SHA-256 at the time of writing; regenerate if any file changes):

```json
{
 "sources/w8-object-list-math-activation-check/activation_check.cpp": "72b0ce651c48aea07144d0a78ce9fb73b7740fee278dac4588b4655969eb70c9",
 "sources/w8-object-list-math-add-to-delete-list/add_to_delete_list.cpp": "37851166ca52700b16883de9d90e222432bdc513094c2d1c1bebf4a1042d5982",
 "sources/w8-object-list-math-common/heap_block.hpp": "02e55b7f4f96fd540a6c2dce0a773f81706daa7de20efdc3c08dba4f1041a83a",
 "sources/w8-object-list-math-common/list_types.hpp": "be652864b60ffbeabc7f3ce8565fb5325d8e8152e322a7f83dffee0a9ffcc52e",
 "sources/w8-object-list-math-common/matrix_types.hpp": "60acc8e1561600c285536a092a00ddd3484c2ba34ec30883aedde3d6f8f4616a",
 "sources/w8-object-list-math-common/object_types.hpp": "067f05f181e7ed4bd3e689ff86c27905480fddcf239578f0e5c8b1a04b5f108e",
 "sources/w8-object-list-math-common/safe_iterator.hpp": "6b926a72eeeaad2f3b336c8a00c01c1c5cacf9ef2608cde4bb2590aff6264a81",
 "sources/w8-object-list-math-common/trig_core.hpp": "5899f3583228413c2e302318f06883862333d985964c4ac836106843b8054069",
 "sources/w8-object-list-math-create-by-name-forward/create_by_name_forward.cpp": "2f17c1b70aed79a905068f06c363f3e74c0752cb0d9ad5dc13540b1b0b0827c1",
 "sources/w8-object-list-math-do-my-actions/do_my_actions.cpp": "81c23d5d60b723111846d3c38cf57d14f74e2e84e27bce85c44af298e05977af",
 "sources/w8-object-list-math-find-animation/find_animation.cpp": "6a21e7b2ea92b99c65848dda6727442a3ca70c8084714c8bdc6a25b160a9ef0a",
 "sources/w8-object-list-math-find-object-by-class/find_object_by_class.cpp": "758683aba90fbc64729cc70880c271ad91b6465b082a68a9257de93a816daddc",
 "sources/w8-object-list-math-global-dtor-lastkey/global_dtor_lastkey.cpp": "55a1e80fe4304f19ea548866ba5cc7a76369708986355d5c7e92759a573fe7ef",
 "sources/w8-object-list-math-global-init-addpmlist/global_init_addpmlist.cpp": "fc772ac42e7e66b2410a9f59e7ccbe241c90adb724cb01b5f52e310b5259c019",
 "sources/w8-object-list-math-global-init-lastkey/global_init_lastkey.cpp": "2b278e895762b5831327cb9d54f9504d4bf79219367bac7c052031c7aed11673",
 "sources/w8-object-list-math-global-init-object-lists/global_init_object_lists.cpp": "7e8adda477b04df769b1728d9ff9c853e58966890fa2284262bb1270dc5af86a",
 "sources/w8-object-list-math-iatan2/iatan2.cpp": "c81d9bb048f7828e728a30bfd61fa8c73b504b85c694d47efcbbdb86074db185",
 "sources/w8-object-list-math-icos/icos.cpp": "95920516dd8fe7ef0ededf4603e875d08b1441713395a996dfc6ab217bba44cd",
 "sources/w8-object-list-math-isin/isin.cpp": "0a6fea253baaef63b0a7631e19b19c0b3eda739f661f171d9b60ecb0589c83c9",
 "sources/w8-object-list-math-line-initialize/line_initialize.cpp": "c6a377ec7fe986eb8775ce1ed305ded01b24bb63b3835f35d1cb815f9b05211d",
 "sources/w8-object-list-math-line-to-y/line_to_y.cpp": "e2832e220192ae837cac7d8be83a4e4055b5ef75ddabb2c8d9f6542ba44de96c",
 "sources/w8-object-list-math-list-oneway-link/list_oneway_link.cpp": "f855fcaa30ab0d12f87a26130f5009b9181dcd8818c55bc0cf0ee0e7bc25ec19",
 "sources/w8-object-list-math-mat-ident33/mat_ident33.cpp": "5d01abb4f7ded14fb53ac2ba80d4d0232c4690c90d22171f96889b1f608813e0",
 "sources/w8-object-list-math-mat-make-trans-yaw/mat_make_trans_yaw.cpp": "f25b324ce549d5e3adeb2def192f6a0f2d6f3c198c030022b18ec1b9f794ecab",
 "sources/w8-object-list-math-mat-rot33/mat_rot33.cpp": "9782780078f5b0343bb928ec4ad56b9aefda204f5b8e694d58ac8f6d070ec588",
 "sources/w8-object-list-math-mat-to-quaternion/mat_to_quaternion.cpp": "a33dc4fce951000a51da3355fe166fbf23aec3851ea2e13e97a095d47cdd9984",
 "sources/w8-object-list-math-mat-transform33/mat_transform33.cpp": "1bb735235a3ac64620f6e8df805086dbf31215d997a4ef1cb0a28d8e020070e3",
 "sources/w8-object-list-math-mat-transform34-vec/mat_transform34_vec.cpp": "401563c6a0a3b740498225e3ea766f6556bbaa726e789e389c57b2a8ed3daebe",
 "sources/w8-object-list-math-mem-alloc/mem_alloc.cpp": "ffb0e3f90009565d7c6f574818fddba661584bc5959bb55774ecfebe33f10adc",
 "sources/w8-object-list-math-mem-check-heap-real/mem_check_heap_real.cpp": "1bd16b825e7164dfa54adaf083deb43d5a7849332fd937e6356c2cf11c2ac27e",
 "sources/w8-object-list-math-mem-free/mem_free.cpp": "7420503b52f8721f0fbc8d47f202a3541fdfeef414327ef5dcc4d2e5609a557f",
 "sources/w8-object-list-math-mem-realloc/mem_realloc.cpp": "aca2044a92eab622c7501f3f1424eaa05754928e0c5ae5fe3f7e00777acbec49",
 "sources/w8-object-list-math-next-index/next_index.cpp": "2e1c5c89a24d05bac6eef1ba3d472f06604df66de02f01ac869958cac98d65a5",
 "sources/w8-object-list-math-object-clear/object_clear.cpp": "ce2da6e60bd3a1735739e91a2bd4b08eb9e0e4fc64c2768f13f630ffd38abbc0",
 "sources/w8-object-list-math-object-cycle-words/object_cycle_words.cpp": "d281ae66948772f5e608764fc8a53724a5688c84739049792cfd3cf808685428",
 "sources/w8-object-list-math-quat-to-mat33/quat_to_mat33.cpp": "4f3d0ac82a01a35038c5bea7adb2edf576b48b68447cc08ca01a76e76e6ad3db",
 "sources/w8-object-list-math-remove-from-grid/remove_from_grid.cpp": "3624a1bbde590b050b37a38bda12c7062b19d7963610f836ad80a98d6da5b324",
 "sources/w8-object-list-math-remove-from-run-list/remove_from_run_list.cpp": "edd2c6fae0a270f799b426b5e533803323092a1d212213ea102ceed2cf7b0f5a",
 "sources/w8-object-list-math-run-list-heads/run_list_heads.cpp": "9ddaa5253a8500655101a864acf28af4268efe9197ebe8f64a16b5ad64021caf",
 "sources/w8-object-list-math-safe-iterator-construct/safe_iterator_construct.cpp": "9a56ba3abcb40511430f21dc5518525c1fa16404d76be0187f38a7d3a60a9939",
 "sources/w8-object-list-math-safe-iterator-destroy/safe_iterator_destroy.cpp": "c8942f80e369609a4d0e689334dd19c43436d70683d34cfcfd7ad46589ce42f5",
 "sources/w8-object-list-math-safe-iterator-next/safe_iterator_next.cpp": "35cd26880c8224412a08df64fa711a416b98c6d230819e33a5dfb0b989bbeb35",
 "sources/w8-object-list-math-save-add-to-string-pool/save_add_to_string_pool.cpp": "bd38695809016a415191522a2fb023603218516b18aa27ce5a9ccddbe99742ba",
 "sources/w8-object-list-math-save-get-from-string-pool/save_get_from_string_pool.cpp": "0ac3ce79c8b044ee98aebf1855f8534cc11954d621413ac7756c4538d851e229",
 "sources/w8-object-list-math-set-create-by-name-function/set_create_by_name_function.cpp": "430c9006087e04668dc44804f5c3eb170951beeb4f746eefb44bb9dfd8574d2d",
 "sources/w8-object-list-math-set-filter-function/set_filter_function.cpp": "7df005cd9fba4ac08d67616aba3b8adc1377bdff797f27bb3dab39f85b2264b3",
 "sources/w8-object-list-math-string-pool-state/string_pool_state.cpp": "ae7edf6742b597435c98b80af264b21d3264cbcb4d2b1c531a8a25bed92efe62",
 "sources/w8-object-list-math-trigger-inside/trigger_inside.cpp": "db1ef793607df4c4cae9a1b28a6e8ee38509affbb88d954dca3b8a98e9d4147d"
}
```

## Authored but never executed

Everything above. The 42 units were compiled and relocatably linked with the reviewed EE toolchain; none was run on the EE, in an emulator, or against the original executable. The host contracts executed on an arm64 host with 8-byte pointers, so they test logic through named fields, not EE layouts; the EE layouts are asserted with `static_assert` under `__mips__` only. The inline `sqrt.s`, `rsqrt.s`, `max.s`, the store to address 0 in `memAlloc__FiPc`, the virtual-table calls in `objectClear__Fv`, `objectFindObjectByClass__FPcPP10GameObjecti` and `objectFindAnimation__FP10GameObjectPc`, and the BSS definitions have only been compiled. The three imports opened above are unimplemented.

## Unknown

- Whether `objectAddToDeleteList` really means to rewrite the object's first word, and what that word is.
- The meaning of 0xabe0 and the bit-28 test in `objectActivationCheck`, what the follow scripts do, and what the 64-bit word at +160 holds.
- The coordinate roles in `objectInside` (which two axes), the structure behind the +36 record in `objectFindAnimation`, and what the NPC inline text at +960 is.
- Who sets `poolOffset`, who fills `freeNodes`, and the capacity of the string pool.
- Whether `iatan2`'s `cvt.w.s` rounds or truncates on the EE; the host checks truncate.
- A one-point open `Line` makes `initializeMe__4Line` read the 64 bytes before its array; whether the game ever builds one is unknown.
- The `mark`/`rawMin`/`startNode`/`movingNode` word meanings beyond their use in `memAlloc` (the search always starts at `startNode`; nothing in this package writes it).
