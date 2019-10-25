":autocmd FileType cpp :nnoremap <F6> :w<CR>:!clear;g++ -Wall -std=c++11 -c %;g++ -Wall -std=c++11 -o %:r.out %:r.o;perl lexer_file.pl;./%:r.out<CR>
:autocmd FileType cpp :nnoremap <F6> :w<CR>:!clear;export PERL5LIB=/home/boechat/modules/lib/perl5;make clean;make all;./%:r.out<CR>
