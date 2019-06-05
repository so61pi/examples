.. contents:: Table of Contents

Unicode Encoding Schemes [`ref <http://www.unicode.org/versions/Unicode12.0.0/ch03.pdf#G28070>`__]
==================================================================================================

- BOM (byte order mark) is a Unicode character ``U+FEFF``, it's used to indicate the byte order of subsequent bytes.
- UTF-8
- UTF-16BE, UTF-16LE, UTF-16

  UTF-16BE, UTF-16LE, UTF-16 are 3 different (but related) encoding schemes. When UTF-16BE or UTF-16LE is used, BOM is unnecessary as endianness is already clear.

  When UTF-16 is used, BOM help to decide the endianess of subsequent bytes.

  * ``<FE FF>`` -> big-endian
  * ``<FF FE>`` -> little-endian

  In case BOM is not present, big-endian is assumed.

  `Summary of UTF-16BE, UTF-16LE, and UTF-16 <https://www.unicode.org/versions/Unicode12.0.0/ch03.pdf#G31866>`__

- UTF-32BE, UTF-32LE, UTF-32

  These encoding schemes are similar to UTF-16(LE|BE).

  `Summary of UTF-32BE, UTF-32LE, and UTF-32 <http://www.unicode.org/versions/Unicode12.0.0/ch03.pdf#G36145>`__

Normalization Forms [`ref <http://www.unicode.org/versions/Unicode12.0.0/ch03.pdf#G49537>`__]
=============================================================================================

    String equivalence is determined by a process called normalization, whereby strings are converted into forms which are compared directly for identity.

Maximal decomposition:

- Normalization Form D (NFD): The Canonical Decomposition of a coded character sequence.
- Normalization Form KD (NFKD): The Compatibility Decomposition of a coded character sequence.

Maximal composition:

- Normalization Form C (NFC): The Canonical Composition of the Canonical Decomposition of a coded character sequence.
- Normalization Form KC (NFKC): The Canonical Composition of the Compatibility Decomposition of a coded character sequence.

Logically, to get the NFD or NFKD (maximally decomposed) normalization form for a Unicode string, one first computes the full decomposition of that string and then applies the Canonical Ordering Algorithm to it.

Logically, to get the NFC or NFKC (maximally composed) normalization form for a Unicode string, one first computes the NFD or NFKD normalization form for that string, and then applies the Canonical Composition Algorithm to it.

Algorithms to convert Unicode strings to one of the Unicode Normalization Forms:

- Canonical Ordering Algorithm
- Canonical Composition Algorithm

Character Properties [`ref <http://www.unicode.org/versions/Unicode12.0.0/ch04.pdf#G138691>`__]
===============================================================================================

Each code point has a set of properties associated with it to determine

- how it should be displayed
- how it affects surrounding code points

  * Some code points jof this type are not displayable.

Some character properties (https://unicode.org/reports/tr44/#Properties):

- General_Category
- Uppercase
- Lowercase
- Numeric_Value
- Canonical_Combining_Class
- Bidi_Class

References
==========

- https://unicode.org/versions/Unicode12.1.0/
