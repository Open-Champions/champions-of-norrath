# World state

This package reconstructs route insertion and the world, restore, dialog, and loader words that a fresh partial link still imported. Each unit was compiled with the reviewed EE `authored_cpp` flags and checked by a host test that compares the initial bytes or the route control flow. None of it has been linked into the partial object. `experimental/build-plan.json` and `experimental/snapshot.json` were left unchanged; the driver rejects a plan whose inputs do not match the snapshot, so these units stay out of that build until both files gain the entries below.

## Route insertion

`norrath_route_add_node` follows `routeAddNode` at `0x002bfcb8`. The original body is 572 bytes. The reviewed compile emits 1048 bytes, so this is a behavior reconstruction, not a byte match. The point arrives as 12 bytes. A free-root word of -1 scans the eight 32-byte cache slots, skips a slot whose marker word is -2, and retires the slot with the greatest unsigned age `drawFrameNm - stamp`. The retired node's link count walks the halfwords at node+12. Each referenced node stores -1 at offset 36 plus twice the old link, and `norrath_route_unroll_vnode` runs only when that node's own link matches. The node then returns to the free list. Allocation copies the point, advances the free root through the halfword at node+56, and stores `norrath_world_find_height(world, point, 6.0) + 36` at node+8.

The host test covers one allocation (free root 2, successor 5, used count 4 becoming 5) and one reclaim (frame 100, ages 10 and 60, two children, only the matching child unrolled, used count returning to 5). Slots that must not win are marked -2. The original failed search, where every slot is skipped and the chosen index stays -1, addresses the 32 bytes before the cache. That predecessor is outside the cache object and is not implemented or tested here.

`norrath_world_find_height` is already defined by `world-find-height-source-v2`. The route object still lists it undefined until that object is linked.

## Recorded objects

Headers declare the symbol. The `.cpp` file defines it. On the EE compiler the definition carries the original section; the host test ignores section names. Mutable objects start at the file bytes below and the test flips the first byte and restores it. Constant objects are compared and left unchanged.

| Symbol | Original | Address | Bytes | Section | Initial | Extent |
|---|---|---|---|---|---|---|
| `norrath_list_copy_object_word` | copyObject | `0x0048451c` | 4 | .sdata | 0 | recorded |
| `norrath_grid_ignore_flags_view` | objectIgnoreFlags | `0x00483ac0` | 4 | .sdata | 0 | recorded |
| `norrath_grid_vertical_extent_view` | unnamed | `0x0047fcc8` | 4 | .lit4 | `00 50 c3 47` (100000.0) | recorded load; no dedicated symbol; neighbors excluded |
| `norrath_game_cut_mode_byte_view` | cutMode | `0x00484318` | 1 | .sdata | 0 | recorded; next byte is cutPlayMode |
| `norrath_game_core_loading_word_view` | coreLoading | `0x00486680` | 4 | .sdata | 0 | recorded |
| `norrath_dialog_active_pointer_word_view` | curDlg | `0x00486b80` | 4 | .sbss | 0 | inferred; ELF size 0; dlgPos at +4 |
| `norrath_restore_defaults_view` | lightStart | `0x0037f960` | 36 | .data | 0 | recorded |
| `norrath_restore_string_base_word` | poolOffset | `0x004868a4` | 4 | .sbss | 0 | inferred; ELF size 0; next symbol at +4 |
| `norrath_restore_filter_word` | userFilterFunction | `0x00483b00` | 4 | .sdata | 0 | recorded |
| `norrath_restore_broadcast_head_view` | deleteList | `0x00486880` | 8 | .sbss | 0 | inferred; ELF size 0; alwaysDrawList at +8 |
| `norrath_loader_special_flag_view` | noNewVags | `0x00483f90` | 4 | .sdata | 0 | recorded |
| `norrath_loader_active_buffer_view` | lumpLoadLump | `0x00484014` | 4 | .sdata | 0 | recorded |
| `norrath_loader_active_size_view` | lumpLoadSize | `0x00484018` | 4 | .sdata | 0 | recorded |
| `norrath_loader_active_marker_view` | lumpLoadCompletionFlag | `0x00484011` | 1 | .sdata | 0 | recorded |
| `norrath_loader_active_name_view` | lumpLoadName | `0x006d86c0` | 192 | .bss | 0 | inferred; ELF size 0; globDir at +192 |
| `norrath_loader_current_name_view` | engineCurrentLevelName | `0x0037f988` | 80 | .data | 0 | recorded |
| `norrath_loader_name_a_view` | unnamed | `0x00463470` | 13 | .rodata | `vamplord.lmp` | C string; padding excluded |
| `norrath_loader_name_b_view` | unnamed | `0x00463480` | 13 | .rodata | `superorc.lmp` | C string; padding excluded |
| `norrath_loader_context_a_view` | unnamed | `0x00484030` | 8 | .sdata | `gothic2` | next symbol starts at the next byte |
| `norrath_loader_context_b_view` | unnamed | `0x00463490` | 9 | .rodata | `ruinint1` | C string; padding excluded |
| `norrath_loader_missing_format_view` | unnamed | `0x00463458` | 22 | .rodata | `!!!!!!lump not found\n` | C string; two pad zeros excluded |
| `norrath_route_nodes_bytes` | nodes | `0x00438f08` | 32768 | .data | 0 | recorded; 512 records of 64 |
| `norrath_route_cache_bytes` | clustercache | `0x00440f08` | 256 | .data | 0 | recorded; 8 records of 32 |
| `norrath_route_node_used_bytes` | nodeused | `0x00486560` | 4 | .sdata | 0 | recorded |
| `norrath_route_free_root_bytes` | freeroot | `0x00486564` | 4 | .sdata | 0 | recorded; runtime -1 means reclaim |
| `norrath_route_world_word_bytes` | world | `0x004844a0` | 4 | .sdata | 0 | recorded |
| `norrath_route_draw_frame_bytes` | drawFrameNm | `0x004844a8` | 4 | .sdata | `00 04 00 00` (1024) | recorded; not zero |

