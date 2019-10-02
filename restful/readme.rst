.. contents:: Table of Contents

Concept
=======

``REST - REpresentational State Transfer`` is about resources, and we use ``URI - Uniform Resource Identifier`` to identify resource location. To perform useful actions on a given resource and get the status back, HTTP verbs (e.g. ``PUT``) and HTTP status codes (e.g. ``201 Created``) are used. 

The formal ``REST`` constraints:

- Client-server architecture
- Statelessness
- Cacheability
- Layered system
- Code on demand (optional)
- Uniform interface

  * Resource identification in requests
  * Resource manipulation through representations
  * Self-descriptive messages
  * Hypermedia as the engine of application state (HATEOAS)

Bear in mind that not all kinds of API can be modeled after ``REST``, sometimes using ``RPC`` is a right choice.

Statelessness
=============

No application/session state should be stored on server.

Authentication
--------------

``REST`` doesn't have notion about login and logout. If clients want to request data that requires authentication, in case of ``HTTP``, they must provide ``Authorization`` header (or something similar in the body) for every request.

The value of ``Authorization`` can be self-contained (e.g. ``JWT``) or not (e.g. ``Basic Authentication``). If it is not self-contained, we have to check its validity with the database, which increases the cost of processing for every request. However, this opens an opportunity to reject requests in case the provided credentials are authentic but temporarily blacklisted.

Idempotence
-----------

- ``POST`` is NOT idempotent.
- ``GET``, ``PUT``, ``DELETE``, ``HEAD``, ``OPTIONS`` and ``TRACE`` are idempotent.

Idempotent operations should be cheap.

Cacheability
============

Response must be defined as cacheable or not by the server.

Uniform interface
=================

Resource identification in requests
-----------------------------------

Resources are identified by URIs.

Resource manipulation through representations
---------------------------------------------

Clients should be able to update only through the received representations from server, no addtional information is needed.

Self-descriptive messages
-------------------------

Each message includes enough information to describe how to process the message. For example, which parser to invoke can be specified by a media type.

HATEOAS
-------

``RESTful API`` must be discoverable. This means users (developers) only need to know 2 following things prior using the API:

- The root URI of the API.
- The convention of how to construct new URIs to access new resources based on a given response from server.

Discoverability is not about the end users, but about the direct users of the API, namely the developers. Discoverability makes it easier for them to learn about the resource hierarchy.

The developers, however, can still hardcode the required URIs in their apps as sometimes navigating through serveral layers of links to retrieve a resource is too much overhead. (After all, we still use bookmarks to save specific URLs while browsing websites instead of clicking link by link.)

*Note:*

- HATEOAS can be really useful when the servers also control the UI, and the clients only render that UI by following a spec. (Like HTML is sent by the servers and web browsers only display that HTML code instead of inventing the UI.)
- In case of API, the servers don't really control the UI. They only give out the URL of APIs (see GitHub APIs below, for instance).
  Of course we can build a generic client (called API browser) that understands that json and presents it on a screen with all the URLs hidden. However, we will need a spec for interoperability, eventually. And the UI again is tied back to the servers.

