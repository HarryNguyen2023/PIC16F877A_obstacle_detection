# include "PIC16F877A_line_sensor.h"

// ----------------------------------- Static functions hidden from users -----------------------------------------

// Function to calibrate the sensor
static void sensorCalibrate(LineSensor* line_sensors)
{
    line_sensors->sensor_distances[0] = 40 + 0.91 * (line_sensors->sensor_values[0] - 45.07);
    line_sensors->sensor_distances[1] = 40 + 0.91 * (line_sensors->sensor_values[1] - 45.07);
    line_sensors->sensor_distances[2] = 40 + 0.91 * (line_sensors->sensor_values[2] - 45.07);
    line_sensors->sensor_distances[3] = 40 + 0.91 * (line_sensors->sensor_values[3] - 45.07);
    line_sensors->sensor_distances[4] = 40 + 0.91 * (line_sensors->sensor_values[4] - 45.07);
}

// ------------------------------------- General functions used by users -------------------------------------------

// Function to initiate the line detecting sensor
void lineSensorInit(LineSensor* line_sensors)
{
    // Initiate the ADC module for those sensors
    ADC_Init(line_sensors->sensor_channels, SENSOR_NUM);
}

// Function to get the values of the line detecting sensors
void lineSensorDetect(LineSensor* line_sensors, float* sensor_values)
{
    float sensor_distance_weight = 0;
    float sensor_distance_sum = 0;
    // Read the ADC value of the sensor
    for(uint8_t i = 0; i < SENSOR_NUM; ++i)
    {
        line_sensors->sensor_values[i] = ADC_Read(line_sensors->sensor_channels[i]);
    }
    // Calibrate the sensors
    sensorCalibrate(line_sensors);
    // Get the distance of the sensor
    for(uint8_t i = 0; i < SENSOR_NUM; ++i)
    {
        sensor_distance_weight += (line_sensors->sensor_gains[i] * line_sensors->sensor_distances[i]);
        sensor_distance_sum += line_sensors->sensor_distances[i];
    }
    *sensor_values = sensor_distance_weight / sensor_distance_sum;
}
