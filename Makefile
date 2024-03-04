SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c = obj/%.o)

CFLAGS = -c -Wall

main.exe : $(OBJS)
	gcc -o bin/$@ $^ -std=c99 -lm -lpthread

obj/%.o : src/%.c
	gcc $(CFLAGS) $<

clean:
	rm -rf bin obj resultats anim.gif
	mkdir bin obj resultats

graph:
	gnuplot gnuplot.sh --persist

gif:
	convert -delay 5 -loop 0 ./resultats/*.pgm anim.gif