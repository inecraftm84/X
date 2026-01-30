using ./so/math.so
using ./so/jump.so
using ./so/time.so
using ./so/print.so
set k 0
start_timer

:LOOP
k + 1 = k
<<<<<<< HEAD:Program/100try.x
if k < 100000 GOTO LOOP
=======
if k < 1 GOTO LOOP

stop_timer
>>>>>>> parent of c8a6105 (web):test.x
print k
stop_timer