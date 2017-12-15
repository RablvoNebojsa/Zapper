#include "remote_controller.h"
#include "stream_controller.h"

static inline void textColor(int32_t attr, int32_t fg, int32_t bg)
{
   char command[13];

   /* command is the control command to the terminal */
   sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
   printf("%s", command);
}

/* macro function for error checking */
#define ERRORCHECK(x)                                                       \
{                                                                           \
if (x != 0)                                                                 \
 {                                                                          \
    textColor(1,1,0);                                                       \
    printf(" Error!\n File: %s \t Line: <%d>\n", __FILE__, __LINE__);       \
    textColor(0,7,0);                                                       \
    return -1;                                                              \
 }                                                                          \
}


static void remoteControllerCallback(uint16_t code, uint16_t type, uint32_t value);
static pthread_cond_t deinitCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t deinitMutex = PTHREAD_MUTEX_INITIALIZER;
static ChannelInfo channelInfo;


int main(int argc, char *argv[])
{
    /* initialize remote controller module */
    ERRORCHECK(remoteControllerInit());
    
    /* register remote controller callback */
    ERRORCHECK(registerRemoteControllerCallback(remoteControllerCallback));
    
    /* initialize stream controller module */
    ERRORCHECK(streamControllerInit(argv[1]));

    /* wait for a EXIT remote controller key press event */
    pthread_mutex_lock(&deinitMutex);
	if (ETIMEDOUT == pthread_cond_wait(&deinitCond, &deinitMutex))
	{
		printf("\n%s : ERROR Lock timeout exceeded!\n", __FUNCTION__);
	}
	pthread_mutex_unlock(&deinitMutex);
    
    /* unregister remote controller callback */
    ERRORCHECK(unregisterRemoteControllerCallback(remoteControllerCallback));

    /* deinitialize remote controller module */
    ERRORCHECK(remoteControllerDeinit());

    /* deinitialize stream controller module */
    ERRORCHECK(streamControllerDeinit());
  
    return 0;
}

void remoteControllerCallback(uint16_t code, uint16_t type, uint32_t value)
{
    switch(code)
	{
		case KEYCODE_1:
			printf("\nKey 1 pressed\n");
			numKeyPressed(1);
			break;
		case KEYCODE_2:
			printf("\nKey 2 pressed\n");
			numKeyPressed(2);
			break;
		case KEYCODE_3:
			printf("\nKey 3 pressed\n");
			numKeyPressed(3);
			break;
		case KEYCODE_4:
			printf("\nKey 4 pressed\n");
			numKeyPressed(4);
			break;
		case KEYCODE_5:
			printf("\nKey 5 pressed\n");
			numKeyPressed(5);
			break;
		case KEYCODE_6:
			printf("\nKey 6 pressed\n");
			numKeyPressed(6);
			break;
		case KEYCODE_7:
			printf("\nKey 7 pressed\n");
			numKeyPressed(7);
			break;
		case KEYCODE_8:
			printf("\nKey 8 pressed\n");
			numKeyPressed(8);
			break;
		case KEYCODE_9:
			printf("\nKey 9 pressed\n");
			numKeyPressed(9);
			break;
		case KEYCODE_0:
			printf("\nKey 0 pressed\n");
			numKeyPressed(0);
			break;
		case KEYCODE_INFO:
		    printf("\nInfo pressed\n");          
		    if (getChannelInfo(&channelInfo) == SC_NO_ERROR)
		    {
		        printf("\n********************* Channel info *********************\n");
		        printf("Program number: %d\n", channelInfo.programNumber);
		        printf("Audio pid: %d\n", channelInfo.audioPid);
		        printf("Video pid: %d\n", channelInfo.videoPid);
		        printf("**********************************************************\n");
		    }
			break;
		case KEYCODE_P_PLUS:
			printf("\nCH+ pressed\n");
            		channelUp();
			break;
		case KEYCODE_P_MINUS:
		    	printf("\nCH- pressed\n");
            		channelDown();
			break;
		case KEYCODE_EXIT:
			printf("\nExit pressed\n");
            		pthread_mutex_lock(&deinitMutex);
		    	pthread_cond_signal(&deinitCond);
		    	pthread_mutex_unlock(&deinitMutex);
			break;
		default:
			printf("\nPress P+, P-, info or exit! \n\n");
	}
}

