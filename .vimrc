set nocompatible
filetype off
filetype plugin indent on
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'gmarik/Vundle.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'jistr/vim-nerdtree-tabs'
Plugin 'Valloric/YouCompleteMe'
Plugin 'fholgado/minibufexpl.vim'
Plugin 'wincent/Command-T'
Plugin 'scrooloose/syntastic'
Plugin 'altercation/vim-colors-solarized'
Plugin 'SirVer/ultisnips'
Plugin 'honza/vim-snippets'
Plugin 'rgarver/Kwbd.vim'
call vundle#end()
filetype plugin indent on

syntax enable
set background=dark
colorscheme solarized

set completeopt-=preview
set tabstop=2
set softtabstop=2
set shiftwidth=2
set expandtab
set colorcolumn=110

set wildignore+=build
set wildignore+=cmake
set wildignore+=lib
set wildignore+=assets
let g:pymode_rope_complete_on_dot = 0

if &term =~ "xterm" || &term =~ "screen"
  let g:CommandTCancelMap = ['<ESC>', '<C-c>']
endif

map <Leader>n :NERDTreeTabsToggle<CR>
map <Leader>q :Kwbd<CR>
map <Leader>w :w<CR>
set clipboard=unnamedplus

function! g:UltiSnips_Complete()
  call UltiSnips_ExpandSnippet()
  if g:ulti_expand_res == 0
    if pumvisible()
      return "\<C-n>"
    else
      call UltiSnips_JumpForwards()
      if g:ulti_jump_forwards_res == 0
        return "\<TAB>"
      endif
    endif
  endif
  return ""
endfunction

au BufEnter * exec "inoremap <silent> " . g:UltiSnipsExpandTrigger . " <C-R>=g:UltiSnips_Complete()<cr>"
set history=1000         " remember more commands and search history
set undolevels=1000      " use many muchos levels of undo
set autoindent    " always set autoindenting on
set copyindent    " copy the previous indentation on autoindenting
set encoding=utf-8
set scrolloff=3
