#CS/COE 447 Lab 4 Part 1 Template

#This template includes testing code, but also has some support code to check
#for a common error.

.text:
        #This is the beginning of the testing code. You should not need to alter this.

        li $a0, 0xFFFF0008      #LED memory starts at this address
        li $a1, 0x7EF965BD      #Pattern to draw. It will then be disrupted.
        jal drawPattern         #Jump and link to drawPattern, to draw an
                                #initial pattern on the display.

        #Jump and link to disruptPattern. This call should alter the display by
        #disrupting the pattern that was drawn via drawPattern. This will occur
        #so fast that you will not see the original pattern that was drawn.
        li $a0, 0xFFFF0008      #LED memory starts at this address
        jal disruptPattern

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
	beq $a3,0x00,jump
	addi $a0,$a0,1
	j draw
jump:
	jr $ra


#========================================
# * DO NOT ALTER THIS NEXT LINE         *
j returnErrorHappened
#========================================




#========================================
# * Place your getPattern code here     *
#========================================
    getPattern:
        lw $v0,($a2)
	jr $ra



#========================================
# * DO NOT ALTER THIS NEXT LINE         *
j returnErrorHappened
#========================================




#========================================
# * Place your disruptPattern code here *
#========================================
    disruptPattern:
    move $s0,$ra
       jal getPattern
       li $v1, 0xF0F009F0
       xor $a1, $v0,$v1 
       jal drawPattern
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
