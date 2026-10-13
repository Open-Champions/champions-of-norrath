# Ant-lantern constructor view data (c)

Package `w9-ctors-lantern-c`, branch `agents/w9-ctors-lantern-c`. Closes 41 data imports that
the authored constructors in `ctors-ant-lantern-constructors` read. Sources are under
`experimental/sources/w9-ctors-lantern-c-views/`, one file per symbol, each with a host
comparison under `host/` (not an EE input). One commit per symbol.

Private reference: `work/private/SLUS_205.65`. Addresses are that file's virtual addresses.
No game bytes beyond the short tag/format strings and one 8-byte constant are reproduced.

## Evidence

- Observed: every view is read-only text or constant data in the image's loaded data.
  Strings sit in 8- or 16-byte slots with zero padding. An object here is the characters plus the NUL
  (tables and the double are the exact observed extent).
- Observed: each object was compiled with the reviewed `authored_cpp` EE profile; the
  emitted section bytes were compared with the image bytes at the address (outside git) and matched,
  and each host test passed. The symbol is a global read-only object.
- Observed: `norrath_ctors_ant_lantern_view_46fd38` is mis-named. `AutoChest` forms its argument with
  `lui 0x47` and `addiu 0x0d38`, giving `0x470d38`, not `0x46fd38`. `0x46fd38` holds the tail of an unrelated
  string. The symbol keeps the import name the callers use and holds the text found at `0x470d38`
  ("deathweb"). I cross-checked every other view name used by the 33 constructors against the
  addresses each original constructor builds; no other name in this package disagreed.
- Observed: `norrath_ctors_ant_lantern_view_471030` is the first of 18 consecutive 16-byte text slots
  (0x471030..0x47114f); AutoContainer indexes it at 16 bytes per entry, so it is one 288-byte object.
- Observed: `norrath_ctors_ant_lantern_view_476ac8` is the 8-byte pattern `0x3fd3333333333333`; the
  Cthulu constructor loads it as two words.
- Inferred: that value is the double nearest 0.3. The original literal text is unknown.
- Unknown: the meaning of the tags beyond their use in the constructors, and the load addresses
  and placement of these objects in a linked game (definitions here are not placed at original addresses).

## Imports closed

All 41 names in the package scope: 46fa38 46fa48 46fa58 46fd38 46ffe0 470df8 470e08 470e18 470e28 470e38
470e48 470e58 470e68 470e78 470e88 470e98 470ea8 470eb8 470ff0 471000 471010 471020 471030 471150 4711f0
471208 471218 471230 471248 471260 471278 471ed0 471ee0 471ef0 471f00 471f10 471f20 476ac8 476b70 476b80
(each prefixed `norrath_ctors_ant_lantern_view_`).

## Imports opened

None. The sources reference no other symbol.

## Not mine

`norrath_ctors_ant_lantern_view_476b90` (the string JumpGate selects against 476b80) and the other
`norrath_ctors_ant_lantern_view_*` names are outside the list I was given and were not defined.

## Build-plan entries needed

Not added to the shared plan. Each unit needs an `inputs` entry with `kind: independently_authored_source`,
`profile: authored_cpp`, `header_closure: []`, `emitted_functions: []` (data only), `object_name: <name>.o`,
`source: sources/w9-ctors-lantern-c-views/<name>.cpp`, the sha256 below (also in `files`), and an
`input_order` from the integrator. Size is the emitted object size in bytes.

