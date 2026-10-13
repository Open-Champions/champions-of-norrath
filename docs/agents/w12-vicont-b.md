# VI container templates (B)

Package `agents/w12-vicont-b`. Addresses and sizes come from the private hash-pinned `SLUS_205.65` symbol table. Nothing here ran on the EE or as gameplay. Compiled sizes are those of the reviewed EE g++ `authored_cpp` objects and are not a claim of byte equality with the image (the originals are 12 to 116 bytes; the authored dtors compile to 148).

## Evidence

Each function is one commit. Every function was compared with the original in a private R5900 interpreter (outside git): the original instructions and the freshly linked authored object run from identical memory snapshots, with identical scripted callees, and the callee call logs (argument registers included), every changed byte outside the stack, and the return value (not for void functions) must agree. The inlined walks (`DestroyPool` for the sprite and material-palette pools, the four arrays and the scene display vector) write nothing, so the comparison also records the sequence of data reads and the highest per-instruction execution count; both must agree. Scenarios: every branch of each original (empty/non-empty storage, callee results negative/zero/positive, a callee that clears the storage word, flag values 0..3 and -1/-2, chains of length 0 to 3 for the pools, counts 0,1,2,3,7,100 for the arrays). Zero mismatches, no halted run. Deliberate mutations (wrong stored word, wrong flag bit, wrong end marker, off-by-one walk length, wrong chain field) made the comparison fail, some of them by a runaway walk that hit the interpreter step limit. The script is `docs/agents/w12-vicont-b-compare.py`; it needs the private harness and image and cannot run from a clean checkout.

No host check source is committed. Host (x86_64) test programs hang on this machine (a trivial `int main(){return 0;}` built with `-arch x86_64` never exits and cannot be killed), and the sources treat stored words as 32-bit addresses, so an arm64 host build cannot exercise them. The R5900 comparison is the only executed check.

## Observed

- Pool `DestroyPool` for `VIRFont`, `VIRefMap`, `VISurface`, `VIStaticLighting`: calls the pool's `Begin`, then loops until the cursor equals `End` (re-called each round), stepping with `Next(container, cursor)`; returns 0.
- Pool `DestroyPool` for `VISprite` and `VIMaterialPal`: the same walk inline. Cursor = word at +16 (or the end marker when -1); entries are 12 bytes at the block in word +8; next index is the word at +4 of the entry, -1 mapping to the marker `0x7fffffff`. Returns 0, writes nothing.
- Array/vector `DestroyPool` (`VIClipEntry`, `VIPlane`, `VIVect3`, `int`, `VISceneDisplayElem`): count at +0; zero returns 0; else a cursor walks 0..count-1 and ends at the marker. Returns 0, reads and writes no storage.
- `End__Ct6VIPool1ZP12VICollBuffer` returns `0x7fffffff`.
- `Init` for five arrays: `Clear`; negative returns -1; store the count at +0; `Allocate`; negative returns -1 else 0.
- Destructors: storage word (+8 pool, +4 array) nonzero -> `DestroyPool(this)`, re-read, if still nonzero `__builtin_vec_delete`; then flag bit 0 -> tail call `__builtin_delete(this)`.
- Constructors: arrays zero +4 then +0; pools store 0,-1(+20),0(+4),0(+8),-1(+12),-1(+16).

## Inferred

Field names (count, entry block, first index, next index) come from how the words are used and only appear in comments. Template arguments come only from the mangled names. That the empty walks are what remains of a per-element teardown is a guess from their shape. The inline loops use an empty `asm volatile` to keep them; the original keeps them too, but whether the author wrote something else (a visible per-entry call that was optimized out) is unknown.

## Imports closed

