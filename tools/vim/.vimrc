"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 一般设定
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 设定默认解码
set fenc=utf-8
set fencs=utf-8,usc-bom,euc-jp,gb18030,gbk,gb2312,cp936

" 不要使用vi的键盘模式，而是vim自己的
set nocompatible

" history文件中需要记录的行数
set history=100

" 在处理未保存或只读文件的时候，弹出确认
set confirm
" 侦测文件类型
filetype on

" 载入文件类型插件
filetype plugin on

" 语法高亮
syntax on

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Display settings section
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set ruler
set showmatch
set showmode
set wildmenu
set wildmode=longest:full,full
if version >= 703
    set colorcolumn=80,100
endif
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 文件设置
" """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" " 不要备份文件
set nobackup
"
" " 不要生成swap文件，当buffer被丢弃的时候隐藏它
setlocal noswapfile
set bufhidden=hide

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 搜索和匹配
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 高亮显示匹配的括号
set showmatch
" support gnu syntaxt
let c_gnu = 1
"
" show error for mixed tab-space
let c_space_errors = 1
" let c_no_tab_space_error = 1
"
" don't show gcc statement expression ({x, y;}) as error
let c_no_curly_error = 1
""""""""""""""""""""""""""""""
" => Statusline
""""""""""""""""""""""""""""""
"Format the statusline
" Nice statusbar
set laststatus=2
set statusline=
set statusline+=%2*%-3.3n%0*\ " buffer number
set statusline+=%f\ " file name
set statusline+=%h%1*%m%r%w%0* " flag
set statusline+=[
if v:version >= 600
set statusline+=%{strlen(&ft)?&ft:'none'}, " filetype
set statusline+=%{&encoding}, " encoding
endif
set statusline+=%{&fileformat}] " file format
if filereadable(expand("$VIMimfiles/pluginimbuddy.vim"))
set statusline+=\ %{VimBuddy()} " vim buddy
endif
set statusline+=%= " right align
set statusline+=%2*0x%-8B\ " current char
set statusline+=%-14.(%l,%c%V%)\ %<%P " offset

" 在状态行上显示光标所在位置的行号和列号
set ruler
set rulerformat=%20(%2*%<%f%=\ %m%r\ %3l\ %c\ %p%%%)

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 文本格式和排版
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" " 自动格式化
set formatoptions=tcrqn
"
" " 继承前一行的缩进方式，特别适用于多行注释
set autoindent
"
" " 为C程序提供自动缩进
set smartindent
"
" " 使用C样式的缩进
set cindent
"
" " 制表符为4
set tabstop=4
"
" " 统一缩进为4
set softtabstop=4
set shiftwidth=4
"

" input settings
set backspace=2
set smarttab
" set softtabstop=4
set expandtab " expand tab to spaces
set cinoptions=:0,g0,t0,(0,Ws,m1
"
" " search settings
set hlsearch
set incsearch
set smartcase

" 用空格键来开关折叠
set foldenable
set foldmethod=manual
nnoremap <space> @=((foldclosed(line('.')) < 0) ? 'zc':'zo')<CR>
""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Key mappings section
""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" alt .h .cpp
map <F1> :A<CR>

" for Tlist
nnoremap <silent> <F2> :TlistToggle<CR>
""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" function
""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" auto insert header
function SetAuthorComment()
    call setline(1, "\/\/ Copyright (c) ".strftime("%Y").", HelightXu")
    call setline(2, "\/\/ Author: Zhwen Xu<HelightXu@gmail.com>")
    call setline(3, "\/\/ Created: ".strftime("%Y-%m-%d"))
    call setline(4, "\/\/ Description:")
    call setline(5, "\/\/")
endfunction
" auto insert gtest header inclusion for test source file
autocmd BufNewFile *_test.{cpp,cxx,cc} nested :normal i#include "gtest/gtest.h"
autocmd BufNewFile *.{c,cpp,cxx,cc} nested call SetAuthorComment()
" auto insert header
function SetAuthorCommentForScript()
    call setline(1, "# Copyright (c) ".strftime("%Y").", HelightXu")
    call setline(2, "# Author: Zhwen Xu<HelightXu@gmail.com>")
    call setline(3, "# Created: ".strftime("%Y-%m-%d"))
    call setline(4, "# Description:")
    call setline(5, "#")
endfunction
autocmd BufNewFile *.{py,sh} nested call SetAuthorCommentForScript()

" auto insert gtest header inclusion for test source file
function! s:InsertHeaderGuard()
   " call SetAuthorComment()
    let fullname = expand("%:p")
    let rootdir = FindProjectRootDir()
    if rootdir != ""
        let path = substitute(fullname, "^" . rootdir . "/", "", "")
    else
        let path = expand("%")
    endif
    let varname = toupper(substitute(path, "[^a-zA-Z0-9]", "_", "g"))
    exec 'norm O#ifndef ' . varname
    exec 'norm o#define ' . varname
    exec 'norm o#pragma once'
    exec '$norm o#endif // ' . varname
endfunction
autocmd BufNewFile *.{h,hh.hxx,hpp} nested call <SID>InsertHeaderGuard()


" locate project dir by README.md file
functio! FindProjectRootDir()
    let rootfile = findfile("README.md", ".;")
    " in project root dir
    if rootfile == "README.md"
        return ""
    endif
    return substitute(rootfile, "/README.md$", "", "")
endfunction

" remove trailing spaces
function! RemoveTrailingSpace()
    if $VIM_HATE_SPACE_ERRORS != '0'
        normal m`
        silent! :%s/\s\+$//e
        normal ``
    endif
endfunction
autocmd BufWritePre * nested call RemoveTrailingSpace()
" last open line
autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") |
            \ exe "normal! g`\"" | endif

" MiniBufExplorer
let g:miniBufExplMapWindowNavArrows = 1
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplMapWindowNavVim = 1
