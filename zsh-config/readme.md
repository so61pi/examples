**add to `~/.zshrc`**

```shell
HISTFILE=$HOME/.histfile
HISTSIZE=1000000                        # Number of lines of history in memory.
SAVEHIST=1000000                        # Number of lines of history in history file.
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


**reload history**

```shell
fc -R
```


**disable git dirty on prompt**

```shel
git config --global --add oh-my-zsh.hide-dirty 1
```


**use `~/.zprofile` instead of `~/.profile`**


**useful things**

- [oh-my-zsh](https://github.com/robbyrussell/oh-my-zsh).
- [zsh-syntax-highlighting](https://github.com/zsh-users/zsh-syntax-highlighting/blob/master/INSTALL.md).
