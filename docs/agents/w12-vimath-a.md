# VI file I/O, matrix, vector, frustum and clip math (A)

Package `w12-vimath-a`, branch `agents/w12-vimath-a`. All 20 assigned imports are closed by 20 independently
authored units under `experimental/sources/w12-vimath-a-*`, one commit each (the commit title names the
function; `Normalize` has a second commit that only removes a system include the plan driver rejects).
Addresses, sizes and instruction order were read from the pinned private image `work/private/SLUS_205.65`
with the reviewed EE binutils; nothing from it is committed. Every source opens with an Observed / Inferred /
Unknown comment. Fresh compilations are not original-byte matches and no byte match is claimed.

## Functions

| source | symbol | image address, size | compiled bytes |
|---|---|---|---|
| `w12-vimath-a-camera-calc-frustum/calc_frustum.cpp` | `CalcFrustum__8VICameraRC10VIMatrix44T1` | `0x01086d08`, 1188 bytes | 276 |
| `w12-vimath-a-clipper-init/init.cpp` | `Init__9VIClipperi` | `0x0108c1b8`, 92 bytes | 72 |
| `w12-vimath-a-clipstack-gettop/gettop.cpp` | `GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane` | `0x01099678`, 92 bytes | 92 |
| `w12-vimath-a-clipstack-pop/pop.cpp` | `Pop__11VIClipStack` | `0x01099548`, 64 bytes | 64 |
| `w12-vimath-a-clipstack-pushfrustum/push_frustum.cpp` | `PushFrustum__11VIClipStackR8VICamera` | `0x01098568`, 452 bytes | 320 |
| `w12-vimath-a-csffile-decompress/decompress.cpp` | `Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm` | `0x010a4968`, 628 bytes | 700 |
| `w12-vimath-a-esfparse-parse/parse.cpp` | `Parse__10VIESFParseR9VIObjFileR7VIScene` | `0x0116d960`, 212 bytes | 232 |
| `w12-vimath-a-file-close/close.cpp` | `Close__6VIFile` | `0x010c9fc8`, 192 bytes | 268 |
| `w12-vimath-a-frustum-init/init.cpp` | `Init__9VIFrustum` | `0x010174b0`, 8 bytes | 8 |
| `w12-vimath-a-frustum-plane/plane.cpp` | `Plane__C9VIFrustumi` | `0x01017590`, 16 bytes | 16 |
| `w12-vimath-a-matrix33-identity/identity.cpp` | `Identity__10VIMatrix33` | `0x0108ae40`, 56 bytes | 52 |
| `w12-vimath-a-matrix44-invert/invert.cpp` | `Invert__10VIMatrix44RC10VIMatrix44` | `0x010d0558`, 584 bytes | 780 |
| `w12-vimath-a-matrix44-mul-matrix/mul_matrix.cpp` | `Mul__10VIMatrix44RC10VIMatrix44T1` | `0x010d0848`, 120 bytes | 116 |
| `w12-vimath-a-matrix44-mul-vect3/mul_vect3.cpp` | `Mul__C10VIMatrix44RC7VIVect3R7VIVect3` | `0x010d09a8`, 136 bytes | 136 |
| `w12-vimath-a-matrix44-mul-vect4/mul_vect4.cpp` | `Mul__C10VIMatrix44RC7VIVect4R7VIVect4` | `0x010d08c0`, 48 bytes | 44 |
| `w12-vimath-a-objfile-open/open.cpp` | `Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode` | `0x010a62e0`, 196 bytes | 192 |
| `w12-vimath-a-rect-intersects/intersects.cpp` | `Intersects__C6VIRectRC6VIRect` | `0x01114eb8`, 92 bytes | 88 |
| `w12-vimath-a-vect3-normalize/normalize.cpp` | `Normalize__7VIVect3` | `0x0104f2f0`, 112 bytes | 112 |
| `w12-vimath-a-world-find-leaf/find_leaf.cpp` | `FindLeaf__C7VIWorldRC7VIVect3RiT2` | `0x010fd248`, 300 bytes | 260 |
| `w12-vimath-a-zone-init/init.cpp` | `Init__6VIZone` | `0x01115318`, 172 bytes | 168 |

## How each function was checked

* Compiled with the reviewed EE g++ and the plan's `authored_cpp` flags; all 20 units and then the whole
  private plan copy built (see below).
* Each unit has a `host_contract.cpp` (not a plan input) that returns nonzero when a checked call order,
  argument, stored value or result is wrong. Build and run natively, for example
  `c++ -std=c++17 -ffp-contract=off -Wall -Wextra -Werror -o /tmp/chk experimental/sources/w12-vimath-a-<unit>/host_contract.cpp && /tmp/chk`.
  Objects store 32-bit target addresses, so contracts that follow pointer words use a small address arena
  (`NORRATH_HOST_ARENA`, the same convention as earlier packages). I used native arm64 rather than the
  `-arch x86_64` of older notes: x86_64 binaries hang in this sandbox (Rosetta).
