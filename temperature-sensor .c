#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>


#define SENSOR_PORT 8080

int temperature;
pthread_mutex_t lock;

void* update_temperature(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        printf("Enter new temperature: ");
        scanf("%d", &temperature); // Get temperature from user input
        printf("Updated Temperature: %d°C\n", temperature);
        pthread_mutex_unlock(&lock);
        sleep(1); // Update every 1 second
    }
    return NULL;
}

void* respond_to_thermostat(void* arg) {
    int sockfd;
    struct sockaddr_in sensor_addr, thermostat_addr;
    char buffer[50];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up sensor address
    memset(&sensor_addr, 0, sizeof(sensor_addr));
    sensor_addr.sin_family = AF_INET;
    sensor_addr.sin_port = htons(SENSOR_PORT);
    sensor_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the sensor address
    if (bind(sockfd, (const struct sockaddr *)&sensor_addr, sizeof(sensor_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        socklen_t len = sizeof(thermostat_addr);

        // Receive request from the thermostat
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &thermostat_addr, &len);

        // Send the current temperature to the thermostat
        pthread_mutex_lock(&lock);
        sprintf(buffer, "%d", temperature);
        pthread_mutex_unlock(&lock);

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &thermostat_addr, len);
    }

    close(sockfd);
    return NULL;
}

int main() {
    pthread_t temp_thread, request_thread;
    pthread_mutex_init(&lock, NULL);

    // Create threads for temperature updating and responding to thermostat
    pthread_create(&temp_thread, NULL, update_temperature, NULL);
    pthread_create(&request_thread, NULL, respond_to_thermostat, NULL);

    // Wait for the threads to finish
    pthread_join(temp_thread, NULL);
    pthread_join(request_thread, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}


