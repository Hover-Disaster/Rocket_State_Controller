# Rocket state controller
This repository defines a set of logical functions which allow for determination of the stages of flight of a rocket  
### Usage
To ensure this code performs as intented, it is critical to follow the example usage guidelines. This system is highly sensitive to frequency domain changes. This includes changes in sample frequency and cut-off frequency of filters  
Filter maximum sample rates are handled in the code provided that functions are polled at an adequate rate however, sensor data filtering that is performed on an external IC or inertial sensor cannot be controlled by this library therefore, it is up to the user to configure the peripherals to match these parameters.  
**Sensor filter table**  
THESE MUST BE FOLLOWED TO ACHIEVE NOMINAL BEHAVIOUR
| Sensor type    | Sample rate (Hz) | Filter bandwidth (Hz) |
|----------------|------------------|-----------------------|
| Accelerometer  | >100             | ~60                   |
| Barometer      | ~100             | ~50                   |

### Folder structure  
Inc -> Header files  
Src -> Source files  
Examples -> Example implementation  
