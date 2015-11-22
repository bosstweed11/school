.data
	enter: .asciiz "Please enter a string: \n"
	out: .asciiz "Here is your string: \n"

	input_str: .space 64

.text
li $t0, 3		# load number of random numbers
la $a0, enter		#print prompt for string
li $v0, 4
syscall

la $a0, input_str			# user enters string
li $v0,8
li $a1,64
syscall
addi $t1,$a0,0
j length

main2:
	la $a0, out			# print output text
	li $v0,4
	syscall
	la $a0,input_str		# print changed input string
	li $v0,4
	syscall
	li $v0,10
	syscall


length:
	lb $t2, ($a0)			#load characer from inputted string
	beqz $t2, prerandom	#check if character is 0 (string is terminated)
	addi $s0,$s0,1		#increment counter
	addi $a0,$a0,1		#increment indice of inputted string
	j length
	
prerandom:
	subi $s0,$s0,1
random:
	
	move $a0,$s0
	add $a1,$s0,$0				#generate random indice for * symbol						
	li $v0, 42				
	syscall
	move $t3,$a0				# save random indice
	j swap

swap:
	la $t1,input_str				#load input string
	li $t6,0x2a					#load asterisk in $t6
	add $t1,$t1,$t3				#compute indice of input string
	lb $t4,($t1)					#load byte, check if already asterisk if so repeat , else put asterisk in
	beq $t4,0x2a,random
	sb,$t6,($t1)
	subi $t0,$t0,1				# decrement number of asterisks to go
	beqz $t0,main2
	
	j random
