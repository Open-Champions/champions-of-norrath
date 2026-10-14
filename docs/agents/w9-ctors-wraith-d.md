# Lava-wraith constructor and view data (d)

Package `w9-ctors-wraith-d`, branch `agents/w9-ctors-wraith-d`. It defines the 36 data
views that the authored `ctors-lava-wraith-*` constructors import. Sources are under
`experimental/sources/w9-ctors-wraith-d-views/`, one per commit, with a host check for
each under `host/` (host `main` programs, not plan inputs).

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that
file's. No game bytes or listings are committed beyond the short tag/format strings
the views must contain.

## Checks performed

- Each source compiled with the reviewed EE g++ and the `authored_cpp` flags.
- Each host check compiled with the host C++ compiler and returned 0.
- Private (not committed) comparison: for each string and word view, the compiled
  object's initialized bytes were compared with the private image bytes at the address,
  over the sized extent. All 34 initialized views matched. The two zero-initialized
  views were checked for object size only.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).
- No full partial build was run, so the effect on the absolute unresolved count is not
  measured.

## Evidence

- Observed: 32 views are NUL-terminated C strings in `.rodata` (`4775xx`, `4778xx`) or
  `.sdata`. The view is the characters plus the NUL; padding after is not part of it.
- Observed: `483f88` is ELF symbol `resourceFrame`, 4 bytes, stored word 1. `4852a8` is
  `_6Wraith$s_wraithCount`, 4 bytes, stored word 0.
- Observed: `601030` is ELF `range`, `601fd0` is `splashInfoBits`, both `.bss` with
  size field 0. The next symbols start 4000 and 504 bytes later, so those are the
  extents; WaveMaker clears exactly 504 bytes at `601fd0`.
- Inferred: `601030` is a per-sample flag byte array and `601fd0` a bit set indexed
  by sample count (from the WaveMaker caller only).
- Unknown: the meaning of the flag values; which other code reads these objects; the
  highest index used in `range`.

## Imports closed

- `norrath_ctors_lava_wraith_view_477568`
- `norrath_ctors_lava_wraith_view_477580`
- `norrath_ctors_lava_wraith_view_477598`
- `norrath_ctors_lava_wraith_view_4775b0`
- `norrath_ctors_lava_wraith_view_4775c8`
- `norrath_ctors_lava_wraith_view_4775d8`
- `norrath_ctors_lava_wraith_view_4778c8`
- `norrath_ctors_lava_wraith_view_4778d8`
- `norrath_ctors_lava_wraith_view_483e28`
- `norrath_ctors_lava_wraith_view_483f88`
- `norrath_ctors_lava_wraith_view_484fa0`
- `norrath_ctors_lava_wraith_view_484fa8`
- `norrath_ctors_lava_wraith_view_485240`
- `norrath_ctors_lava_wraith_view_4852a8`
- `norrath_ctors_lava_wraith_view_485330`
- `norrath_ctors_lava_wraith_view_4853b8`
- `norrath_ctors_lava_wraith_view_4853d0`
- `norrath_ctors_lava_wraith_view_4853d8`
- `norrath_ctors_lava_wraith_view_4857e8`
- `norrath_ctors_lava_wraith_view_4857f0`
- `norrath_ctors_lava_wraith_view_4857f8`
- `norrath_ctors_lava_wraith_view_485800`
- `norrath_ctors_lava_wraith_view_485808`
- `norrath_ctors_lava_wraith_view_485810`
- `norrath_ctors_lava_wraith_view_485818`
- `norrath_ctors_lava_wraith_view_485820`
- `norrath_ctors_lava_wraith_view_485828`
- `norrath_ctors_lava_wraith_view_4858b8`
- `norrath_ctors_lava_wraith_view_4858c0`
- `norrath_ctors_lava_wraith_view_4858c8`
- `norrath_ctors_lava_wraith_view_486118`
- `norrath_ctors_lava_wraith_view_486120`
- `norrath_ctors_lava_wraith_view_486128`
- `norrath_ctors_lava_wraith_view_486130`
- `norrath_ctors_lava_wraith_view_601030`
- `norrath_ctors_lava_wraith_view_601fd0`

## Imports opened

