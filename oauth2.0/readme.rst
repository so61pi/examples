.. contents:: Table of Contents

Common Grant Types
==================

Authorization Code Grant [`ref <https://tools.ietf.org/html/rfc6749#section-4.1>`__]
------------------------------------------------------------------------------------

- The authorization code grant type is used to obtain both **access tokens** and **refresh tokens** and is optimized for **confidential clients**.
- Example of logging to GitLab with Google account

  - When ``Resource Owner (Human)`` wants to use Google account to log in, ``Resource Owner (Human)`` clicks a button on GitLab web page to send that request to ``Client (GitLab server)``.
  - **A**: ``Client (GitLab server)`` then instructs ``User-Agent (Web browser)`` to redirect to ``Authorization Server (Google server)``.
  - **B**: ``Resource Owner (Human)`` now authenticates with ``Authorization Server (Google server)`` by logging to their account and grant the access.
  - **C**: ``Authorization Server (Google server)`` then send a response (with ``Authorization Code``) to ask the ``User-Agent (Web browser)`` to redirect to ``redirect_uri`` which points to ``Client (GitLab server)``.

    - Basically, this is ``Authorization Server (Google server)`` asking ``User-Agent (Web browser)`` to make a request to ``Client (GitLab server)`` with the generated ``Authorization Code``.

  - **D**: ``Client (GitLab server)`` then internally tries to get an **access token** and optionally a **refresh token** by sending a request to ``Authorization Server (Google server)``.

    - ``redirect_uri`` used in this step is for verification purpose only.
    - In this example, **access token** may only have one function, that is to get the email address.

- Another example is having Twitter posts to your Facebook.

  - Here, **access token** is used to post, hence **access token** must have sufficient permissions.

::

     +----------+
     | Resource |
     |   Owner  |
     |          |
     +----------+                             .-- response_type[REQ]="code" + client_id[REQ]
          v                                   |   + redirect_uri[OPT] + scope[OPT]
          |                                   |   + state[REC]
          |                                   |   (Above are obtained from step A, you can see them in URL bar in step B)
         (B)                                  |
     +----|-----+          Client Identifier  |   +---------------+
     |    v   >-+----(A)-- & Redirection URI -+-->|               |
     |  User-   |                            /    | Authorization |
     |  Agent >-+----(B)-- User authenticates --->|     Server    |
     |          |                                 |               |
     | ^    v <-+----(C)-- Authorization Code ---<|               |
     +-|----|---+          \                      +---------------+
       |    |               `-- code[REQ]             ^      v
      (A)  (C)                  + state[REQ]          |      |
       |    |                                         |      |
       ^    v                                .--------+------+---- grant_type[REQ]="authorization_code" + code[REQ]
     +---------+                            /         |      |     + redirect_uri[REQ] + client_id[REQ]
     |         |>---(D)-- Authorization Code ---------'      |
     |  Client |          & Redirection URI                  |
     |         |                                             |
     |         |<---(E)----- Access Token -------------------'
     +---------+       (w/ Optional Refresh Token)
                                        \
                                         `-- access_token[REQ] + token_type[REQ]
                                             + expires_in[REC]
                                             + refresh_token[OPT] + scope[OPT]

Implicit Grant [`ref <https://tools.ietf.org/html/rfc6749#section-4.2>`__]
--------------------------------------------------------------------------

- The implicit grant type is used to obtain **access tokens** (it does not support the issuance of refresh tokens) and is optimized for **public clients** known to operate a particular redirection URI. These clients are typically implemented in a browser using a scripting language such as JavaScript.
- A silly example is an utility app like one that can get your sent emails from Gmail. This app doesn't have its own webserver so it's unnecessary to have 2 rounds of authentication/authorization as in ``Authorization Code Grant``.
- **access token** in this case can be leaked if the app's author or user is not careful.

