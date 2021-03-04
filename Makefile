ww: ww.o strbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o ww ww.o strbuf.o

ww.o: ww.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined ww.c

strbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined strbuf.c

clean:
	rm -f *.o $(OUTPUT)