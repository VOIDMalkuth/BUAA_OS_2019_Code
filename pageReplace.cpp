// pageReplace.cpp
#include "pageReplace.h" // 测评需求，请务必包含该头文件
#define MAX_PHY_PAGE 64
#define MAX_PAGE 12
#define get_Page(x) (x >> MAX_PAGE)

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

int helpArray[MAX_PHY_PAGE + 1][2];

void pageReplace(long *physic_memery, long nwAdd)
{
    static int cursor = 1;
    int pg = get_Page(nwAdd);
    int frameVisited = find(physic_memery, MAX_PHY_PAGE, pg);
    if (frameVisited != -1)
    {
        helpArray[frameVisited][0] = cursor++;
        helpArray[frameVisited][1] += 1;
        return;
    }
    // find min in helpArray
    int swapIndex = 0;
    int swapDistance = helpArray[0][0] + helpArray[0][1] * 2;
    for (int i = 0; i < MAX_PHY_PAGE; i++)
    {
        int dist = helpArray[i][0] + helpArray[i][1] * 2;
        if (dist < swapDistance)
        {
            swapIndex = i;
            swapDistance = dist;
        }
    }
    helpArray[swapIndex][0] = cursor++;
    helpArray[swapIndex][1] = 0;
    physic_memery[swapIndex] = pg;

    return;
}
