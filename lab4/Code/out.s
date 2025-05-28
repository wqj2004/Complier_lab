.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
x: .word 20
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

fact:
  li $t0, 1
  li $t1, 1
label0:
  move $t2, $t1
  move $t3, $a0
  ble $t2, $t3, label1
  j label2
label1:
  move $t4, $t0
  move $t5, $t1
  mul $t6, $t4, $t5
  move $t0, $t6
  move $t7, $t1
  li $s0, 1
  add $s1, $t7, $s0
  move $t1, $s1
  j label0
label2:
  move $s2, $t0
  move $v0, $s2
  jr $ra

main:
  li $t0, 0
  li $t1, 5
label3:
  move $t2, $t0
  move $t3, $t1
  blt $t2, $t3, label4
  j label5
label4:
  move $t4, $t5
  move $t6, $t0
  mul $t7, $t6, $s0
  add $s1, $t4, $t7
  move $s2, $t0
  li $s3, 2
  add $s4, $s2, $s3
  lw $s1, 0($s4)
  move $s5, $t5
  move $s6, $t0
  mul $s7, $s6, $s0
  add $t8, $s5, $s7
  sw $t8, 0($t9)
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
  move $a0, $t9
  jal fact
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
  move $s0, $v0
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t2, $t0
  li $t3, 1
  add $t4, $t2, $t3
  move $t0, $t4
  j label3
label5:
  li $t6, 0
  move $v0, $t6
  jr $ra
