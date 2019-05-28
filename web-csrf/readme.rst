.. contents:: Table of Contents

Definition
==========

Cross-Site Request Forgery (CSRF) is an attack that makes an end user to accidentally execute unwanted actions on a web application in which they're currently authenticated.

Example [`ref <https://www.owasp.org/index.php/Cross-Site_Request_Forgery_(CSRF)#Examples>`__]
==============================================================================================

Assume that

- Victim is logged into her bank account at https://bank.example.com.
- The request to transfer money to another account is

  .. code-block:: text

      GET https://bank.example.com/transfer.do?acct=ATTACKER&amount=100 HTTP/1.1

Then, attacker will try to make the victim click on https://bank.example.com/transfer.do?acct=ATTACKER&amount=100 by

- Sending an email to user with that link.
- Or embed it to a web page that the victim may visit and click on the link.

Other methods (``POST``...) have different ways to attack.

Prevention [`ref <https://www.owasp.org/index.php/Cross-Site_Request_Forgery_(CSRF)#Related_Controls>`__]
=========================================================================================================

References
==========

- https://www.owasp.org/index.php/Cross-Site_Request_Forgery_(CSRF)
