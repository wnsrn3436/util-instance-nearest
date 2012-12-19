namespace PointVar
{
    typedef struct _OBJ
    {
        struct _OBJ *next;
        struct _LIST *first, *last;
        double objid;
        INT8 list_check;
    } OBJ;
    
    typedef struct _LIST
    {
        struct _LIST *next;
        double x, y, id;
    } LIST;
    
    OBJ *FIRST, *LAST;
    INT8 Obj_Check = 0;
}
