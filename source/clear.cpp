#include <windows.h>
#include "export.h"

export double in_point_id_clear();
export double in_bbox_id_clear();

export double in_all_clear()
{
    in_point_id_clear();
    in_bbox_id_clear();
    return 0;
}
