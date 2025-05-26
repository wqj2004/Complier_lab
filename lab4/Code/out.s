FUNCTION main :
a := #0
READ t0
n := t0
LABEL label0 :
t1 := i
t2 := n
IF t1 < t2 GOTO label1
GOTO label2
LABEL label1 :
t3 := a
t4 := b
c := t3 + t4
t5 := b
WRITE t5
t6 := b
a := t6
t7 := c
b := t7
t9 := i
t10 := #1
t8 := t9 + t10
i := t8
GOTO label0
LABEL label2 :
t11 := #0
RETURN t11
