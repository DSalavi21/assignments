#ifndef __EVENTLIST_H__
#define __EVENTLIST_H__

#include<string.h>
#include<stdlib.h>

typedef struct node
{
        char                    *str;
        struct node             *next;
}EventNode;

int DeleteEventFrmListAtFront(EventNode **eventListFront ,EventNode **eventListRear);
int AddNodeToEventsList(char * event , EventNode **eventListFront ,EventNode **eventListRear);

#endif
