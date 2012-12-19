# InstanceNearest

범위 안에서 자기 자신을 뺀 가장 가까운 인스턴스를 찾아 주는 게임메이커용 C++ DLL 이다. 오브젝트마다 인스턴스의 좌표를 연결 리스트에 등록해 두고, 찾을 때 그 리스트를 훑어 거리의 제곱을 비교한다. 중심점 사이의 거리로 재는 방식과 바운딩박스까지의 거리로 재는 방식을 따로 두어 크기가 다른 오브젝트 사이에서도 실제로 가까운 쪽을 고를 수 있다. DLL 과 게임메이커 확장 패키지, 예제로 구성된다.

<p>
  <img src="docs/screenshots/screenshot-1.png" width="480" alt="예제 화면">
</p>


## 사용 방법

Releases에서 받은 압축 파일에는 게임메이커 8 확장 패키지 `instance-nearest.gex` 와 예제 프로젝트가 들어 있다. 확장 패키지를 설치하고 프로젝트에 포함하면 된다.

먼저 오브젝트를 등록하고, 그 오브젝트의 인스턴스를 좌표와 함께 등록한다. 찾을 때는 기준 위치와 자기 id, 거리를 준다.

```gml
in_point_id_add(obj_enemy)                                   // 오브젝트 등록. 한 번만
with (obj_enemy) in_point_data_add(obj_enemy, x, y, id)      // 인스턴스 등록

target = in_point_data_find(obj_enemy, x, y, id, 90)         // 90 안에서 가장 가까운 것. 없으면 noone
```

| 함수 | 동작 |
|---|---|
| `in_point_id_add(objid)` | 오브젝트를 등록한다 |
| `in_point_data_add(objid, x, y, id)` | 인스턴스를 좌표와 id 로 등록한다 |
| `in_point_data_find(objid, x, y, id, distance)` | `(x, y)` 에서 `distance` 안에 있는 `objid` 인스턴스 중 `id` 를 뺀 가장 가까운 것의 id 를 돌려준다. 없으면 `noone` |
| `in_point_data_reset(objid, id, x, y, id2)` | 등록된 인스턴스의 좌표와 id 를 바꾼다. 움직이는 인스턴스는 매 스텝 호출한다 |
| `in_point_data_delete(objid, id)` | 인스턴스 하나를 뺀다 |
| `in_point_data_clear(objid)` | 그 오브젝트의 인스턴스를 전부 뺀다 |
| `in_point_id_clear()` | 등록을 전부 지운다 |

바운딩박스 계열은 이름이 `in_bbox_` 로 시작하고 `in_bbox_data_add(objid, x, y, bbox_left, bbox_right, bbox_top, bbox_bottom, id)`, `in_bbox_data_reset(objid, id, x, y, bbox_left, bbox_right, bbox_top, bbox_bottom, id2)` 처럼 상자 좌표를 더 받는다. 나머지 함수는 인자가 같다. 두 계열은 서로 독립이고 `in_all_clear()` 가 둘 다 지운다.

예제는 방향키로 캐릭터를 움직인다. 반지름 90 안에 바위(바운딩박스 계열)나 불(점 계열)이 들어오면 캐릭터 위에 I found it! 이 뜬다.


## 구현 원리

**오브젝트마다 인스턴스 리스트를 둔다.** 등록한 오브젝트가 원형 연결 리스트로 이어지고, 각 오브젝트 노드가 자기 인스턴스의 원형 연결 리스트를 갖는다. 찾을 때는 오브젝트 노드를 찾아 그 리스트만 훑는다.

```cpp
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
```

**거리는 제곱끼리 비교한다.** 범위를 먼저 제곱해 두고 인스턴스마다 좌표 차의 제곱합을 구해 견준다. 제곱근을 한 번도 구하지 않는다. 더 가까운 것이 나올 때마다 범위를 그 값으로 줄이므로 끝까지 훑으면 가장 가까운 것이 남는다.

```cpp
distance *= distance;
do{
    if(id!=Lbuffer->id)
    {
        distance_temp = pow(x - Lbuffer->x, 2) + pow(y - Lbuffer->y, 2);
        if(distance>distance_temp)
        {
            id_temp = Lbuffer->id;
            distance = distance_temp;
        }
    }
    Lbuffer = Lbuffer->next;
}while(Lbuffer != buffer->first);
```

**바운딩박스 거리는 상자 변과 만나는 점까지 잰다.** 기준 위치에서 인스턴스 중심으로 선을 긋고 상자의 네 변과 차례로 교차 검사해 처음 만나는 점을 구한 뒤, 그 점까지의 거리로 비교한다. 큰 오브젝트는 중심이 멀어도 가장자리가 가까우면 잡힌다. 기준 위치가 상자 안에 있으면 만나는 변이 없어 건너뛴다.

**값은 전부 double 이다.** 게임메이커가 DLL 과 주고받는 값이 double 뿐이라 오브젝트와 인스턴스 id, 좌표를 모두 double 로 두었고, 없을 때는 게임메이커의 `noone` 과 같은 -4 를 돌려준다.


## 파일

| 경로 | 내용 |
|---|---|
| `source/point_distance.cpp`, `source/point_distance.h` | 점 거리 계열 |
| `source/object_distance.cpp`, `source/object_distance.h` | 바운딩박스 거리 계열 |
| `source/clear.cpp`, `source/export.h` | 전체 지우기와 내보내기 선언 |
| `source/InstanceNearest.dev`, `source/Makefile.win` | wxDev-C++ 프로젝트와 MinGW 메이크파일 |
| `docs/screenshots/` | 스크린샷 |
| Releases | 확장 패키지와 예제 |


## 라이선스

zlib 라이선스다. 자세한 내용은 [LICENSE](LICENSE) 에 있다.
