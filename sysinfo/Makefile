CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lX11 -lImlib2
TARGET = custom_tray

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(LDFLAGS)

$(TARGET).o: $(TARGET).c 
	$(CC) $(CFLAGS) -c $(TARGET).c 

clean:
	rm -f $(TARGET) $(TARGET).o
