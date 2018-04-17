**global config**

```shell
git config --global user.useConfigOnly true

# remove email address from global config
git config --global --unset-all user.email

# add ggrep
git config --global alias.ggrep "grep --break --heading --line-number"

# config difftool and mergetool
git config --global diff.tool meld
git config --global merge.tool meld

# disable git dirty on prompt for oh-my-zsh
git config --global --add oh-my-zsh.hide-dirty 1
```


**freeze `~/.gitconfig` file so no one can accidentally change it**

```shell
# never use `chmod -w ~/.gitconfig` alone, git will copy `.gitconfig`
# to `.gitconfig.lock` and edit then copy it back to old file
#
# we have to change the file's attribute to immutable
chmod -w ~/.gitconfig
sudo chattr +i ~/.gitconfig

# must have `i` in the attribute list
lsattr ~/.gitconfig

# use `sudo chattr -i ~/.gitconfig` to edit it again
```