* Each function was also run against the original in the private R5900 interpreter (outside git): identical
  arguments and memory snapshot, callees replaced by identical Python stubs that log their arguments, then
  comparing the call log, every changed byte outside the stack and (unless noted) the return value. Scenario
  counts: Init/Frustum 2, Plane 6, Normalize 9, Intersects 61, Pop 8, GetTop 8, PushFrustum 8, Identity 6,
  Clipper Init 12, Zone Init 10, CalcFrustum 3, Invert 31, Mul (vector 4) 9, Mul (vector 3) 9, Mul (matrix) 12,
  Parse 7, Open 45, Decompress 32, Close 56, World FindLeaf 200. Every function had at least one deliberate
  mutation of the authored source (a stored offset, comparison, constant, order of two operations or an
  omitted store) and the comparison failed each time. Exceptions listed under "Unknown".
* To run the matrix functions the private interpreter needed a model of VU0 macro mode (`lqc2`, `sqc2`,
  `qmtc2`, `vmulax`, `vmadday`, `vmaddaz`, `vmaddw`, `vmulx`, `vadd`, `vsub`, `vmr32`) and full 128-bit `lq`/`sq`;
  the existing copy zeroed the upper half of a quadword, which made a first `Invert` run disagree. Both
  models round each operation to nearest single precision; hardware VU0 rounding is not modelled.

## What the structures look like (observed unless marked)

* `VIClipStack`: count at +0, array of 16-byte entries at +8 (plane count at +0, plane start at +4, vector
  count at +8, vector start at +12), running totals at +12 (vectors) and +24 (planes), vector array pointer at
  +20 (12-byte records), plane array pointer at +32 (16-byte records). `PushFrustum` copies five planes
  (camera +416 to +495) and five vectors (camera +28, +352, +364, +376, +388) and returns the old count; `Pop`
  subtracts the entry's counts from the totals; `GetTop` returns the entry's counts and the two region
  addresses. Inferred: the camera frustum is at camera +304 (eight 12-byte corner vectors, then 16-byte
  planes from +96, which `Plane(i)` addresses).
* `VICamera::CalcFrustum` pushes the eight clip-cube corners (+-1, +-1, +-1, w = 1) through a supplied
  matrix with a perspective divide, transforms them with a second matrix, writes them to camera +304..+399
  and calls `CalcPlanes__9VIFrustum`.
* `VIMatrix44`: 16 floats as four 16-byte rows; multiplication treats vectors as rows (`v * M`).
  `Invert` is Gauss-Jordan with partial pivoting and an exact-zero pivot test (returns -1).
* `VIWorld` +52 holds a plane tree (24-byte nodes: plane n0 n1 n2 d, front child +16, back child +20, leaf
  flag 0x40000000, index mask 0x0fffffff), +64 a leaf-to-zone int table, +76 the zone objects (592 bytes each).
  The zone's own `FindLeaf` has the same walk (not authored here).
* `VICSFFile::Decompress` reads a 40-byte header (magic 0x46534543, count, offsets), seeks, then reads
  `count` chunks (8-byte header: compressed size, uncompressed size) and calls zlib `uncompress` into
  separately allocated 16-byte segment records.

## Measured effect (private plan copy, outside git)

Built with the shared plan plus these 20 units (`docs/agents/w12-vimath-a-plan-entries.py --private-root`):
5,133 source compilations, one link, no duplicate definitions, 3,929 function bodies, 827 unresolved imports.
Compared with the 827-name list of the shared branch (`priv-w12-hud-loop-a/b/base`), the 20 assigned names
are no longer unresolved and 20 new names appear, so the count is unchanged (827 to 827).

## Imports closed

