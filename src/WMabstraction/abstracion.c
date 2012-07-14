#include <stdio.h>
#include <stdlib.h>


#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>

#define MONSTERWM_MAX_NUMBER_OF_DESKTOPS 4
#define MONSTERWM_MAX_NUMBER_OF_CLIENTS 100

struct WMClient
{
   unsigned char state;
   Window win,bar;
};

typedef unsigned int WMClientIndex;


struct WMDesktop
{
   unsigned int clients_on_desktop;
   WMClientIndex * clientIndex;
};

static int activeLayout=0;
static int activeDesktop=0;
static WMClientIndex focusedClientIndex=0;

unsigned int clients_loaded;
struct WMClient client[MONSTERWM_MAX_NUMBER_OF_CLIENTS]={0};

struct WMDesktop desktop[MONSTERWM_MAX_NUMBER_OF_DESKTOPS]={0};

void Client_AddNewWindow(Window *newWindow)
{
    client[clients_loaded].win=*newWindow;
    desktop[activeDesktop].clientIndex[desktop[activeDesktop].clients_on_desktop]=clients_loaded;
    ++desktop[activeDesktop].clients_on_desktop;
    ++clients_loaded;
}


int main()
{
    printf("Hello world!\n");
    return 0;
}
