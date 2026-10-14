# w15-m-data-b: plain data views and tables (2 of 2)

Worktree `agents/w15-m-data-b`. Sources are under `experimental/sources/w15-m-data-b-*`, one directory per symbol.
Scope: the 21 unresolved imports of this package in the wave brief. All 21 are data. The package had no function imports.

## Result

- **Closed 21 of 21** assigned imports with zero-filled storage. Each symbol is its own directory, its own commit, and was pushed
  right after its commit.
- Closing an import here means the link name and extent exist. It does not supply contents. Every unit is zeroed, and nothing in
  these commits establishes a field value. Read "Conflicts and runtime risks" before linking any of them into an image.
- No unresolved symbols remain in any of the 21 objects (`nm -u` is empty for each).

## Evidence

Observed (source and reference-image level):
- Each caller declares its name as an external array with a fixed or open extent (for example `extern char* skelightName[4];`,
  `extern const unsigned char sRainParticleDist[600];`, `extern unsigned char norrath_w13_spell_msg_a_view_<hex>[80];`).
- Sizes for the five non-view data names come from the symbol-table sizes of the reference executable, read locally with the
  reviewed EE `nm -S`: `skeletonResourceMap` 0x100, `skelightResourceMap` 0x50, `skelightName` 0x10, `skelightParams` 0x20,
  `sRainParticleDist` 0x258. Addresses: 0x0042d7d0, 0x0042dbf0, 0x00438ef8, 0x00438ed8, 0x0118eec0.
- The sixteen address-named views have a bss symbol at the address in the name; `nm -S` gives no size for them.

Inferred:
- 80 bytes and 16-byte alignment for the views. This follows the particle-definition record contract and the 0x50 spacing of
  neighbouring views, and matches the w13-spell-msg-a note that calls them 80-byte objects.
- 4-byte pointers on the target for `skelightName` (16 bytes for four entries). The EE `static_assert` checks this; the host
  check uses `sizeof(void*)`.
- 8-byte records for `skelightParams` (caller stride `8 * index`, guard `index < 4`).
- 64-byte records for `skeletonResourceMap` (caller forms `base + (kind << 6)`).

Unknown: every field value, every string the pointer tables name, the values behind `sRainParticleDist`, the bound behind the
caller guards, and whether the game writes any of these storages after start-up.

## Conflicts and runtime risks

- **Duplicate storage at two addresses.** `skeletonResourceMap` (0x42d7d0) and `skelightResourceMap` (0x42dbf0) are the same
  image bytes as the w9 sibling views `norrath_ctors_lava_wraith_view_42d7d0` and `norrath_ctors_lava_wraith_view_42dbf0`, which
  are already in the plan with contents. This package zeroes its copies, so the link would have two storages for one address.
  The integrator must keep one storage per address. This package does not alias them.
- **`sRainParticleDist` in a host test.** `w11-vi-render-a-atmosphere-init/host_contract.cpp` defines a table with the same name
  and values of its own. It is a host test, not a plan input, so it does not conflict with the EE link. Keep it out of any
  combined host build with this unit.
- **Zeroed pointer table.** `skelightName` is all null. `can_cast.cpp` passes `skelightName[index]` to `objectFindObject__FPc`
  without a null check, so a real run would pass null until the owner supplies the strings.
- **Zeroed parameter blocks.** `skelightParams` and the views give the callee zero data. No run has tested the effect.

## Checks performed

- The reviewed toolchain profile: the binary hashes in `experimental/build-plan.json` `toolchain.binary_hashes` match
  `work/toolchains/ee` for each listed binary.
- Each of the 21 units compiles alone with the `authored_cpp` flags from the plan profile (no includes needed).
- Each object's `nm -S` shows the expected symbol, size and type (`B`, zero-filled).
- Each `host_check.cpp` compiles with the host `c++ -std=c++17` and returns 0. It checks the extent, the alignment, and that every
  byte is zero. These checks would fail for a wrong extent, a wrong alignment, or non-zero contents. They do not check any field
  value.
- `nm -u` reports no undefined symbols in any of the 21 objects.
- `python3 -m unittest discover -s tools -p 'test_*.py'` ran 214 tests, all passing.
- I did not run `tools/build_partial.py`. No full partial build was run, and the plan was not edited.

## Imports closed (21)

Data, in the order the callers need them:

