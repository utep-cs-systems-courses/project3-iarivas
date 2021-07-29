	.arch msp430g2553
	.p2align 1, 0
	.data
	.extern switch_state

jt:
	.word case0
	.word case1
	.word case2
	.word case3
	.text
	.global switch_advance

switch_advance:
	cmp	#4, r12 	
	jc	end		
	add	r12, r12
	mov	jt(r12), r0
case0:
	mov #0, &switch_state
	jmp end
case1:
	mov #1, &switch_state
	jmp end
case2:
	mov #2, &switch_state
	jmp end
case3:
	mov #3, &switch_state
	jmp end
end:
	ret
