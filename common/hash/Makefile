TARGET=test
OBJ=hash.o test.o
CFLAG=-g -Wall
CC=gcc

$(TARGET): $(OBJ)

hash.o: hash.c hash.h
	$(CC) $(CFLAG) -c -o $@ $<
test.o: test.c hash.h
	$(CC) $(CFLAG) -c -o $@ $<
#%.o: %.c %.h
#	$(CC) $(CFLAG) -c -o $@ $<
	
clean:
	rm *.o $(TARGET)
