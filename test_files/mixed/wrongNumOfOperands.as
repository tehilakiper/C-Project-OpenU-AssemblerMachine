;this file will include errors- wrong number of operands
;to many operands for mov
mov #3, @r1, @r5, A
sub @r2, @r5
X: .string "ngh"
;to many operands for lea
lea Y, @r7, @r3
;to many operands for not
not W, @r4
red X
Y: .data -6, 14   ,   9    ,-32,5
;to many operands for clr
clr Y, Z
.extern W
;to many operands for prn
A: prn 4, -68
.entry Z
;to many operands for stop
stop @r6
;to many operands for rts
rts 7, -2
.entry A
;to little operands for add
add @r5
;to little operands for cmp
cmp 
;to little operands for mov
mov Z
;to little operands for jmp
jmp
;to little operands for jsr
jsr
;to little operands for inc
inc 
Z: .string "hijfi3nfn=*"
