1st byte: 11100000 indicates it's a 3 byte sequence, the following bytes are continuation bytes.
Extracting the actual bits: (0000)(000000)(100001)
this combines to 0000 0000 0010 0001
This represents code point U+0021, which is the exclamation mark "!"

The three other ways to encode "!" in UTF-8 could be:
single byte ASCII: 00100001
two-byte: 11000000 10100001
four-byte: 11110000 10000000 10000000 10100001

An example of a character with exactly 3 encodings and not four would be the ASCII character 'A' (U+0041):
standard one-byte: 01000001
two-byte: 11000001 10000001
three-byte: 11100000 10000001 10000001
-It can't have a valid four-byte encoding because its value is too small

Problems with multiple encodings:
-Security vulnerabilities ( Attackers can use overlong encodings t bypass security filters and validation checks. For example. using different encodings of forward or backslashes could enable path traversal attacks )
-String comparison failures ( When the same character has different byte representations, direct string comparisons won't match even when they visually represent the same text. A normal ASCII "!" won't match its overlong UTF-8 encoding. )