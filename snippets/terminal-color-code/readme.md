http://wiki.bash-hackers.org/scripting/terminalcodes

http://misc.flogisoft.com/bash/tip_colors_and_formatting


* **Code sequence**
```
Set format or color           \033[NUMm
                              \033[NUM(;NUM)*m
Reset all format and color    \033[0m
```


* **Format**
```
Bold              1
Dim               2
Italic            3
Underlined        4
Blink             5
Reverse           7
Hidden            8
```


* **Foreground color**
```
Black             30
Red               31
Green             32
Yellow            33
Blue              34
Magenta           35
Cyan              36
 Light gray       37
Default           39
Dark gray         90
Light red         91
Light green       92
Light yellow      93
Light blue        94
Light magenta     95
Light cyan        96
White             97
```


* **Background color**
```
Black             40
Red               41
Green             42
Yellow            43
Blue              44
Magenta           45
Cyan              46
Light gray        47
Default           49
Dark gray         100
Light red         101
Light green       102
Light yellow      103
Light blue        104
Light magenta     105
Light cyan        106
White             107
```
