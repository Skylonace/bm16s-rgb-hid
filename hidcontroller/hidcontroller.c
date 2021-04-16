#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hidapi.h"

#define MAX_STR 255

#define VENDOR_ID       0x4B50
#define PRODUCT_ID      0x016B
#define DEVICE_VER      0x0001
#define RAW_USAGE_PAGE 0xFF60
#define RAW_USAGE_ID 0x61

int main(int argc, char **argv) {

    //checking if we have a correct ammount of arguments
    if(argc > 65 || (argc - 1) % 4) {
        printf("\n[Invalid arguments] Usage: \"%s <led_n (1-16)> <R (0-255)> <G (0-255)> <B (0-255)> ... (1-16)\"\n\n",argv[0]);
        exit(1);
    }

    //checking if the format of the arguments is correct
    for(int i = 1; i < argc; i++) {
        int val = atoi(argv[i]);
        if((i-1) % 4 == 0) {
            //check if the led n is between 1 and 16
            if(val > 16 || val < 1) {
                printf("\n[Invalid arguments] led_n must be between 1 and 16 \n\n",argv[0]);
                exit(1);
            }
        } else {
            //check if the rgb value is between 0 and 255
            if(val > 255 || val < 0) {
                printf("\n[Invalid arguments] R G B values must be between 0 and 255 \n\n",argv[0]);
                exit(1);
            }
        }
    }

    int res;
    unsigned char buf[33];
	hid_device *handle;
	int i;
    
    res = hid_init();
    if(res != 0) {
        printf("\n[ERROR] HIDAPI library error\n\n");
        exit(1);
    }

    //searching for a bm16s
    struct hid_device_info* hid;
    hid = hid_enumerate(VENDOR_ID,PRODUCT_ID);
    if(hid == NULL) {
        printf("\n[ERROR] bm16s not connected\n\n");
        exit(1);
    }
    while(1) {
        if(hid->usage_page == 65376 && hid->usage == 97) {
            handle = hid_open_path(hid->path);
            break;
        }
        if(hid->next != NULL)
            hid = hid->next;
        else {
            printf("\n[ERROR] bm16s found but missing usage page\n\n");
            exit(1);
        }
    }

    //sending over the RGB data
    buf[0] = 0x00;
    int limit = argc;
    //if the data exceeds 32 bytes then it will be split into two packets
    if(argc > 33)
        limit = 33;
    for(int i = 1; i < limit; i++) {
        buf[i] = atoi(argv[i]);
    }
    hid_write(handle, buf, 33);
    hid_read(handle, buf, 33);
    //second packet if needed
    if(argc > 33) {
        limit -= 32;
        for(int i = 1; i < limit; i++) {
        buf[i] = atoi(argv[i+32]);
    }
    hid_write(handle, buf, 33);
    hid_read(handle, buf, 33);
    }
    
    hid_close(handle);
    exit(0);
}