1. `skeletonResourceMap`: w12-actor-spell-a-ctor-skeleton/ctor.cpp.
2. `skelightResourceMap`: w14-projectile-a-ctor-skelight/ctor.cpp.
3. `skelightName`: w13-skill-cast-b-can-cast-summon-skeleton/can_cast.cpp.
4. `skelightParams`: w13-skill-cast-b-cast-summon-skeleton/cast.cpp.
5. `sRainParticleDist`: w11-vi-render-a-atmosphere-init/init.cpp.

Views (`unsigned char norrath_w13_spell_msg_a_view_<hex>[80]`, zeroed at the address the name gives). Each has two caller
directories, one from w13-spell-msg-a and one from w14-projectile-b:

- `7d7260`, `7d72b0`, `7d7300`, `8d6320`, `8d6370`, `8d67d0`, `8d6820`, `8d6870`, `8d7b30`, `8d7b80`, `8d7bd0`, `8d80a0`,
  `8d80f0`, `8d8140`, `8d8190`, `8d81e0`.

## Imports opened

None by the 21 objects: `nm -u` reports no undefined symbols for any of them. The reference list also names six address-named
views that are not in this package's brief: `norrath_w13_spell_msg_a_view_7d6970`, `..._7d6a10`, `..._7d6a60`, `..._7d6b00`,
`..._7d6fc0`, `..._7d7010`. I did not create them; they belong to another package. `7d6fc0` and `7d7010` are referenced by
w13-spell-msg-a-msg-run-14hateprojectile, so that owner should check them.

## Build-plan entries needed

The shared `experimental/build-plan.json` and `experimental/snapshot.json` are not edited here. Each unit is one
`independently_authored_source` input with an empty `header_closure`, `profile` `authored_cpp`, and empty `emitted_functions`
(data units emit no functions). `input_order` is for the integrator to assign after the current last input. Each `host_check.cpp`
is not a plan input. The `snapshot.json` input entries must carry the same `kind`, `object_name`, `source`, and empty
`emitted_functions`.

