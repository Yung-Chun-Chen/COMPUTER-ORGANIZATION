main:
    addi $a0, $zero, 10 # n = 10
    addi $a1, $zero, 5  # m = 5
    jal pascal          # call pascal(10, 5)
    j exit

pascal:
    addi $sp, $sp, -16
    sw $ra, 0($sp)    # Save $ra register into stack
    sw $s0, 4($sp)
    sw $a0,8($sp)
    sw $a1,12($sp)
    beq $a0,$a1,L #a0=a1 branch to L
    beq $a1,$zero,L #a1=0 branch to L
    addi $a0,$a0,-1 #pascal(n-1,m)
    jal pascal
    
    add $s0, $zero, $v0 #s0 save value
    
   
    addi $a1,$a1,-1 #pascal(n-1,m-1)
    jal pascal
    
    add $v0, $v0, $s0 #pascal(n-1,m)+pascal(n-1,m-1)
   
exitpascal:
        lw $ra, 0($sp)  
        lw $s0, 4($sp)  
        lw $a0, 8($sp)  
        lw $a1, 12($sp)
        addi $sp, $sp, 16    
        jr $ra  #return
L:
    addi $v0,$zero,1 #v0=1
    j exitpascal
exit:
