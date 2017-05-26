**Require setting user.name and email per-repo**

```shell
git config --global user.useConfigOnly true
```


**Remove email address from global config**

```shell
git config --global --unset-all user.email
```


**Add alias `ggrep`**

```shell
git config --global alias.ggrep "grep --break --heading --line-number"
```


**Freeze `~/.gitconfig` file so no one can accidentally change it**

```shell
# Never use `chmod -w ~/.gitconfig` alone, git will copy `.gitconfig`
# to `.gitconfig.lock` and edit then copy it back to old file.
# We have to change the file's attribute to immutable.
chmod -w ~/.gitconfig
sudo chattr +i ~/.gitconfig

# Must have `i` in the attribute list.
lsattr ~/.gitconfig

# Use `sudo chattr -i ~/.gitconfig` to edit it again.
```

