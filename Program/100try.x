using ./so/math.so
using ./so/jump.so
using ./so/time.so
using ./so/print.so
set k 0
start_timer

:LOOP
k + 1 = k
if k < 100000 GOTO LOOP
print k
stop_timer