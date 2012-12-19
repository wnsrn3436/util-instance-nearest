#include <windows.h>
#include <math.h>
#include "export.h"
#include "object_distance.h"

using namespace ObjectVar;

/*Function*/

Point* intersection(double x1, double y1, double x2, double y2, LIST* list, Point* point)
{
    double d, pre, post, x, y, x3, y3, x4, y4;
    for(int i=0; i!=4; i+=1)
    {
        switch(i)
        {
            case 0:{x3=list->bbox_left; y3=list->bbox_top; x4=list->bbox_left; y4=list->bbox_bottom; break;}
            case 1:{x3=list->bbox_left; y3=list->bbox_bottom; x4=list->bbox_right; y4=list->bbox_bottom; break;}
            case 2:{x3=list->bbox_right; y3=list->bbox_bottom; x4=list->bbox_right; y4=list->bbox_top; break;}
            case 3:{x3=list->bbox_right; y3=list->bbox_top; x4=list->bbox_left; y4=list->bbox_top; break;}
        }
        d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (d == 0) continue;
        
        pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
        x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
        y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
        
        if ( x < min(x1, x2) || x > max(x1, x2) || x < min(x3, x4) || x > max(x3, x4) ) continue;
        if ( y < min(y1, y2) || y > max(y1, y2) || y < min(y3, y4) || y > max(y3, y4) ) continue;
        
        point->x = x;
        point->y = y;
        return point;
    }
    return NULL;
}

/*Export*/

export double in_bbox_id_add(double objid)
{
    if(!Obj_Check)
    {
        FIRST = new OBJ;
        FIRST->objid = objid;
        FIRST->list_check = 0;
        FIRST->next = FIRST;
        LAST = FIRST;
        Obj_Check = 1;
    }
    else
    {
        OBJ *buffer = new OBJ;
        buffer->objid = objid;
        buffer->list_check = 0;
        LAST->next = buffer;
        LAST = buffer;
        LAST->next = FIRST;
    }
    return 0;
}

export double in_bbox_id_clear()
{
    if(Obj_Check)
    {
        OBJ *buffer = FIRST, *buffer2;
        do{
            buffer2 = buffer->next;
            if(buffer->list_check)
            {
                LIST *Lbuffer = buffer->first, *Lbuffer2;
                do{
                    Lbuffer2 = Lbuffer->next;
                    delete Lbuffer;
                    Lbuffer = Lbuffer2;
                }while(Lbuffer != buffer->first);
            }
            delete buffer;
            buffer = buffer2;
        }while(buffer != FIRST);
        Obj_Check = 0;
    }
    return 0;
}

export double in_bbox_data_add(double objid, double x, double y, double bbox_left, double bbox_right, double bbox_top, double bbox_bottom, double id)
{
    if(Obj_Check)
    {
        OBJ *buffer = FIRST;
        do{
            if(buffer->objid==objid)
            {
                if(!buffer->list_check)
                {
                    buffer->first = new LIST;
                    buffer->first->x = x;
                    buffer->first->y = y;
                    buffer->first->bbox_left = bbox_left;
                    buffer->first->bbox_right = bbox_right;
                    buffer->first->bbox_top = bbox_top;
                    buffer->first->bbox_bottom = bbox_bottom;
                    buffer->first->id = id;
                    buffer->first->next = buffer->first;
                    buffer->last = buffer->first;
                    buffer->list_check = 1;
                }
                else
                {
                    LIST *Lbuffer = new LIST;
                    Lbuffer->x = x;
                    Lbuffer->y = y;
                    Lbuffer->bbox_left = bbox_left;
                    Lbuffer->bbox_right = bbox_right;
                    Lbuffer->bbox_top = bbox_top;
                    Lbuffer->bbox_bottom = bbox_bottom;
                    Lbuffer->id = id;
                    buffer->last->next = Lbuffer;
                    buffer->last = Lbuffer;
                    Lbuffer->next = buffer->first;
                }
                break;
            }
            buffer = buffer->next;
        }while(buffer != FIRST);
    }
    return 0;
}

