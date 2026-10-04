CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread
LIBS = -lcrypto

SRC = src/main.c \
      src/copier.c \
      src/directory.c \
      src/verify.c

TARGET = copyfast

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run:
	./$(TARGET)
