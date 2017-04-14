**Read [extend-qtcreator-go-back-steps-by-patching](https://bitbucket.org/snippets/so61pi/LqEXe)**


**Search for locator widget width 730 (measured by screenshot), we get here**

```C
// https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/locator/locatorwidget.cpp#L216
m_preferredSize = QSize(730, shint.height() * 17 + frameWidth() * 2);
```


**Load `libCore.so` into IDA**

We cannot search for containing function `CompletionList::updatePreferredSize` so we find nearby functions then check their callers.

Search `itemDelegate` and find its callers, we get here
```
.text:00000000001D8059                 call    QAbstractItemView::itemDelegate(void)
...
.text:00000000001D80DA                 mov     dword ptr [rbp+30h], 2DAh
```

Now we can change `2DAh` (730) to something bigger but that not enough because in `CompletionList::CompletionList` the maximum widget width is limited to 900 by calling `setMaximumWidth(900);`
```C
// https://github.com/qtproject/qt-creator/blob/v4.1.0/src/plugins/coreplugin/locator/locatorwidget.cpp#L197
setMaximumWidth(900);
```

Its assembly is right above `CompletionList::updatePreferredSize` as in C++ code.
```
.text:00000000001D7FC8                 mov     esi, 384h
.text:00000000001D7FCD                 mov     rdi, rbx
.text:00000000001D7FD0                 call    QWidget::setMaximumWidth(int)
```

After patching this, the widget becomes bigger as we want it.


**Search pattern for patching**

`2DAh` (700) patch search pattern
```
C7 45 30 DA 02 00 00 89 45 34 E8 47 BF EB FF 48 8B BC 24 E8 00 00 00
```

`384h` (900) patch search pattern
```
BE 84 03 00 00 48 89 DF E8 6B DD EB FF 48 89 DF E8 C3 02 EC FF
```
