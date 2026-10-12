# floor-world-view

Takeover note. The previous agent left 14 uncommitted source directories. I rebuilt and re-ran every one, then committed each separately.

## Checks performed

- Each unit compiled with the reviewed EE `authored_cpp` flags (`-G0 -O2 ... -march=r5900 -std=c++17`). All 14 succeeded.
- Each `host_check.cpp` was built with `c++ -std=c++17 -Wall -Wextra` and run. All 14 exited 0. The door and draw checks use `-DNORRATH_HOST_DOOR` / `-DNORRATH_HOST_DRAW_ADD`, which swap pointer words for arena offsets on the host.
- I did not run `build_partial.py` or the tools unit tests in this takeover; the shared plan was not edited.

## Observed

Addresses are from the private SLUS_205.65 as recorded in each source header comment. Function sizes in the EE objects differ from the original sizes; none is claimed as a byte match.

## Inferred

- `floorAdd__FG6Point3sfii` is only a forwarding entry (8 bytes) to the already authored `norrath_floor_add`. The original is 956 bytes. It exists to close the import name.
- `norrath_floor_program_start_view` / `_end_view` are zero BSS spans of 816 and 64 bytes. They are not the VU program.
- Door and water helpers use `norrath_world_*` names already imported by other packages.

## Imports closed

`__11ResourceMapPcT1`, `drawAddToList__FPviP8_textureP11_drawRecordi`, `farClip`, `floorAdd__FG6Point3sfii`, `norrath_floor_program_end_view`, `norrath_floor_program_start_view`, `norrath_world_check_for_water`, `norrath_world_find_closest_door`, `norrath_world_get_door_height`, `norrath_world_set_door_collision`, `worldCameraPos`, `worldClipCubeToFrustum__FPA1_fi`, `worldSetFarClip__Ff`, `worldSetFrustum__Fffff`, `worldWorldViewMatrix`.

Also defined here and not previously imports: `worldFrustum`, `frustumNormalSign`, `viewFrustum`, `clipViewFrustum`, `worldScrz`.

## Imports opened

`worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi`, `drawRecords`, `drawHead`, `writeBuffer`, `nmDrawUsed`, `textureMaxBlockSize`, `drawFrameNm`, `tanf` (libm), `norrath_floor_add` (already authored elsewhere). `worldTransformFrustum__FP8Matrix34` writes `worldFrustum` and is not in this package.

## Build-plan entries needed

Append with profile `authored_cpp`, empty header closure, `input_order` assigned at integration. `emitted_functions` should be filled from the integrator's build. Do not add `host_check.cpp`.

```json
[
  {
    "source": "sources/floor-world-view-camera-pos/camera_pos.cpp",
    "object_name": "camera_pos.o",
    "sha256": "ef7a027af5cd4332c33185b02ddf0a3a125f63a1755764e93b86eb3346ce513f"
  },
  {
    "source": "sources/floor-world-view-check-water/check_water.cpp",
    "object_name": "check_water.o",
    "sha256": "85714dc5b4df2594085ac495678fea2c9a64a5fd804fad3f4c7e18b8c85d4d13"
  },
  {
    "source": "sources/floor-world-view-clip-cube/clip_cube.cpp",
    "object_name": "clip_cube.o",
    "sha256": "8dbd2305cd82abfdfcd0a971a3878977a008e8cf232ff3e7a162e607f38992b0"
  },
  {
    "source": "sources/floor-world-view-closest-door/closest_door.cpp",
    "object_name": "closest_door.o",
    "sha256": "857347960e97a79cea75d02169d4f6544157a9f430c9a39c61030215f6d51d18"
  },
  {
    "source": "sources/floor-world-view-door-collision/door_collision.cpp",
    "object_name": "door_collision.o",
    "sha256": "8ed494859e3ac9c8a23b15cff9c6baa2619307a8dcda0168eacca4d716eb9502"
  },
  {
    "source": "sources/floor-world-view-door-height/door_height.cpp",
    "object_name": "door_height.o",
    "sha256": "fe8f32ddedd5faebf08ee972310bcd2c4e09696586b3fc5eb51096fe490ceb18"
  },
  {
    "source": "sources/floor-world-view-draw-add/draw_add.cpp",
    "object_name": "draw_add.o",
    "sha256": "f96a83391370ebb608f7be4bbefd69699508510c335e65a3b9b52bd5067e9a6f"
  },
  {
    "source": "sources/floor-world-view-far-clip/far_clip.cpp",
    "object_name": "far_clip.o",
    "sha256": "1c0c197a50f03efa846b8ef03f8437c1ec642e511eb2fb01df178cd751565147"
  },
  {
    "source": "sources/floor-world-view-floor-add/floor_add.cpp",
    "object_name": "floor_add.o",
    "sha256": "260bb78d6812254808be307023cb99cfebe542ec710fc1eb691f45b2eb93a346"
  },
  {
    "source": "sources/floor-world-view-program-views/program_views.cpp",
    "object_name": "program_views.o",
    "sha256": "5ac6039a24b8668b123c7bc6fee8f8b621ba650ff55ee8284e2e43c582e5c8ae"
  },
  {
    "source": "sources/floor-world-view-resource-map/resource_map.cpp",
    "object_name": "resource_map.o",
    "sha256": "8386a7a2bc0e7f53f353f0d4b505eecee49876e2a8fee6350d60fa4999181fb1"
  },
  {
    "source": "sources/floor-world-view-set-far-clip/set_far_clip.cpp",
    "object_name": "set_far_clip.o",
    "sha256": "ab4da0f336e919cfd54d25f4766d7fc5e1db1c4b5c1c21ee5088d95e3244cdc9"
  },
  {
    "source": "sources/floor-world-view-set-frustum/set_frustum.cpp",
    "object_name": "set_frustum.o",
    "sha256": "2f8ee54a3b9e92fb97375682906d5db0655419f6cb5602d95af25298b15c57a3"
  },
  {
    "source": "sources/floor-world-view-world-matrix/world_matrix.cpp",
    "object_name": "world_matrix.o",
    "sha256": "b59373f74f452683706a4a709b5b6bba8ab2948b270b4ae855887057b5032647"
  }
]
```

## Authored but never executed

Everything. Host checks ran on this machine against fixtures, with `tanf`, `worldFindWaterPatches` and `norrath_floor_add` replaced by test doubles. Nothing ran on the EE or in the original process.

## Unknown

- Layouts of `_worldHeader`, the 28-byte door record, the 36-byte and 60-byte element records, and `_texture`/`_drawRecord` are known only by the offsets these functions touch.
- Whether `floorAdd` in the original differs from `norrath_floor_add` beyond the argument forwarding.
- The VU program bytes and the purpose of the 64-byte original symbol sizes.
- `frustumNormalSign` contents: zero in the image, written by code outside this package.
