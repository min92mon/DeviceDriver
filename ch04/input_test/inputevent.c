/***************************************
Filename: inputevent.c 
Title: Input Event Test
Desc: event handler
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define EVENT_BUF_NUM 64

int fd = -1;    /* the file descriptor for the device */

int main(int argc, char **argv) {
    unsigned int i;       
    size_t read_bytes; // How many Bytes read
    struct input_event event_buf[EVENT_BUF_NUM];  // POINT: How many Events read
    char *device;

    if( argc != 2 ) {
        printf("Usage: inputevent [key | touch]\n");
        exit(1);
    }
    
    // POINT: designate the 'key' or 'touch' argument as the device file 
    if( !strncmp("key", argv[1], 3) ) {
        device = "/dev/input/event3";
    }
    else if( !strncmp("touch", argv[1], 5) ) {
        device = "/dev/input/event0";
    }
    else {
        printf("device argument error\n");
        exit(1);
    }
    
    // TODO: input device open
    if ((fd = open(device, O_RDONLY)) < 0) {
        printf("%s: open error", device);
        exit(1);
    }

    while (1) {
        // TODO: read the event occurring as EVENT_BUF_NUM
        read_bytes = read( fd, event_buf, 
                     (sizeof(struct input_event)*EVENT_BUF_NUM)
                );
        if( read_bytes < sizeof(struct input_event) ) {
            printf("%s: read error", device);
            exit(1);
        }
        // event loop
        for( i=0; i<(read_bytes/sizeof(struct input_event)); i++ )
        {
            // process the event type
            switch( event_buf[i].type )
            {
            case EV_SYN:
                printf("---------------------------------------\n");
                break;
            
            case EV_KEY:
                printf("Button code %d", event_buf[i].code);
                switch (event_buf[i].value)
                {
                case 1:
                    printf(": pressed\n");
                    break;
                case 0:
                    printf(": released\n");
                    break;
                default:
                    printf("Unknown: type %d, code %d, value %d",
                          event_buf[i].type, 
                          event_buf[i].code, 
                          event_buf[i].value);
                    break;
                }
                break;
                
            case EV_ABS:
                switch (event_buf[i].code)
                {
                case ABS_MT_POSITION_X:
                    printf("X position: %d\n", event_buf[i].value);
                    break;
                case ABS_MT_POSITION_Y:
                    printf("Y position: %d\n", event_buf[i].value);
                    break;
                case BTN_TOUCH:    
                    printf("Pressure  : %s\n", (event_buf[i].value == 1)? "yes":"no" );
                    break;
                default:
                    printf("Touch : type %d, code %d, value %d\n",
                          event_buf[i].type, event_buf[i].code, event_buf[i].value);
                    break;
                }
                event_buf[i].type = 0;
                break;            
            default:
                printf("Unknown: type %d, code %d, value %d\n",
                       event_buf[i].type, event_buf[i].code, event_buf[i].value);

                break;
            }
        }
    }
    close(fd);
    exit(0);
}
