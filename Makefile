exec: 
	g++ -std=gnu++11 -O3 -Wall -Wextra -Wshadow -Wfatal-errors main.cpp -o exec

exec_debug:
	g++ -std=gnu++11 -O3 -Wall -Wextra -Wshadow -Wfatal-errors -DDEBUG main.cpp -o exec_debug

sandmark: exec
	./exec sandmark.umz 2>/tmp/um_log

sandmark_debug: exec_debug
	./exec_debug sandmark.umz 2>/tmp/um_log

codex: exec
	{ echo "(\b.bb)(\v.vv)06FHPVboundvarHRAk"; cat /dev/stdin; } | ./exec codex.umz 2>/tmp/um_log

clean:
	rm exec exec_debug