| symbol | source | object | SHA-256 of source |
|---|---|---|---|
| `skeletonResourceMap` | `experimental/sources/w15-m-data-b-skeleton-resource-map/skeletonResourceMap.cpp` | `w15_m_data_b_skeleton_resource_map.o` | `ab8257512e103f2bb60d02aa77ee88e2993c745de6556d2a54f2e6357e06b391` |
| `skelightResourceMap` | `experimental/sources/w15-m-data-b-skelight-resource-map/skelightResourceMap.cpp` | `w15_m_data_b_skelight_resource_map.o` | `d4d83ba46a6d4b23b6477f4648fc8f8274c4129fbc2adc26fa155043b6951742` |
| `skelightName` | `experimental/sources/w15-m-data-b-skelight-name/skelightName.cpp` | `w15_m_data_b_skelight_name.o` | `496f20b48e9b6a787b9571bb4c71f904c15fe1e9ca237c04e0495052bab3c79c` |
| `skelightParams` | `experimental/sources/w15-m-data-b-skelight-params/skelightParams.cpp` | `w15_m_data_b_skelight_params.o` | `c1d44498163ac18d78aa589d16eee550dd2ffdc8f7b084b16804053623560282` |
| `sRainParticleDist` | `experimental/sources/w15-m-data-b-s-rain-particle-dist/sRainParticleDist.cpp` | `w15_m_data_b_s_rain_particle_dist.o` | `d0949925d949cc308395b8832ce4eeb3e2609f5a00f6170c3b1b99062760845b` |
| `norrath_w13_spell_msg_a_view_7d7260` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-7d7260/norrath_w13_spell_msg_a_view_7d7260.cpp` | `w15_m_data_b_w13_spell_msg_a_view_7d7260.o` | `51593df0ab9e5454b01ecf64b54eaacb8e0d5a7771aac234db9771881f1a4919` |
| `norrath_w13_spell_msg_a_view_7d72b0` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-7d72b0/norrath_w13_spell_msg_a_view_7d72b0.cpp` | `w15_m_data_b_w13_spell_msg_a_view_7d72b0.o` | `085ebc5df7b1e0d0c883f85be2847e9171f16c16909c18914bb2f23ed69dd628` |
| `norrath_w13_spell_msg_a_view_7d7300` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-7d7300/norrath_w13_spell_msg_a_view_7d7300.cpp` | `w15_m_data_b_w13_spell_msg_a_view_7d7300.o` | `f48ff19f8c24d5b100c57b8a3c607587602abd08655e4cf56602d7f0970ece22` |
| `norrath_w13_spell_msg_a_view_8d6320` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d6320/norrath_w13_spell_msg_a_view_8d6320.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d6320.o` | `1283eecd357b6b4bc07030dec7e2f8bee2e25929ac4570d26586659f55ccf2f4` |
| `norrath_w13_spell_msg_a_view_8d6370` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d6370/norrath_w13_spell_msg_a_view_8d6370.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d6370.o` | `1ba3b7ced5e0a7987010836a28dde1685ab7230f113c2f0efa9ea940c18538d7` |
| `norrath_w13_spell_msg_a_view_8d67d0` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d67d0/norrath_w13_spell_msg_a_view_8d67d0.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d67d0.o` | `2ffa0cd78923246eb835f76b191d1fdf87e3652e3d869c09c163238421d256c9` |
| `norrath_w13_spell_msg_a_view_8d6820` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d6820/norrath_w13_spell_msg_a_view_8d6820.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d6820.o` | `d99dc6546228069d35f981ea855b9045bc3f1e277671489409c64e60f339c28b` |
| `norrath_w13_spell_msg_a_view_8d6870` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d6870/norrath_w13_spell_msg_a_view_8d6870.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d6870.o` | `06d2d1843db10b71c3b694fe3a69c35e412afd8431de0ed3cf2a2e44a00f2d42` |
| `norrath_w13_spell_msg_a_view_8d7b30` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d7b30/norrath_w13_spell_msg_a_view_8d7b30.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d7b30.o` | `5976a0bafa5768ceae5e0e5a3202c1c1dcf647a8194a884b60919302a21c0efb` |
| `norrath_w13_spell_msg_a_view_8d7b80` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d7b80/norrath_w13_spell_msg_a_view_8d7b80.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d7b80.o` | `d67e586bfd26445171f8158b6cc9bf0a88568d77d91981c4232b6a54b3f303bc` |
| `norrath_w13_spell_msg_a_view_8d7bd0` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d7bd0/norrath_w13_spell_msg_a_view_8d7bd0.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d7bd0.o` | `8058ec523530c7233c485a1b757fbecae29d425a508178b0e18f2c244964c165` |
| `norrath_w13_spell_msg_a_view_8d80a0` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d80a0/norrath_w13_spell_msg_a_view_8d80a0.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d80a0.o` | `c6bef9c0b6600f11045ae0211dfb9bcba42782f314064b91d326413fbb50b4a3` |
| `norrath_w13_spell_msg_a_view_8d80f0` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d80f0/norrath_w13_spell_msg_a_view_8d80f0.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d80f0.o` | `611f185a50c2360822accccfe992b0b4f7b02ad414b4bc28fef09b2905909999` |
| `norrath_w13_spell_msg_a_view_8d8140` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d8140/norrath_w13_spell_msg_a_view_8d8140.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d8140.o` | `4d3e1ae0df679d7e6d4c7d650dda971394eac2eef63cda4ecaf0ecea065bacc5` |
| `norrath_w13_spell_msg_a_view_8d8190` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d8190/norrath_w13_spell_msg_a_view_8d8190.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d8190.o` | `7d3812929fab79feda69ef7053313394ff1361f833bab52a81ba676b1480cec1` |
| `norrath_w13_spell_msg_a_view_8d81e0` | `experimental/sources/w15-m-data-b-w13-spell-msg-a-view-8d81e0/norrath_w13_spell_msg_a_view_8d81e0.cpp` | `w15_m_data_b_w13_spell_msg_a_view_8d81e0.o` | `ef5af61a7a9ddd7c6e4c484b0a41f33803527f4a51650e40ba8ad5f05045e929` |

## Authored but never executed

All 21 units. Nothing was linked into an image, run on an EE, or compared against the original. The units were compiled and
checked only as above.

## Unknown

- Every field value and every string named by the pointer tables.
- The values behind `sRainParticleDist`.
- The bound behind the `index < 4` guards in the summon-skeleton callers.
- Whether the views are 80 bytes; the figure is inferred from neighbours and an earlier note, not from a size in the image.
- Whether the game writes any of these storages after start-up.
- Which storage survives integration for the two duplicated addresses.
