// Written by Ch. Tronche (http://tronche.lri.fr:8000/)
#include <X11/Xlib.h> // Every Xlib program must include this
#include <assert.h>   // I include this to test return values the lazy way

#define FOCUSED_WINDOW   0x01
#define HIGHLIGHT_MINIMIZE 0x02
#define HIGHLIGHT_MAXIMIZE 0x04
#define HIGHLIGHT_CLOSE    0x08
#define OVER_BUTTON     0x16
#define ACTIVE_BUTTON  0x32

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


void drawWindowBar(unsigned int x,unsigned int y,unsigned int barWidth,unsigned int barHeight,unsigned int settings,char * message,Display *dpy)
{
      unsigned int barHeightMiddle = (unsigned int ) barHeight/2;

      int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
      int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

      unsigned long *barBackColor,*barColor1,*barColor2,*barColor3;
      if (settings&FOCUSED_WINDOW) { barBackColor=&barActiveColor;   barColor1=&barActiveColor1;   barColor2=&barActiveColor2;   barColor3=&barActiveColor3; } else
                                   { barBackColor=&barInactiveColor; barColor1=&barInactiveColor1; barColor2=&barInactiveColor2; barColor3=&barInactiveColor3; }

      // Create the window
      Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, barWidth , barHeight , 0 , *barBackColor , *barBackColor);

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

           if ( (settings&ACTIVE_BUTTON) || (settings&OVER_BUTTON) )
           { //Minimize , Maximize or Close button are active , drawing them
            if (settings&OVER_BUTTON) { XSetForeground(dpy, gc, overColor); } else
            if (settings&ACTIVE_BUTTON) { XSetForeground(dpy, gc, selectColor); }

            if (settings&HIGHLIGHT_MINIMIZE) {  XFillRectangle(dpy,w,gc,barWidth-45,1,15,barHeight-2); } else
            if (settings&HIGHLIGHT_MAXIMIZE) {  XFillRectangle(dpy,w,gc,barWidth-30,1,15,barHeight-2); } else
            if (settings&HIGHLIGHT_CLOSE)    {  XFillRectangle(dpy,w,gc,barWidth-15,1,15,barHeight-2); }
           }

           // Tell the GC we draw using the white color
           XSetForeground(dpy, gc, blackColor);

           XSetLineAttributes(dpy,gc,3,0,0,0);
           /*X*/
           XDrawLine(dpy, w, gc, barWidth - 10, 6 , barWidth - 4 , barHeight-6);
           XDrawLine(dpy, w, gc, barWidth - 10, barHeight-6 , barWidth - 4 , 6);

           /*Maximize*/
           XDrawRectangle(dpy, w ,  gc, barWidth - 25, 7 , 5, barHeight-14 );

           /*Minimize*/
           XDrawLine(dpy, w ,  gc, barWidth - 40, barHeightMiddle , barWidth - 35 , barHeightMiddle );



           if (!(settings&FOCUSED_WINDOW))  XSetForeground(dpy, gc, whiteColor); // We are inactive so we want a white font due to the dark background
           /* Title of Window */
           XDrawString(dpy,w,gc, 40,15, "Title of window goes here",25); //todo add handler for large strings that appends ...

           //Send the requests to the server
           XFlush(dpy);
	     }
      }
}



main()
{
      // Open the display
      Display *dpy = XOpenDisplay(0);
      assert(dpy);

      drawWindowBar(0,0,300,20,FOCUSED_WINDOW|HIGHLIGHT_MINIMIZE,"Title of window goes here",dpy);

      return 0;
}

