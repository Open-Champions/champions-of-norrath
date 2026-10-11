"""Narrow binding for three reviewed compiler-generated memory initializations.

These hashes identify authored objects, never original game code. A successor
consumer or provider needs review before it can use this binding.
"""

PROVIDER = 'norrath_block_memory_set'
PROVIDER_HASH = '894c177c8f6f1a17de1a00bde471d1ba4f5b39d1f2587adbaaa0fcc8f4490e23'
CONSUMER_HASHES = {
    '0d46c79a4b7e65fe03e12e78cefe14d321d9e579cb38b1cdc75d68906f641a31',
    'd1ce1dcc0c245cae8097df52ef6b7f319aceb13a384d94f2e08f49cb72fa4808',
    'c8abb8644fe718da0207231dfc133e5e02e253be8a331aaab7ea395cb747ec0f',
}
LINK_FLAG = '--defsym=memset=' + PROVIDER


def validate_inputs(inventories):
    providers = [i for i in inventories
                 if any(f['name'] == PROVIDER for f in i['functions'])]
    if len(providers) != 1 or providers[0]['sha256'] != PROVIDER_HASH:
        raise ValueError('Memory binding requires the reviewed authored provider')
    if any(f['name'] == 'memset' for i in inventories for f in i['functions']):
        raise ValueError('Memory binding would replace an existing memset definition')
    consumers = [i['sha256'] for i in inventories if 'memset' in i['undefined_symbols']]
    if len(consumers) != len(CONSUMER_HASHES) or set(consumers) != CONSUMER_HASHES:
        raise ValueError('Memory binding requires exactly the three reviewed consumers')
    return {'symbol': 'memset', 'provider': PROVIDER,
            'provider_object_sha256': PROVIDER_HASH,
            'consumer_object_sha256': sorted(consumers), 'link_flag': LINK_FLAG}


def validate_output(linked):
    def unique(name):
        found = [f for f in linked['functions'] if f['name'] == name]
        if len(found) != 1:
            raise ValueError('Missing or ambiguous memory binding symbol: ' + name)
        return found[0]
    alias, provider = unique('memset'), unique(PROVIDER)
    if ('memset' in linked['undefined_symbols'] or
            alias['section_index'] != provider['section_index'] or
            alias['section_index'] >= 0xff00 or alias['section_index'] == 0 or
            alias['value'] != provider['value'] or
            alias['binding'] != 1 or provider['binding'] != 1 or
            alias['compiled_body_bytes'] != 0 or provider['compiled_body_bytes'] != 40):
        raise ValueError('Memory alias does not identify the reviewed source body')
