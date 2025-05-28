.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
x: .space 20
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

main:
  li $t0, 0
  li $t1, 5
label0:
  move $t2, $t0
  move $t3, $t1
  blt $t2, $t3, label1
  j label2
label1:
  move $t4, $t5
  move $t6, $t0
  mul $t7, $t6, $s0
  add $s1, $t4, $t7
  move $s2, $t0
  li $s3, 2
  add $s4, $s2, $s3
  lw $s1, 0($s4)
  j label0
label2:
  li $s5, 0
  move $v0, $s5
  jr $ra
