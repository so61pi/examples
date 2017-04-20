**Add to `~/.zshrc`**

```shell
setopt      incappendhistory            # Immediately append to the history file, not just when a term is killed.
setopt      histignoredups              # Don't add duplicated commands.
setopt      histexpiredupsfirst         # Save unique hist entries longer.
setopt      histverify                  # Edit recalled history before running.
setopt      extendedhistory             # Save timestamp on history entries.
setopt      extendedglob				# Enable extended glob.
setopt      autocd                      # Enable auto cd.
unsetopt    sharehistory                # Disable share history across terminals.
bindkey -e                              # Emacs mode.
```


**Reload history**

```shell
fc -R
```


**Disable git dirty on prompt**

```shel
git config --global --add oh-my-zsh.hide-dirty 1
```


**Use `~/.zprofile` instead of `~/.profile`**
