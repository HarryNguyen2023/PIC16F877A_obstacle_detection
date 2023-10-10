#ifndef PIC16F877A_LINE_SENSOR_H
#define PIC16F877A_LINE_SENSOR_H

#include "PIC16F877A_ADC.h"

#define SENSOR_NUM 5

typedef struct
{
    uint8_t sensor_channels[SENSOR_NUM];
    int8_t sensor_gains[SENSOR_NUM];
    uint16_t sensor_values[SENSOR_NUM];
    float sensor_distances[SENSOR_NUM];
}LineSensor;

void lineSensorInit(LineSensor* line_sensors);
void lineSensorDetect(LineSensor* line_sensors, float* sensor_values);

#endif