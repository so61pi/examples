.. contents:: Table of Contents

RFC-2046 [`ref <https://tools.ietf.org/html/rfc2046>`__]
========================================================

Common Syntax [`ref <https://tools.ietf.org/html/rfc2046#section-5.1.1>`__]
---------------------------------------------------------------------------

.. code-block:: text

    multipart-body := [preamble CRLF]
                      dash-boundary transport-padding CRLF
                      body-part *encapsulation
                      close-delimiter transport-padding
                      [CRLF epilogue]

    body-part := MIME-part-headers [CRLF *OCTET]
                 ; Lines in a body-part must not start
                 ; with the specified dash-boundary and
                 ; the delimiter must not appear anywhere
                 ; in the body part.  Note that the
                 ; semantics of a body-part differ from
                 ; the semantics of a message, as
                 ; described in the text.

    encapsulation := delimiter transport-padding
                     CRLF body-part

    preamble := discard-text

    epilogue := discard-text

    dash-boundary := "--" boundary
                     ; boundary taken from the value of
                     ; boundary parameter of the
                     ; Content-Type field.

    close-delimiter := delimiter "--"

    delimiter := CRLF dash-boundary

    transport-padding := *LWSP-char
                         ; Composers MUST NOT generate
                         ; non-zero length transport
                         ; padding, but receivers MUST
                         ; be able to handle padding
                         ; added by message transports.

    boundary := 0*69<bchars> bcharsnospace

    bchars := bcharsnospace / " "

    bcharsnospace := DIGIT / ALPHA / "'" / "(" / ")" /
                     "+" / "_" / "," / "-" / "." /
                     "/" / ":" / "=" / "?"

    discard-text := *(*text CRLF)
                    ; May be ignored or discarded.

Subtype
-------

