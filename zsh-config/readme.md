## Add to `~/.zshrc`

```shell
HISTFILE=$HOME/.zhistfile
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

export LESS=-XFR

alias ggrep="grep -Frn"

hg_get_branch_name () {
    if [ $(in_hg) ]
    then
        if [[ -n $(hg prompt "{bookmark}") ]]; then
            echo $(hg prompt "{branch}:{bookmark}")
        else
            echo $(hg prompt "{branch}")
        fi
    fi
}

hg_prompt_info () {
    if [ $(in_hg) ]
    then
        _DISPLAY=$(hg_get_branch_name) 
        echo "$ZSH_PROMPT_BASE_COLOR$ZSH_THEME_HG_PROMPT_PREFIX$ZSH_THEME_REPO_NAME_COLOR$_DISPLAY$ZSH_PROMPT_BASE_COLOR$ZSH_PROMPT_BASE_COLOR$ZSH_THEME_HG_PROMPT_SUFFIX$ZSH_PROMPT_BASE_COLOR "
        unset _DISPLAY
    fi
}

ZSH_THEME_HG_PROMPT_PREFIX="%{$fg_bold[magenta]%}hg:(%{$fg[red]%}"
ZSH_THEME_HG_PROMPT_SUFFIX="%{$fg[magenta]%})%{$reset_color%}"

PROMPT="$PROMPT"'$(hg_prompt_info)'
```

## Reload history

```shell
fc -R
```

## Disable git dirty on prompt

```shel
git config --global --add oh-my-zsh.hide-dirty 1
```

## Use `~/.zprofile` instead of `~/.profile`

## Useful things

- [oh-my-zsh](https://github.com/robbyrussell/oh-my-zsh).
- bgnotify
- [zsh-autosuggestions](https://github.com/zsh-users/zsh-autosuggestions/blob/master/INSTALL.md).
- [zsh-syntax-highlighting](https://github.com/zsh-users/zsh-syntax-highlighting/blob/master/INSTALL.md).
