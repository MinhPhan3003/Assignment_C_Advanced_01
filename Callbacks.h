#ifndef __CALLBACKS_H
#define __CALLBACKS_H
#include "Measurement.h"

void Print_Measurement_Callback(const Measurement* item);
void Average_Measurement_Callback(const Measurement* item);
void CheckStatus_Measurement_Callback(const Measurement* item);

#endif 