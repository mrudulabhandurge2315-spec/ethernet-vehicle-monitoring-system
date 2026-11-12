
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DASHBOARD_PORT 5000

struct VehicleData
{
    int speed;
    int engine_rpm;
    float steering_angle;
};

int main(int argc, char *argv[])
{
    int sockfd;
    int clientfd;
    int tcp_mode = 0;

    struct sockaddr_in dashboard_addr;
    struct sockaddr_in vehicle_addr;

    socklen_t addr_len = sizeof(vehicle_addr);
    struct VehicleData vehicle;

    if (argc != 2 ||
        (strcmp(argv[1], "udp") != 0 &&
         strcmp(argv[1], "tcp") != 0))
    {
        printf("Usage: %s <udp|tcp>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "tcp") == 0)
        tcp_mode = 1;

    sockfd = socket(AF_INET,
                    tcp_mode ? SOCK_STREAM : SOCK_DGRAM,
                    0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    memset(&dashboard_addr, 0, sizeof(dashboard_addr));

    dashboard_addr.sin_family = AF_INET;
    dashboard_addr.sin_addr.s_addr = INADDR_ANY;
    dashboard_addr.sin_port = htons(DASHBOARD_PORT);

    if (bind(sockfd, (struct sockaddr *)&dashboard_addr,
             sizeof(dashboard_addr)) < 0)
    {
        perror("Socket binding failed");
        close(sockfd);
        return 1;
    }

    printf("====================================\n");
    printf("       DASHBOARD ECU STARTED\n");
    printf("       Mode: %s\n", argv[1]);
    printf("====================================\n");

    if (tcp_mode)
    {
        if (listen(sockfd, 5) < 0)
        {
            perror("Listen failed");
            close(sockfd);
            return 1;
        }

        printf("Waiting for TCP connection...\n");

        clientfd = accept(sockfd,
                          (struct sockaddr *)&vehicle_addr,
                          &addr_len);

        if (clientfd < 0)
        {
            perror("Accept failed");
            close(sockfd);
            return 1;
        }

        printf("Vehicle ECU connected successfully.\n");
    }
    else
    {
        printf("Waiting for UDP vehicle data...\n");
        clientfd = sockfd;
    }

    while (1)
    {
        ssize_t received = 0;

        if (tcp_mode)
        {
            char *ptr = (char *)&vehicle;
            size_t remaining = sizeof(vehicle);

            while (remaining > 0)
            {
                ssize_t n = recv(clientfd, ptr,
                                 remaining, 0);

                if (n <= 0)
                    break;

                ptr += n;
                remaining -= n;
            }

            if (remaining > 0)
            {
                printf("TCP connection closed or reception failed.\n");
                break;
            }

            received = sizeof(vehicle);
        }
        else
        {
            received = recvfrom(sockfd, &vehicle,
                                sizeof(vehicle), 0,
                                (struct sockaddr *)&vehicle_addr,
                                &addr_len);
        }

        if (received < 0)
        {
            perror("Data reception failed");
            break;
        }

        if (received != sizeof(vehicle))
        {
            printf("Invalid vehicle data received.\n");
            continue;
        }

        printf("\n====================================\n");
        printf("         VEHICLE DASHBOARD\n");
        printf("====================================\n");

        printf("Vehicle Speed  : %d km/h\n", vehicle.speed);
        printf("Engine RPM     : %d RPM\n", vehicle.engine_rpm);
        printf("Steering Angle : %.1f degrees\n",
               vehicle.steering_angle);

        printf("Data received successfully.\n");
    }

    if (tcp_mode)
        close(clientfd);

    close(sockfd);

    return 0;
}
