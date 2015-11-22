.data
	a: .asciiz "What is the first value?\n"
	c: .asciiz "What is the second value?\n"
	d: .asciiz "The difference of "
	e: .asciiz " and "
	f: .asciiz " is "
	
	

.text
	li $v0,4
	la $a0,a
	syscall
	li $v0,5
	syscall
	move $t0,$v0
	li $v0,4
	la $a0,c
	syscall
	li $v0,5
	syscall
	move $t1,$v0
	sub $t2,$t0,$t1
	li $v0,4
	la $a0,d
	syscall
	li $v0,1
	add $a0,$t0,$zero
	syscall
	li $v0,4
	la $a0,e
	syscall
	li $v0,1
	add $a0,$t1,$zero
	syscall
	li $v0,4
	la $a0,f
	syscall
	li $v0,1
	add $a0,$t2,$zero
	syscall
