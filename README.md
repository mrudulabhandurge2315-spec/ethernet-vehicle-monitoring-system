# Ethernet-Based Vehicle Monitoring System Using Linux Socket Programming

## 1. Project Overview

This project demonstrates communication between two simulated automotive Electronic Control Units (ECUs) using Linux socket programming in C.

The Vehicle ECU generates simulated vehicle parameters and transmits them to the Dashboard ECU. The Dashboard ECU receives and displays the vehicle information.

Both TCP and UDP communication protocols are implemented and compared.

## 2. Vehicle Parameters

| Parameter | Unit |
|---|---|
| Vehicle Speed | km/h |
| Engine RPM | RPM |
| Steering Angle | degrees |

## 3. System Architecture

The system consists of two main components:

- **Vehicle ECU (Sender):** Generates simulated vehicle data and transmits it using TCP or UDP.
- **Dashboard ECU (Receiver):** Receives the vehicle data and displays it on the terminal.

<img width="1177" height="645" alt="image" src="https://github.com/user-attachments/assets/532ecfa9-172b-4a7d-919b-da8ae4ddd620" />

Communication configuration:

- IP Address: 127.0.0.1
- Port Number: 5000
- Data transmission interval: 1 second

## 4. Technologies Used

- C Programming
- Linux Socket Programming
- TCP and UDP Protocols
- GCC Compiler
- GNU Make
- Wireshark and TShark
- Ubuntu 22.04 under WSL2

## 5. Project Structure

```text
ethernet-vehicle-monitoring-system/
├── README.md
├── src/
│   ├── vehicle_ecu.c
│   ├── dashboard_ecu.c
│   └── Makefile
├── docs/
│   ├── Design_Document.pdf
│   ├── Validation_Report.pdf
│   └── TCP_UDP_Comparison.pdf
├── screenshots/
└── wireshark/
    ├── tcp_capture.pcapng
    └── udp_capture.pcapng
```

## 6. Compilation

Open the terminal in the project root directory and run:

```bash
make -f src/Makefile
```

This compiles both ECU programs.

## 7. Execution

Open two terminal windows.

### UDP Communication

Terminal 1 – Start the Dashboard ECU:

```bash
./dashboard_ecu udp
```

Terminal 2 – Start the Vehicle ECU:

```bash
./vehicle_ecu udp
```

### TCP Communication

Terminal 1 – Start the Dashboard ECU:

```bash
./dashboard_ecu tcp
```

Terminal 2 – Start the Vehicle ECU:

```bash
./vehicle_ecu tcp
```

Press Ctrl+C to stop the running programs.

## 8. Wireshark Analysis

Packet captures are available in the `wireshark/` directory:

- `udp_capture.pcapng` – Captured UDP communication.
- `tcp_capture.pcapng` – Captured TCP communication.

The captures can be opened in Wireshark to examine packet headers, IP addresses, port numbers, data transfer, TCP connection establishment and connection termination.

## 9. Project Validation

The following scenarios were tested:

1. Normal UDP communication between the Vehicle ECU and Dashboard ECU.
2. Normal TCP communication between the Vehicle ECU and Dashboard ECU.
3. TCP connection behaviour when the Dashboard ECU is not running.
4. Continuous data transmission for 60 seconds.
5. Invalid dashboard IP address handling.
6. TCP and UDP packet inspection using TShark.

## 10. Project Status

Both TCP and UDP communication modes have been implemented, compiled and tested successfully using Ubuntu 22.04 under WSL2.

The current demonstration uses the localhost address `127.0.0.1`. It simulates communication between two automotive ECUs without requiring physical vehicle hardware.

## License

MIT License.
