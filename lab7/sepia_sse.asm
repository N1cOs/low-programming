section .text

global sepia_one_sse
global set_coeffs

;rdi - c1, rsi - c2, rdx - c3
set_coeffs:
  movdqa xmm3, [rdi]
  movdqa xmm4, [rsi]
  movdqa xmm5, [rdx]
  ret

;rdi - blue, rsi - green, rdx - red, rcx - output
sepia_one_sse:
  movdqa xmm0, [rdi]
  movdqa xmm1, [rsi]
  movdqa xmm2, [rdx]

  mulps xmm0, xmm3
  mulps xmm1, xmm4
  mulps xmm2, xmm5

  addps xmm0, xmm1
  addps xmm0, xmm2
  movdqa [rcx], xmm0
  ret
