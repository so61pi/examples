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
Ctrl+B [                        # enter copy mode (use emacs key binding (e.g. Ctrl+S for search))

Ctrl+B %                        # create pane  |
Ctrl+B "                        # create pane ---

Ctrl+B O                        # cycle through panes
Ctrl+B ;                        # go to last pane
Ctrl+B Up|Down|Left|Right

Ctrl+B Ctrl+O                   # rotate window

Ctrl+B Z                        # maximize current pane

Ctrl+B !                        # move current pane to new window
```

## Some window commands

```shell
Ctrl+B C            # create window

Ctrl+B 0..9         # switch window
Ctrl+B N|P          # go to next/previous window
Ctrl+B L            # go to last window

Ctrl+B W            # choose window from list
```

## Note

- Press `Shift` to select and copy text.
