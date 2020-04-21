// pageReplace.cpp
#include "pageReplace.h" // 测评需求，请务必包含该头文件
#define MAX_PHY_PAGE 64
#define MAX_PAGE 12
#define get_Page(x) (x >> MAX_PAGE)

#define MAX_SINGLEIN 32

int find(long *array, int n, long val);

inline int find(long *array, int n, long val)
{
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

void pageReplace(long *physic_memery, long nwAdd)
{
    static int cursor = 1;
    int pg = get_Page(nwAdd);
    int frameVisited = find(physic_memery, MAX_PHY_PAGE, pg);
    if (frameVisited != -1)
    {
        helpArray[frameVisited] = cursor;
        cursor++;
        return;
    }
    // find min in helpArray
    int swapIndex = 0;
    int swapDistance = helpArray[0];
    for (int i = 0; i < MAX_PHY_PAGE; i++)
    {
        if (helpArray[i] < swapDistance)
        {
            swapIndex = i;
            swapDistance = helpArray[i];
        }
    }

    helpArray[swapIndex] = cursor++;
    physic_memery[swapIndex] = pg;
    if (find(physic_memery, MAX_PHY_PAGE, pg + 1) == -1) {
        helpArray[swapIndex + MAX_SINGLEIN] = cursor++;
        physic_memery[swapIndex + MAX_SINGLEIN] = pg + 1;         
    }

    return;
}
