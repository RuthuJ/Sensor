#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SENSOR_PORT 8080
#define THERMOSTAT_PORT 9090

int main() {
    int sockfd;
    struct sockaddr_in sensor_addr, thermostat_addr;
    char buffer[50];
    int threshold, temperature;
    int ac_state = 0; // 0 = OFF, 1 = ON

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up thermostat address
    memset(&thermostat_addr, 0, sizeof(thermostat_addr));
    thermostat_addr.sin_family = AF_INET;
    thermostat_addr.sin_port = htons(THERMOSTAT_PORT);
    thermostat_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the thermostat address
    if (bind(sockfd, (const struct sockaddr *)&thermostat_addr, sizeof(thermostat_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set up sensor address
    memset(&sensor_addr, 0, sizeof(sensor_addr));
    sensor_addr.sin_family = AF_INET;
    sensor_addr.sin_port = htons(SENSOR_PORT);
    sensor_addr.sin_addr.s_addr = INADDR_ANY;

    // Get the temperature threshold from the user
    printf("Enter temperature threshold (20-25°C): ");
    scanf("%d", &threshold);

    printf("Initial AC State: OFF\n");

    while (1) {
        // Request temperature from the sensor
        sendto(sockfd, "TEMP", strlen("TEMP"), 0, (const struct sockaddr *) &sensor_addr, sizeof(sensor_addr));

        // Receive the temperature from the sensor
        socklen_t len = sizeof(sensor_addr);
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &sensor_addr, &len);

        // Convert the received data to an integer
        temperature = atoi(buffer);
        printf("Received Temperature: %d°C\n", temperature);

        // Control the AC based on the temperature
        if (temperature > threshold) {
            if (ac_state == 0) {
                ac_state = 1;
                printf("Turn ON AC.\n");
            }
        } else if (temperature < threshold) {
            if (ac_state == 1) {
                ac_state = 0;
                printf("Turn OFF AC.\n");
            }
        }

        // Poll for every 5 sec
        sleep(5);
    }

    close(sockfd);
    return 0;
}
