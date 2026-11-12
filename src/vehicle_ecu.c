
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DASHBOARD_IP "127.0.0.1"
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
    int tcp_mode = 0;

    struct sockaddr_in dashboard_addr;
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

    srand(time(NULL));

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
    dashboard_addr.sin_port = htons(DASHBOARD_PORT);

    if (inet_pton(AF_INET, DASHBOARD_IP,
                  &dashboard_addr.sin_addr) <= 0)
    {
        perror("Invalid dashboard IP");
        close(sockfd);
        return 1;
    }

    if (tcp_mode)
    {
        if (connect(sockfd,
                    (struct sockaddr *)&dashboard_addr,
                    sizeof(dashboard_addr)) < 0)
        {
            perror("TCP connection failed");
            close(sockfd);
            return 1;
        }

        printf("TCP connection established.\n");
    }

    printf("====================================\n");
    printf("       VEHICLE ECU STARTED\n");
    printf("       Mode: %s\n", argv[1]);
    printf("====================================\n");

    while (1)
    {
        vehicle.speed = rand() % 121;
        vehicle.engine_rpm = 800 + rand() % 4201;
        vehicle.steering_angle =
            -45.0f + (rand() % 901) / 10.0f;

        printf("\n[VEHICLE ECU]\n");
        printf("Speed          : %d km/h\n", vehicle.speed);
        printf("Engine RPM     : %d RPM\n", vehicle.engine_rpm);
        printf("Steering Angle : %.1f degrees\n",
               vehicle.steering_angle);

        ssize_t sent;

        if (tcp_mode)
        {
            sent = send(sockfd, &vehicle,
                        sizeof(vehicle), 0);
        }
        else
        {
            sent = sendto(sockfd, &vehicle,
                          sizeof(vehicle), 0,
                          (struct sockaddr *)&dashboard_addr,
                          sizeof(dashboard_addr));
        }

        if (sent != sizeof(vehicle))
        {
            perror("Vehicle data transmission failed");
            break;
        }

        printf("Vehicle data transmitted successfully.\n");

        sleep(1);
    }

    close(sockfd);
    return 0;
}