`CalcFrustum__8VICameraRC10VIMatrix44T1`, `Close__6VIFile`, `Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm`, `FindLeaf__C7VIWorldRC7VIVect3RiT2`, `GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane`, `Identity__10VIMatrix33`, `Init__6VIZone`, `Init__9VIClipperi`, `Init__9VIFrustum`, `Intersects__C6VIRectRC6VIRect`, `Invert__10VIMatrix44RC10VIMatrix44`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`, `Mul__C10VIMatrix44RC7VIVect4R7VIVect4`, `Normalize__7VIVect3`, `Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode`, `Parse__10VIESFParseR9VIObjFileR7VIScene`, `Plane__C9VIFrustumi`, `Pop__11VIClipStack`, `PushFrustum__11VIClipStackR8VICamera`.

## Imports opened

`BufferFlush__6VIFile`, `CalcPlanes__9VIFrustum`, `Close__9VIObjFile`, `FileType__9VIObjFile`,
`FindLeaf__C6VIZoneRC7VIVect3`, `Init__10VIZoneRoomiiUiUiR6VIBBox`, `Init__10VIZoneTreeii`,
`Init__t7VIArray1Z10VIZoneRoomi`, `IsAsynchComplete__6VIFile`, `Mul__C10VIMatrix44RC7VIVect4R7VIVect3`,
`NumObjects__9VIObjFile`, `Open__6VIFileiPQ26VIFile7MemSegmil`, `ParseObject__10VIESFParse`,
`PostOpen__9VIObjFile`, `SwapRows__10VIMatrix44ii`, `VIBufferSwap__FiPUc`, `VIEndian` (data),
`Zero__10VIMatrix33`, `__ml__FRC7VIVect4RC10VIMatrix44`, `uncompress`. Also referenced but already satisfied
or imported by others: `Init__t7VIArray1Z7VIVect3i`, `_$_10VIMatrix44`, `Read__6VIFilePvi`,
`Seek__6VIFilelQ26VIFile10SeekOrigin`, `__builtin_vec_new`, `__builtin_vec_delete`, `printf`.
Several belong to the sibling packages (the VIZone, VIArray and VIObjFile families); I did not author them.

## Build-plan entries needed

Run `python3 docs/agents/w12-vimath-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]` to regenerate
them and to write a private plan/snapshot copy. The 20 entries (append in this order; `input_order`,
file digests and the `plan`/`snapshot` hashes are for the integrator to assign):

```json
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "CalcFrustum__8VICameraRC10VIMatrix44T1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_camera_calc_frustum.o", "profile": "authored_cpp", "sha256": "1b5dd5089e99c221a1c9455cabaeea506891a52a6b62b8f0681b5b6995ed7351", "source": "sources/w12-vimath-a-camera-calc-frustum/calc_frustum.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Init__9VIClipperi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_clipper_init.o", "profile": "authored_cpp", "sha256": "24fac72f62f8a85ae3468985d9b20d7adbd50ae095e08029f9e9a4214ce037df", "source": "sources/w12-vimath-a-clipper-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_clipstack_gettop.o", "profile": "authored_cpp", "sha256": "5d092ffb31c8d29ccfdef6ef3fd0e8c079f0b6ab851d84111004b197bbf2ce2b", "source": "sources/w12-vimath-a-clipstack-gettop/gettop.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "Pop__11VIClipStack"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_clipstack_pop.o", "profile": "authored_cpp", "sha256": "8d29d01ab7e1a01a94a35bbd941bf717b30dc59b259102276dd4cf6fe26bbbcb", "source": "sources/w12-vimath-a-clipstack-pop/pop.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "PushFrustum__11VIClipStackR8VICamera"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_clipstack_pushfrustum.o", "profile": "authored_cpp", "sha256": "47b4b66c516928669445979f73ec6dd5a9e24ee05953e9913d68f87f635265ed", "source": "sources/w12-vimath-a-clipstack-pushfrustum/push_frustum.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 700, "name": "Decompress__9VICSFFileR6VIFileRiPPQ26VIFile7MemSegm"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_csffile_decompress.o", "profile": "authored_cpp", "sha256": "45cc20585791eb4c7e8ebf5094ef2a6da0b083632cd3ecd3bff264afaed9d842", "source": "sources/w12-vimath-a-csffile-decompress/decompress.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 232, "name": "Parse__10VIESFParseR9VIObjFileR7VIScene"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_esfparse_parse.o", "profile": "authored_cpp", "sha256": "59df4b3e1348c5441691f5a52c978c56e5c7bab248c480a94a4380575535a7be", "source": "sources/w12-vimath-a-esfparse-parse/parse.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 268, "name": "Close__6VIFile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_file_close.o", "profile": "authored_cpp", "sha256": "be618c0a87c526f5799a969db24f8f6fd32eeafa5c48854276112e2d02b6fe60", "source": "sources/w12-vimath-a-file-close/close.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Init__9VIFrustum"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_frustum_init.o", "profile": "authored_cpp", "sha256": "4b0fa392cd2ec54f1ffa664d03a772710363a8b760be8c8da924fddfcf80c7b9", "source": "sources/w12-vimath-a-frustum-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "Plane__C9VIFrustumi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_frustum_plane.o", "profile": "authored_cpp", "sha256": "842c969b3da94a68623dc20363a839e4232caffed71dafbcfd308d1c97deb79f", "source": "sources/w12-vimath-a-frustum-plane/plane.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "Identity__10VIMatrix33"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_matrix33_identity.o", "profile": "authored_cpp", "sha256": "a81d51eb2ce3136ca4c90e5f7e88046d92b36cbb9e23fc73e1ac78777a340c70", "source": "sources/w12-vimath-a-matrix33-identity/identity.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 780, "name": "Invert__10VIMatrix44RC10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_matrix44_invert.o", "profile": "authored_cpp", "sha256": "68b78252057ce36f150883840743e1d8829120732977919d53509a1feb94a5ec", "source": "sources/w12-vimath-a-matrix44-invert/invert.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "Mul__10VIMatrix44RC10VIMatrix44T1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_matrix44_mul_matrix.o", "profile": "authored_cpp", "sha256": "661d1ada29434214f210dbb7f62bf150ddab32a1343bf3e882d92ca8968d729b", "source": "sources/w12-vimath-a-matrix44-mul-matrix/mul_matrix.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "Mul__C10VIMatrix44RC7VIVect3R7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_matrix44_mul_vect3.o", "profile": "authored_cpp", "sha256": "6e3b7341d67bb0ab56ba1b2c80d7be87c4b402ade8e7e6f690f2545439683036", "source": "sources/w12-vimath-a-matrix44-mul-vect3/mul_vect3.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "Mul__C10VIMatrix44RC7VIVect4R7VIVect4"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_matrix44_mul_vect4.o", "profile": "authored_cpp", "sha256": "d61fb667b30fdc88e851969ef2d2746528cdb6bcad35399aa5629cf6d473a752", "source": "sources/w12-vimath-a-matrix44-mul-vect4/mul_vect4.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_objfile_open.o", "profile": "authored_cpp", "sha256": "c6a23ccc6064a2bc4e8f610d7512a10e8ffabf49dbda91599da07f7a40b15345", "source": "sources/w12-vimath-a-objfile-open/open.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "Intersects__C6VIRectRC6VIRect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_rect_intersects.o", "profile": "authored_cpp", "sha256": "eeb0266826eb40f006ad3c104d6b384d78f1f7e1c53fdbea31dc7c5c3ecf7321", "source": "sources/w12-vimath-a-rect-intersects/intersects.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "Normalize__7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_vect3_normalize.o", "profile": "authored_cpp", "sha256": "23d1f1a5b680c1b2c661d419a0e13ad5ee05d9a2b3c14767082c54d3ccb790ff", "source": "sources/w12-vimath-a-vect3-normalize/normalize.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 260, "name": "FindLeaf__C7VIWorldRC7VIVect3RiT2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_world_find_leaf.o", "profile": "authored_cpp", "sha256": "3362983c702e7470270bd2adba871b92d0435804b34d1636e1999ec2b1dce7ac", "source": "sources/w12-vimath-a-world-find-leaf/find_leaf.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "Init__6VIZone"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vma_zone_init.o", "profile": "authored_cpp", "sha256": "4619985999e0723e46ef985a4d6b4f7162e22e803a7b5244ae6555bf2c6b889b", "source": "sources/w12-vimath-a-zone-init/init.cpp"}
```

## Authored but never executed

All 20. They have run only in the private interpreter against logging stubs and in native host contracts with
substituted callees. None has run on the EE or in gameplay. The matrix multiplies use inline VU0 macro-mode
assembly (`__mips__` only; the host build models the sums with plain floats), and the private VU0 model rounds
to nearest, so hardware rounding agreement is not shown. `Invert` and `CalcFrustum` rely on those multiplies and
on callees (`SwapRows`, the vector-times-matrix operator, `CalcPlanes`) that were stubbed.

## Unknown

* Member names of every class touched; which `VIObjFile::FileMode` value is read, write or append (only
  the mapping 0 to 33, 1 to 50, other to 42 is observed); the meaning of header words +8 and +16 of the CSF
  container and of the four frustum vectors at camera +352..+388; the types behind the plane-tree words.
* `Identity__10VIMatrix33` returns void here; the original leaves `Zero`'s incidental `v0` (this+12) behind.
* `Invert` row scaling and adds are ordinary float arithmetic, not VU0 instructions; last-bit equivalence with
  the VU0 is not established. `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`'s fourth result lane is discarded.
* `Decompress`: the 64-bit size store sign-extends the 32-bit uncompressed size and the `Seek` offset is the
  full 64-bit header field; negative sizes and offsets above 32 bits are not exercised by the paired run, so the
  sign-extension mutation went undetected. Buffers are leaked on failure (as observed).
* `Close__6VIFile` prints "VIEEFile: Close" although the symbol's class is `VIFile`; why is unknown.
* The original source's use of inline assembly versus library routines for the matrix operations.
