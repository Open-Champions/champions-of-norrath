# w12-vimath-b: VI file I/O, matrix and Quat helpers (B)

All 20 task-list names are authored, one function per commit, each compiled with the reviewed
EE `authored_cpp` flags. Branch `agents/w12-vimath-b`. Addresses below are provenance in the pinned
private image `SLUS_205.65`; they are not bindings in the authored link.

## How each function was checked

- **Paired run.** A private R5900 interpreter (outside git) loads the original image, compiles the
  authored unit with the reviewed flags, links it with every callee bound to the original's address, and
  runs the original and the authored function from identical memory snapshots with identical stub
  callees. It compares the return value, the full call log (arguments, and pointed-to contents where
  noted) and every changed byte outside the stack. Mutating one expression of the authored source made the
  run fail in every case listed under "Mutation checks", so the comparisons are not vacuous.
- **Host check.** Where the function has no 32-bit-pointer state, a `host_check.cpp` (not a build input)
  drives every path with recording stubs. They were built and run natively (arm64) because the x86_64
  binaries hang on this machine. `Remove__7VILNode` stores 32-bit addresses, so it has no host check;
  its paired run covers all four null/non-null combinations.
- No unit was run on the EE, in the full game, or in a full partial build. I compiled each unit alone (the
  brief asked for single-unit compiles) and linked all 20 objects with `ld -r` for the symbol lists below.
  `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).

## Functions

| source | symbol | original extent | compiled bytes | check |
|---|---|---|---|---|
| `w12-vimath-b-extract-rotation/extract_rotation.cpp` | `extractRotation__FP4Quat` | 0x0015fc08, 164 bytes | 164 | paired run, mutations |
| `w12-vimath-b-make-quaternion/make_quaternion.cpp` | `makeQuaternion__FP4Quatsfff` | 0x00159e00, 192 bytes | 168 | paired run, mutations |
| `w12-vimath-b-mat-mul33/mat_mul33.cpp` | `matMul__FP8Matrix33N20` | 0x00159150, 204 bytes | 172 | paired run, mutations |
| `w12-vimath-b-mat-mul34/mat_mul34.cpp` | `matMul__FP8Matrix34P8Matrix33T0` | 0x00156230, 280 bytes | 196 | paired run, mutations |
| `w12-vimath-b-mat-scale/mat_scale.cpp` | `matScale__FP8Matrix34fff` | 0x00159770, 112 bytes | 112 | paired run, mutations |
| `w12-vimath-b-mat-transpose33/mat_transpose33.cpp` | `matTranspose__FP8Matrix33T0` | 0x00159cd8, 168 bytes | 132 | paired run, mutations |
| `w12-vimath-b-quat-transform/quat_transform.cpp` | `quatTransform__FP6Point3P4QuatT0` | 0x00157140, 452 bytes | 224 | paired run, mutations |
| `w12-vimath-b-querytrilist-volume/volume.cpp` | `Volume__14VIQueryTriListRC6VIBBox` | 0x010d11d0, 32 bytes | 16 | host check, paired run, mutations |
| `w12-vimath-b-vifile-read/read.cpp` | `Read__6VIFilePvi` | 0x010cafa8, 124 bytes | 112 | host check, paired run, mutations |
| `w12-vimath-b-vifile-seek/seek.cpp` | `Seek__6VIFilelQ26VIFile10SeekOrigin` | 0x010caee8, 92 bytes | 72 | host check, paired run, mutations |
| `w12-vimath-b-vifile-tell/tell.cpp` | `Tell__6VIFile` | 0x010caf48, 8 bytes | 8 | host check, paired run (no mutation test: the body is one load) |
| `w12-vimath-b-vilnode-remove/remove.cpp` | `Remove__7VILNode` | 0x01072680, 56 bytes | 52 | paired run, mutations |
| `w12-vimath-b-vimatrix44-rotate-hpr/rotate_hpr.cpp` | `RotateHPR__10VIMatrix44RC7VIVect3` | 0x010996d8, 356 bytes | 296 | paired run, mutations |
| `w12-vimath-b-vimatrix44-rotate-x/rotate_x.cpp` | `RotateX__10VIMatrix44f` | 0x0109ad20, 100 bytes | 104 | host check, paired run, mutations |
| `w12-vimath-b-vimatrix44-rotate-y/rotate_y.cpp` | `RotateY__10VIMatrix44f` | 0x0109ad88, 100 bytes | 104 | host check, paired run, mutations |
| `w12-vimath-b-vimatrix44-rotate-z/rotate_z.cpp` | `RotateZ__10VIMatrix44f` | 0x0109adf0, 100 bytes | 104 | host check, paired run, mutations |
| `w12-vimath-b-vimatrix44-scale/scale.cpp` | `Scale__10VIMatrix44f` | 0x0109ac98, 60 bytes | 52 | host check, paired run, mutations |
| `w12-vimath-b-viobjfile-ctor/ctor.cpp` | `__9VIObjFile` | 0x010a60b0, 40 bytes | 32 | host check, paired run, mutations |
| `w12-vimath-b-viobjfile-dtor/dtor.cpp` | `_$_9VIObjFile` | 0x010a60d8, 108 bytes | 136 | host check, paired run, mutations |
| `w12-vimath-b-vq-decompress/vq_decompress.cpp` | `vqDecompress__FP8_texturei` | 0x001712f8, 4676 bytes | 4360 | paired run (61 scenarios), mutations |

Compiled sizes differ from the original extents; no byte match is claimed. No fused multiply-add is
emitted (`madd` count zero in every object).

## Evidence (observed / inferred / unknown)

**VIFile (`Tell`, `Read`, `Seek`).** Observed: the word at offset 0 selects the path (non-negative: the
buffered routine; negative with the word at 48 set: the memory routine; negative with the word at 56 set:
the segmented-memory routine; otherwise -1). `Read` returns 0 when the buffered read returned exactly the
requested count, else -1; the memory and segmented routines are tail calls whose result is returned
unchanged. `Tell` returns the 64-bit word at offset 16. Inferred only: the roles of offsets 0, 48 and 56.
Unknown: the object size and the callees' return types.

**VILNode `Remove`.** Observed: words A (offset 0) and B (offset 4); when both are nonzero B[0] = A, then
A[4] = B (A and B re-read between the stores), then offset 4 and offset 0 are cleared. Nothing is
returned (v0 holds a leftover; the comparison ignores it). Inferred: unlink of a doubly linked node.

**VIQueryTriList `Volume`.** Observed: tail call `StoreOutward__C6VIBBoxP7VIPlane(box, this + 44)`.

**VIObjFile constructor and destructor.** Observed: ctor runs `__6VIFile`, then tail calls
`ResetState__9VIObjFile`. dtor calls `Close__9VIObjFile` when the word at 560 is nonzero, then
`_$_6VIFile(this, 2)`, then deletes when bit 0 of the flags is set. Unknown: the object size, the meaning of
the word at 560, and whether the original constructor returned a value.

**VIMatrix44 `Scale`, `RotateX/Y/Z`, `RotateHPR`.** Observed: row-major 16-float layout from the stores.
`Scale` calls Identity then stores into floats 0, 5 and 10. `RotateX/Y/Z` call `sinf`, `cosf`, then
Identity, and store cosine into the two diagonal floats of the axis, sine and negated sine into the two
off-diagonal floats (X: 6 and 9; Y: 8 and 2; Z: 1 and 4). `RotateHPR` makes six trig calls (sin, cos of each
of the three angles in order), no Identity call, and stores all sixteen floats in the multiply and add
order given in its source comment. Inferred: radians (the arguments go straight to `sinf`/`cosf`); the
heading/pitch/roll reading of the vector comes from the symbol name only.

**Matrix33 / Matrix34 (`matMul`, `matTranspose`, `matScale`).** Observed: row-major 3x3 products summed
left to right through a stack temporary, so the destination may alias an operand; `matMul` with a Matrix34
destination also copies floats 9 to 11 of its **second source** into the result, so that object has at least
twelve floats although its mangled type is Matrix33; `matTranspose` is `t[3i+k] = src[i+3k]`; `matScale`
multiplies the three rows of the rotation part by x, y, z and leaves floats 9 to 11 alone. Unknown: the real
Matrix33 and Matrix34 definitions and which of the rows are axes.

**Quat (`makeQuaternion`, `extractRotation`, `quatTransform`).** Observed: `makeQuaternion` halves the
short angle with an arithmetic shift, normalises the axis with `rsqrt.s` (1.0 over the square root of the
squared length, summed x, y, z), calls `icos` then `isin`, stores w = icos result and xyz = isin result times
the normalised axis. `extractRotation` calls `iatan2(2(xy+zw), x*x-y*y-z*z+w*w)`, builds a quaternion with
`makeQuaternion(&r, (short)-a, 0, 0, 1.0f)`, calls `quatMul(q, &r, q)` and returns `a`. `quatTransform`
(arguments Point3*, Quat*, Point3* by the mangled name) computes q*P*conj(q) with the zero w term of P
dropped in the first product, writes only xyz, reads every input before the first store and returns the
destination. Inferred: the rotation-about-an-axis and point-rotation readings. Unknown: the angle unit
(that of `icos`/`isin`), the Point3 and Quat definitions, which game axis `extractRotation` removes.

**`vqDecompress__FP8_texturei`** (0x001712f8, 4676 bytes, a local symbol in the image).
Observed (the full step list is in the source header comment): claim 64 `uncompMap` entries from
`uncompSpin` with the same take/skip rule as the table decoder in `texDecompressThread__FPv`; build the GS
upload packet at the 0x70000000 scratchpad cursor (words and register numbers 80 to 83 are in the source);
swizzle 256 palette words into it; walk two code lists (a count word then pairs ending at a pair whose
second word is -1; both are handed to `processHuffman__FPUi`); decode N four-symbol codebook entries into
`huffmanTemp` with the 32-bit-window table decode; read rectangles (A, B, C, D) of
bits-for-block-count width from the stream at texture+16 plus 4; C < A ends the stream; each 16x16 block is
inline or a 24-bit reference resolved through `texturePageTable[extra]`; blocks are built from 2x2 groups,
permuted through `unscrambleTable`, written as eight 32-byte lines, and the scratchpad is flushed with
`putBlock` whenever it exceeds 4096 bytes. The original's multimedia compare/pack/leading-zero sequence
(`pextlw`, `pcgtw`, `ppach`, `ppacb`, `plzcw` over `gbits`) is written as plain C that returns the same
count (the group-of-four result is the number of leading `gbits` lanes at or below the probe). It also
starts with a read loop over the 64-byte slots below the texture whose result is not used; it is kept as a
volatile read loop. Inferred: this is the vector-quantised texture decoder (palette, codebook of 2x2 pixel
groups, indexed 16x16 blocks); the GS register meanings come from register numbers only. Unknown: the
stream producer and the texture structure beyond the fields named above.
Paired run: 61 scenarios (the commit message for this function says 74; the correct count is 61) (full and partial blocks at several sizes, empty and non-empty code lists, inline
and referenced blocks through segment and direct pages, large rectangles that force `putBlock` loops, an
exact 4096-byte end flush, claim busy/stale/wrapped/failed states, a timer register that returns varying
values so every `SleepThread` site runs, the locked and clearing flags, full-frame mode). The original and
authored runs matched on the return value, every `putBlock` and `SleepThread` call (including the flushed
scratchpad bytes), and all changed memory; 1159 of 1169 original words were executed (the other ten are
nine nops and one delay-slot load on a path that cannot be taken, because the reference reader starts
with a zero bit count and so always has to refill before skipping its bit offset). `putBlock` was a deterministic Python stand-in
that logs its arguments and the flushed bytes; `processHuffman__FPUi` ran as the original code. The
interpreter needed extending (128-bit registers, `lq`/`sq`, `pextlw`, `pcgtw`, `ppach`, `ppacb`, `plzcw`);
that extension lives in a private copy and nothing derived from the image is committed.

## Mutation checks

Small unit mutations (changing one operand, offset, constant or call order) were each rejected by the
paired run: Remove (link store offset), Read (result sense), Seek (offset 56 test), Volume (+44), VIObjFile
ctor (call order) and dtor (flag), Scale, RotateX/Y/Z (sign, constant, call order), RotateHPR (three
mutations), matMul33/34, matScale, matTranspose, makeQuaternion (two), extractRotation (two),
quatTransform (two). For `vqDecompress` 41 mutations (40 rejected, one equivalent survivor) covering the claim count and frame threshold, packet
constants, palette swizzle, list walking, the table decode (comparison, length, bit count), packet field
widths, rectangle logic, reference decoding (offset shift, page limit, segment shift), pair copy, unscramble
copy, timer limit, sleep calls, flush flags, texture word stores and wrap handling were each rejected except one:
the surviving mutant is equivalent in practice: changing the row-flush test from `>= 4097` to `>= 4098`
differs only when the cursor is exactly 4097 bytes past the base, and every byte count added to the
scratchpad cursor is a multiple of 16. After the first round, scenarios were added for the claim count and
threshold boundaries, equality with a table threshold, the segment shift, the cleared chain-end word and
wrapping at index 1279, and those mutants then failed the run as well.

## Imports closed

`Read__6VIFilePvi`, `Remove__7VILNode`, `RotateHPR__10VIMatrix44RC7VIVect3`, `RotateX__10VIMatrix44f`, `RotateY__10VIMatrix44f`, `RotateZ__10VIMatrix44f`, `Scale__10VIMatrix44f`, `Seek__6VIFilelQ26VIFile10SeekOrigin`, `Tell__6VIFile`, `Volume__14VIQueryTriListRC6VIBBox`, `_$_9VIObjFile`, `__9VIObjFile`, `extractRotation__FP4Quat`, `makeQuaternion__FP4Quatsfff`, `matMul__FP8Matrix33N20`, `matMul__FP8Matrix34P8Matrix33T0`, `matScale__FP8Matrix34fff`, `matTranspose__FP8Matrix33T0`, `quatTransform__FP6Point3P4QuatT0`, `vqDecompress__FP8_texturei`

## Imports opened

References from my 20 objects that none of them defines (from an `ld -r` link of only my objects; I did
not run a full partial build, so I cannot say which of these the shared build already lists):

`BufferedRead__6VIFilePvi`, `BufferedSeek__6VIFilelQ26VIFile10SeekOrigin`, `Close__9VIObjFile`, `Identity__10VIMatrix44`, `MemoryRead__6VIFilePvi`, `MemorySeek__6VIFilelQ26VIFile10SeekOrigin`, `MemorySegmRead__6VIFilePvi`, `MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin`, `ResetState__9VIObjFile`, `SleepThread`, `StoreOutward__C6VIBBoxP7VIPlane`, `_$_6VIFile`, `__6VIFile`, `__builtin_delete`, `clearing`, `cosf`, `curVqPalette`, `drawFrameNm`, `engineFullFrameMode`, `gbits`, `gsubtracts`, `huffmanTemp`, `iatan2__Fff`, `icos__Fi`, `isin__Fi`, `memset`, `processHuffman__FPUi`, `putBlock__FP8_textureP6PacketPPP9uncompMapPPii`, `quatMul__FP4QuatN20`, `sinf`, `texDecodeStarted`, `texturePageLoc`, `texturePageSegLoc`, `texturePageTable`, `textureUncompBufferLocked`, `texturesAreDecompressing`, `uncompCache`, `uncompMap`, `uncompSpin`, `unscrambleTable`, `world`

Of these, the shared build plan already emits a function for `Identity__10VIMatrix44`, `SleepThread`,
`__builtin_delete`, `iatan2__Fff`, `icos__Fi`, `isin__Fi`, `putBlock__FP8_textureP6PacketPPP9uncompMapPPii`
and `quatMul__FP4QuatN20`. A source search of the plan found data definitions for `clearing`,
`engineFullFrameMode`, `texDecodeStarted`, `texturePageLoc`, `texturePageTable`,
`textureUncompBufferLocked`, `texturesAreDecompressing`, `uncompCache`, `uncompMap` and `unscrambleTable`.
Not found by that search, and so likely open: the VIFile family (`BufferedRead__6VIFilePvi`,
`BufferedSeek__6VIFilelQ26VIFile10SeekOrigin`, `MemoryRead__6VIFilePvi`, `MemorySeek__6VIFilelQ26VIFile10SeekOrigin`,
`MemorySegmRead__6VIFilePvi`, `MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin`, `__6VIFile`, `_$_6VIFile`),
`Close__9VIObjFile`, `ResetState__9VIObjFile`, `StoreOutward__C6VIBBoxP7VIPlane`, `sinf`, `cosf`, `memset`,
`processHuffman__FPUi`, and the data `curVqPalette`, `drawFrameNm`, `gbits`, `gsubtracts`, `huffmanTemp`,
`texturePageSegLoc`, `uncompSpin`, `world`. Their layouts are not established by this package.

## Build-plan entries needed

Twenty inputs, profile `authored_cpp`, kind `independently_authored_source`, empty header closure;
`input_order` is left to the integrator. Body sizes are from the reviewed EE g++ build of exactly these
files. `host_check.cpp` files are not inputs. I did not edit the shared plan or snapshot.

```json
{"emitted_functions":[{"compiled_body_bytes":164,"name":"extractRotation__FP4Quat"}],"header_closure":[],"kind":"independently_authored_source","object_name":"extract_rotation.o","profile":"authored_cpp","sha256":"778f10d4ec9a2f7bdf9baedcba1cffa31188fdf73969929c3b356754449c9235","source":"sources/w12-vimath-b-extract-rotation/extract_rotation.cpp"}
{"emitted_functions":[{"compiled_body_bytes":168,"name":"makeQuaternion__FP4Quatsfff"}],"header_closure":[],"kind":"independently_authored_source","object_name":"make_quaternion.o","profile":"authored_cpp","sha256":"f40de23f144499ba6f068121cc7e72458766041fb66af161b5842fd5e4073676","source":"sources/w12-vimath-b-make-quaternion/make_quaternion.cpp"}
{"emitted_functions":[{"compiled_body_bytes":172,"name":"matMul__FP8Matrix33N20"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mat_mul33.o","profile":"authored_cpp","sha256":"794750d78b9f843f8a8b86a3a30d19fe76825d4a66393fc6810b5a96e2bc7ace","source":"sources/w12-vimath-b-mat-mul33/mat_mul33.cpp"}
{"emitted_functions":[{"compiled_body_bytes":196,"name":"matMul__FP8Matrix34P8Matrix33T0"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mat_mul34.o","profile":"authored_cpp","sha256":"f4fbc730d8cd996239f2a1ed640c11667f7038888bd0ef76e059c6665a1f79ed","source":"sources/w12-vimath-b-mat-mul34/mat_mul34.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"matScale__FP8Matrix34fff"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mat_scale.o","profile":"authored_cpp","sha256":"7b3e8300131a8a8e770ebf4d674db48b20407771834e00263b020404dad7d34c","source":"sources/w12-vimath-b-mat-scale/mat_scale.cpp"}
{"emitted_functions":[{"compiled_body_bytes":132,"name":"matTranspose__FP8Matrix33T0"}],"header_closure":[],"kind":"independently_authored_source","object_name":"mat_transpose33.o","profile":"authored_cpp","sha256":"a63e7de422a0f5be2bb7c9587347af6830bbc024acb8329d6d9269347da895aa","source":"sources/w12-vimath-b-mat-transpose33/mat_transpose33.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"quatTransform__FP6Point3P4QuatT0"}],"header_closure":[],"kind":"independently_authored_source","object_name":"quat_transform.o","profile":"authored_cpp","sha256":"f8ede585eaa2b2cdad5520fdb5b5841889b4fe86c8f4d3dc3bdacf53114491e5","source":"sources/w12-vimath-b-quat-transform/quat_transform.cpp"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"Volume__14VIQueryTriListRC6VIBBox"}],"header_closure":[],"kind":"independently_authored_source","object_name":"volume.o","profile":"authored_cpp","sha256":"baf35ad8eb730f8b9ba7eb4600836b58923f48795357eb606fa7321849e93f3a","source":"sources/w12-vimath-b-querytrilist-volume/volume.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"Read__6VIFilePvi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"read.o","profile":"authored_cpp","sha256":"4ced501e162ac793a01e04472006caedd82762ce9b6f4e622dc1ce65b1374a38","source":"sources/w12-vimath-b-vifile-read/read.cpp"}
{"emitted_functions":[{"compiled_body_bytes":72,"name":"Seek__6VIFilelQ26VIFile10SeekOrigin"}],"header_closure":[],"kind":"independently_authored_source","object_name":"seek.o","profile":"authored_cpp","sha256":"6303d2460cbf81f62d553e97d51d739188e8540b92f8f3a9e882dd133b3594ce","source":"sources/w12-vimath-b-vifile-seek/seek.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Tell__6VIFile"}],"header_closure":[],"kind":"independently_authored_source","object_name":"tell.o","profile":"authored_cpp","sha256":"19233c999ac09c9066f7b95f990644cab31e43f4828cbabdd58dda844c8752bf","source":"sources/w12-vimath-b-vifile-tell/tell.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"Remove__7VILNode"}],"header_closure":[],"kind":"independently_authored_source","object_name":"remove.o","profile":"authored_cpp","sha256":"ad5ed3517e6edb0089042a9b4d1d1e400d02cf99a32a899de6d8dc9bb0638d42","source":"sources/w12-vimath-b-vilnode-remove/remove.cpp"}
{"emitted_functions":[{"compiled_body_bytes":296,"name":"RotateHPR__10VIMatrix44RC7VIVect3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"rotate_hpr.o","profile":"authored_cpp","sha256":"52d26583893fe366359cb2f1f09d8a1fe33b57515d53054559c484e88547e643","source":"sources/w12-vimath-b-vimatrix44-rotate-hpr/rotate_hpr.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"RotateX__10VIMatrix44f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"rotate_x.o","profile":"authored_cpp","sha256":"c0d74a9bb6ac449271cf13fc17929bc7f5b864f5d33cf333b546d49f8a85e1a2","source":"sources/w12-vimath-b-vimatrix44-rotate-x/rotate_x.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"RotateY__10VIMatrix44f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"rotate_y.o","profile":"authored_cpp","sha256":"5730139963c977296a986e80e6f621883a9daaa65fd232fc92faa9225f6a4140","source":"sources/w12-vimath-b-vimatrix44-rotate-y/rotate_y.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"RotateZ__10VIMatrix44f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"rotate_z.o","profile":"authored_cpp","sha256":"8fdd1a8b9415b34b600372cd6f1d834d569fabc13276b3cbb95784c22180eadc","source":"sources/w12-vimath-b-vimatrix44-rotate-z/rotate_z.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"Scale__10VIMatrix44f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"scale.o","profile":"authored_cpp","sha256":"657ed6e797a0a814e598af398a566e8dc3e231c9ba41ee579202ffb9f7e49b54","source":"sources/w12-vimath-b-vimatrix44-scale/scale.cpp"}
{"emitted_functions":[{"compiled_body_bytes":32,"name":"__9VIObjFile"}],"header_closure":[],"kind":"independently_authored_source","object_name":"ctor.o","profile":"authored_cpp","sha256":"92bc44834ce7882ff2cd4a4176c1ba2f5af311a698d287dcf897de19f285e71b","source":"sources/w12-vimath-b-viobjfile-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":136,"name":"_$_9VIObjFile"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"41af91b80c2006a37de65dba55b8180f562d6deb0c720244e920c5173eb5ed71","source":"sources/w12-vimath-b-viobjfile-dtor/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":4360,"name":"vqDecompress__FP8_texturei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"vq_decompress.o","profile":"authored_cpp","sha256":"d757b98ff9e2843479c15602bc5ace52136aec6a1e3cf822bea646439bdea0f5","source":"sources/w12-vimath-b-vq-decompress/vq_decompress.cpp"}
```

The plan's `files` map needs these path-to-hash pairs:

```json
{
 "sources/w12-vimath-b-extract-rotation/extract_rotation.cpp": "778f10d4ec9a2f7bdf9baedcba1cffa31188fdf73969929c3b356754449c9235",
 "sources/w12-vimath-b-make-quaternion/make_quaternion.cpp": "f40de23f144499ba6f068121cc7e72458766041fb66af161b5842fd5e4073676",
 "sources/w12-vimath-b-mat-mul33/mat_mul33.cpp": "794750d78b9f843f8a8b86a3a30d19fe76825d4a66393fc6810b5a96e2bc7ace",
 "sources/w12-vimath-b-mat-mul34/mat_mul34.cpp": "f4fbc730d8cd996239f2a1ed640c11667f7038888bd0ef76e059c6665a1f79ed",
 "sources/w12-vimath-b-mat-scale/mat_scale.cpp": "7b3e8300131a8a8e770ebf4d674db48b20407771834e00263b020404dad7d34c",
 "sources/w12-vimath-b-mat-transpose33/mat_transpose33.cpp": "a63e7de422a0f5be2bb7c9587347af6830bbc024acb8329d6d9269347da895aa",
 "sources/w12-vimath-b-quat-transform/quat_transform.cpp": "f8ede585eaa2b2cdad5520fdb5b5841889b4fe86c8f4d3dc3bdacf53114491e5",
 "sources/w12-vimath-b-querytrilist-volume/volume.cpp": "baf35ad8eb730f8b9ba7eb4600836b58923f48795357eb606fa7321849e93f3a",
 "sources/w12-vimath-b-vifile-read/read.cpp": "4ced501e162ac793a01e04472006caedd82762ce9b6f4e622dc1ce65b1374a38",
 "sources/w12-vimath-b-vifile-seek/seek.cpp": "6303d2460cbf81f62d553e97d51d739188e8540b92f8f3a9e882dd133b3594ce",
 "sources/w12-vimath-b-vifile-tell/tell.cpp": "19233c999ac09c9066f7b95f990644cab31e43f4828cbabdd58dda844c8752bf",
 "sources/w12-vimath-b-vilnode-remove/remove.cpp": "ad5ed3517e6edb0089042a9b4d1d1e400d02cf99a32a899de6d8dc9bb0638d42",
 "sources/w12-vimath-b-vimatrix44-rotate-hpr/rotate_hpr.cpp": "52d26583893fe366359cb2f1f09d8a1fe33b57515d53054559c484e88547e643",
 "sources/w12-vimath-b-vimatrix44-rotate-x/rotate_x.cpp": "c0d74a9bb6ac449271cf13fc17929bc7f5b864f5d33cf333b546d49f8a85e1a2",
 "sources/w12-vimath-b-vimatrix44-rotate-y/rotate_y.cpp": "5730139963c977296a986e80e6f621883a9daaa65fd232fc92faa9225f6a4140",
 "sources/w12-vimath-b-vimatrix44-rotate-z/rotate_z.cpp": "8fdd1a8b9415b34b600372cd6f1d834d569fabc13276b3cbb95784c22180eadc",
 "sources/w12-vimath-b-vimatrix44-scale/scale.cpp": "657ed6e797a0a814e598af398a566e8dc3e231c9ba41ee579202ffb9f7e49b54",
 "sources/w12-vimath-b-viobjfile-ctor/ctor.cpp": "92bc44834ce7882ff2cd4a4176c1ba2f5af311a698d287dcf897de19f285e71b",
 "sources/w12-vimath-b-viobjfile-dtor/dtor.cpp": "41af91b80c2006a37de65dba55b8180f562d6deb0c720244e920c5173eb5ed71",
 "sources/w12-vimath-b-vq-decompress/vq_decompress.cpp": "d757b98ff9e2843479c15602bc5ace52136aec6a1e3cf822bea646439bdea0f5"
}
```

## Authored but never executed

Everything, on the EE: nothing here ran on hardware or in the game. The paired private-interpreter runs
and the host checks above are the only execution. The timer register, the scratchpad and the DMA state of
`vqDecompress` exist only as plain memory in the interpreter; `putBlock` was replaced by a stand-in for
the comparison. The multimedia sequence inside `vqDecompress` was replaced by plain C and compared with
the original's own instructions in the interpreter, whose MMI emulation I wrote myself (it agrees with the
original on all 61 scenarios, which is also the only evidence for it).

## Unknown

The Matrix33/Matrix34/Quat/Point3 type definitions, the angle units, field meanings of the VIFile object,
the callees' return types (`Seek`, `Read` pass them through as 32-bit values), whether the original
constructors and `Remove` return anything, what the dead read loop at the start of `vqDecompress` is for,
the stream producer for the VQ format, and the GS register semantics beyond their numbers.

## Summary

- Imports closed: all 20 names (`Read__6VIFilePvi`, `Remove__7VILNode`, `RotateHPR__10VIMatrix44RC7VIVect3`, `RotateX__10VIMatrix44f`, ...).
- Imports opened: 41 references listed above; none defined by me.
- Build-plan entries: 20 entries and 20 file hashes, above.
- Never executed on hardware: everything; interpreter comparisons only.
- Blocked on the owner: nothing.
