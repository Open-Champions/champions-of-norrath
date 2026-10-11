# Floor init and floor drawing

Package work is on `agents/floor`. New source is under `experimental/sources/floor-*`. Host checks sit beside that source and must stay out of the EE plan: they include host headers. The reviewed EE profile is `authored_cpp` from `experimental/build-plan.json`. A private link, outside git, added the three units below to the fresh 344-input partial object. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

The fresh partial link on `ecceafb` before this package had 763 unresolved imports and zero COMMON. After these three objects, that private link had 761 unresolved imports and zero COMMON. `floorInit__Fv` was not in the 763, so defining it does not change the count. Nothing in the selected sources calls it yet.

## Observed

`floorInit__Fv` at `0x00192f00` is 56 bytes. It calls `floorClear__Fv`, then `engineAddTask__FPFv_viPci` with `floorDraw__Fv`, order 28, the unnamed label at `0x00464400`, and mode 1. The label bytes are `drawFloorSprites (must be before drawworld)` and a NUL. `floorInit__Fv` in `floor-init/floor_init.cpp` is that call order. The clear it calls is the existing `norrath_floor_clear`. The compiled body is 52 bytes. The host check fails unless clear runs first and the task arguments are the callback, 28, that label, and 1.

`floorDraw__Fv` at `0x001922b8` is 2908 bytes. The CPU routine reads `floorDrawEnable` and `spriteCount`, and returns when either is zero. Unless `gameIsPaused` is nonzero, it advances 16 expansion records of 20 bytes. Kind 2 adds the `.lit4` word `0x3d4ccccc` at `0x004801dc`. Every other kind adds `0x3e4ccccc` at `0x004801e0`. Those words are one ulp below the decimals `0.05f` and `0.2f`. It then builds the GIF/VIF chain, walks the 30 by 30 grid from rectangle `[0,29] x [0,29]`, calls `worldClipCubeToFrustum__FPA1_fi`, references `vu1floorstart` and `vu1floorend`, and calls `__11ResourceMapPcT1`, `get__11ResourceMap`, and `drawAddToList__FPviP8_textureP11_drawRecordi`. The fifth draw argument is zero. `floorDraw__Fv` in `floor-draw/floor_draw.cpp` is that CPU flow. The compiled body is 6400 bytes. The host check fails unless a disabled or empty draw makes no calls, kind 2 stores `0x3d4ccccc`, the other kind stores `0x3e4ccccc`, a paused draw skips the expansion, a fully visible sprite with contribution 50 loses one sprite, and a nonzero blood level publishes the fade packet without removing the sprite. Host floats stand in for the EE arithmetic on those ordinary values.

`get__11ResourceMap` at `0x00166f48` compares the map epoch, calls `lumpFindResource`, and stores the result at offset 12. The published `norrath_player_resource_map_get` does those steps, so the draw calls it.

Storage sizes are `st_size` when that field is nonzero, otherwise the span to the next symbol in the same section. Image bytes are zero except `floorDrawEnable`, which is the byte 1.

| Authored name | Original symbol | Address | Size |
|---|---|---|---|
| `norrath_floor_blood_level_word_view` | `g_BloodLevel` | `0x00484498` | 4 |
| `norrath_floor_sprite_count_word_view` | `spriteCount` | `0x004844d8` | 4 |
| `norrath_floor_fade_packet_word_view` | `fadePacket` | `0x004844dc` | 4 |
| `norrath_floor_free_list_word_view` | `freeList` | `0x00486a68` | 4 |
| `norrath_floor_light_interface_mode_word` | `interfaceMode` | `0x00483aac` | 4 |
| `norrath_floor_light_scene_view` | `scene` | `0x004f4060` | 56 |
| `norrath_floor_expand_list_view` | `expandList` | `0x007940b0` | 320 |
| `norrath_floor_sprite_grid_view` | `sprGrid` | `0x007ac1f0` | 3600 |
| `norrath_floor_sprite_packet_view` | `sprPacket` | `0x007941f0` | 98304 |
| `norrath_floor_light_grid_view` | `lightGrid` | `0x004f40f0` | 32400 |
| `floorDrawEnable` | `floorDrawEnable` | `0x004844e0` | 1 |
| `norrath_floor_sprite_texture_map` | `tex.678` | `0x007940a0` | 16 |
| `norrath_floor_sprite_texture_ready_word` | `_$tmp_0.679` | `0x004844e4` | 4 |
| `norrath_floor_sprite_texture_group` | none | `0x00464430` | `engine.lmp` |
| `norrath_floor_sprite_texture_name` | none | `0x00464440` | `floorsprite.tex` |

`scene` ends at `savedScene`. `lightInit` writes through byte 55, which is inside those 56 bytes. `lightGrid` is 90 by 90 words, the same bound `findLightsAtPoint` clamps to. `sprPacket` is 128 records of 768 bytes and ends where `sprGrid` begins. `expandList` is 16 records of 20 bytes.

`vu1floorstart` is 64 bytes at `0x00333df0`. `vu1floorend` is the same address as `vu1mapstart`, `0x00334120`. The draw and clear routines subtract those addresses: 816 bytes, 619 of them nonzero. That image is the original program payload and is not in this source.

