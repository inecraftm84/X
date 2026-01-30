using ./so/web.so
using ./so/jump.so
using ./so/math.so
using ./so/sleep.so
using ./so/print.so
using ./so/time.so

WEB_LISTEN 8080
set counter 0

:LOOP
start_timer

counter + 1 = counter
print counter

stop_timer

sleep 100

GOTO LOOP