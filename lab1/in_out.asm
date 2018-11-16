global _start

section .data
msg: db "hello", 0

section .text
_start:
  mov rdi, msg
  call print_string
  call exit
