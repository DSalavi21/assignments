
#include"eventlist.h"

/*********************************************************************
  Add event line into events Link List                                           
  Parameters:
  event : Realtime event line
  eventListFront : Pointer to front node of events link list	
  eventListRear : Pointer to rear node of events link list	
*********************************************************************/

int AddNodeToEventsList(char * event, EventNode **eventListFront, EventNode **eventListRear)
{
        if( event == NULL || strlen(event) == 0 )
        {
                return -1;
        }

        EventNode *temp = NULL;
        int eventLineLength = strlen(event);

        temp = (EventNode *)malloc(sizeof(EventNode));
        if(temp == NULL)
        {
                return -1;
        }

        bzero(temp, sizeof(EventNode));

        temp->str = (char *) malloc( eventLineLength + 1 );
        if(temp->str == NULL)
        {
                if(temp)
                {
                        free(temp);
                        temp = NULL;
                }

                return -1;
        }

        bzero(temp->str , eventLineLength + 1);

        strncpy(temp->str, event, eventLineLength );

        temp->next = NULL;

        //Add current node to Event Link List

        if( *eventListRear == NULL || *eventListFront == NULL)
        {
                *eventListFront = (*eventListRear = temp);
        }
        else
        {
                //Already few node are in Event Link List. So add new node at the Rear
                (*eventListRear)->next = temp;
                *eventListRear = temp;
        }

        return 0;
}

/********************************************************************************
*   Delete node from events Link List                                                   *
********************************************************************************/

int DeleteEventFrmListAtFront(EventNode **eventListFront ,EventNode **eventListRear)
{
        //Empty events Link List
        if(*eventListFront == NULL)
        {
                *eventListFront = NULL;
                return 0;
        }

        //Only one node exist
        else if(*eventListFront == *eventListRear)
        {
                if( (*eventListFront)->str )
                {
                        free((*eventListFront)->str);
                        (*eventListFront)->str = NULL;
                }
                EventNode *temp = *eventListFront;
                *eventListFront = (*eventListFront)->next;

                if( temp )
                {
                        free( temp );
                        temp = NULL;
                }
                *eventListFront = NULL;
                *eventListRear = NULL;
        }

        //Delete first node from event Link List if more than one node exist
        else
        {
                if( (*eventListFront)->str )
                {
                        free((*eventListFront)->str);
                        (*eventListFront)->str = NULL;
                }
                EventNode *temp = *eventListFront;
                *eventListFront = (*eventListFront)->next;

                if( temp )
                {
                        free( temp );
                        temp = NULL;
                }
        }
return 0;
}

