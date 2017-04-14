* **Syntax**
```
set prompt \001\033[COLORm\002TEXT \001\033[0m\002
set prompt \001\033[FORMATm\002TEXT \001\033[0m\002
set prompt \001\033[FORMAT;COLORm\002TEXT \001\033[0m\002
```


* **COLOR**
```
Name                 Code
---------------------------
Default              39
Black                30
Red                  31
Green                32
Yellow               33
Blue                 34
Magenta              35
Cyan                 36
Light gray           37
Dark gray            90
Light red            91
Light green          92
Light yellow         93
Light blue           94
Light magenta        95
Light cyan           96
White                97
```


* **FORMAT**
```
Name                 Code
---------------------------
Reset                0
Bold                 1
Underlined           4
Reverse              7
```


* **TEXT**
```
(gdb)
gdb>
>>>
```


* **Reference**

http://misc.flogisoft.com/bash/tip_colors_and_formatting

http://askubuntu.com/questions/771925/avoid-terminal-characters-from-previous-terminal-commands

```
\001  Begin of non-printable character
\002  End of non-printable character
\033  Escape
```