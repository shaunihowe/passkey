BIN   = passkey
SRC   = passkey.c

CC    = gcc

all : $(BIN)

clean : 
	rm -f $(BIN)

run : $(BIN)
	./$(BIN)

$(BIN) :
	$(CC) $(SRC) -o $(BIN) 

