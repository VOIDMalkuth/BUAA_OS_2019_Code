// pageReplace.cpp
#include "pageReplace.h" // 测评需求，请务必包含该头文件
#define MAX_PHY_PAGE 64
#define MAX_PAGE 12
#define get_Page(x) (x >> MAX_PAGE)

void pageReplace(long *physic_memery, long nwAdd)
{
    int flag = 0;
    static int clock = 0;
    for (int i = 0; i < MAX_PHY_PAGE; i++)
    {
        if ((nwAdd >> MAX_PAGE) == physic_memery[i])
        {
            return;
        }
    }
    for (int i = 0; i < MAX_PHY_PAGE; i++)
    {
        if (physic_memery[i] == 0)
        { //初始物理页框内容全部为0
            physic_memery[i] = get_Page(nwAdd);
            flag = 1;
            break;
        }
    }
    if (flag == 0)
        physic_memery[(clock++) % MAX_PHY_PAGE] =
            get_Page(nwAdd);
}
