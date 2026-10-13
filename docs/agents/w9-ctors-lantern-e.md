# w9-ctors-lantern-e: ant-lantern constructor view data

Branch `agents/w9-ctors-lantern-e`. Private reference: `work/private/SLUS_205.65`
(in the main checkout), SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
No game bytes are committed; each value below was read from the file's `.lit4` section.

## What was authored

Forty four-byte single-float constants, one source per symbol, in
`experimental/sources/w9-ctors-lantern-e-{ghoul,froglock,cyclops,ant,innoruuk}-views/`.
Each is `extern "C" const float norrath_ctors_ant_lantern_view_<addr>` with the observed
bit pattern. Host checks (`w9-ctors-lantern-e-host-checks/`, not EE inputs) fail unless the
compiled definition's bit pattern equals the observed one.

Check: `docs/agents/w9-ctors-lantern-e-check.py <SLUS_205.65> <ee-toolchain> <outdir> <addr>...`
compiles each source with the reviewed EE g++ `authored_cpp` flags, requires a 4-byte symbol,
compares its bytes with the executable's bytes at the address, and runs the host check. All 40 passed.

Commits (one per constructor group): Ghoul 2, Froglock 6, Cyclops 8, Ant 9, Innoruuk 15.
Grouping follows which authored constructor reads the address (observed in
`ctors-ant-lantern-constructors`); a group is one commit rather than one per constant.

## Observed / inferred / unknown

- Observed: address, size 4, section `.lit4`, bit pattern; no ELF symbol sits at these addresses.
- Inferred: the reading constructor, from the authored callers' use.
- Unknown: what each value means (timing, distance, probability); names are address-derived only.

## Imports closed

All 40: norrath_ctors_ant_lantern_view_ 480e6c, 480e70, 480f4c, 480f50, 480f54, 480f58, 480f5c,
480f60, 480f64, 480f68, 480f6c, 480f70, 480f74, 480f78, 480f7c, 480f80, 480f84, 48109c, 4810a0,
4810a4, 4810a8, 4810ac, 4810b0, 481160, 481164, 481168, 48116c, 481170, 481174, 481178, 48117c,
48124c, 481250, 481254, 481258, 48125c, 481260, 481264, 481268, 48126c.

## Imports opened

None (the sources have no includes or references).

## Build-plan entries needed

