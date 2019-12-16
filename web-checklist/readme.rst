.. contents:: Table of Contents

General
=======

**These settings can be on reverse proxy server or the application itself.**

- Put maximum limitation on everything.

  * Rate limiting
  * Turn off all un-used ports (both in and out).
  * Only give accounts minimal level of permission.
  * Run services as non-root user, non-root group.
  * ...

- Don't log sensitive information.
- Validate client's inputs.

  * SQL injection
  * ...

- Check CORS list.
- Check/Disable directory access.
- Enable HTTPS.
- Set HTTP limits.

  * KeepAlive=on
  * KeepAliveTimeout
  * LimitRequestBody
  * LimitRequestFields
  * LimitRequestFieldSize
  * LimitRequestLine
  * LimitXMLRequestBody
  * MaxClients
  * MaxKeepAliveRequests
  * MaxRequestWorkers
  * RequestReadTimeout
  * TimeOut

- Enable ``HttpOnly`` and ``Secure`` for cookies.

  * https://developer.mozilla.org/en-US/docs/Web/HTTP/Cookies

- Enable security-related headers [`ref <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers#Security>`__].

  * `Content-Security-Policy (CSP) <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Security-Policy>`__
  * `Cross-Origin-Resource-Policy <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Cross-Origin-Resource-Policy>`__
  * `Strict-Transport-Security (HSTS) <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Strict-Transport-Security>`__

References
==========

- https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/security_guide/index
