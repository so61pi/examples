.. contents:: Table of Contents

Definition
==========

    Cross-Site Scripting (XSS) attacks are a type of injection, in which malicious scripts are injected into otherwise benign and trusted websites.

    --- https://www.owasp.org/index.php/Cross-site_Scripting_(XSS)#Overview

Simply put, an attacker sends a malicious script to web server. When a victim visits that website, the malicious script will be automatically downloaded and executed in victim's browser.

XSS Types [`ref <https://www.owasp.org/index.php/Cross-site_Scripting_(XSS)#Stored_and_Reflected_XSS_Attacks>`__]
=================================================================================================================

Prevention
==========

- Don't allow users to send any kind of script to the server.
- In case that is required, validate input.

References
==========

- https://www.owasp.org/index.php/Cross-site_Scripting_(XSS)
