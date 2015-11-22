.data 
	wel: .asciiz "Welcome to Word Soup!\n"
	game:  .asciiz "I am thinking of a word.  The word is  "
	game2: .asciiz "The word is "
	sco: .asciiz "  Round score is "
	guess: .asciiz "\nGuess any letter a-z\nenter a '.' to forfeit\nenter a '?' for a hint\nenter a'!' if you think you can solve the word on one guess\nGuess a letter?\n"
	yes: .asciiz "\nYes! "
	no: .asciiz "\nNo! "
	end1: .asciiz "\nYour final game tally is "
	end2: .asciiz ". Goodbye!"
	win: .asciiz "You won the round! Your final guess was: \n"
	lose: .asciiz "You lost the round!"
	corr: .asciiz "\nCorrect word was: \n"
	forf: .asciiz "\nYou have chosen to give up on the current word, and are awarded 0 points\n"
	score: .asciiz "\nYour round score was " 
	gam: .asciiz " The game tally is "
	again: .asciiz "\nDo you want to play again (y/n)?\n"
	fguess: .asciiz "\nYou have chosen to guess the entire word at one shot, \nif you are not correct you lose, if you are you win, proceed with your guess:\n"
	w1: .asciiz "computer"
	w2: .asciiz "engineering"
	w3: .asciiz "processor"
	w4: .asciiz "transistor"
	w5: .asciiz "resistor"
	w6: .asciiz "capacitor"
	w7: .asciiz "inductor"
	w8: .asciiz "electricity"
	w9: .asciiz "programming"
	w10: .asciiz "type"
.align 2 
	begin: .word wel,game,sco,guess
	words:  .word w1,w2,w3,w4,w5,w6,w7,w8,w9,w10
	mainloop: .word game2,sco,guess
	endloop: .word corr,score,gam,again,end1
	g:  .space 4
	str: .space 64
	fulguess: .space 64
.text 
la $fp, endloop
beginning:li $s0,0
	li $a3,0
	li $s1,0
	li $s3,0
	li $t6,0
	li $k1,3					
	la $t1, begin				#$t1 pointer for table look up of intro strings
	addi $t2, $t1,0			#$t2 is a copy of the beginning of the table										
text:	lw $a0, 0($t1)			#print string from table depending on where you are on in the program(1st, 1st, 2nd,2nd,etc)
	li $v0,4
	syscall
	addi $t1,$t1,4			#increment the table index to get to the next string
	sub $t3,$t1,$t2			#take the difference of $t1 and $t2 to see how many times the loop has been run through
	beq $t3,8,word			#branch to word if run through twice
	beq $t3,12,currscore		#branch tocurrscore if run through 3 times
	beq $t3,16,input			#branch to input if run through 4 times			
	j text					#loop back
j main
premain:beq $s7, 1,yesl
	beqz $s7,nol
backpremain:la $t1, mainloop		#load main words pointer
	li $t3,0					#set counter to 0	
	j main	
yesl:addi $s3,$s3,1 			#counter for correct loop
	j pregoodend
repeated:la $a0, yes		#get print yes word
	li $v0,4
	syscall
	j backpremain
nol:subi $s1,$s1,1			#decrement score	
	beqz $s1,badend
	la $a0,no
	li $v0,4
	syscall
	j backpremain
main:lw $a0, ($t1)				#load partially guessed word
	li $v0,4			
	syscall
	addi $t1,$t1,4			#increment main words
	addi $t3,$t3,1			#increment counter
	beq $t3,1,printword		#if 1 print partially guessed word
	beq $t3,2,currscore		#print score if 2
	beq $t3,3,input			#if 3 go for input char
printword:beq $s0,0,word
	la $a0, str
	syscall
	j main
word:la $t0,words			#$t0 contains the pointer for the table look up of words
	li $v0,30
	syscall
	move $a1,$a0
	li $a0,1
	li $v0,40
	syscall
	li $a1, 9					#generate random number from 0-11 (number of words)
	li $v0, 42				
	syscall
	move $t4,$a0			#save random number in $t4
	bnez $t9,continueword #if first time, choose computer as the word
computerword:addi $t4,$0,0
continueword:sll $t4,$t4,2				#multiply the number by 4 for next index in words
	add $t0,$t4,$t0			#add offset to $t0 for the correct index				
precount:lw $t7,0($t0)			#$t7 gets address of pointer of current word
	li $t9, 0x5f
	li $k0,0x20
	la $t8, str				#$t8 gets pointer to str
count:lb $t5, 0($t7)				#extract bit by bit for length of word
	beqz $t5, printlength
	sb $t9, 0($t8)			#while extracting fill trick word with 
	sb $k0,1($t8)	
	addi $t6, $t6,1			#increment counter	
	addi $t7,$t7,1			#next character
	addi $t8,$t8,2			#next digit in str
	j count					#repeat
printlength:move $s1,$t6
	move $t5,$t6
	srl $t6,$t6,1
	add $a1, $0,$t6					#generate random number from 0-length/2 (number of words)
	li $v0, 42				
	syscall
	move $t6,$a0
	move $a3,$t6
	bnez $a0, random
prilength:la $a0,str				#load ?string into $a0 to print
	li $v0, 4
	syscall
	bne $s0,0,main		#if not first time repeat, else text
	j text
