#!/usr/bin/env bash

#########################################################   CONFIGURATIONS   ###
enable_apache2=0
enable_vim=1
enable_htop=1
enable_gui="lubuntu"
#enable_gui="unity"
#enable_gui="kubuntu"
#enable_gui="xfce"
enable_terminator=1
enable_firefox=1
enable_guest_additions=1


###########################################################   PREPARATIONS   ###
# export http_proxy="http://proxy/"

# Add user `vagrant` to `sudo` group.
usermod -aG sudo vagrant


#######################################################   INSTALL PACKAGES   ###
apt-get update

if [ $enable_apache2 -eq 1 ]; then
    apt-get install -y apache2
fi

if [ $enable_vim -eq 1 ]; then
    apt-get install -y vim
fi

if [ $enable_htop -eq 1 ]; then
    apt-get install -y htop
fi

case "$enable_gui" in
  "xfce") apt-get install -y xfce4
          ;;

  "unity")  apt-get install -y --no-install-recommends ubuntu-desktop
            apt-get install -y gnome-terminal nautilus-open-terminal network-manager-gnome indicator-network indicator-applet-complete indicator-appmenu overlay-scrollbar
            ;;

  "lubuntu")  apt-get install -y lubuntu-desktop
              ;;

  "kubuntu")  apt-get install -y --no-install-recommends kubuntu-desktop
              ;;
esac

if [ $enable_terminator -eq 1 ]; then
    apt-get install -y terminator
fi

if [ $enable_firefox -eq 1 ]; then
    apt-get install -y firefox
fi

if [ $enable_guest_additions -eq 1 ]; then
    :
fi

apt-get clean


########################################################   PRINT OUT NOTES   ###
if [ $enable_apache2 -eq 1 ]; then
    echo "********************"
    echo "NOTE for apache2"
    echo "----"
    echo "    Use the following commands to set up apache2 pointing to ~."
    echo "        sudo rm -rf /var/www/html"
    echo "        sudo ln -fs /home/vagrant /var/www/html"
    echo "    or"
    echo "        sudo rm -rf /var/www"
    echo "        sudo ln -fs /home/vagrant /var/www"
    echo "********************"
fi

case "$enable_gui" in
  "xfce")   echo "********************"
            echo "NOTE for xfce4"
            echo "----"
            echo "    Use startxfce4 & to start GUI."
            echo "    Configure so anybody can start GUI."
            echo "        vim /etc/X11/Xwrapper.config"
            echo "            allowed_users=anybody"
            echo "********************"
            ;;

  "unity")  echo "********************"
            echo "NOTE for unity"
            echo "----"
            echo "    You can disable all Unity effects by using compizconfig-settings-manager."
            echo "        sudo apt-get install compizconfig-settings-manager"
            echo "        ccsm &"
            echo "            In the effects category, untick all of the boxes."
            echo "            In the accessibility tab, untick all of the boxes."
            echo "    To make network indicator works correctly, you may need to change something."
            echo "        sudo vim /etc/NetworkManager/NetworkManager.conf"
            echo "            change the line managed=false to managed=true"
            echo "        sudo service network-manager restart"
            echo "********************"
            ;;
esac

if [ $enable_guest_additions -eq 1 ]; then
    echo "********************"
    echo "NOTE for Guest Additions"
    echo "----"
    echo "    Please install VirtualBox Guest Addition."
    echo "        Devices > Insert Guest Additions CD Image..."
    echo "        sudo mount /dev/sr0 /mnt"
    echo "        sudo /mnt/VBoxLinuxAdditions.run"
    echo "********************"
fi