::

     +----------+
     | Resource |
     |  Owner   |
     |          |
     +----------+                             .-- response_type[REQ]="token" + client_id[REQ]
          v                                   |   + redirect_uri[OPT] + scope[OPT]
          |                                   |   + state[REC]
          |                                   |   (Above are obtained from step A, you can see them in URL bar in step B)
         (B)                                  |
     +----|-----+          Client Identifier  |  +---------------+
     |    v   >-+----(A)-- & Redirection URI -+->|               |
     |  User-   |                            /   | Authorization |
     |  Agent >-|----(B)-- User authenticates -->|     Server    |
     |          |                                |               |
     |          |<---(C)--- Redirection URI ----<|               |
     |          |          with Access Token     +---------------+
     |          |            in Fragment    \
     |          |                            `------------------------ access_token[REQ] + token_type[REQ]
     |          |                                                      + expires_in[REC]
     |          |                                +---------------+     + scope[OPT] + state[REQ]
     |          |----(D)--- Redirection URI ---->|   Web-Hosted  |
     |          |          without Fragment      |     Client    |
     |          |                                |    Resource   |
     |     (F)  |<---(E)------- Script ---------<|               |
     | ^        |                                +---------------+
     +-|--------+
       |    |
      (A)  (G) Access Token
       |    |
       ^    v
     +---------+
     |         |
     |  Client |
     |         |
     +---------+

Some Excerpts From `RFC-6749 <https://tools.ietf.org/html/rfc6749>`__
=====================================================================

Roles [`ref <https://tools.ietf.org/html/rfc6749#section-2.1>`__]
-----------------------------------------------------------------

::

   OAuth defines four roles:

   resource owner
      An entity capable of granting access to a protected resource.
      When the resource owner is a person, it is referred to as an
      end-user.

   resource server
      The server hosting the protected resources, capable of accepting
      and responding to protected resource requests using access tokens.

   client
      An application making protected resource requests on behalf of the
      resource owner and with its authorization.  The term "client" does
      not imply any particular implementation characteristics (e.g.,
      whether the application executes on a server, a desktop, or other
      devices).

   authorization server
      The server issuing access tokens to the client after successfully
      authenticating the resource owner and obtaining authorization.

Client Types [`ref <https://tools.ietf.org/html/rfc6749#section-2.1>`__]
------------------------------------------------------------------------

::

   OAuth defines two client types, based on their ability to
   authenticate securely with the authorization server (i.e., ability to
   maintain the confidentiality of their client credentials):

   confidential
      Clients capable of maintaining the confidentiality of their
      credentials (e.g., client implemented on a secure server with
      restricted access to the client credentials), or capable of secure
      client authentication using other means.

   public
      Clients incapable of maintaining the confidentiality of their
      credentials (e.g., clients executing on the device used by the
      resource owner, such as an installed native application or a web
      browser-based application), and incapable of secure client
      authentication via any other means.

::

   This specification has been designed around the following client
   profiles:

   web application
      A web application is a confidential client running on a web
      server.  Resource owners access the client via an HTML user
      interface rendered in a user-agent on the device used by the
      resource owner.  The client credentials as well as any access
      token issued to the client are stored on the web server and are
      not exposed to or accessible by the resource owner.

   user-agent-based application
      A user-agent-based application is a public client in which the
      client code is downloaded from a web server and executes within a
      user-agent (e.g., web browser) on the device used by the resource
      owner.  Protocol data and credentials are easily accessible (and
      often visible) to the resource owner.  Since such applications
      reside within the user-agent, they can make seamless use of the
      user-agent capabilities when requesting authorization.

   native application
      A native application is a public client installed and executed on
      the device used by the resource owner.  Protocol data and
      credentials are accessible to the resource owner.  It is assumed
      that any client authentication credentials included in the
      application can be extracted.  On the other hand, dynamically
      issued credentials such as access tokens or refresh tokens can
      receive an acceptable level of protection.  At a minimum, these
      credentials are protected from hostile servers with which the
      application may interact.  On some platforms, these credentials
      might be protected from other applications residing on the same
      device.

Protocol Flow [`ref <https://tools.ietf.org/html/rfc6749#section-1.2>`__]
-------------------------------------------------------------------------

