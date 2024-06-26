CC     = gcc
INC    = -I.
CFLAGS = $(INC) -Wall -Wextra -Werror
CFILES = $(wildcard testParallel.c)
OBJS   = $(patsubst %.c, %.o,$(CFILES))
BIN    = test

%.o:%.c
	$(info Compiling $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(BIN):$(OBJS)
	$(CC) -o $@ $^

exec:$(BIN)
	./testLockless

clean:
	@rm -f $(BIN) $(OBJS) *~
