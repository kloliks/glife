set path+=include

map <F9> :w\|make! --no-print-directory -C build<CR>
imap <F9> <ESC>:w\|make! --no-print-directory -C build<CR>
vmap <F9> <ESC>:w\|make! --no-print-directory -C build<CR>

map <F12> :!./build/test.a<CR>
