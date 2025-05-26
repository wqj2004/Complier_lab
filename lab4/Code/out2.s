.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
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
  li $t0, 0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $t1, $v0
  move $t2, $t1
label0:
  move $t3, $t4
  move $t5, $t2
  blt $t3, $t5, label1
  j label2
label1:
  move $t6, $t0
  move $t7, $s0
  add $s1, $t6, $t7
  move $s2, $s0
  move $a0, $s2
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  move $s3, $s0
  move $t0, $s3
  move $s4, $s1
  move $s0, $s4
  move $s5, $t4
  li $s6, 1
  add $s7, $s5, $s6
  move $t4, $s7
  j label0
label2:
  li $t8, 0
  move $v0, $t8
  jr $ra
