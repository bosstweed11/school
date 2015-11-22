#CS/COE 447 Lab 4 Part 2 Template

#This template includes testing code, but also has some support code to check
#for a common error.

.text:
        #This is the beginning of the testing code. 

		# you may put additional instructions to calculate the addresses and bit patterns

	li $a0, 0xFFFF000F	# replace your_address with the actual address
        li $a1, 0x7EF965BD  # replace your_pattern with the actual pattern
        li $s2, 7               #Draw the pattern 7 times vertically.
        jal drawRepeatedPattern #Jump and link to drawRepeatedPattern.

	
	li $a0, 0xFFFF003C	# replace your_address with the actual address
	li $a1,0x8E096C4D   # replace your_pattern with the actual pattern
        li $s2, 7               #Draw the pattern 7 times vertically.
        jal drawRepeatedPattern #Jump and link to drawRepeatedPattern.

		# do not alter 
        la $a0, successfulQuitMessage
        li $v0, 4
        syscall

        li $v0, 10             #Exit syscall
        syscall

        #This is the end of the testing code.

#========================================
# * Place your drawPattern code here    *
#========================================
    drawPattern:
    	move $a3,$a1
    	move $a2,$a0
  	
    draw:
        sb $a3, ($a0)
	srl $a3,$a3,8
	addi $a0,$a0,1
	beq $a3,0x00,jump
	
	j draw
jump:
	jr $ra



#========================================
# * DO NOT ALTER THIS NEXT LINE         *
j returnErrorHappened
#========================================




#========================================
# * Place drawRepeatedPattern code here *
#========================================
    drawRepeatedPattern:
    	move $s0,$ra
    drawRepeated:
        subi $s2,$s2,1
        jal drawPattern
        beqz $s2,jump2
        addi $a0,$a0,0x1c
       	j drawRepeated
    	jump2:
       	jr $s0



#========================================
# * DO NOT ALTER THIS NEXT LINE         *
j returnErrorHappened
#========================================


returnErrorHappened:
    #If this code is executed, your function did not properly return.
    la $a0, badReturnMessage
    li $v0, 4
    syscall
    li $v0, 10
    syscall

.data:
    badReturnMessage:       .asciiz "A function did not properly return!"
    successfulQuitMessage:  .asciiz "The program has finished."
