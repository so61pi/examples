**Create `~/.inputrc` file with following content to enable history search with the text of the left.**

```shell
"\e[A":history-search-backward
"\e[B":history-search-forward
```


**Change `HISTFILESIZE` and `HISTSIZE` to larger numbers.**

```shell
HISTSIZE=10000
HISTFILESIZE=100000
```


**Enable `shopt -s globstar` so we can do recursive search like `ls **/*.c`**


**Enable `force_color_prompt=yes`**