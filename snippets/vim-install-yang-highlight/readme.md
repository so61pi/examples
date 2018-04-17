1. Download [yang.vim](http://www.yang-central.org/twiki/pub/Main/YangTools/yang.vim) and put it in:
    - For current user: `~/.vim/syntax/`
    - For everyone: `$VIM/vimfiles/syntax/`
        - The expanded path can be checked by command (type it in vim): `:echo expand('$VIM/vimfiles/syntax')`


2. Create `~/.vim/ftdetect/yang.vim` with below content:

        au BufRead,BufNewFile *.yang set filetype=yang

