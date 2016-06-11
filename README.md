# QP/C Example Applications Port
## About the Project
An on-going project to port all the example applications that are included in Quantum Leaps, LLC's
[QP/C active object (actor) framework] (https://sourceforge.net/projects/qpc/files/QP-C/) to STMicroelectronics'
STM32F469 Discovery kit, a high performance ARM Cortex-M4 development board whose standout feature is a
4" WVGA TFT LCD with capacitive touch sensing.

For a brief preview of what the project is all about, click [here] (https://drive.google.com/file/d/0B5YTU7YQ4HooaWp1bDZscXAxVVU/view?usp=sharing) for a short video demonstration.

The driving force behind this project is the author's desire to further his own experience in the
integration of STM32CubeF4 firmware package components (HAL low level drivers, 3rd party middleware)
together with the [QP/C Framework] (http://state-machine.com/qpc/index.html) and the use of the [QM Modeling Tool] (http://state-machine.com/qm/index.html)
when developing bare-metal real-time embedded applications for microcontrollers.

Every attempt is made to emulate the file structure, naming convention and programming style of the
original work on which this project is based in order to stay as close as possible to the source. All
changes made are necessary to succesfully port the example applications to a completely new target device.