None. The sources have no imports.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values. Do not add anything under
`w9-ctors-wraith-d-views/host`. `profile` is `authored_cpp`, `header_closure` is empty,
no `private_object_sha256` is claimed, and the units emit no functions.
`default_authored_inputs` increases by 36.

```json
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_477568.cpp","object_name":"norrath_ctors_lava_wraith_view_477568.o","sha256":"fab207a09d42ee0e54939507a0a347aa8dc05f5f32750072d08c2e4d11b021b6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_477580.cpp","object_name":"norrath_ctors_lava_wraith_view_477580.o","sha256":"d418115a5a5b840a77724c6e22a1b99ca40aaff897165ff0445dcba17c11204d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_477598.cpp","object_name":"norrath_ctors_lava_wraith_view_477598.o","sha256":"db3a377eca6c71289fa329b4d6aed87e9d1b11d947f7015166234fefb30fdd82","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4775b0.cpp","object_name":"norrath_ctors_lava_wraith_view_4775b0.o","sha256":"e430ddc612585c451c4852a06050526e41c910f903801041e6f6ad0b695b6dbe","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4775c8.cpp","object_name":"norrath_ctors_lava_wraith_view_4775c8.o","sha256":"2b34f69c21ad09a0bf9589d92071b48c6c777c0cffdf06a8fe3410e923289850","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4775d8.cpp","object_name":"norrath_ctors_lava_wraith_view_4775d8.o","sha256":"804357853bfab6dc70acb081a9dfb62bf29ab83e27a8f54047a169164a1ff893","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4778c8.cpp","object_name":"norrath_ctors_lava_wraith_view_4778c8.o","sha256":"0d07ab5f1fff7279e8d28d72e123171c4ceac23ba677328486677e5e40c1a3c9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4778d8.cpp","object_name":"norrath_ctors_lava_wraith_view_4778d8.o","sha256":"6e3b3f272b9e9cf91ca876343117ff8985323f67b92ef3dbcb7da8bbdf1700c8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_483e28.cpp","object_name":"norrath_ctors_lava_wraith_view_483e28.o","sha256":"6d9c6ecba37cda2490278a1c77d10c66150e8d9c7b6b9531664eaf0365b47a5b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_483f88.cpp","object_name":"norrath_ctors_lava_wraith_view_483f88.o","sha256":"654f368a5c886f57d41acaccd01ec03c758619694d5304de489b8f14f6e52007","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_484fa0.cpp","object_name":"norrath_ctors_lava_wraith_view_484fa0.o","sha256":"0f16f0000200cff62210730f881b439351d03ee0e6125883a5efb8dc4462153f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_484fa8.cpp","object_name":"norrath_ctors_lava_wraith_view_484fa8.o","sha256":"3998ede16feb458535f92fae5d00033d4b52d1eb6f206cb16289b10bae020d54","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485240.cpp","object_name":"norrath_ctors_lava_wraith_view_485240.o","sha256":"253b74820d567feba9fbcce54c8c645e39da3d7b5822b53184e51fb173873bfa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4852a8.cpp","object_name":"norrath_ctors_lava_wraith_view_4852a8.o","sha256":"11434fd547ee48f6c16e6dd7c20acb472c623d7830dbeb3406e41a83f4c8167b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485330.cpp","object_name":"norrath_ctors_lava_wraith_view_485330.o","sha256":"71c7946b4d1f548d6da69179d485b85c5680c0f2b11cc34d7fd404561a0cd3f8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4853b8.cpp","object_name":"norrath_ctors_lava_wraith_view_4853b8.o","sha256":"35f74fc8649a1f87eee6fbbf8fbb4ec8689d286caae08c30d11e738be12e6873","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4853d0.cpp","object_name":"norrath_ctors_lava_wraith_view_4853d0.o","sha256":"c0f00a018de630845b920c081b4a9ee0814f1103230e387261877d02c9941520","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4853d8.cpp","object_name":"norrath_ctors_lava_wraith_view_4853d8.o","sha256":"b78fadc6d2bd08936f1f9d4172a7d8574fa25b74f3ac6e5a1eb0dc5504a8c489","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4857e8.cpp","object_name":"norrath_ctors_lava_wraith_view_4857e8.o","sha256":"4c381ef1574c6ce294b69c5e55175fbc64ef330d67d645e035d752e659a07285","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4857f0.cpp","object_name":"norrath_ctors_lava_wraith_view_4857f0.o","sha256":"cd2982e1a81d670d20ab29e5d7409f22d4c47445cbaa3ecd73d13d5467543763","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4857f8.cpp","object_name":"norrath_ctors_lava_wraith_view_4857f8.o","sha256":"294cd41c3d2439ef368353e00eb0cd9835a2d6c0b5c24ec0cca76ac77df61132","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485800.cpp","object_name":"norrath_ctors_lava_wraith_view_485800.o","sha256":"4663e4a7c24c315b57f79cf8d257f5af9687f64bfbfcb2d7f72088495d65d5b8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485808.cpp","object_name":"norrath_ctors_lava_wraith_view_485808.o","sha256":"1268133ef4067e8ba382b8ddc22b6b649dcf4829135fe359b14643afc0a7c2c4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485810.cpp","object_name":"norrath_ctors_lava_wraith_view_485810.o","sha256":"35dd40221685667cb476eee54dfa443d74331cf68c4568602cc1a0cdb2287c3c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485818.cpp","object_name":"norrath_ctors_lava_wraith_view_485818.o","sha256":"a425442900e78f0cd40a19991be1b0f268b1b51c35f6cddf15dffa08c615c626","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485820.cpp","object_name":"norrath_ctors_lava_wraith_view_485820.o","sha256":"197b26ea4fb72f11666a36d9c680401d0275ae109cfa08c07c3d17c3fa74f186","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_485828.cpp","object_name":"norrath_ctors_lava_wraith_view_485828.o","sha256":"e1edbdaf6651fc15b5d3a57363582a1b3d3d73f382bdcdaa3511dbd037c13fa8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4858b8.cpp","object_name":"norrath_ctors_lava_wraith_view_4858b8.o","sha256":"7ac15106f2ebcbea4d367dbcea0745359da00c675353c593270fd7f7c73f9068","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4858c0.cpp","object_name":"norrath_ctors_lava_wraith_view_4858c0.o","sha256":"1becbd32ccc2872fa1e7abe668ee37ec9440831f36bd8e0c2ed2a643b26cd7e0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_4858c8.cpp","object_name":"norrath_ctors_lava_wraith_view_4858c8.o","sha256":"de0b757d61397e6deed2a180470d033b0d6b8271ad4a8c9bab87c6720d19da99","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_486118.cpp","object_name":"norrath_ctors_lava_wraith_view_486118.o","sha256":"51baccd6ddab2a268b219a1feb39cb456ce45aa4efc475b7c630369c21121f3f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_486120.cpp","object_name":"norrath_ctors_lava_wraith_view_486120.o","sha256":"a927a1fd9e4086fb1461398f05930c28d937cd4617c750d0aae24449c215787d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_486128.cpp","object_name":"norrath_ctors_lava_wraith_view_486128.o","sha256":"0a25460cab6c5afcade73558ac35fef75448e11450d784795286ce81583b015f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_486130.cpp","object_name":"norrath_ctors_lava_wraith_view_486130.o","sha256":"f193e903813e90acea6a08966b938c6d5b0de2faf143703990a7a17b979736ad","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_601030.cpp","object_name":"norrath_ctors_lava_wraith_view_601030.o","sha256":"ca0895c2a66b6b2332f4deab06ee6a31741ec42ef3104f27266e3b5d45c7c3ff","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-wraith-d-views/norrath_ctors_lava_wraith_view_601fd0.cpp","object_name":"norrath_ctors_lava_wraith_view_601fd0.o","sha256":"14ccfe78cf4c894127b76f7fd6afd247a49d45a0963d83673596a87b12d5b871","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
```

## Authored but never executed

All of it. The views have been compiled and compared with the image bytes, but nothing
ran on the EE or as gameplay, and no linked build was produced for this package.

## Unknown

- Whether the original placed the objects at the observed addresses; definitions here
  do not.
- The role of `range` flag values and the use of `splashInfoBits` beyond WaveMaker.
- `norrath_ctors_ant_lantern_view_483f88` (another package) names the same address as
  `norrath_ctors_lava_wraith_view_483f88`; linking both would define `resourceFrame`
  twice as separate objects. The integrator should pick one definition.
