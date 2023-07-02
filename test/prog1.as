	lw	1,0,ten
	lw	2,1,0
	addi    5,1,5
loop	addi	2,2,-1
	addi	1,1,-1
	slti    3,2,10
	slti    4,1,5
	beq	3,4,loop
	lui 3,100
	halt
ten	.fill	10