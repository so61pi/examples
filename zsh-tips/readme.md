**Add to `~/.zshrc`**

```shell
setopt      incappendhistory            # Immediately append to the history file, not just when a term is killed.
setopt      histignoredups              # Don't add duplicated commands.
setopt      histexpiredupsfirst         # Save unique hist entries longer.
setopt      histverify                  # Edit recalled history before running.
setopt      extendedhistory             # Save timestamp on history entries.
unsetopt    sharehistory                # Disable share history across terminals.
```


**Reload history**

```shell
fc -R
```


**Disable git status on prompt**

```shel
git config --global --add oh-my-zsh.hide-status 1
git config --global --add oh-my-zsh.hide-dirty 1
```
