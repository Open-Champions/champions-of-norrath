import copy
import unittest

import memory_binding as binding


class MemoryBindingTests(unittest.TestCase):
    def inputs(self):
        return [{'sha256': binding.PROVIDER_HASH,
                 'functions': [{'name': binding.PROVIDER}], 'undefined_symbols': []},
                *[{'sha256': sha, 'functions': [], 'undefined_symbols': ['memset']}
                  for sha in sorted(binding.CONSUMER_HASHES)]]

    def output(self):
        common = {'section_index': 1, 'value': 96, 'binding': 1}
        return {'functions': [dict(common, name='memset', compiled_body_bytes=0),
                              dict(common, name=binding.PROVIDER, compiled_body_bytes=40)],
                'undefined_symbols': []}

    def test_reviewed_binding(self):
        receipt = binding.validate_inputs(self.inputs())
        self.assertEqual(receipt['link_flag'], '--defsym=memset=norrath_block_memory_set')
        self.assertEqual(len(receipt['consumer_object_sha256']), 3)
        binding.validate_output(self.output())

    def test_changed_or_missing_provider(self):
        cases = [self.inputs()[1:], self.inputs() + [self.inputs()[0]]]
        changed = self.inputs()
        changed[0]['sha256'] = 'changed'
        cases.append(changed)
        for inputs in cases:
            with self.subTest(inputs=inputs), self.assertRaises(ValueError):
                binding.validate_inputs(inputs)

    def test_changed_missing_added_and_duplicate_consumers(self):
        changed = self.inputs()
        changed[1]['sha256'] = 'changed'
        duplicate = self.inputs()
        duplicate[1] = copy.deepcopy(duplicate[2])
        for inputs in [changed, duplicate, self.inputs()[:-1],
                       self.inputs() + [self.inputs()[1]]]:
            with self.subTest(inputs=inputs), self.assertRaises(ValueError):
                binding.validate_inputs(inputs)

    def test_existing_memset_definition(self):
        inputs = self.inputs()
        inputs.append({'sha256': 'other', 'functions': [{'name': 'memset'}],
                       'undefined_symbols': []})
        with self.assertRaises(ValueError):
            binding.validate_inputs(inputs)

    def test_alias_must_be_zero_size_global_same_address(self):
        for key, value in [('section_index', 2), ('section_index', 0xfff1),
                           ('section_index', 0), ('value', 100), ('binding', 2),
                           ('compiled_body_bytes', 40)]:
            output = self.output()
            output['functions'][0][key] = value
            with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                binding.validate_output(output)

    def test_provider_must_retain_body_and_binding(self):
        for key, value in [('compiled_body_bytes', 0), ('binding', 2)]:
            output = self.output()
            output['functions'][1][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                binding.validate_output(output)

    def test_alias_missing_ambiguous_or_unresolved(self):
        for mode in ('missing', 'ambiguous', 'unresolved'):
            output = self.output()
            if mode == 'missing':
                output['functions'].pop(0)
            elif mode == 'ambiguous':
                output['functions'].append(output['functions'][0])
            else:
                output['undefined_symbols'] = ['memset']
            with self.subTest(mode=mode), self.assertRaises(ValueError):
                binding.validate_output(output)


if __name__ == '__main__':
    unittest.main()
