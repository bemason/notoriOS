#include "main.h"

//this files holds all the "business" callbacks, which are abstracted from the main OS





/*
TODO:

    X initilizwe modem parameters and cell tower time at bootup
    X Write real maxbotixdata to server
    X fix string header length constant
    - Throw eeeor if string lengfth exceeds size
    X implement 4G modem
    X implement a backup timer, so sync shuts down if it can't get a signal with network
    - GPS lock
    -- start alarm on 0 (on the hour), rather then arbitrary time
    - watchdog -- built-in is too short (2sec), so may need to build one using a timer
    - test power
    - write data to SD card
    - bootloader
    - Read form influx anbd parse  Meta DB
    - test power

*/