- `__t7VIArray1Z13VIWorldRegion`
- `__t7VIArray1Z6VIZone`
- `__t6VIPool1ZP20VIParticleDefinition`
- `__t6VIPool1ZP13VISpellEffect`
- `DestroyPool__t7VIArray1Z11VIClipEntry`
- `DestroyPool__t7VIArray1Zi`
- `DestroyPool__t7VIArray1Z7VIPlane`
- `DestroyPool__t7VIArray1Z7VIVect3`
- `DestroyPool__t6VIPool1ZP13VIMaterialPal`
- `DestroyPool__t6VIPool1ZP8VIRefMap`
- `DestroyPool__t6VIPool1ZP7VIRFont`
- `DestroyPool__t6VIPool1ZP8VISprite`
- `DestroyPool__t6VIPool1ZP16VIStaticLighting`
- `DestroyPool__t6VIPool1ZP9VISurface`
- `DestroyPool__t8VIVector1Z18VISceneDisplayElem`
- `_$_t7VIArray1Z17VIRadialFloraInst`
- `_$_t7VIArray1Z13VIWorldRegion`
- `_$_t7VIArray1Z6VIZone`
- `_$_t6VIPool1ZP20VIParticleDefinition`
- `_$_t6VIPool1ZP13VISpellEffect`
- `End__Ct6VIPool1ZP12VICollBuffer`
- `Init__t7VIArray1Z11VIClipEntryi`
- `Init__t7VIArray1Z7VIPlanei`
- `Init__t7VIArray1Z7VIVect3i`
- `Init__t7VIArray1Z13VIWorldRegioni`
- `Init__t7VIArray1Z6VIZonei`

## Imports opened

Callees the new sources need (from `nm -u` of every compiled unit), not defined by this package. Names marked with a source already have a definition in another package's directory by a text search; the integrator should settle that against the combined build.

- `Allocate__t7VIArray1Z11VIClipEntry`
- `Allocate__t7VIArray1Z13VIWorldRegion`
- `Allocate__t7VIArray1Z6VIZone`
- `Allocate__t7VIArray1Z7VIPlane`
- `Allocate__t7VIArray1Z7VIVect3`
- `Begin__Ct6VIPool1ZP16VIStaticLighting` (a definition exists in `w11-tmpl-a-pool-staticlighting-begin`)
- `Begin__Ct6VIPool1ZP7VIRFont`
- `Begin__Ct6VIPool1ZP8VIRefMap` (a definition exists in `w11-tmpl-a-pool-refmap-begin`)
- `Begin__Ct6VIPool1ZP9VISurface`
- `Clear__t7VIArray1Z11VIClipEntry`
- `Clear__t7VIArray1Z13VIWorldRegion`
- `Clear__t7VIArray1Z6VIZone`
- `Clear__t7VIArray1Z7VIPlane`
- `Clear__t7VIArray1Z7VIVect3`
- `DestroyPool__t6VIPool1ZP13VISpellEffect`
- `DestroyPool__t6VIPool1ZP20VIParticleDefinition`
- `DestroyPool__t7VIArray1Z13VIWorldRegion`
- `DestroyPool__t7VIArray1Z17VIRadialFloraInst`
- `DestroyPool__t7VIArray1Z6VIZone`
- `End__Ct6VIPool1ZP16VIStaticLighting` (a definition exists in `w11-tmpl-a-pool-staticlighting-end`)
- `End__Ct6VIPool1ZP7VIRFont`
- `End__Ct6VIPool1ZP8VIRefMap` (a definition exists in `w11-tmpl-a-pool-refmap-end`)
- `End__Ct6VIPool1ZP9VISurface`
- `Next__Ct6VIPool1ZP16VIStaticLightingi` (a definition exists in `w11-tmpl-b-next-p16vistaticlighting`)
- `Next__Ct6VIPool1ZP7VIRFonti`
- `Next__Ct6VIPool1ZP8VIRefMapi` (a definition exists in `w11-tmpl-b-next-p8virefmap`)
- `Next__Ct6VIPool1ZP9VISurfacei`
- `__builtin_delete` (a definition exists in `w8-runtime-libc-a-builtin-delete`)
- `__builtin_vec_delete` (a definition exists in `w11-crt-a-builtin-vec-delete`)

