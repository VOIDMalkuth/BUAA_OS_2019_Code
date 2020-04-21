// pageReplace.cpp
//#include "pageReplace.h" // 测评需求，请务必包含该头文件
#define MAX_PHY_PAGE 64
#define MAX_PAGE 12
#define get_Page(x) (x >> MAX_PAGE)

int find(long *array, int n, long val);

inline int find(long *array, int n, long val) {
    for (int i = 0; i < n; i++)
    {
        if (array[i] == val)
        {
            return i;
        }
    }
    return -1;
}

int helpArray[MAX_PHY_PAGE + 1];

void pageReplace(long *physic_memery, long nwAdd) {
    static int head = 0;
    int pg = get_Page(nwAdd);

    if (find(physic_memery, MAX_PHY_PAGE, pg) != -1) {
        return;
    }
    physic_memery[head] = pg;
    head = (head + 1) % MAX_PAGE;

    return;
}
