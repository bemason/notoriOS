// ==============================================
//  I got stacks on stacks (on stacks)
//      - Soulja Boy (2011)
//
// This defines methods for the core data stack, which will be syunced with the server
// ==============================================
#include "data.h"


// ==============================================
//  Push It
//      - Salt-n-Peppa (1987)
//
// This pushes data onto the stack
// ==============================================
uint8 pushData(char * key, char *value, int32 timestamp){
    
    sprintf(data[dataPointsInStack].key, "%s", key);
    sprintf(data[dataPointsInStack].value, "%s", value);
    data[dataPointsInStack].timeStamp = timestamp;
  
    dataPointsInStack++;
    
    //cricular buffer
    //if stack is full, loop around and delete first point
    //this should be fixed to store the overwritte data onto SD card
    if(dataPointsInStack >= DATA_MAX_VALUES){
        dataPointsInStack = 0;
        printNotif(NOTIF_TYPE_ERROR,"Data stack full. Begin overwrite of circular buffer.");
        return 0;
    }
    
    return 1;
}

key_value_t popData(){
    
    if(dataPointsInStack>0){
        dataPointsInStack--;
        return data[dataPointsInStack];
    }else{
        dataPointsInStack = 0;
        return data[0];   
    }
}


void Initialize_Data_Stack(){
    Clear_Data_Stack();
}

void Clear_Data_Stack(){
    
    for(uint16 i;i<DATA_MAX_VALUES;i++){
        data[i].key[0] = '\0';
        data[i].value[0] = '\0';
        data[i].timeStamp = 0;
    }
    dataPointsInStack =0;
   
}

uint16 sizeOfDataStack(){
    return dataPointsInStack;
}


//thsi function could really use a test
void construct_generic_HTTP_request(char* header, char* body, char* host, char* route,
                               int port, char* method, char* connection_type,
	                           char *extra_headers, int extra_len, char* http_protocol){

    sprintf(header,"%s /%s HTTP/%s\r\n", method, route, http_protocol);
    sprintf(header,"%s%s%s%s%d%s%s%s%s",
            header, // 1
            "Host: ", host, ":", port, "\r\n", // 2 3 4 5 6
            "Connection: ", connection_type, "\r\n"); // 7 8 9
	if (extra_headers && strlen(extra_headers) > 0){
		sprintf(header, "%s%s", header, extra_headers);
	}
	if (strcmp(method, "GET") != 0){
		sprintf(body, "%s%s%s%d%s%s",
			header,
            "Content-Type: text/plain\r\n", // 10
            "Content-Length: ", (extra_len + strlen(body)), //11 12 (Extra len should be 2 for flask server)
            "\r\n\r\n", body); // 13 14 15
	}
	sprintf(body, "%s%s", body, "\r\n"); 
    
}
    




