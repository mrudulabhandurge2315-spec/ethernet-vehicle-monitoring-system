
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: vehicle_ecu dashboard_ecu

vehicle_ecu: src/vehicle_ecu.c
	$(CC) $(CFLAGS) src/vehicle_ecu.c -o vehicle_ecu

dashboard_ecu: src/dashboard_ecu.c
	$(CC) $(CFLAGS) src/dashboard_ecu.c -o dashboard_ecu

clean:
	rm -f vehicle_ecu dashboard_ecu

.PHONY: all clean
