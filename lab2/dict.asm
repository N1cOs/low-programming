extern string_equals
global find_word

section .text
;rdi - pointer to the key string
;rsi - pointer to the last word in the dictionary
find_word:
  push r12
.loop:
  cmp rsi, 0
  je .end
  mov r12, rsi
  add rsi, 8
  call string_equals
  cmp rax, 0
  je .next_word
  mov rax, r12
  jmp .end
.next_word:
  mov rsi, qword[r12]
  jmp .loop
.end:
  pop r12
  ret