Mixed [`ref <https://tools.ietf.org/html/rfc2046#section-5.1.3>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    The "mixed" subtype of "multipart" is intended for use when the body
    parts are independent and need to be bundled in a particular order.
    Any "multipart" subtypes that an implementation does not recognize
    must be treated as being of subtype "mixed".

Alternative [`ref <https://tools.ietf.org/html/rfc2046#section-5.1.4>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    The "multipart/alternative" type is syntactically identical to
    "multipart/mixed", but the semantics are different.  In particular,
    each of the body parts is an "alternative" version of the same
    information.

.. code-block:: text

    From: Nathaniel Borenstein <nsb@bellcore.com>
    To: Ned Freed <ned@innosoft.com>
    Date: Mon, 22 Mar 1993 09:41:09 -0800 (PST)
    Subject: Formatted text mail
    MIME-Version: 1.0
    Content-Type: multipart/alternative; boundary=boundary42

    --boundary42
    Content-Type: text/plain; charset=us-ascii

      ... plain text version of message goes here ...

    --boundary42
    Content-Type: text/enriched

      ... RFC 1896 text/enriched version of same message
          goes here ...

    --boundary42
    Content-Type: application/x-whatever

      ... fanciest version of same message goes here ...

    --boundary42--

Digest [`ref <https://tools.ietf.org/html/rfc2046#section-5.1.5>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    This document defines a "digest" subtype of the "multipart" Content-
    Type.  This type is syntactically identical to "multipart/mixed", but
    the semantics are different.  In particular, in a digest, the default
    Content-Type value for a body part is changed from "text/plain" to
    "message/rfc822".  This is done to allow a more readable digest
    format that is largely compatible (except for the quoting convention)
    with RFC 934.

.. code-block:: text

    From: Moderator-Address
    To: Recipient-List
    Date: Mon, 22 Mar 1994 13:34:51 +0000
    Subject: Internet Digest, volume 42
    MIME-Version: 1.0
    Content-Type: multipart/mixed;
                  boundary="---- main boundary ----"

    ------ main boundary ----

      ...Introductory text or table of contents...

    ------ main boundary ----
    Content-Type: multipart/digest;
                  boundary="---- next message ----"

    ------ next message ----

    From: someone-else
    Date: Fri, 26 Mar 1993 11:13:32 +0200
    Subject: my opinion

      ...body goes here ...

    ------ next message ----

    From: someone-else-again
    Date: Fri, 26 Mar 1993 10:07:13 -0500
    Subject: my different opinion

      ... another body goes here ...

    ------ next message ------

    ------ main boundary ------

Parallel [`ref <https://tools.ietf.org/html/rfc2046#section-5.1.6>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    This document defines a "parallel" subtype of the "multipart"
    Content-Type.  This type is syntactically identical to
    "multipart/mixed", but the semantics are different.  In particular,
    in a parallel entity, the order of body parts is not significant.

A Complex Multipart Example [`ref <https://tools.ietf.org/html/rfc2049#page-15>`__]
-----------------------------------------------------------------------------------

.. code-block:: text

    MIME-Version: 1.0
    From: Nathaniel Borenstein <nsb@nsb.fv.com>
    To: Ned Freed <ned@innosoft.com>
    Date: Fri, 07 Oct 1994 16:15:05 -0700 (PDT)
    Subject: A multipart example
    Content-Type: multipart/mixed;
                  boundary=unique-boundary-1

    This is the preamble area of a multipart message.
    Mail readers that understand multipart format
    should ignore this preamble.

    If you are reading this text, you might want to
    consider changing to a mail reader that understands
    how to properly display multipart messages.

    --unique-boundary-1

      ... Some text appears here ...

    [Note that the blank between the boundary and the start
     of the text in this part means no header fields were
     given and this is text in the US-ASCII character set.
     It could have been done with explicit typing as in the
     next part.]

    --unique-boundary-1
    Content-type: text/plain; charset=US-ASCII

    This could have been part of the previous part, but
    illustrates explicit versus implicit typing of body
    parts.

    --unique-boundary-1
    Content-Type: multipart/parallel; boundary=unique-boundary-2

    --unique-boundary-2
    Content-Type: audio/basic
    Content-Transfer-Encoding: base64

      ... base64-encoded 8000 Hz single-channel
          mu-law-format audio data goes here ...

    --unique-boundary-2
    Content-Type: image/jpeg
    Content-Transfer-Encoding: base64

      ... base64-encoded image data goes here ...

    --unique-boundary-2--

    --unique-boundary-1
    Content-type: text/enriched

    This is <bold><italic>enriched.</italic></bold>
    <smaller>as defined in RFC 1896</smaller>

    Isn't it
    <bigger><bigger>cool?</bigger></bigger>

    --unique-boundary-1
    Content-Type: message/rfc822

    From: (mailbox in US-ASCII)
    To: (address in US-ASCII)
    Subject: (subject in US-ASCII)
    Content-Type: Text/plain; charset=ISO-8859-1
    Content-Transfer-Encoding: Quoted-printable

      ... Additional text in ISO-8859-1 goes here ...

    --unique-boundary-1--

Form-Data [`ref <https://tools.ietf.org/html/rfc7578>`__]
=========================================================

- `"Boundary" Parameter of multipart/form-data <https://tools.ietf.org/html/rfc7578#section-4.1>`__
- `Content-Disposition Header Field for Each Part <https://tools.ietf.org/html/rfc7578#section-4.2>`__
- `Content-Type Header Field for Each Part (optional) <https://tools.ietf.org/html/rfc7578#section-4.4>`__
- `Multiple Files for One Form Field (deprecated) <https://tools.ietf.org/html/rfc7578#section-4.3>`__
- `The _charset_ Field for Default Charset <https://tools.ietf.org/html/rfc7578#section-4.6>`__

.. code-block:: text

    POST /test.html HTTP/1.1
    Host: example.org
    Content-Type: multipart/form-data;boundary="boundary"

    --boundary
    Content-Disposition: form-data; name="field1"

    value1
    --boundary
    Content-Disposition: form-data; name="field2"; filename="example.txt"

    value2
    --boundary--
