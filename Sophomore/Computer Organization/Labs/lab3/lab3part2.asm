.data
	string:
		.space 64
	a:	.asciiz "Please enter your string:\n"
	c:	.asciiz "Here is the output: "

.text

la $a0,a
li $v0,4
syscall



li $v0, 8
la $a0, string
li $a1,64
syscall
move $t0,$a0

li $t2, 0
li $t3,0
loop:
	lb $t1, 0($t0)
	beq $t1,$t2, END
	beq $t1,0x0a,space
	bgt $t1,0x60, upper
	beq $t1,0x20,space
	
	blt $t1,0x61,lower
lower:
	add $t1,$t1,0x20
	sb $t1,0($t0)
	addi $t3, $t3, 1
	addi $t0, $t0, 1
	j loop
upper:
	sub $t1,$t1,0x20
	sb $t1,0($t0)
	addi $t3, $t3, 1
	addi $t0, $t0, 1
	j loop
space:

	addi $t3, $t3, 1
	addi $t0, $t0, 1
	j loop
END:

	la $a0,c
	li $v0,4
	syscall
	la $a0,string
	li $v0,4
	syscall
	
	li $v0,10
	syscall
	
	