`objectIgnoreFlags` is also the word `objectPutSaveData` passes as the broadcast mask. `norrath_restore_broadcast_mask_word` is that same address. This package defines it once, under the grid name. A second link symbol is not defined.

## Imports closed

`norrath_route_add_node`, `norrath_list_copy_object_word`, `norrath_grid_ignore_flags_view`, `norrath_grid_vertical_extent_view`, `norrath_game_cut_mode_byte_view`, `norrath_game_core_loading_word_view`, `norrath_dialog_active_pointer_word_view`, `norrath_restore_defaults_view`, `norrath_restore_string_base_word`, `norrath_restore_filter_word`, `norrath_restore_broadcast_head_view`, `norrath_loader_special_flag_view`, `norrath_loader_active_buffer_view`, `norrath_loader_active_size_view`, `norrath_loader_active_marker_view`, `norrath_loader_active_name_view`, `norrath_loader_current_name_view`, `norrath_loader_name_a_view`, `norrath_loader_name_b_view`, `norrath_loader_context_a_view`, `norrath_loader_context_b_view`, `norrath_loader_missing_format_view`, `norrath_route_nodes_bytes`, `norrath_route_cache_bytes`, `norrath_route_node_used_bytes`, `norrath_route_free_root_bytes`, `norrath_route_world_word_bytes`, `norrath_route_draw_frame_bytes`.

## Imports opened

`norrath_route_unroll_vnode`. Route insertion calls it when a retired node's child still points at the retired link. It has no definition in this package.

`norrath_world_find_height` remains an undefined symbol in the route object and is already provided by `sources/world-find-height-source-v2/find_height.cpp`.

## Build-plan entries needed

Append these to `experimental/build-plan.json` and add matching `experimental/snapshot.json` inputs. `input_order` is the next free index at merge. `sha256` is the source file. `emitted_functions` belongs on the snapshot input; data units emit none. The route function's compiled body is 1048 bytes under the reviewed flags, including `-x c++`. Host contracts are not inputs. `tools/check_source_snapshot.py` will reject these sources until the snapshot lists them.

