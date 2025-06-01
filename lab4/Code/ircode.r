FUNCTION hanoi :
PARAM n
PARAM p1
PARAM p2
PARAM p3
t0 := n
t1 := #1
IF t0 == t1 GOTO label0
GOTO label1
IF t2 != #0 GOTO label0
GOTO label1
LABEL label0 :
t6 := p1
t7 := #1000000
t4 := t6 * t7
t5 := p3
t3 := t4 + t5
WRITE t3
GOTO label2
LABEL label1 :
t9 := n
t10 := #1
t8 := t9 - t10
ARG t8
t11 := p1
ARG t11
t12 := p3
ARG t12
t13 := p2
ARG t13
t14 := CALL hanoi
t18 := p1
t19 := #1000000
t16 := t18 * t19
t17 := p3
t15 := t16 + t17
WRITE t15
t21 := n
t22 := #1
t20 := t21 - t22
ARG t20
t23 := p2
ARG t23
t24 := p1
ARG t24
t25 := p3
ARG t25
t26 := CALL hanoi
LABEL label2 :
t27 := #0
RETURN t27
FUNCTION main :
sum := #3
t28 := sum
ARG t28
t29 := #1
ARG t29
t30 := #2
ARG t30
t31 := #3
ARG t31
t32 := CALL hanoi
t33 := #0
RETURN t33
