global _start
extern read_word
extern find_word
extern string_length
extern print_string
extern print_newline
extern exit

section .text
%include "colon.inc"

section .data
msg: db "No such word in the dictionary", 0
buffer: times 256 db 0x00
%include "words.inc"

section .text
_start:
  xor r12, r12
  mov rdi, buffer
  mov rsi, 256
  call read_word
  mov rdi, rax
  mov rsi, next_word
  xor rcx, rcx
  inc rcx
.next_key:
  mov rsi, next_word
  xor rax, rax
.loop:
  mov dl, byte[rdi + rcx]
  push rdx ;save the last symbol of the string
  mov byte[rdi + rcx], 0
  push rsi
  call find_word
  pop rsi
  cmp rax, 0
  je .next_symbol
  push rdi
  add rax, 8
  mov rdi, rax
  call string_length
  add rdi, rax
  inc rdi
  push rcx
  call print_string
  pop rcx
  pop rdi
  pop rdx
  mov byte[rdi + rcx], dl
  add rdi, rcx
  xor rcx, rcx
  inc rcx
  inc r12
  jmp .next_key
.next_symbol:
  pop rdx
  mov byte[rdi + rcx], dl
  test rdx, rdx
  jz .last_symbol
  inc rcx
  jmp .loop
.last_symbol:
  test r12, r12
  jnz .end
  mov rdi, msg
  call string_length
  mov rsi, rdi
  mov rdi, 2
  mov rdx, rax
  mov rax, 1
  syscall
.end:
  call exit
