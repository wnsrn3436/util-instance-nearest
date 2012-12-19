namespace ObjectVar
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
        double x, y, bbox_left, bbox_right, bbox_top, bbox_bottom, id;
    } LIST;
    
    typedef struct _Point
    {
        double x, y;
    } Point;
    
    OBJ *FIRST, *LAST;
    INT8 Obj_Check = 0;
    Point point;
}
