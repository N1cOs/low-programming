global _start

section .data
correct: dq -1
section .text

_start:
jmp _start
