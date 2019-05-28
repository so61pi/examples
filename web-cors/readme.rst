.. contents:: Table of Contents

How Do Browsers Send Requests In Regard To CORS?
================================================

- Simple requests [`ref <https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS#Simple_requests>`__]

    - Send request directly to web server **without** sending OPTIONS request first.

- Preflighted requests [`ref <https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS#Preflighted_requests>`__]

    * Send ``OPTIONS`` request first to get ``Access-Control-Allow-Origin``.

      + This request is called preflight request.

    * Base on the received value, decide to actually send the request or display an error.

Import Headers
==============

Assume that

  - ``http://example.com`` wants to call ``http://example.net``.
  - ``http://example.com`` is in the allowed origin list of ``http://example.net``.

- ``OPTIONS`` request (preflight request)

  * Request

    .. code-block:: text

        Origin: http://example.com
        Access-Control-Request-Method
        Access-Control-Request-Headers

  * Response

    .. code-block:: text

        Access-Control-Allow-Origin: http://example.com
        Access-Control-Allow-Methods
        Access-Control-Allow-Headers
        Access-Control-Max-Age
        Access-Control-Allow-Credentials

- Normal request (can be either *simple request* or *preflighted request*)

  This request can also carry ``Cookie`` (of the destination) with it.

  * Request

    .. code-block:: text

        Origin: http://example.com

  * Response

    .. code-block:: text

        Access-Control-Allow-Origin: http://example.com
        Access-Control-Allow-Credentials

References
----------

- https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS#The_HTTP_request_headers
- https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS#The_HTTP_response_headers

Server Side Handling Considerations
===================================

- Simple cross-site requests: https://developer.mozilla.org/en-US/docs/Web/HTTP/Server-Side_Access_Control#Simple_cross-site_requests
- Preflighted requests: https://developer.mozilla.org/en-US/docs/Web/HTTP/Server-Side_Access_Control#Preflighted_requests
- Credentialed requests: https://developer.mozilla.org/en-US/docs/Web/HTTP/Server-Side_Access_Control#Credentialed_requests

Security Implications
=====================

CORS is for protecting clients, not web servers because only allows trusted ogirins to make requests.

  - Note that some requests will still be sent without preflight request.

References
==========

- https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS#Examples_of_access_control_scenarios
- https://en.wikipedia.org/wiki/Cross-origin_resource_sharing#How_CORS_works
- https://mobilejazz.com/blog/which-security-risks-do-cors-imply/
