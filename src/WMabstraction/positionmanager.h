#ifndef POSITIONMANAGER_H_INCLUDED
#define POSITIONMANAGER_H_INCLUDED

int Window_FindNewPosition(unsigned int * x,unsigned int * y , unsigned int * width,unsigned int * height);
void Window_IncreaseSize(void * window , unsigned int * width,unsigned int * height);
void Window_DecreaseSize(void * window , unsigned int * width,unsigned int * height);
void ChangeLayout();

#endif // POSITIONMANAGER_H_INCLUDED
