## Roles

- A: Client (Browser)
- B: Identity Provider
- C: Web Server

## Workflow

1. A calls B using implicit code grant to get access token.

1. There are 2 ways to to the obtained access token.

   - Use access token to exchange for cookie.

     1. A calls C with obtained access token.

        - HAProxy at C calls up a module to check the requests as follows:

          * Validate access token,
          * Then, set the cookies,
          * Or, response with 401 Unauthorized.

     1. A makes subsequent requests to C, cookies are automatically sent with each request.

        - HAProxy at C checks the cookies in the requests, and redirect to login page if necessary (e.g. `https://example.com/login?continue=https://example.com/lastpage`).

   - Use access token directly.

     This strategy can be used for single-page applications.

     1. A makes requests to C, explicitly and manually sets the `Authorization: Bearer ...` header.

        - HAProxy at C calls up a module to check the requests as follows:

          * Validate access token, response with 401 Unauthorized if necessary.

            + The front-end code will ask for relogging in if it receives 401.