## Build-plan entries needed

Profile `authored_cpp`, no headers; no `host_check.cpp` exists. The integrator assigns `input_order`. 26 entries:

```json
{"source":"sources/w12-vicont-b-ctor-array-worldregion/ctor.cpp","object_name":"ctor.o","sha256":"513fa50a85f3bbbfce477c963f5305924d0ccc4fe8bb210633cc33ab0bb4d3aa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__t7VIArray1Z13VIWorldRegion","compiled_body_bytes":12}]}
{"source":"sources/w12-vicont-b-ctor-array-zone/ctor.cpp","object_name":"ctor.o","sha256":"2d77398f65d773ffdf9ac82e4ae8f6c9623e7c46ad86d67adfa906c0cad2f9b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__t7VIArray1Z6VIZone","compiled_body_bytes":12}]}
{"source":"sources/w12-vicont-b-ctor-pool-particledefinition/ctor.cpp","object_name":"ctor.o","sha256":"95a4afe657c39dc9bd1adc51d742700ddfdb8fd1ed9a2409c1edec1880805b00","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__t6VIPool1ZP20VIParticleDefinition","compiled_body_bytes":32}]}
{"source":"sources/w12-vicont-b-ctor-pool-spelleffect/ctor.cpp","object_name":"ctor.o","sha256":"9ca0a63c1c218eea9a30942a7fdf0b366eaa11fe22933236e703fcdf305e36fe","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__t6VIPool1ZP13VISpellEffect","compiled_body_bytes":32}]}
{"source":"sources/w12-vicont-b-destroy-array-clipentry/destroy.cpp","object_name":"destroy.o","sha256":"474c136718fceaaa56d4fcde50f5c6437547fa9570958321373799de8ecf1cbb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t7VIArray1Z11VIClipEntry","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-destroy-array-int/destroy.cpp","object_name":"destroy.o","sha256":"95dc4373ac5525a50be1692620e339c1880eea89231a73099bcc377f986c16f9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t7VIArray1Zi","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-destroy-array-plane/destroy.cpp","object_name":"destroy.o","sha256":"6847d5f1b63a66242aa0f32117a473e0ce959d16d7f3c103c095cdefe6ad65fa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t7VIArray1Z7VIPlane","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-destroy-array-vect3/destroy.cpp","object_name":"destroy.o","sha256":"d89516bb9272f24f7fd692f0180cb40e1acf0673f1a973a158e55f2fe50e5b39","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t7VIArray1Z7VIVect3","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-destroy-pool-materialpal/destroy.cpp","object_name":"destroy.o","sha256":"f9458597f747a23035a5da778eddcfdf7ea43208ee51c1cfc3fc43223e087037","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP13VIMaterialPal","compiled_body_bytes":96}]}
{"source":"sources/w12-vicont-b-destroy-pool-refmap/destroy.cpp","object_name":"destroy.o","sha256":"8f0e56c4c341438ab367fa3e4f9712126b4325e4c9c7ae07a15b600cb48f320d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP8VIRefMap","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-b-destroy-pool-rfont/destroy.cpp","object_name":"destroy.o","sha256":"b6fbae34aad34433b9dca8398813cffdcb22f12d16475f597013a68f161f37d8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP7VIRFont","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-b-destroy-pool-sprite/destroy.cpp","object_name":"destroy.o","sha256":"2cfd57d36d960fc6d11150ab95002d6e32eea6a7efc86b9b251c6f7e84b0ae40","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP8VISprite","compiled_body_bytes":96}]}
{"source":"sources/w12-vicont-b-destroy-pool-staticlighting/destroy.cpp","object_name":"destroy.o","sha256":"77595e52a80b0e251075f2fdb768831eb4558cee699300126e12567705a9e189","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP16VIStaticLighting","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-b-destroy-pool-surface/destroy.cpp","object_name":"destroy.o","sha256":"7e8509e5b03913632296c8548953e74c9f344ce1e2d9789d0652815ed8a4a72f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP9VISurface","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-b-destroy-vector-scenedisplayelem/destroy.cpp","object_name":"destroy.o","sha256":"7342caf5a2a0068c48f9ef076cd441dc0724fd1ad081f6c009bf932f4767a7e5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t8VIVector1Z18VISceneDisplayElem","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-dtor-array-radialflorainst/dtor.cpp","object_name":"dtor.o","sha256":"2a95d5683e72fc5fd0b79f2721157e486bd3d6c0f90bf53d0235318bd15d502d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_t7VIArray1Z17VIRadialFloraInst","compiled_body_bytes":148}]}
{"source":"sources/w12-vicont-b-dtor-array-worldregion/dtor.cpp","object_name":"dtor.o","sha256":"2e58b868811c47f321550ffd74be3608a03bc0413872de2e3aa356ea2ee3f4bb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_t7VIArray1Z13VIWorldRegion","compiled_body_bytes":148}]}
{"source":"sources/w12-vicont-b-dtor-array-zone/dtor.cpp","object_name":"dtor.o","sha256":"3ccb98ccae0cd710ec70957c9ae3c5c7d29d530c951dd61769b8e8a2a59cc2b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_t7VIArray1Z6VIZone","compiled_body_bytes":148}]}
{"source":"sources/w12-vicont-b-dtor-pool-particledefinition/dtor.cpp","object_name":"dtor.o","sha256":"dcb2341c2fcf0d199eb2040945f4472123828b391c3043f72c8a8373713e551f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_t6VIPool1ZP20VIParticleDefinition","compiled_body_bytes":148}]}
{"source":"sources/w12-vicont-b-dtor-pool-spelleffect/dtor.cpp","object_name":"dtor.o","sha256":"cb6a5a5d6952bcfc62f627c0e784e77a80f7d00a6ab57e1c54d6f8696ec674d2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_t6VIPool1ZP13VISpellEffect","compiled_body_bytes":148}]}
{"source":"sources/w12-vicont-b-end-pool-collbuffer/end.cpp","object_name":"end.o","sha256":"4c6ff0c080eb3b98c0678421a27f68552640b1451a9dd812b8283b7436d63d1b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"End__Ct6VIPool1ZP12VICollBuffer","compiled_body_bytes":12}]}
{"source":"sources/w12-vicont-b-init-array-clipentry/init.cpp","object_name":"init.o","sha256":"cb44b4582b14e4d79684fa7807d53ac2637d475ae89c62d5422e3cf2517d235c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t7VIArray1Z11VIClipEntryi","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-init-array-plane/init.cpp","object_name":"init.o","sha256":"4719e5c9517b14067d526e0c43a27cc4e108faddd62e557ab0404b8839555596","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t7VIArray1Z7VIPlanei","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-init-array-vect3/init.cpp","object_name":"init.o","sha256":"2c99b6053e7207f565446638521088e0b11103be982649c784dd1d55d47ce0a3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t7VIArray1Z7VIVect3i","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-init-array-worldregion/init.cpp","object_name":"init.o","sha256":"24d5e564300a5c11d8e9f65a6fc793d8918f8c2a2ebaef9231436f52c3e2f17f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t7VIArray1Z13VIWorldRegioni","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-b-init-array-zone/init.cpp","object_name":"init.o","sha256":"2eb8f680c3bae74fdd9e3c6dfdaac3d151d817239987dc6092e3b182b2364018","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t7VIArray1Z6VIZonei","compiled_body_bytes":72}]}
```

## Authored but never executed

All of it on real hardware or in the game. Everything ran only inside the private interpreter with scripted callees. The walks in the seven `DestroyPool` functions with no callee were run against synthetic chains; real pool contents were never involved.

## Unknown

Whether any caller reaches a negative array count (the original walk would run about 2^31 steps; not tested). What the empty walks replaced in the original source. The element sizes of the arrays other than the 12-byte pool entries. Compiled sizes differ from the originals, so no byte match is claimed.