```json
{
  "files": {
    "sources/world-state-route-add-node/route_add_node.cpp": "4300a59ce76dc11188710ff06d300e5a6d62d66be6e2aca1476ef7f912f59707",
    "sources/world-state-route-add-node/route_add_node.hpp": "3c1e67a7ec0dc6824fe0a12635938304877522fe941ba87701734b4cf0b57c02",
    "sources/world-state-copy-object/storage.cpp": "373eabe186793f655c349949197db6d5d45c4a0015d30d7e742c99150b46f593",
    "sources/world-state-copy-object/storage.hpp": "4dce002754cd378038c8aa8ccc7c0c76025779c040fae26833c46913714a4dbb",
    "sources/world-state-ignore-flags/storage.cpp": "3f2b2d4bb42c7ccb1b5f2b5153fef8432698f95f2ccead552ed702f05573ff60",
    "sources/world-state-ignore-flags/storage.hpp": "9a68f5928a2e29dd5914c898cd51612be9592754eb53ff9c2b19ce8db4b7c13e",
    "sources/world-state-vertical-extent/storage.cpp": "a41807d4c042e63fe652f2ed654ad732bc0956456f3252abd04e48a6d8ec8d14",
    "sources/world-state-vertical-extent/storage.hpp": "80d9496453a8ce21a2d5ca7a17626f81df12d2d748fb68db8c7defc81fcf811a",
    "sources/world-state-cut-mode/storage.cpp": "e54618541c5df8ee21f5f25eb7a6935035a623769960ef67644b5deebd55c737",
    "sources/world-state-cut-mode/storage.hpp": "5477e92267512696c06e039ba45a5c1acc37411b7113d81bcc1b48e0f04a659e",
    "sources/world-state-core-loading/storage.cpp": "e71d66289391819e861aa9e14f43330a39505a2ac1e8febf9a00eef94202c374",
    "sources/world-state-core-loading/storage.hpp": "f333bc5cdbb968eb5809d04d88e66d4765adf75315dbc144e345eae954d9ee0b",
    "sources/world-state-dialog-active/storage.cpp": "189f16a0d37cc43b9adc3e8c7b5ad37f79566769565fe962846f4a903158eb40",
    "sources/world-state-dialog-active/storage.hpp": "e869eee5d316aabcdf7289c73fdf406461a811ee5466f180e6d4557f5fcb8fbb",
    "sources/world-state-restore-defaults/storage.cpp": "92369dd4e13f9264167f4a43907e2eb4d877df4d649dfbb327adbbef7d895084",
    "sources/world-state-restore-defaults/storage.hpp": "01037d16c3b9380681f437935c2708b41da6d9285e530cc05c241f5e312ae12d",
    "sources/world-state-restore-string-base/storage.cpp": "b54b7efaab7da38df088d90e40d023738714bcd68844aa67fb6750411dde0d90",
    "sources/world-state-restore-string-base/storage.hpp": "6888687e05dac0880ae274dbd30ac3c506bddf9e9e14397630dd8c8bcdd05cea",
    "sources/world-state-restore-filter/storage.cpp": "ac7edb75791cbe1bc4d54da5aa2e391cc4bef4765cbc373fb2b27c713a2278bb",
    "sources/world-state-restore-filter/storage.hpp": "58ba536be2aa2b8506265c37b8073cd55ddeda6d60eb6a10e4bc06671aac048b",
    "sources/world-state-restore-broadcast-head/storage.cpp": "a3958c83fc43bd2cd2fe2af058c792875ed952fa8162e5d7c18d60060ec692ff",
    "sources/world-state-restore-broadcast-head/storage.hpp": "2c08fb4c34aedf9176131bfa8e747199ce3e8f298b203bad223c55c6333f61ab",
    "sources/world-state-loader-special-flag/storage.cpp": "3e1872a6dccad8b58b360494bd7939c33ef4bdba6f3d2efc6557e27321f10b0f",
    "sources/world-state-loader-special-flag/storage.hpp": "43a757846f4e94b16969590239ba0790160a2c9b8ed7a5440bb35a938567170a",
    "sources/world-state-loader-active-buffer/storage.cpp": "a2c0c8c3f97b897ce68de31d0614a26c34307e67d046c0aac50bb1a18afb4a9f",
    "sources/world-state-loader-active-buffer/storage.hpp": "e07d86390c5cf24b22c128701803a29b13142573d288a4c00b6497132f47e451",
    "sources/world-state-loader-active-size/storage.cpp": "c91d64342bd8c425bfb85a147aef17ee90474127d3573a0a108168653cc79157",
    "sources/world-state-loader-active-size/storage.hpp": "7093041c7b1e998f686f28939c23ef4897bdaa96c0d7be400b285b114a4a33bf",
    "sources/world-state-loader-active-marker/storage.cpp": "feb19203f6c0c827bf0ba639e861e9b894af54c36ee54a83202a1df65dd98377",
    "sources/world-state-loader-active-marker/storage.hpp": "fe193c73eab21c333a5cae773f806c3e689b91ca6333f509113ae99717a91a38",
    "sources/world-state-loader-active-name/storage.cpp": "d7f7b6eb78e03d4aa64c8a477f1154536b50cc595f7ff6c4e2bdb82aae5d8a84",
    "sources/world-state-loader-active-name/storage.hpp": "c64b69d3552a6000b994ebc35f9a8a0198967c8263dd16b5f539a2fcad5e7966",
    "sources/world-state-loader-current-name/storage.cpp": "67aa0be01d692e9d019d91b126c2c29128fb342fb182cd1f416e4f146c831803",
    "sources/world-state-loader-current-name/storage.hpp": "3f55158f9ecacb16b781b41205b716e2b6b11a7268e0dafcc71a3ca6c06468b6",
    "sources/world-state-loader-name-a/storage.cpp": "697b26b6490446bf59e2146ae3cb7a65b20cfb8d1341649fef4eedaed0741e36",
    "sources/world-state-loader-name-a/storage.hpp": "178f16376385a2afc71818e829362defa75c880f50059a362cbbb94709172873",
    "sources/world-state-loader-name-b/storage.cpp": "a05017d722ef0045e776104d9b547644d767ab82310e0091ddeffc8ec68b5ba2",
    "sources/world-state-loader-name-b/storage.hpp": "0b2f30800a2220c0b9dbf80dec1e5252be400f17ecf0d95dbe8ab6cd29b3e6e9",
    "sources/world-state-loader-context-a/storage.cpp": "504ec9ff41c9865c242eb01cd4ef4d48a58d615ed57bd0e0e49ea898bf96e99c",
    "sources/world-state-loader-context-a/storage.hpp": "c798fb0881f7eb489adc0c0f5f3bd40bb0dd61ef8d9903eaf8209d2c0678fdef",
    "sources/world-state-loader-context-b/storage.cpp": "ec22b225e9e1c0bc727cedffc11dc49d72e726c8827ec45a46d2ff01ac6381a7",
    "sources/world-state-loader-context-b/storage.hpp": "cb22d4ff618df63f7a8b4eeeb8c6a9d6e51fa5f243171d0c4f82cd3c5724350d",
    "sources/world-state-loader-missing-format/storage.cpp": "168b0b55fd2c0fd0f448659897ecceace4a9232b03c9e0fa2cbc4a592de0a902",
    "sources/world-state-loader-missing-format/storage.hpp": "653bdc6903ec7bd17d9cebaded0df93b6bc5a4ef9b7cffcc67f6a4b3707f933c",
    "sources/world-state-route-nodes/storage.cpp": "b31c43e13e5b02866d88b905571a3c2a2056c9633c2c24b3cc79f324ff8e6efd",
    "sources/world-state-route-nodes/storage.hpp": "d42358db6dbe36d6018bb7cdc7be42bd56b2fbf1b0713811f120c9a72271cc21",
    "sources/world-state-route-cache/storage.cpp": "aba3e52a3faa98c7bfa7c17d6df045c9245889d3cb78526e79bd4d47a1007362",
    "sources/world-state-route-cache/storage.hpp": "8f93dfc57db11ea1cf02077c1c923fcff1dde04e94d52c53b04405b5abdc911b",
    "sources/world-state-route-node-used/storage.cpp": "1a3cba86d1d4e7e2c13d2ba67add360d99c40b6a0f28b6a518abcc5eed409fcd",
    "sources/world-state-route-node-used/storage.hpp": "37a74594c5b2e6b3574d8a57107aa43cbe9d146d09e260c0c3d613b512ab4300",
    "sources/world-state-route-free-root/storage.cpp": "8f0c452ac54ac291a8524f0d125ec16e935687d686165fdc0c37ab9c2685c1da",
    "sources/world-state-route-free-root/storage.hpp": "e84adf2b1dc55fc6d8accdb53ee87b07d09e4159b8f8ff960a6b5d26728872c1",
    "sources/world-state-route-world/storage.cpp": "c4f204f538e6a97993abe05c3db354ce4c6703e2bd5d8ba10694a47c4414b0ae",
    "sources/world-state-route-world/storage.hpp": "696acf8fbce2b86a5e2df93c18e9eef0024c41e3e55ea6cbbf19fd769f10d1b6",
    "sources/world-state-route-draw-frame/storage.cpp": "e003230effa3cf442909e89fce352ee6ef792506014dd657599af3d48ac70735",
    "sources/world-state-route-draw-frame/storage.hpp": "5430647e8879b62a947ebf52d1eeb720e9a83d16a1b1a0490abec335fdc93131"
  },
  "inputs": [
    {
      "header_closure": [
        "sources/world-state-route-add-node/route_add_node.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_add_node.o",
      "profile": "authored_cpp",
      "sha256": "4300a59ce76dc11188710ff06d300e5a6d62d66be6e2aca1476ef7f912f59707",
      "source": "sources/world-state-route-add-node/route_add_node.cpp",
      "emitted_functions": [
        {
          "name": "norrath_route_add_node",
          "compiled_body_bytes": 1048
        }
      ],
      "component": "world-state-route-add-node"
    },
    {
      "header_closure": [
        "sources/world-state-copy-object/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_list_copy_object.o",
      "profile": "authored_cpp",
      "sha256": "373eabe186793f655c349949197db6d5d45c4a0015d30d7e742c99150b46f593",
      "source": "sources/world-state-copy-object/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-copy-object"
    },
    {
      "header_closure": [
        "sources/world-state-ignore-flags/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_grid_ignore_flags.o",
      "profile": "authored_cpp",
      "sha256": "3f2b2d4bb42c7ccb1b5f2b5153fef8432698f95f2ccead552ed702f05573ff60",
      "source": "sources/world-state-ignore-flags/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-ignore-flags"
    },
    {
      "header_closure": [
        "sources/world-state-vertical-extent/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_grid_vertical_extent.o",
      "profile": "authored_cpp",
      "sha256": "a41807d4c042e63fe652f2ed654ad732bc0956456f3252abd04e48a6d8ec8d14",
      "source": "sources/world-state-vertical-extent/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-vertical-extent"
    },
    {
      "header_closure": [
        "sources/world-state-cut-mode/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_cut_mode.o",
      "profile": "authored_cpp",
      "sha256": "e54618541c5df8ee21f5f25eb7a6935035a623769960ef67644b5deebd55c737",
      "source": "sources/world-state-cut-mode/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-cut-mode"
    },
    {
      "header_closure": [
        "sources/world-state-core-loading/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_core_loading.o",
      "profile": "authored_cpp",
      "sha256": "e71d66289391819e861aa9e14f43330a39505a2ac1e8febf9a00eef94202c374",
      "source": "sources/world-state-core-loading/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-core-loading"
    },
    {
      "header_closure": [
        "sources/world-state-dialog-active/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_dialog_active.o",
      "profile": "authored_cpp",
      "sha256": "189f16a0d37cc43b9adc3e8c7b5ad37f79566769565fe962846f4a903158eb40",
      "source": "sources/world-state-dialog-active/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-dialog-active"
    },
    {
      "header_closure": [
        "sources/world-state-restore-defaults/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_restore_defaults.o",
      "profile": "authored_cpp",
      "sha256": "92369dd4e13f9264167f4a43907e2eb4d877df4d649dfbb327adbbef7d895084",
      "source": "sources/world-state-restore-defaults/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-restore-defaults"
    },
    {
      "header_closure": [
        "sources/world-state-restore-string-base/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_restore_string_base.o",
      "profile": "authored_cpp",
      "sha256": "b54b7efaab7da38df088d90e40d023738714bcd68844aa67fb6750411dde0d90",
      "source": "sources/world-state-restore-string-base/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-restore-string-base"
    },
    {
      "header_closure": [
        "sources/world-state-restore-filter/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_restore_filter.o",
      "profile": "authored_cpp",
      "sha256": "ac7edb75791cbe1bc4d54da5aa2e391cc4bef4765cbc373fb2b27c713a2278bb",
      "source": "sources/world-state-restore-filter/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-restore-filter"
    },
    {
      "header_closure": [
        "sources/world-state-restore-broadcast-head/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_restore_broadcast_head.o",
      "profile": "authored_cpp",
      "sha256": "a3958c83fc43bd2cd2fe2af058c792875ed952fa8162e5d7c18d60060ec692ff",
      "source": "sources/world-state-restore-broadcast-head/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-restore-broadcast-head"
    },
    {
      "header_closure": [
        "sources/world-state-loader-special-flag/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_special_flag.o",
      "profile": "authored_cpp",
      "sha256": "3e1872a6dccad8b58b360494bd7939c33ef4bdba6f3d2efc6557e27321f10b0f",
      "source": "sources/world-state-loader-special-flag/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-special-flag"
    },
    {
      "header_closure": [
        "sources/world-state-loader-active-buffer/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_active_buffer.o",
      "profile": "authored_cpp",
      "sha256": "a2c0c8c3f97b897ce68de31d0614a26c34307e67d046c0aac50bb1a18afb4a9f",
      "source": "sources/world-state-loader-active-buffer/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-active-buffer"
    },
    {
      "header_closure": [
        "sources/world-state-loader-active-size/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_active_size.o",
      "profile": "authored_cpp",
      "sha256": "c91d64342bd8c425bfb85a147aef17ee90474127d3573a0a108168653cc79157",
      "source": "sources/world-state-loader-active-size/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-active-size"
    },
    {
      "header_closure": [
        "sources/world-state-loader-active-marker/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_active_marker.o",
      "profile": "authored_cpp",
      "sha256": "feb19203f6c0c827bf0ba639e861e9b894af54c36ee54a83202a1df65dd98377",
      "source": "sources/world-state-loader-active-marker/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-active-marker"
    },
    {
      "header_closure": [
        "sources/world-state-loader-active-name/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_active_name.o",
      "profile": "authored_cpp",
      "sha256": "d7f7b6eb78e03d4aa64c8a477f1154536b50cc595f7ff6c4e2bdb82aae5d8a84",
      "source": "sources/world-state-loader-active-name/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-active-name"
    },
    {
      "header_closure": [
        "sources/world-state-loader-current-name/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_current_name.o",
      "profile": "authored_cpp",
      "sha256": "67aa0be01d692e9d019d91b126c2c29128fb342fb182cd1f416e4f146c831803",
      "source": "sources/world-state-loader-current-name/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-current-name"
    },
    {
      "header_closure": [
        "sources/world-state-loader-name-a/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_name_a.o",
      "profile": "authored_cpp",
      "sha256": "697b26b6490446bf59e2146ae3cb7a65b20cfb8d1341649fef4eedaed0741e36",
      "source": "sources/world-state-loader-name-a/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-name-a"
    },
    {
      "header_closure": [
        "sources/world-state-loader-name-b/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_name_b.o",
      "profile": "authored_cpp",
      "sha256": "a05017d722ef0045e776104d9b547644d767ab82310e0091ddeffc8ec68b5ba2",
      "source": "sources/world-state-loader-name-b/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-name-b"
    },
    {
      "header_closure": [
        "sources/world-state-loader-context-a/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_context_a.o",
      "profile": "authored_cpp",
      "sha256": "504ec9ff41c9865c242eb01cd4ef4d48a58d615ed57bd0e0e49ea898bf96e99c",
      "source": "sources/world-state-loader-context-a/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-context-a"
    },
    {
      "header_closure": [
        "sources/world-state-loader-context-b/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_context_b.o",
      "profile": "authored_cpp",
      "sha256": "ec22b225e9e1c0bc727cedffc11dc49d72e726c8827ec45a46d2ff01ac6381a7",
      "source": "sources/world-state-loader-context-b/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-context-b"
    },
    {
      "header_closure": [
        "sources/world-state-loader-missing-format/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_loader_missing_format.o",
      "profile": "authored_cpp",
      "sha256": "168b0b55fd2c0fd0f448659897ecceace4a9232b03c9e0fa2cbc4a592de0a902",
      "source": "sources/world-state-loader-missing-format/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-loader-missing-format"
    },
    {
      "header_closure": [
        "sources/world-state-route-nodes/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_nodes.o",
      "profile": "authored_cpp",
      "sha256": "b31c43e13e5b02866d88b905571a3c2a2056c9633c2c24b3cc79f324ff8e6efd",
      "source": "sources/world-state-route-nodes/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-route-nodes"
    },
    {
      "header_closure": [
        "sources/world-state-route-cache/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_cache.o",
      "profile": "authored_cpp",
      "sha256": "aba3e52a3faa98c7bfa7c17d6df045c9245889d3cb78526e79bd4d47a1007362",
      "source": "sources/world-state-route-cache/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-route-cache"
    },
    {
      "header_closure": [
        "sources/world-state-route-node-used/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_node_used.o",
      "profile": "authored_cpp",
      "sha256": "1a3cba86d1d4e7e2c13d2ba67add360d99c40b6a0f28b6a518abcc5eed409fcd",
      "source": "sources/world-state-route-node-used/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-route-node-used"
    },
    {
      "header_closure": [
        "sources/world-state-route-free-root/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_free_root.o",
      "profile": "authored_cpp",
      "sha256": "8f0c452ac54ac291a8524f0d125ec16e935687d686165fdc0c37ab9c2685c1da",
      "source": "sources/world-state-route-free-root/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-route-free-root"
    },
    {
      "header_closure": [
        "sources/world-state-route-world/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_world.o",
      "profile": "authored_cpp",
      "sha256": "c4f204f538e6a97993abe05c3db354ce4c6703e2bd5d8ba10694a47c4414b0ae",
      "source": "sources/world-state-route-world/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-route-world"
    },
    {
      "header_closure": [
        "sources/world-state-route-draw-frame/storage.hpp"
      ],
      "kind": "independently_authored_source",
      "object_name": "ws_route_draw_frame.o",
      "profile": "authored_cpp",
      "sha256": "e003230effa3cf442909e89fce352ee6ef792506014dd657599af3d48ac70735",
      "source": "sources/world-state-route-draw-frame/storage.cpp",
      "emitted_functions": [],
      "component": "world-state-route-draw-frame"
    }
  ]
}
```

## Authored but never executed

No gameplay, runtime, or partial-link execution. `norrath_route_add_node` was compiled with the reviewed EE toolchain and checked by the host allocation and reclaim test. Each data object was compiled with the same flags and checked by a host byte comparison. The private plan was not submitted to `tools/build_partial.py` because that driver requires the snapshot input list to match.

## Unknown

`norrath_monster_spawn` (`monsterSpawn` at `0x0026aa80`, 2112 bytes) is not reconstructed. The argument-key scan and the chance comparison against the `eRandState` step are observed. The later `%d` scan, the type-string walk, and the two calls to `spawn` are not translated into a tested function.

`norrath_restore_broadcast_mask_word` stays unresolved. It is `objectIgnoreFlags` at `0x00483ac0`, already defined as `norrath_grid_ignore_flags_view`. The link needs an alias; this package does not emit a second object.

`norrath_loader_failure_word_view` is not an ELF object. The referenced instruction is `sw zero, 0(zero)` at `0x00165d04`.

The dialog pointer, string base, broadcast-list head, and loader active name have ELF size 0. Their extents above are inferred from the next symbol. The route failed-search address before the cache is not reconstructed. The route function is not an original-byte match.