export double in_bbox_data_find(double objid, double x, double y, double id, double distance)
{
    double id_temp = -4, distance_temp;
    if(Obj_Check)
    {
        OBJ *buffer = FIRST;
        do{
            if(buffer->objid==objid)
            {
                if(buffer->list_check)
                {
                    LIST *Lbuffer = buffer->first;
                    distance *= distance;
                    do{
                        if(id!=Lbuffer->id)
                        {
                            if(intersection(x, y, Lbuffer->x, Lbuffer->y, Lbuffer, &point)!=NULL)
                            {
                                distance_temp = pow(x - point.x, 2) + pow(y - point.y, 2);
                                if(distance>distance_temp) //and...
                                {
                                    id_temp = Lbuffer->id;
                                    distance = distance_temp;
                                }
                            }
                        }
                        Lbuffer = Lbuffer->next;
                    }while(Lbuffer != buffer->first);
                }
                break; 
            }
            buffer = buffer->next;
        }while(buffer != FIRST);
    }
    return id_temp;
}

export double in_bbox_data_clear(double objid)
{
    if(Obj_Check)
    {
        OBJ *buffer = FIRST;
        do{
            if(buffer->objid==objid)
            {
                if(buffer->list_check)
                {
                    LIST *Lbuffer = buffer->first, *Lbuffer2;
                    do{
                        Lbuffer2 = Lbuffer->next;
                        delete Lbuffer;
                        Lbuffer = Lbuffer2;
                    }while(Lbuffer != buffer->first);
                    buffer->list_check = 0;
                }
                break; 
            }
            buffer = buffer->next;
        }while(buffer != FIRST);
    }
    return 0;
}

export double in_bbox_data_delete(double objid, double id)
{
    if(Obj_Check)
    {
        OBJ *buffer = FIRST;
        do{
            if(buffer->objid==objid)
            {
                if(buffer->list_check)
                {
                    LIST *Lbuffer = buffer->first, *Lbuffer2;
                    do{
                        if(id==Lbuffer->id)
                        {
                            if(Lbuffer!=buffer->first)
                            {
                                if(Lbuffer==buffer->last){buffer->last = Lbuffer2;}
                                LIST *Lbuffer3 = Lbuffer->next;
                                delete Lbuffer;
                                Lbuffer2->next = Lbuffer3;
                            }
                            else
                            {
                                if(Lbuffer->next!=buffer->first)
                                {
                                    buffer->first = Lbuffer->next;
                                    buffer->last->next = buffer->first;
                                    delete Lbuffer;
                                }
                                else
                                {
                                    delete Lbuffer;
                                    buffer->list_check = 0;
                                }
                            }
                            break;
                        }
                        Lbuffer2 = Lbuffer;
                        Lbuffer = Lbuffer->next;
                    }while(Lbuffer != buffer->first);
                }
                break; 
            }
            buffer = buffer->next;
        }while(buffer != FIRST);
    }
    return 0;
}

export double in_bbox_data_reset(double objid, double id, double x, double y, double bbox_left, double bbox_right, double bbox_top, double bbox_bottom, double id2)
{
    if(Obj_Check)
    {
        OBJ *buffer = FIRST;
        do{
            if(buffer->objid==objid)
            {
                if(buffer->list_check)
                {
                    LIST *Lbuffer = buffer->first;
                    do{
                        if(id==Lbuffer->id)
                        {
                            Lbuffer->x = x;
                            Lbuffer->y = y;
                            Lbuffer->bbox_left = bbox_left;
                            Lbuffer->bbox_right = bbox_right;
                            Lbuffer->bbox_top = bbox_top;
                            Lbuffer->bbox_bottom = bbox_bottom;
                            Lbuffer->id = id2;
                            break;
                        }
                        Lbuffer = Lbuffer->next;
                    }while(Lbuffer != buffer->first);
                }
                break; 
            }
            buffer = buffer->next;
        }while(buffer != FIRST);
    }
    return 0;
}
