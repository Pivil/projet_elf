	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"file_1.c"
	.text
	.align	2
	.global	foo_0
	.type	foo_0, %function
foo_0:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-20]
	add	r3, r2, r3
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r3, r3, asl #2
	str	r3, [fp, #-8]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	foo_0, .-foo_0
	.align	2
	.global	foo_1
	.type	foo_1, %function
foo_1:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	mov	r3, r0
	str	r1, [fp, #-12]
	strb	r3, [fp, #-5]
	ldr	r3, [fp, #-12]
	and	r3, r3, #255
	ldrb	r2, [fp, #-5]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L3
	mov	r3, #1
	b	.L4
.L3:
	mov	r3, #0
.L4:
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	foo_1, .-foo_1
	.align	2
	.global	foo_3
	.type	foo_3, %function
foo_3:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #8]
	cmp	r3, #0
	bne	.L6
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #4]
.L6:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #4]
	add	r2, r3, #1
	ldr	r3, [fp, #-8]
	str	r2, [r3, #4]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	foo_3, .-foo_3
	.ident	"GCC: (GNU) 4.5.3"
