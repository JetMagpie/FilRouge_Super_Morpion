CC = gcc
CFLAGS = -Wall -std=c99
SRCS =  minimax.c morpion.c supermorpion.c main.c

all: sm_refresh tttree sm_bot

tttree: $(SRCS)
	$(CC) $(CFLAGS) -DTTTREE_ENABLE $(SRCS) -o $@

test_tttree: tttree
	./tttree "x21o11xo o" > g1.dot
	dot g1.dot -T png -o g1.png

sm_refresh: $(SRCS)
	$(CC) $(CFLAGS) -DSM_REFRESH_ENABLE $(SRCS) -o $@

sm_bot: $(SRCS)
	$(CC) $(CFLAGS) -DSM_BOT_ENABLE $(SRCS) -o $@

clean:
	rm -f tttree sm_refresh sm_bot *.dot *.png *.o