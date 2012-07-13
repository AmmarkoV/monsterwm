#include <X11/Xlib.h>

#define UNUSABLE_BYTE      1
#define UNUSABLE_BYTE2     2
#define FOCUSED_WINDOW     4
#define HIGHLIGHT_MINIMIZE 8
#define HIGHLIGHT_MAXIMIZE 16
#define HIGHLIGHT_CLOSE    32
#define OVER_BUTTON        64
#define ACTIVE_BUTTON      128

const unsigned long    overColor  = 0x000011aa;
const unsigned long    selectColor  = 0x00ff1100;
const unsigned long    barActiveColor  = 0x00ff9944;
const unsigned long    barActiveColor1 = 0x00ffaa55;
const unsigned long    barActiveColor2 = 0x00ffbb66;
const unsigned long    barActiveColor3 = 0x00ffcc77;

const unsigned long    barInactiveColor  = 0x00444444;
const unsigned long    barInactiveColor1 = 0x00777777;
const unsigned long    barInactiveColor2 = 0x00999999;
const unsigned long    barInactiveColor3 = 0x00aaaaaa;

#define inside(X,Y,WIDTH,HEIGHT,PROC_X,PROC_Y)  ( ( (PROC_X) > (X) )&&( (PROC_Y) > (Y) )&&( (PROC_X) < (X+WIDTH) )&&( (PROC_Y) < (Y+HEIGHT) ) )


void drawWindowBar(unsigned int x,unsigned int y,unsigned int barWidth,unsigned int barHeight,unsigned short settings,char * message,Display *dpy)
{
      unsigned int barHeightMiddle = (unsigned int ) barHeight/2;

      int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

      unsigned long *barBackColor,*barColor1,*barColor2,*barColor3;
      if (settings&FOCUSED_WINDOW) { barBackColor=&barActiveColor;   barColor1=&barActiveColor1;   barColor2=&barActiveColor2;   barColor3=&barActiveColor3; } else
                                   { barBackColor=&barInactiveColor; barColor1=&barInactiveColor1; barColor2=&barInactiveColor2; barColor3=&barInactiveColor3; }

      // Create the window
      Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), x,y, barWidth,barHeight,0, *barBackColor , *barBackColor);

      // We want to get MapNotify events
      XSelectInput(dpy, w, StructureNotifyMask);

      // "Map" the window (that is, make it appear on the screen)
      XMapWindow(dpy, w);

      // Create a "Graphics Context"
      GC gc = XCreateGC(dpy, w, 0, 0);


      // Wait for the MapNotify event

      for(;;)
      {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    if (e.type == MapNotify)
	     {
           //We want thick lines
           XSetLineAttributes(dpy,gc,3,0,0,0);

           // Drawing some gradient color on the bar
           XSetForeground(dpy, gc, *barColor1);
           XDrawLine(dpy, w, gc, 1, barHeightMiddle-6 , barWidth , barHeightMiddle-6 );
           XDrawLine(dpy, w, gc, 1, barHeightMiddle+6 , barWidth , barHeightMiddle+6 );

           XSetForeground(dpy, gc, *barColor2);
           XDrawLine(dpy, w, gc, 1, barHeightMiddle-3 , barWidth , barHeightMiddle-3 );
           XDrawLine(dpy, w, gc, 1, barHeightMiddle+3 , barWidth , barHeightMiddle+3 );

           XSetForeground(dpy, gc, *barColor3);
           XDrawLine(dpy, w, gc, 1, barHeightMiddle , barWidth , barHeightMiddle );


	       //We want thin lines
           XSetLineAttributes(dpy,gc,1,0,0,0);

           //fprintf(stderr,"%u&%u=%u , %u&%u=%u \n",settings,HIGHLIGHT_MINIMIZE,settings&HIGHLIGHT_MINIMIZE,settings,HIGHLIGHT_MAXIMIZE,settings&HIGHLIGHT_MAXIMIZE);

           if ( ((settings&ACTIVE_BUTTON)==ACTIVE_BUTTON) || ((settings&OVER_BUTTON)==OVER_BUTTON ) )
           { //Minimize , Maximize or Close button are activated or hovered over , drawing them
            if ((settings&OVER_BUTTON)==OVER_BUTTON) { XSetForeground(dpy, gc, overColor); } else
            if ((settings&ACTIVE_BUTTON)==ACTIVE_BUTTON) { XSetForeground(dpy, gc, selectColor); }

            if ((settings&HIGHLIGHT_MINIMIZE)==HIGHLIGHT_MINIMIZE) {  XFillRectangle(dpy,w,gc,barWidth-45,1,15,barHeight-2); } else
            if ((settings&HIGHLIGHT_MAXIMIZE)==HIGHLIGHT_MAXIMIZE) {  XFillRectangle(dpy,w,gc,barWidth-30,1,15,barHeight-2); } else
            if ((settings&HIGHLIGHT_CLOSE)==HIGHLIGHT_CLOSE)       {  XFillRectangle(dpy,w,gc,barWidth-15,1,15,barHeight-2); }
           }

           // Tell the GC we draw using the white color
           XSetForeground(dpy, gc, blackColor);

           XSetLineAttributes(dpy,gc,3,0,0,0);
           /*Minimize Icon*/ XDrawLine(dpy, w ,  gc, barWidth - 40, barHeightMiddle , barWidth - 35 , barHeightMiddle );
           /*Maximize Icon*/ XDrawRectangle(dpy, w ,  gc, barWidth - 25, 7 , 5, barHeight-14 );
           /*X*/ XDrawLine(dpy, w, gc, barWidth - 10, 6 , barWidth - 4 , barHeight-6);
                 XDrawLine(dpy, w, gc, barWidth - 10, barHeight-6 , barWidth - 4 , 6);


           if ((settings&FOCUSED_WINDOW)!=FOCUSED_WINDOW)  XSetForeground(dpy, gc, whiteColor); // We are inactive so we want a white font due to the dark background
           /* Title of Window */
           XDrawString(dpy,w,gc, 40,15, "Title of window goes here",25); //todo add handler for large strings that appends ... and centers text

           //Send the requests to the server
           XFlush(dpy);
	     }
      }
}

int windowBarButtonClicked(unsigned int x,unsigned int y,unsigned int barWidth,unsigned int barHeight)
{

}

main()
{
      //Test
      if (!inside(0,0,400,60,50,50)) return 1;

      // Open the display
      Display *dpy = XOpenDisplay(0);
      if (!dpy ) return 1;

      drawWindowBar(0,0,400,20,FOCUSED_WINDOW|HIGHLIGHT_CLOSE|ACTIVE_BUTTON,"Title of window goes here",dpy);
     // drawWindowBar(0,20,400,20,FOCUSED_WINDOW|HIGHLIGHT_MINIMIZE,"Title of window goes here",dpy);


      return 0;
}

