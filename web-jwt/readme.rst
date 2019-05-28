.. contents:: Table of Contents

JSON Web Signature
==================

Basic Elements
--------------

.. code-block:: text

    <JOSE Header> = <JWS Protected Header> U <JWS Unprotected Header>
    <JWS Payload>
    <JWS Signature>

Serialization
-------------

JWS Compact Serialization [`ref <https://tools.ietf.org/html/rfc7515#section-7.1>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    BASE64URL(UTF8(JWS Protected Header)) || '.' ||
    BASE64URL(JWS Payload) || '.' ||
    BASE64URL(JWS Signature)

JWS JSON Serialization
~~~~~~~~~~~~~~~~~~~~~~

General JWS JSON Serialization [`ref <https://tools.ietf.org/html/rfc7515#section-7.2.1>`__]
````````````````````````````````````````````````````````````````````````````````````````````

.. code-block:: json

    {
        "payload": "<BASE64URL(JWS Payload)>"
        "signatures": [
            {
                "protected":    "<BASE64URL(UTF8(JWS Protected Header))>"
                "header":       "<JWS Unprotected Header>"
                "signature":    "<BASE64URL(JWS Signature)>"
            },
            ...
        ]
    }

Flattened JWS JSON Serialization [`ref <https://tools.ietf.org/html/rfc7515#section-7.2.2>`__]
``````````````````````````````````````````````````````````````````````````````````````````````

.. code-block:: json

    {
        "protected":    "<BASE64URL(UTF8(JWS Protected Header))>"
        "header":       "<JWS Unprotected Header>"
        "payload":      "<BASE64URL(JWS Payload)>"
        "signature":    "<BASE64URL(JWS Signature)>"
    }

JOSE Header (JSON) [`ref <https://tools.ietf.org/html/rfc7515#section-4>`__]
----------------------------------------------------------------------------

Registered Header Parameter
~~~~~~~~~~~~~~~~~~~~~~~~~~~

- "alg" [MUST]: Digital Signature or MAC Algorithm

  + https://tools.ietf.org/html/rfc7515#section-4.1.1
  + https://tools.ietf.org/html/rfc7518#section-3.1

- "typ" [OPTIONAL]

  + https://tools.ietf.org/html/rfc7515#section-4.1.9
  + https://tools.ietf.org/html/rfc7519#section-3.1

Producing and Consuming JWSs [`ref <https://tools.ietf.org/html/rfc7515#section-5>`__]
--------------------------------------------------------------------------------------

Example [`ref <https://tools.ietf.org/html/rfc7515#appendix-A.1.1>`__]
----------------------------------------------------------------------

- JWS Protected Header

  .. code-block:: json

      {"typ":"JWT",
       "alg":"HS256"}

  ``BASE64URL(UTF8(JWS Protected Header)) = eyJ0eXAiOiJKV1QiLA0KICJhbGciOiJIUzI1NiJ9``

- JWS Payload

  .. code-block:: json

      {"iss":"joe",
       "exp":1300819380,
       "http://example.com/is_root":true}

  ``BASE64URL(JWS Payload) = eyJpc3MiOiJqb2UiLA0KICJleHAiOjEzMDA4MTkzODAsDQogImh0dHA6Ly9leGFtcGxlLmNvbS9pc19yb290Ijp0cnVlfQ``

- JWS Signature

  .. code-block:: text

      HMAC SHA-256 Key = AyM1SysPpbyDfgZld3umj1qzKObwVMkoqQ-EstJQLr_T-1qS0gZH75aKtMN3Yj0iPS4hcgUuTwjAzZr1Z9CAow

  ``BASE64URL(Sign BASE64URL(UTF8(JWS Protected Header)).BASE64URL(JWS Payload)) = dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk``

  Result in JWS Compact Serialization

  .. code-block:: text

      eyJ0eXAiOiJKV1QiLA0KICJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJqb2UiLA0KICJleHAiOjEzMDA4MTkzODAsDQogImh0dHA6Ly9leGFtcGxlLmNvbS9pc19yb290Ijp0cnVlfQ.dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk
                                              ^                                                                                              ^

JSON Web Encryption
===================

Basic Elements
--------------

.. code-block:: text

    <JOSE Header> = <JWE Protected Header> U <JWE Shared Unprotected Header> U <JWE Per-Recipient Unprotected Header>
    <JWE Encrypted Key>
    <JWE Initialization Vector>
    <JWE AAD>
    <JWE Ciphertext> = Encrypt <Plaintext>
    <JWE Authentication Tag>

Serialization
-------------

JWS Compact Serialization [`ref <https://tools.ietf.org/html/rfc7516#section-3.1>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    BASE64URL(UTF8(JWE Protected Header)) || '.' ||
    BASE64URL(JWE Encrypted Key) || '.' ||
    BASE64URL(JWE Initialization Vector) || '.' ||
    BASE64URL(JWE Ciphertext) || '.' ||
    BASE64URL(JWE Authentication Tag)

JWE JSON Serialization [`ref <https://tools.ietf.org/html/rfc7516#section-3.2>`__]
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: json

    {
        "protected":        "<BASE64URL(UTF8(JWE Protected Header))>"
        "unprotected":      "<JWE Shared Unprotected Header>"
        "header":           "<JWE Per-Recipient Unprotected Header>"
        "encrypted_key":    "<BASE64URL(JWE Encrypted Key)>"
        "iv":               "<BASE64URL(JWE Initialization Vector)>"
        "ciphertext":       "<BASE64URL(JWE Ciphertext)>"
        "tag":              "<BASE64URL(JWE Authentication Tag)>"
        "aad":              "<BASE64URL(JWE AAD)>"
    }

JOSE Header (JSON) [`ref <https://tools.ietf.org/html/rfc7516#section-4>`__]
----------------------------------------------------------------------------

Registered Header Parameter
~~~~~~~~~~~~~~~~~~~~~~~~~~~

- "alg" [MUST]: Digital Signature or MAC Algorithm

  * https://tools.ietf.org/html/rfc7516#section-4.1.1
  * https://tools.ietf.org/html/rfc7515#section-4.1.1

- "enc" [MUST]: Encryption Algorithm

  * https://tools.ietf.org/html/rfc7516#section-4.1.2
  * https://tools.ietf.org/html/rfc7518#section-5.1

- "zip" [OPTIONAL]: Compression Algorithm

  * https://tools.ietf.org/html/rfc7516#section-4.1.3

Producing and Consuming JWEs [`ref <https://tools.ietf.org/html/rfc7516#section-5>`__]
--------------------------------------------------------------------------------------

Example [`ref <https://tools.ietf.org/html/rfc7516#appendix-A.1>`__]
--------------------------------------------------------------------

JSON Web Token
==============

.. code-block:: text

    JWT = JWS with <JWS Payload = JWT Claims Set>
        = JWE with <Plaintext = JWT Claims Set>

JWT Claims (JSON) [`ref <https://tools.ietf.org/html/rfc7519#section-4>`__]
---------------------------------------------------------------------------

Registered Claim Names
~~~~~~~~~~~~~~~~~~~~~~

- "iss" [OPTIONAL]: Issuer
- "sub" [OPTIONAL]: Subject
- "aud" [OPTIONAL]: Audience
- "exp" [OPTIONAL]: Expiration Time
- "nbf" [OPTIONAL]: Not Before
- "iat" [OPTIONAL]: Issued At
- "jti" [OPTIONAL]: JWT ID

Usage
=====

We shouldn't use JWT for session management

- It doesn't bring additional benefits compared to normal session management.
- We cannot invalidate JWT token immediately.

  * We can set small timeout (`"exp" <https://tools.ietf.org/html/rfc7519#section-4.1.4>`__ and `"iat" <https://tools.ietf.org/html/rfc7519#section-4.1.6>`__), but it's not **instantly**.

We should use JWT when a client needs to talk to 2 servers which are separated but one can understand the JWT of the other.

- Like when a client receives a JWT token from server A and then shows that to server B to do something.
- The server B, by verifying the token, will allow or disallow the requested action to be carried out.

References
==========

- `JSON Web Signature <https://tools.ietf.org/html/rfc7515>`__
- `JSON Web Encryption <https://tools.ietf.org/html/rfc7516>`__
- `JSON Web Token <https://tools.ietf.org/html/rfc7519>`__

  * This RFC may refine some stuff from other 2 RFCs.

- http://cryto.net/~joepie91/blog/2016/06/13/stop-using-jwt-for-sessions/
