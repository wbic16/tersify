# tersify
tersify is a unix-style cli program similar to tar.

It makes use of the Terse File Format (.t) to encode arbitrary dimensions in plain text.
See tersify.h for details.

# Usage
./trsi output.t <files>

# Changelog
0.0.2 - Performance improvements - 1M buffer size for fread and stack allocation tweaks
0.0.1 - Baseline
