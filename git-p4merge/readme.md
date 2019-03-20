- Put this to `p4v-2018.4.1753667/lib/P4VResources/skins/Default.qss` after backing up the old file.

```css
LightweightTextView
{
    color: #000000;
    background-color: #ffffff;
}

QTextEdit[readOnly="true"] 
{ 
    color: #000000;
    background-color: transparent;
}

QTextEdit[readOnly="false"]
{
    color: #000000;
    background-color: #ffffff;
}

LineNumbers
{
    color: #000000;
    background-color: #ffffff;
}
```

- Put this to `~/.gitconfig`.

```
[difftool]
        prompt = false
[merge]
        tool = p4merge
[diff]
        tool = p4merge
[mergetool "p4merge"]
        cmd = p4merge "$BASE" "$LOCAL" "$REMOTE" "$MERGED"
        trustExitCode = true
[difftool "p4merge"]
        cmd = p4merge "$LOCAL" "$REMOTE"
```