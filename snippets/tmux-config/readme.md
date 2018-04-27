## Note

- This file change default prefix (`Ctrl+B`) to `Ctrl+A`
- Press `Shift` to select and copy text.

## `~/.tmux.conf`

```txt
# Change status color
set-window-option -g window-status-current-bg blue
set-window-option -g window-status-current-fg white

# History size
set -g history-limit 100000

# 256color
set -g default-terminal "screen-256color"

# Enable mouse support
set -g mouse on

# Notifications
setw -g monitor-activity on
set -g visual-activity on

# Remap prefix from 'C-b' to 'C-a'
unbind C-b
set-option -g prefix C-a
bind-key C-a send-prefix

# Split panes using | and -
bind | split-window -h
bind - split-window -v
unbind '"'
unbind %

# Switch panes using Alt-arrow without prefix
bind -n M-Left select-pane -L
bind -n M-Right select-pane -R
bind -n M-Up select-pane -U
bind -n M-Down select-pane -D
```

## `.zshrc`

```shell
# zsh tmux settings
ZSH_TMUX_AUTOSTART="true"

# add tmux-plugin to plugin list
plugins=(tmux)
```

## Some pane commands

```shell
<prefix> [                      # enter copy mode (use emacs key binding (e.g. Ctrl+S for search))

<prefix> |                      # create pane | (default key is %)
<prefix> -                      # create pane - (default key is ")

<prefix> O                      # cycle through panes
<prefix> ;                      # go to last pane
# <prefix> Up|Down|Left|Right

<prefix> Ctrl+O                 # rotate window

<prefix> Z                      # maximize current pane

<prefix> !                      # move current pane to new window
```

## Some window commands

```shell
<prefix> C          # create window

<prefix> 0..9       # switch window
<prefix> N|P        # go to next/previous window
<prefix> L          # go to last window

<prefix> W          # choose window from list
```