| Unit | size | sha256 |
|---|---|---|
| `norrath_ctors_ant_lantern_view_46fa38` | 14 | `eecc7a19767eb6dd89bb165f0ffdec026e2df57d25cfc03c116eb3c7a5a4fbb3` |
| `norrath_ctors_ant_lantern_view_46fa48` | 9 | `81f2ba2f150811e78508cf32f45a4cbbecc3fbaba82ecefe54377f7a43f17c8d` |
| `norrath_ctors_ant_lantern_view_46fa58` | 11 | `543b5aa304dd23feb99d031f8427fac7abcbb1516a9aed54662eec0be413ed36` |
| `norrath_ctors_ant_lantern_view_46fd38` | 9 | `8dc802db24149e25c3b51be644c16a441af06f831052072208f9ad4bc2a48380` |
| `norrath_ctors_ant_lantern_view_46ffe0` | 14 | `222581e5ede9e27c8d42aa74397e51df61c6770a45e445a122e6cd7da76203aa` |
| `norrath_ctors_ant_lantern_view_470df8` | 10 | `37184deb63a192d2b215bc68d2bcb897818dc478d19bbbc9c0033159c835bbc8` |
| `norrath_ctors_ant_lantern_view_470e08` | 9 | `e4c3a9c8c81c4995b5915959627bf50b40981b2bd3019adb413d7c2d3734f1bf` |
| `norrath_ctors_ant_lantern_view_470e18` | 10 | `4154b50cd19a7086f27026767309ae4fe16389330f280ac0704392d7feb2d0b0` |
| `norrath_ctors_ant_lantern_view_470e28` | 12 | `790b12d894524a7df179b9bc25468365640f05791f00c146e1d78501040c266e` |
| `norrath_ctors_ant_lantern_view_470e38` | 13 | `15356c42bb64acbd17617f65fdb1b636d505662685a7f64432b0f93c54e6b4e0` |
| `norrath_ctors_ant_lantern_view_470e48` | 13 | `22f68dc387de2c2ea4b70c8cf2f9074cad81a76583c282a2d62dd43f3b5924d4` |
| `norrath_ctors_ant_lantern_view_470e58` | 9 | `1c2097cfc2d7e69d6d7622066f1bbae4b528c3cbcdc2c7ecd979cec0176e3afa` |
| `norrath_ctors_ant_lantern_view_470e68` | 9 | `d915d9f90ecf8866f0216a530d5c06be2ba8806781bf13ba8c14dd4907620c7e` |
| `norrath_ctors_ant_lantern_view_470e78` | 9 | `a3c677706a8f7b2ef67f3887ae90b4546aa73c0e7a6d1c4157d2bdfe44e18d4f` |
| `norrath_ctors_ant_lantern_view_470e88` | 12 | `4fa76ab08f520f4d1641a40bc7961663b8d3d92675364abd8dd92c0540716957` |
| `norrath_ctors_ant_lantern_view_470e98` | 11 | `2b8922eb91b454708b0de71890eef7ded6450daa3ccc91e943eae0503b0287e8` |
| `norrath_ctors_ant_lantern_view_470ea8` | 11 | `c12d8db9db0eee7a1d98a8c0bc28b571c95eb34099cec0aaa85108ed8e87d9cf` |
| `norrath_ctors_ant_lantern_view_470eb8` | 12 | `cf9485a783762b658747462e511bd29e802b6760a715101415a7881e5cd6aba5` |
| `norrath_ctors_ant_lantern_view_470ff0` | 13 | `902231a3ddd28b5dc99ad8eaef6c3b61cd70b4ade35b2efe7f04facaef5af1f5` |
| `norrath_ctors_ant_lantern_view_471000` | 13 | `d2c170cfff96a878c3273bba037125e903fa5111d61da7df1879fefa0e2772b8` |
| `norrath_ctors_ant_lantern_view_471010` | 12 | `4790b65a55b311e3d21bd0158d26336332524684e7d76c2d4431988bdf5878ae` |
| `norrath_ctors_ant_lantern_view_471020` | 10 | `06392dfc440a1e2a110428d6d82166886a53c3671777f70db23991293eb4f1d4` |
| `norrath_ctors_ant_lantern_view_471030` | 288 | `bdb2b3e45d39f1736af9732f020d6ceb413d65e3fae952e58d354372fb352d34` |
| `norrath_ctors_ant_lantern_view_471150` | 12 | `92fa8803d1bc5469730ef3ccbace4e87b8d9b880530f5611a7618b5448eb9f4b` |
| `norrath_ctors_ant_lantern_view_4711f0` | 20 | `0aafe0f5954ee90a1a5f1ba3cca5ace2844bf16d064841cd2e86d1d0316e1f2c` |
| `norrath_ctors_ant_lantern_view_471208` | 11 | `3ee436ef2b3c127d2ae425219f714f64eafd71f364433db882ac9b488f321103` |
| `norrath_ctors_ant_lantern_view_471218` | 20 | `f00a3bba3f5054c55de4e189326e497fb5a7a7d9cc78c58f042c9dd3e4824957` |
| `norrath_ctors_ant_lantern_view_471230` | 21 | `799f85569895861d2748d8b39321eb95b90f41663faef6b507d951306f128f59` |
| `norrath_ctors_ant_lantern_view_471248` | 17 | `fa2af7a60d54461948efd4a5a07203a139f771e0fcedfc42e02c1ffccddc92ee` |
| `norrath_ctors_ant_lantern_view_471260` | 18 | `f129be7d5267fb15363d3429df13375c22f17d0ac31f91c8d9703499632d6fe2` |
| `norrath_ctors_ant_lantern_view_471278` | 9 | `f3196926a09c0e2cc97200ed736bb42213ae94a28fb5728ccbd9fc691b47d9e4` |
| `norrath_ctors_ant_lantern_view_471ed0` | 14 | `2cb7a946e6fd6b69ddb606fab0abdc922c1a131f78a5e621b08a83155b3c8faa` |
| `norrath_ctors_ant_lantern_view_471ee0` | 14 | `4dc8448f31c3f7f6c061eb19c7296087dc4338cc12c7bc41e0226ac71ee23426` |
| `norrath_ctors_ant_lantern_view_471ef0` | 14 | `d6231fb79223b466396e101c91045422b5014379d2b550f4847d3d33545e7013` |
| `norrath_ctors_ant_lantern_view_471f00` | 13 | `534eb5c0811eb89d698e9f79adb9061541e2b1abda6cc3d156f0084470ca1a97` |
| `norrath_ctors_ant_lantern_view_471f10` | 13 | `5e75252dc41e9eff579931b6f5fdee860b659ede8ac06caadc3392c67e443472` |
| `norrath_ctors_ant_lantern_view_471f20` | 13 | `a78a7ef61e6c0454b64ee6d5edb3f4f4ff1bac41639f535cf139278306546011` |
| `norrath_ctors_ant_lantern_view_476ac8` | 8 | `274204f558b579a5800fdf2fe2f8616762d666ff836c9e7a5e78abd139aa4278` |
| `norrath_ctors_ant_lantern_view_476b70` | 9 | `abcd83b5f7c9d29b929261dd9c712c59222dbff873435a7690e6f94323dec3ff` |
| `norrath_ctors_ant_lantern_view_476b80` | 9 | `1d1444baf545b571a955d48599954bb149a8dcb2140c2c5d613d4fc585c39e7a` |

## Authored but never executed

Nothing here has run on the EE or in a game. Only the host comparisons ran, plus the byte comparison of the
compiled section against the private image. The consumers (the constructors) are likewise unrun.

## Unknown

- Whether the build-plan schema accepts a data-only unit with an empty `emitted_functions` list; earlier view
  packages are in the same position and the integrator decides.
- The meaning of the 18 names in the 471030 table beyond their use as AutoContainer comparison names.
