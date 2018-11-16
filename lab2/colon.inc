%define next_word 0

%macro colon 2
%%entry: dq next_word
db %1, 0
%2:
%define next_word %%entry
%endmacro
