# Sensor
<br>
Application in C to simulate IoT Sensor
<br>
Write an application in C to simulate an IoT sensor and to control a device based on it. 
Let's divide this to two components for this assignment. 1) Thermostat+AirConditioner 2) Temperature Sensor 
Let these two components run as two different processes. You are free to use signals or TCP/UDP or any IPC mechanism between them.
A thermostat process continuosly monitors (every 5 seconds) the data from the temperature sensor and based on the threshold value set, it will create a Control Command to the Air Condition function to TURN ON/OFF.
Temperature Sensor runs a process where it will update the value (let this be anywhere between 15degree-35degree) of the temperature every 1 second. It will have a mechanism to receive Requests from the Thermostat to return temperature data (just integer value is okay)

Things to do: 
- Thermostat+AC : it will first ask for user input to set the temperature threshold (between 20-25degree Celsius). The initial state of AC will be OFF. Print this state. It will ask the Temperature Sensor to fetch the data and this data will be compared with the threshold. 
If temp>threshold : TURN ON AC
elif temp<threshold : TURN OFF AC 
Print the AC state everytime it polls the temp data. 

- Temperature Sensor: It will run two threads, one that will update the temp value every 1second and prints this value And the other thread is responsible to respond to Thermostat the latest value recorded. 
