- Don't log sensitive information
- Check SQL injection
- Check CORS list
- Check/Disable directory access
- Enable https
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

- Enable ``HttpOnly`` and ``Secure`` for cookie

  * https://developer.mozilla.org/en-US/docs/Web/HTTP/Cookies

- Enable security related headers

  * Content-Security-Policy
  * Cross-Origin-Resource-Policy
  * Strict-Transport-Security
  * https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers#Security

- Run service as non-root
- https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/security_guide/index
