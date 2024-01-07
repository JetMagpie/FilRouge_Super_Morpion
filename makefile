CC = gcc
CFLAGS = -Wall -std=c99
SRCS = main.c minimax.c morpion.c supermorpion.c

all: sm_refresh

tttree: $(SRCS)
	$(CC) $(CFLAGS) -DTTTREE_ENABLE $(SRCS) -o $@

test_tttree: tttree
	./tttree "1o11o1oxx x" > g1.dot
	dot g1.dot -T png -o g1.png

sm_refresh: $(SRCS)
	$(CC) $(CFLAGS) -DSM_REFRESH_ENABLE $(SRCS) -o $@

sm_bot: $(SRCS)
	$(CC) $(CFLAGS) -DSM_BOT_ENABLE $(SRCS) -o $@

clean:
	rm -f tttree sm_refresh sm_bot g1.dot g1.png *.o