.. code-block:: text

    $ curl -i https://api.github.com/                  
    HTTP/1.1 200 OK
    Date: Wed, 02 Oct 2019 03:35:49 GMT
    Content-Type: application/json; charset=utf-8
    Content-Length: 2165
    Server: GitHub.com
    Status: 200 OK
    X-RateLimit-Limit: 60
    X-RateLimit-Remaining: 52
    X-RateLimit-Reset: 1569990949
    Cache-Control: public, max-age=60, s-maxage=60
    Vary: Accept
    ETag: "7dc470913f1fe9bb6c7355b50a0737bc"
    X-GitHub-Media-Type: github.v3; format=json
    Access-Control-Expose-Headers: ETag, Link, Location, Retry-After, X-GitHub-OTP, X-RateLimit-Limit, X-RateLimit-Remaining, X-RateLimit-Reset, X-OAuth-Scopes, X-Accepted-OAuth-Scopes, X-Poll-Interval, X-GitHub-Media-Type
    Access-Control-Allow-Origin: *
    Strict-Transport-Security: max-age=31536000; includeSubdomains; preload
    X-Frame-Options: deny
    X-Content-Type-Options: nosniff
    X-XSS-Protection: 1; mode=block
    Referrer-Policy: origin-when-cross-origin, strict-origin-when-cross-origin
    Content-Security-Policy: default-src 'none'
    Vary: Accept-Encoding
    X-GitHub-Request-Id: D6A4:42A5:1C6E5D:24F0DD:5D941B14
    
    {
      "current_user_url": "https://api.github.com/user",
      "current_user_authorizations_html_url": "https://github.com/settings/connections/applications{/client_id}",
      "authorizations_url": "https://api.github.com/authorizations",
      "code_search_url": "https://api.github.com/search/code?q={query}{&page,per_page,sort,order}",
      "commit_search_url": "https://api.github.com/search/commits?q={query}{&page,per_page,sort,order}",
      "emails_url": "https://api.github.com/user/emails",
      "emojis_url": "https://api.github.com/emojis",
      "events_url": "https://api.github.com/events",
      "feeds_url": "https://api.github.com/feeds",
      "followers_url": "https://api.github.com/user/followers",
      "following_url": "https://api.github.com/user/following{/target}",
      "gists_url": "https://api.github.com/gists{/gist_id}",
      "hub_url": "https://api.github.com/hub",
      "issue_search_url": "https://api.github.com/search/issues?q={query}{&page,per_page,sort,order}",
      "issues_url": "https://api.github.com/issues",
      "keys_url": "https://api.github.com/user/keys",
      "notifications_url": "https://api.github.com/notifications",
      "organization_repositories_url": "https://api.github.com/orgs/{org}/repos{?type,page,per_page,sort}",
      "organization_url": "https://api.github.com/orgs/{org}",
      "public_gists_url": "https://api.github.com/gists/public",
      "rate_limit_url": "https://api.github.com/rate_limit",
      "repository_url": "https://api.github.com/repos/{owner}/{repo}",
      "repository_search_url": "https://api.github.com/search/repositories?q={query}{&page,per_page,sort,order}",
      "current_user_repositories_url": "https://api.github.com/user/repos{?type,page,per_page,sort}",
      "starred_url": "https://api.github.com/user/starred{/owner}{/repo}",
      "starred_gists_url": "https://api.github.com/gists/starred",
      "team_url": "https://api.github.com/teams",
      "user_url": "https://api.github.com/users/{user}",
      "user_organizations_url": "https://api.github.com/user/orgs",
      "user_repositories_url": "https://api.github.com/users/{user}/repos{?type,page,per_page,sort}",
      "user_search_url": "https://api.github.com/search/users?q={query}{&page,per_page,sort,order}"
    }

Content Negotiation/Hypermedia Versioning
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Clients can use ``Accept*`` headers to indicate what type of representation they can accept for a given URI. This, in turn, can help versioning our ``RESTful API`` by using vendor media type with attached version number like below.

.. code-block:: text

    Accept: application/vnd.api.data+json; version=1.0
    Accept: application/vnd.api.data.v1+json

    Accept: application/vnd.api.data+xml; version=2.0
    Accept: application/vnd.api.data.v2+xml

With this approach, we can upgrade some URIs to new version and leave the rest unaffected. Implementation of both client and server will be more complex though.

Some servers use ``/api/v1/...`` to version their API, but this technique is hard to maintain. Due to the fact that we have to support multiple versions, there will be a few endpoints that appear in multiple versions but actually have the same semantics.

References
==========

- https://en.wikipedia.org/wiki/Representational_state_transfer
- https://www.ics.uci.edu/~fielding/pubs/dissertation/rest_arch_style.htm
- http://blog.steveklabnik.com/posts/2011-07-03-nobody-understands-rest-or-http
- http://blog.steveklabnik.com/posts/2011-08-07-some-people-understand-rest-and-http
- http://timelessrepo.com/haters-gonna-hateoas
- https://martinfowler.com/articles/richardsonMaturityModel.html
- http://barelyenough.org/blog/2008/05/versioning-rest-web-services/
- https://en.wikipedia.org/wiki/HATEOAS
- https://roy.gbiv.com/untangled/2008/rest-apis-must-be-hypertext-driven
- https://stackoverflow.com/questions/26830431/web-service-differences-between-rest-and-rpc
- https://en.wikipedia.org/wiki/Content_negotiation