Host checks:

```sh
c++ -std=c++17 -Wall -Wextra -o /tmp/floor-storage-check \
  experimental/sources/floor-storage/host_check.cpp
/tmp/floor-storage-check
c++ -std=c++17 -Wall -Wextra -o /tmp/floor-init-check \
  experimental/sources/floor-init/host_check.cpp
/tmp/floor-init-check
c++ -std=c++17 -Wall -Wextra -o /tmp/floor-draw-check \
  experimental/sources/floor-draw/host_check.cpp
/tmp/floor-draw-check
```

## Inferred

The `norrath_floor_*_view` names are the names already imported by the selected sources. The table above is the original object each one refers to. `tex.678` and `_$tmp_0.679` are compiler temporaries; the texture map and ready-word names are inferred. The task-label and texture-string names are inferred. There is no ELF symbol on those three addresses.

`norrath_player_resource_map_get` is used as `get__11ResourceMap`. The published body matches the original steps. This package does not define a second copy.

The reviewed `-G0` profile does not recreate `.sdata` or `.sbss`. Zero words are BSS. `floorDrawEnable` stays initialized data because its byte is 1.

The original fifth `drawAddToList` value is written in `t0`. This source passes that zero as the fifth C argument. The reviewed EABI's register assignment is not claimed to be that `t0` slot.

## Imports closed

`norrath_floor_blood_level_word_view`, `norrath_floor_expand_list_view`, `norrath_floor_fade_packet_word_view`, `norrath_floor_free_list_word_view`, `norrath_floor_light_grid_view`, `norrath_floor_light_interface_mode_word`, `norrath_floor_light_scene_view`, `norrath_floor_sprite_count_word_view`, `norrath_floor_sprite_grid_view`, `norrath_floor_sprite_packet_view`.

`floorInit__Fv` and `floorDraw__Fv` are defined. They were not unresolved imports in the 763, because `engineInit` is not authored yet. `floorDrawEnable`, the texture map, the ready word, and the three strings are defined with them and were not previous imports.

## Imports opened

`__11ResourceMapPcT1`, `drawAddToList__FPviP8_textureP11_drawRecordi`, `glistp`, `worldCameraPos`, `worldClipCubeToFrustum__FPA1_fi`, `worldWorldViewMatrix`, `zwritesOff`, `zwritesOn`.

`engineAddTask__FPFv_viPci` was already unresolved. `norrath_floor_clear`, `norrath_game_is_paused`, `norrath_integer_cosine`, `norrath_integer_sine`, `ee_cvt_w_s_word`, and `norrath_player_resource_map_get` were already defined.

Still unresolved, and left that way: `norrath_floor_program_start_view`, `norrath_floor_program_end_view`.

## Build-plan entries needed

Append these authored inputs. Do not add the `host_check.cpp` files. Assign `input_order` at integration. Profile `authored_cpp`. Header closure is empty. `snapshot.json` must record the `.cpp` sources and the host checks before `tools/check_source_snapshot.py` will accept the tree. This note does not edit that snapshot.

```json
[
  {
    "source": "sources/floor-storage/floor_storage.cpp",
    "object_name": "floor_storage.o",
    "sha256": "4bee1cf0d05450b9a8dccad9fdcc24a8f07c6947515a72f8f90638dbc0fcca6d",
    "emitted_functions": []
  },
  {
    "source": "sources/floor-draw/floor_draw.cpp",
    "object_name": "floor_draw.o",
    "sha256": "6258e2eb8b666a095dd7c075a9ba01834c8cbf6b3ea589bfaa3980eeb45dba31",
    "emitted_functions": [{"name": "floorDraw__Fv", "compiled_body_bytes": 6400}]
  },
  {
    "source": "sources/floor-init/floor_init.cpp",
    "object_name": "floor_init.o",
    "sha256": "a81aa00394a34ee018104cbe90be80e387ad3646ca234aebdba49abd088eb16a",
    "emitted_functions": [{"name": "floorInit__Fv", "compiled_body_bytes": 52}]
  }
]
```

## Authored but never executed

`floorInit__Fv`, `floorDraw__Fv`, and every object in `floor_storage.cpp`. Host checks ran on this machine. The reviewed EE compiler produced the three relocatable objects, and a private relocatable link with the current partial object succeeded. No routine from this package has run on the EE, under the original process, or as gameplay. The VU program was not uploaded. The GS was not driven. The scheduler did not run the new task.

## Unknown

The 816 bytes from `vu1floorstart` to `vu1floorend` are not reproduced. A zero span of that length would make the size arithmetic match and would still be the wrong program.

`floorInit__Fv` is 52 compiled bytes against 56 original bytes. `floorDraw__Fv` is 6400 compiled bytes against 2908 original bytes. Neither is an original-byte match. The draw's C++ frame, the 528-byte original frame, FCSR exceptions, and the `t0` slot for the fifth draw argument were not compared.

`glistp`, `zwritesOn`, `zwritesOff`, `worldCameraPos`, and `worldWorldViewMatrix` are shared objects. This package does not own them. `worldClipCubeToFrustum`, the `ResourceMap` constructor, and `drawAddToList` are unimplemented here. `engineAddTask` remains the scheduler package's import.
