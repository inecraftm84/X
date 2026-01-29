using ./so/web.so
using ./so/jump.so
using ./so/math.so
using ./so/sleep.so
using ./so/print.so

WEB_LISTEN 8080
set counter 0
:LOOP
counter + 1 = counter
print counter
sleep 100
GOTO LOOP