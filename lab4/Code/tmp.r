FUNCTION main :
DEC x 20
i := #0
N := #5
LABEL label0 :
t0 := i
t1 := N
IF t0 < t1 GOTO label1
GOTO label2
LABEL label1 :
t2 := x
t3 := i
t4 := t3 * #4
t5 := t2 + t4
t7 := i
t8 := #2
t6 := t7 + t8
*t5 := t6
t10 := x
t11 := i
t12 := t11 * #4
t13 := t10 + t12
t9 := *t13
i := t9
GOTO label0
LABEL label2 :
t14 := #0
RETURN t14
