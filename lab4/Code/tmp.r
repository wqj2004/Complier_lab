FUNCTION fact :
PARAM m
result := #1
j := #1
LABEL label0 :
t0 := j
t1 := m
IF t0 <= t1 GOTO label1
GOTO label2
LABEL label1 :
t3 := result
t4 := j
t2 := t3 * t4
result := t2
t6 := j
t7 := #1
t5 := t6 + t7
j := t5
GOTO label0
LABEL label2 :
t8 := result
RETURN t8
FUNCTION main :
DEC x 20
i := #0
N := #5
LABEL label3 :
t9 := i
t10 := N
IF t9 < t10 GOTO label4
GOTO label5
LABEL label4 :
t11 := x
t12 := i
t13 := t12 * #4
t14 := t11 + t13
t16 := i
t17 := #2
t15 := t16 + t17
*t14 := t15
t20 := x
t21 := i
t22 := t21 * #4
t23 := t20 + t22
t19 := *t23
ARG t19
t18 := CALL fact
WRITE t18
t25 := i
t26 := #1
t24 := t25 + t26
i := t24
GOTO label3
LABEL label5 :
t27 := #0
RETURN t27
