* Create `/var/lib/polkit-1/localauthority/50-local.d/shutdown-requires-password.pkla`, you may need `sudo -i` as `cd` cannot work with `sudo`.
* Add following lines to newly created file:

        [Shutdown Authentication]
        Identity=unix-user:*
        Action=org.freedesktop.login1.reboot;org.freedesktop.login1.reboot-multiple-sessions;org.freedesktop.login1.power-off;org.freedesktop.login1.power-off-multiple-sessions;
        ResultAny=no
        ResultInactive=auth_admin_keep
        ResultActive=auth_admin_keep
