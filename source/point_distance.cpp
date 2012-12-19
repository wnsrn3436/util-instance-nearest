#include <windows.h>
#include <math.h>
#include "export.h"
#include "point_distance.h"

using namespace PointVar;

export double in_point_id_add(double objid)
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

export double in_point_id_clear()
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

export double in_point_data_add(double objid, double x, double y, double id)
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

export double in_point_data_find(double objid, double x, double y, double id, double distance)
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
                            distance_temp = pow(x - Lbuffer->x, 2) + pow(y - Lbuffer->y, 2);
                            if(distance>distance_temp) //and...
                            {
                                id_temp = Lbuffer->id;
                                distance = distance_temp;
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

export double in_point_data_clear(double objid)
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

export double in_point_data_delete(double objid, double id)
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

export double in_point_data_reset(double objid, double id, double x, double y, double id2)
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