Append to `experimental/build-plan.json` `inputs` with fresh `input_order` values, and add the
40 source hashes to `files`. The `sha256` below is of the source file; `header_closure` is empty.
No `private_object_sha256` is claimed. Do not add the host-check files. Not yet built by the
partial build driver, only compiled singly (per the lead's request to avoid full builds).

```
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_48124c.cpp","object_name":"norrath_ctors_ant_lantern_view_48124c.o","sha256":"9b25d33b31b5df044a8280d4fc8ced36c81cd509e6f14802962e7e705d09da28","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_481250.cpp","object_name":"norrath_ctors_ant_lantern_view_481250.o","sha256":"2fc3ef79c4709c13ac4b9f176b232f6f05edb6deb9686611c4d14b0a5d285f31","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_481254.cpp","object_name":"norrath_ctors_ant_lantern_view_481254.o","sha256":"6ea72099c709812bc45a1c5aa5837959f141d1a45930e41c0847649a69b4e2d4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_481258.cpp","object_name":"norrath_ctors_ant_lantern_view_481258.o","sha256":"7f104491c64cbed92f404e95b5604c8a981a9549bf147b327c8c0d99a60f4c2a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_48125c.cpp","object_name":"norrath_ctors_ant_lantern_view_48125c.o","sha256":"26bda43a30c4ba84aa42a1b8be8f1a7518c07e03e204982283d25c223ab84fa6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_481260.cpp","object_name":"norrath_ctors_ant_lantern_view_481260.o","sha256":"7843623a0555c079ae9091f29aa7f3a8d03df87238343ac3a739baa077059700","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_481264.cpp","object_name":"norrath_ctors_ant_lantern_view_481264.o","sha256":"11412081f0ee8d9633e4af3d414ee4bff93e340d6e09a2b3f9774ee0b70a6a4f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_481268.cpp","object_name":"norrath_ctors_ant_lantern_view_481268.o","sha256":"09bbaeebffc7f7e269fe8f4d3c1825a1324c9a2f8e986d8337f63a6b9a1f25e7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ant-views/norrath_ctors_ant_lantern_view_48126c.cpp","object_name":"norrath_ctors_ant_lantern_view_48126c.o","sha256":"96276fd246dc8f68d325a17a4c15b793f9f6b50f6709d4a8987cd1f615f3bbf4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_481160.cpp","object_name":"norrath_ctors_ant_lantern_view_481160.o","sha256":"f1c089b98ab0e841d3c2dc3375bb6f69fae96877aa779c51962e39fe97b15f88","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_481164.cpp","object_name":"norrath_ctors_ant_lantern_view_481164.o","sha256":"bd3aa2d1cede66c5665e1f98600ddb7f1e06ee41b448f01d3369ef531efec958","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_481168.cpp","object_name":"norrath_ctors_ant_lantern_view_481168.o","sha256":"1f56882c2af2d2cd5d1615526b7a03430ec6fd7b15d385837448304137ce02e3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_48116c.cpp","object_name":"norrath_ctors_ant_lantern_view_48116c.o","sha256":"b3747bf65f9d0315d646b4d4b89bbd75694d6b3b3030df168a61fe91afd8ced0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_481170.cpp","object_name":"norrath_ctors_ant_lantern_view_481170.o","sha256":"1a5e87deeb2bf714d6b813b5be7af55ff6a6aef05001cd7d4e6df4487be8354a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_481174.cpp","object_name":"norrath_ctors_ant_lantern_view_481174.o","sha256":"1b54588c25ae76a173476ed2b36d1b60bc659d59d95270b2a8a3e504ecbaa8f3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_481178.cpp","object_name":"norrath_ctors_ant_lantern_view_481178.o","sha256":"ddd3855a3aecfbfc36d86a5a27486bc8f9dfb6eea6f0786e494fecbe929e5e87","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-cyclops-views/norrath_ctors_ant_lantern_view_48117c.cpp","object_name":"norrath_ctors_ant_lantern_view_48117c.o","sha256":"5738f6514bc6035ceab144aecc59737e06cd23dc6776100fc8918f2ea992f5c3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-froglock-views/norrath_ctors_ant_lantern_view_48109c.cpp","object_name":"norrath_ctors_ant_lantern_view_48109c.o","sha256":"b99ee7c8f2e2f192973ed25f3a4b28adb4648aa390fb5becc20a20c0f7e26ebb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-froglock-views/norrath_ctors_ant_lantern_view_4810a0.cpp","object_name":"norrath_ctors_ant_lantern_view_4810a0.o","sha256":"d5ca20f1293c18115a43e59075457a08184d14ff838564d16e8cb52d22d33a55","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-froglock-views/norrath_ctors_ant_lantern_view_4810a4.cpp","object_name":"norrath_ctors_ant_lantern_view_4810a4.o","sha256":"758098bf4b4172f323cd843fa74abe5c46e09f6e827df3e17eee8b26b246289e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-froglock-views/norrath_ctors_ant_lantern_view_4810a8.cpp","object_name":"norrath_ctors_ant_lantern_view_4810a8.o","sha256":"e890cf45ed3ce56c5750be68094a4170073da041622e9fedc04c5e5ff729f4cd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-froglock-views/norrath_ctors_ant_lantern_view_4810ac.cpp","object_name":"norrath_ctors_ant_lantern_view_4810ac.o","sha256":"567a557db7616286e56e83115cb9856e023a0331b7934dcc5689877291bcb8ff","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-froglock-views/norrath_ctors_ant_lantern_view_4810b0.cpp","object_name":"norrath_ctors_ant_lantern_view_4810b0.o","sha256":"7a6b8d1182c49c4d5151cf832a48840de80f5d2252beb75ddfae1f285c8b0dfe","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ghoul-views/norrath_ctors_ant_lantern_view_480e6c.cpp","object_name":"norrath_ctors_ant_lantern_view_480e6c.o","sha256":"3b0a996c8482eaa28f0d5bd7f194ad514a9e7ded0c13cc3ff2ab6dc93cfb2c03","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-ghoul-views/norrath_ctors_ant_lantern_view_480e70.cpp","object_name":"norrath_ctors_ant_lantern_view_480e70.o","sha256":"a70667c77f0e0215448457f9651d0a03b0cecd8b5a747547876a707020f2e3c7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f4c.cpp","object_name":"norrath_ctors_ant_lantern_view_480f4c.o","sha256":"fc4f54f4c2ac0239d54307e2124a143117af9fce2fc471aae8fdad87cb4b4466","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f50.cpp","object_name":"norrath_ctors_ant_lantern_view_480f50.o","sha256":"1a2d06db44619f92cd2b413b58af62a7d5eb2b0ae72dbe774ef7644555900771","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f54.cpp","object_name":"norrath_ctors_ant_lantern_view_480f54.o","sha256":"2c7c782dba0c9e42c34f7f2d3bc178af7b2f4990b2bf1c3648676db67a6d981d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f58.cpp","object_name":"norrath_ctors_ant_lantern_view_480f58.o","sha256":"b2ef7b4c1ebdea15f4dfedd68862415c56f094fe765cfc084b27efb7fc1b0dbf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f5c.cpp","object_name":"norrath_ctors_ant_lantern_view_480f5c.o","sha256":"950a4d450bda2ad853e58f82c6267750b7ac869d388038cc43c69e90c77d8b0e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f60.cpp","object_name":"norrath_ctors_ant_lantern_view_480f60.o","sha256":"7608664043a304ae78f460cb669149ec138dbce9d321791372d442b2f5fc8570","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f64.cpp","object_name":"norrath_ctors_ant_lantern_view_480f64.o","sha256":"b33040b7c5f4c1c88cc4658b009e46e52443e96382eac938b6746d9bbd5c1ab2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f68.cpp","object_name":"norrath_ctors_ant_lantern_view_480f68.o","sha256":"695a4f2de648e8bda83be55c74a9ed26b7d874d33541b3ce1379ab72aa2151c6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f6c.cpp","object_name":"norrath_ctors_ant_lantern_view_480f6c.o","sha256":"81ce809b85a4fca2749b7376f461c7e8b846753640d9a9839de1d4b98ffa6e78","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f70.cpp","object_name":"norrath_ctors_ant_lantern_view_480f70.o","sha256":"acaa45c527165af963b6afb19b13b2f0caae7d8c28ec970c593bfe2dd9735972","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f74.cpp","object_name":"norrath_ctors_ant_lantern_view_480f74.o","sha256":"67c75e7c9f2cc2fd4e3cc8be7d6772b6d1b2da286a869683a201b868883c0d44","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f78.cpp","object_name":"norrath_ctors_ant_lantern_view_480f78.o","sha256":"6c6a0e5707355dd5e7c3b2144acbcf0711975b8e9295b20af7ac2a79b14e113e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f7c.cpp","object_name":"norrath_ctors_ant_lantern_view_480f7c.o","sha256":"e9c68a8769efebbcc9cf6b41e04c8b4840d282cb2f7600a65de5e4736f755639","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f80.cpp","object_name":"norrath_ctors_ant_lantern_view_480f80.o","sha256":"9a02c157dbcec0db6ffaa9e6d271e65262ca898f3823ded460050aad276952b4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w9-ctors-lantern-e-innoruuk-views/norrath_ctors_ant_lantern_view_480f84.cpp","object_name":"norrath_ctors_ant_lantern_view_480f84.o","sha256":"53f31306a85803256f6f0dddd53ea50e631238878f9aaaa1993302596ba5baed","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
```

## Authored but never executed

All of it. The objects were compiled for the EE and compared byte-for-byte with the executable
data, but nothing ran on the EE. Host checks ran on this Mac only.

## Unknown

Meaning of every constant; whether other code reads these addresses (the package only
saw the authored constructors). `python3 -m unittest discover -s tools
-p 'test_*.py'` passed (214 tests) at the end.