random:li $a0,1
	add $a1,$s1,$0		#generate random indice for * symbol
	li $v0, 42				
	syscall
	sll $a0,$a0,1
	la $a1, str
	add $a1,$a1,$a0
	li $t9,0x2a
	lb $k0,($a1)
	beq $k0,0x2a,random # if indice has *, retry
	sb $t9,($a1)
	subi $t6,$t6,1
	beqz $t6,prilength	# when done print word
	j random
currscore:move $a0, $s1		#print out score
	li $v0,1
	syscall
	bne $s0,0,main		#if not first time go back to main
	j text			#else back to initial loop
input:li $v0,8
	la $a0, g
	li $a1, 4
	syscall
	move $s0, $a0 	#save input character to $s0
	lb $a0,($a0)
	beq $a0,0x3f,prehint		# branch to hint function
	beq $a0,0x2e,forfeit		# branch to forfeit functon
	beq $a0,0x21,fullguess    # branch to guess entire word
	j compare
prehint:li $t9,0				#initialize count to 0
	la $s6,($t0)			#get address of pointer for word
	lw $s6,($s6)			#get word
hint:beqz $k1,backpremain
	la $t8,str
currhint:lb $s4,($t8)
	lb $s5,($s6)
	beq $s4,0x5f,showhint	# first underscore, show letter for 
	addi $s6,$s6,1
	addi $t8,$t8,2
	addi $t9,$t9,1
	beq $t5,$t9,backpremain
	j currhint
showhint:sb $s5,($t8)
	subi $k1,$k1,1 		# decrement hint counter
	subi $s1,$s1,1
	j backpremain
fullguess:la $k0, fguess
	move $a0,$k0
	li $v0,4
	syscall
	li $v0,8
	la $a0, fulguess
	li $a1,64
	syscall
	move $t8,$a0
	li $t9,0				#initialize count to 0
	la $s6,($t0)			#get address of pointer for word
	lw $s6,($s6)			#get word
	j fullgoodend		# branch to full guess check
compare:la $t8, str		#load ? string to $t8
	lw $a2,($t0)		#load current word pointer to $a2
	li $s7,0
comp:lb $t9, ($a2)			#load byte of current word
	lb $a0,($s0)			#load byte of guess
	beq $a0,$t9, switch	#compare bytes, if same, show in ? string, else go to next character
com:addi $a2,$a2,1		#increment to next character of word
	addi $t8,$t8,2		#increment to next character of ? string
	beqz $t9,premain	#if current word is done .. byte =0 go to beginning of main loop
	j comp
switch:li $s7,1
	lb $v1,($t8)
	beq $v1,0x2a,countast
	sb $a0,($t8)			#change byte to correct guess
	j com
countast:subi $a3,$a3,1
	j com
pregoodend:li $t9,0				#initialize count to 0
	la $s6,($t0)			#get address of pointer for word
	lw $s6,($s6)			#get word
	la $t8,str			#get partial word
goodend:lb $s4,($s6)			#test byte from original word, to game word
	lb $s5,($t8)
	addi $s6,$s6,1
	addi $t8,$t8,2
	beq $t9,$t5,winround
	addi $t9,$t9,1
	bne $s4,$s5,prerepeated			#if not the same go back to main loop
	j goodend
fullgoodend:lb $s4,($s6)			#test byte from original word, to game word
	lb $s5,($t8)
	addi $s6,$s6,1
	addi $t8,$t8,1
	beq $t9,$t5,fwinround
	addi $t9,$t9,1
	bne $s4,$s5,fbadend		#if not the same go back to main loop
	j fullgoodend
prerepeated:bne $s5,0x2a,repeated
nexttest:blez $a3,goodend
	j repeated
forfeit:la $a0, forf
	li $v0,4
	syscall
	j badend
fbadend:sll $s1,$s1,1
	sub $s2,$s2,$s1
badend:la $a0, lose
	li $v0,4
	syscall
	li $s1,0
	j endround
fwinround:sll $s1,$s1,1 # times score by 2 if directly guessed correctly
winround:la $a0, win
	li $v0,4
	syscall
	la $a0, str
	li $v0,4
	syscall
	add $s2,$s2,$s1
endround1:addi $fp,$fp,4
endround:lw $a0, ($fp)
	li $v0,4
	syscall
	addi $a3,$a3,1
	beq $a3,1,printendword
	beq $a3,3,endroundscore
	beq $a3,5,endgamescore
	beq $a3,7,againentry
	beq $a3,9,endtotalscore
	j endround
printendword:la $s6,($t0)
	lw $a0,($s6)
	syscall
	j endround1
endroundscore:addi $a0,$s1,0
	li $v0,1
	syscall
	j endround1
endgamescore:addi $a0,$s2,0
	li $v0,1
	syscall
	j endround1
againentry:
	li $v0,8
	la $a0, g
	li $a1, 4
	syscall
	lb $a0,($a0)
	beq $a0,0x79,clear
	j endround1
endtotalscore:move $a0,$s2
	li $v0,1
	syscall
	li $v0 10
	syscall
clear:la $s0,str
clearing: lb $s1,($s0)
	beqz $s1,beginning
	sb,$0,($s0)
	addi $s0,$s0,1
	lb $s1,($s0)
	j clearing
