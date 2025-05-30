.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
array: .space 12
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

squaRever:
  li $t0, 0
  li $t1, 0
  la $t3, array
  move $t2, $t3
  li $t4, 0
  li $t6, 4
  mul $t5, $t4, $t6
  add $t7, $t2, $t5
  move $s0, $a0
  li $s1, 100
  div $s0, $s1
  mflo $s2
  sw $s2, 0($t7)
  move $s3, $t3
  li $s4, 1
  li $t6, 4
  mul $s5, $s4, $t6
  add $s6, $s3, $s5
  move $s7, $a0
  li $t8, 10
  div $s7, $t8
  mflo $t9
  li $t6, 10
  move $t2, $t3
  li $t4, 0
  li $t7, 4
  mul $t5, $t4, $t7
  add $s0, $t2, $t5
  lw $t7, 0($s0)
  mul $s1, $t6, $t7
  sub $s2, $t9, $s1
  sw $s2, 0($s6)
  move $s3, $t3
  li $s4, 2
  li $s6, 4
  mul $s5, $s4, $s6
  add $s7, $s3, $s5
  move $s6, $a0
  li $t8, 100
  move $t9, $t3
  li $t6, 0
  li $t4, 4
  mul $t2, $t6, $t4
  add $t5, $t9, $t2
  lw $t4, 0($t5)
  mul $s0, $t8, $t4
  sub $t7, $s6, $s0
  li $s1, 10
  move $s2, $t3
  li $s3, 1
  li $s5, 4
  mul $s4, $s3, $s5
  add $s7, $s2, $s4
  lw $s5, 0($s7)
  mul $s6, $s1, $s5
  sub $t8, $t7, $s6
  sw $t8, 0($t9)
  move $t6, $t3
  li $t2, 0
  li $t4, 4
  mul $t5, $t2, $t4
  add $s0, $t6, $t5
  lw $t4, 0($s0)
  move $t7, $t3
  li $s1, 2
  li $s3, 4
  mul $s2, $s1, $s3
  add $s4, $t7, $s2
  lw $s3, 0($s4)
  bne $t4, $s3, label0
  j label1
label0:
  li $s7, 0
  move $t0, $s7
  j label2
label1:
label3:
  move $s5, $t1
  li $s6, 12
  blt $s5, $s6, label4
  j label5
label4:
  move $t8, $t1
  move $t9, $t1
  mul $t6, $t8, $t9
  move $t2, $a0
  sub $t5, $t6, $t2
  li $s0, 0
  beq $t5, $s0, label6
  j label7
label6:
  li $t4, 1
  move $t0, $t4
  j label8
label7:
label8:
  move $t7, $t1
  li $s1, 1
  add $s2, $t7, $s1
  move $t1, $s2
  j label3
label5:
label2:
  move $s4, $t0
  li $s3, 1
  beq $s4, $s3, label9
  j label10
label9:
  li $s7, 1
  move $v0, $s7
  jr $ra
  j label11
label10:
  li $s5, 0
  move $v0, $s5
  jr $ra
label11:

main:
  li $t0, 100
label12:
  move $t1, $t0
  li $t2, 150
  blt $t1, $t2, label13
  j label14
label13:
  move $t3, $t0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  addi $sp, $sp, -72
  sw $t0, 0($sp)
  sw $t1, 4($sp)
  sw $t2, 8($sp)
  sw $t3, 12($sp)
  sw $t4, 16($sp)
  sw $t5, 20($sp)
  sw $t6, 24($sp)
  sw $t7, 28($sp)
  sw $s0, 32($sp)
  sw $s1, 36($sp)
  sw $s2, 40($sp)
  sw $s3, 44($sp)
  sw $s4, 48($sp)
  sw $s5, 52($sp)
  sw $s6, 56($sp)
  sw $s7, 60($sp)
  sw $t8, 64($sp)
  sw $t9, 68($sp)
  move $a0, $t3
  jal squaRever
  addi $sp, $sp, 0
  lw $t0, 0($sp)
  lw $t1, 4($sp)
  lw $t2, 8($sp)
  lw $t3, 12($sp)
  lw $t4, 16($sp)
  lw $t5, 20($sp)
  lw $t6, 24($sp)
  lw $t7, 28($sp)
  lw $s0, 32($sp)
  lw $s1, 36($sp)
  lw $s2, 40($sp)
  lw $s3, 44($sp)
  lw $s4, 48($sp)
  lw $s5, 52($sp)
  lw $s6, 56($sp)
  lw $s7, 60($sp)
  lw $t8, 64($sp)
  lw $t9, 68($sp)
  addi $sp, $sp, 72
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t4, $v0
  li $t5, 1
  beq $t4, $t5, label15
  j label16
label15:
  move $t6, $t0
  move $a0, $t6
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  j label17
label16:
label17:
  move $t7, $t0
  li $s0, 1
  add $s1, $t7, $s0
  move $t0, $s1
  j label12
label14:
  li $s2, 0
  move $v0, $s2
  jr $ra
