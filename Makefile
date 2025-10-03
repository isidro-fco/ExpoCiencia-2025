# Makefile para proyecto raylib
CC = C:/raylib/w64devkit/bin/gcc.exe
TARGET = proyecto.exe
SRC = src/main.c
INCLUDE_PATH = C:/raylib/raylib/src
LIB_PATH = C:/raylib/raylib/src
LIBS = -lraylib -lwinmm -lgdi32 -luser32 -lshell32
CFLAGS = -I$(INCLUDE_PATH) -L$(LIB_PATH)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS) $(LIBS)

clean:
	del $(TARGET) 2>nul || true

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run