::

     +--------+                               +---------------+
     |        |--(A)- Authorization Request ->|   Resource    |
     |        |                               |     Owner     |
     |        |<-(B)-- Authorization Grant ---|               |
     |        |                               +---------------+
     |        |
     |        |                               +---------------+
     |        |--(C)-- Authorization Grant -->| Authorization |
     | Client |                               |     Server    |
     |        |<-(D)----- Access Token -------|               |
     |        |                               +---------------+
     |        |
     |        |                               +---------------+
     |        |--(E)----- Access Token ------>|    Resource   |
     |        |                               |     Server    |
     |        |<-(F)--- Protected Resource ---|               |
     +--------+                               +---------------+

                     Figure 1: Abstract Protocol Flow

   The abstract OAuth 2.0 flow illustrated in Figure 1 describes the
   interaction between the four roles and includes the following steps:

   (A)  The client requests authorization from the resource owner.  The
        authorization request can be made directly to the resource owner
        (as shown), or preferably indirectly via the authorization
        server as an intermediary.

   (B)  The client receives an authorization grant, which is a
        credential representing the resource owner's authorization,
        expressed using one of four grant types defined in this
        specification or using an extension grant type.  The
        authorization grant type depends on the method used by the
        client to request authorization and the types supported by the
        authorization server.

   (C)  The client requests an access token by authenticating with the
        authorization server and presenting the authorization grant.

   (D)  The authorization server authenticates the client and validates
        the authorization grant, and if valid, issues an access token.

   (E)  The client requests the protected resource from the resource
        server and authenticates by presenting the access token.

   (F)  The resource server validates the access token, and if valid,
        serves the request.

Refresh Token [`ref <https://tools.ietf.org/html/rfc6749#section-1.5>`__]
-------------------------------------------------------------------------

::

   Refresh tokens are credentials used to obtain access tokens.  Refresh
   tokens are issued to the client by the authorization server and are
   used to obtain a new access token when the current access token
   becomes invalid or expires, or to obtain additional access tokens
   with identical or narrower scope (access tokens may have a shorter
   lifetime and fewer permissions than authorized by the resource
   owner).  Issuing a refresh token is optional at the discretion of the
   authorization server.  If the authorization server issues a refresh
   token, it is included when issuing an access token (i.e., step (D) in
   Figure 1).

  +--------+                                           +---------------+
  |        |--(A)------- Authorization Grant --------->|               |
  |        |                                           |               |
  |        |<-(B)----------- Access Token -------------|               |
  |        |               & Refresh Token             |               |
  |        |                                           |               |
  |        |                            +----------+   |               |
  |        |--(C)---- Access Token ---->|          |   |               |
  |        |                            |          |   |               |
  |        |<-(D)- Protected Resource --| Resource |   | Authorization |
  | Client |                            |  Server  |   |     Server    |
  |        |--(E)---- Access Token ---->|          |   |               |
  |        |                            |          |   |               |
  |        |<-(F)- Invalid Token Error -|          |   |               |
  |        |                            +----------+   |               |
  |        |                                           |               |
  |        |--(G)----------- Refresh Token ----------->|               |
  |        |                                           |               |
  |        |<-(H)----------- Access Token -------------|               |
  +--------+           & Optional Refresh Token        +---------------+

               Figure 2: Refreshing an Expired Access Token

   The flow illustrated in Figure 2 includes the following steps:

   (A)  The client requests an access token by authenticating with the
        authorization server and presenting an authorization grant.

   (B)  The authorization server authenticates the client and validates
        the authorization grant, and if valid, issues an access token
        and a refresh token.

   (C)  The client makes a protected resource request to the resource
        server by presenting the access token.

   (D)  The resource server validates the access token, and if valid,
        serves the request.

   (E)  Steps (C) and (D) repeat until the access token expires.  If the
        client knows the access token expired, it skips to step (G);
        otherwise, it makes another protected resource request.

   (F)  Since the access token is invalid, the resource server returns
        an invalid token error.

   (G)  The client requests a new access token by authenticating with
        the authorization server and presenting the refresh token.  The
        client authentication requirements are based on the client type
        and on the authorization server policies.

   (H)  The authorization server authenticates the client and validates
        the refresh token, and if valid, issues a new access token (and,
        optionally, a new refresh token).

References
==========

- `The OAuth 2.0 Authorization Framework (RFC-6749) <https://tools.ietf.org/html/rfc6749>`__

  - `Client Registration <https://tools.ietf.org/html/rfc6749#section-2>`__
  - `Issuing an Access Token <https://tools.ietf.org/html/rfc6749#section-5>`__
  - `Refreshing an Access Token <https://tools.ietf.org/html/rfc6749#section-6>`__
  - `Accessing Protected Resources <https://tools.ietf.org/html/rfc6749#section-7>`__

- `OAuth 2.0 for Native Apps (RFC-8252) <https://tools.ietf.org/html/rfc8252>`__
- `Proof Key for Code Exchange by OAuth Public Clients (RFC-7636) <https://tools.ietf.org/html/rfc7636>`__
