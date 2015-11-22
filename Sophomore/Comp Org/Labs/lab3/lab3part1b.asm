.data
#.byte 0x23,0x45,0x67,0x19
a: .asciiz "Please Enter your integer:\n"
c: .asciiz "Here is the output: "
.word 
.text

#Enter integer
la $a0, a
li $v0, 4
syscall

#retreive integer
li $v0,5
syscall
#save input
move $t0,$v0

#print output
la $a0, c
li $v0, 4
syscall 
#bitwise and with appropriate mask
andi $t1,$t0, 0x001c0000
#shift right to get the 3 selected bits
srl $a0,$t1,18

#print int
li $v0,1
syscall

#end program
li $v0,10
syscall
