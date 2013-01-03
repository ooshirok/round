round: round.o
	gcc round.c -o round -lm

round.o:	round.c
	gcc -c round.c

clean:
	@rm -f round.o round
