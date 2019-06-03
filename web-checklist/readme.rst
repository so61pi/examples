.. contents:: Table of Contents

Security
========

- Don't log sensitive information
- Check SQL injection
- Check CORS list
- Check/Disable directory access
- Enable HTTPS
- Set HTTP limits

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

- Enable ``HttpOnly`` and ``Secure`` for cookies

  * https://developer.mozilla.org/en-US/docs/Web/HTTP/Cookies

- Enable security-related headers [`ref <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers#Security>`__]

  * `Content-Security-Policy (CSP) <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Security-Policy>`__
  * `Cross-Origin-Resource-Policy <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Cross-Origin-Resource-Policy>`__
  * `Strict-Transport-Security (HSTS) <https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Strict-Transport-Security>`__

- Run service as non-root user

References
==========

- https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/security_guide/index
