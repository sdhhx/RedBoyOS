#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HIGH_MEM (1*1024*1024)    //最大物理内存
#define LOW_MEM (64*1024)      //内存低端
#define START_MEM (256*1024)      //主存起始
#define PAGING_MEMORY (768*1024)          //分页内存，主内存最大值
#define PAGING_PAGES (PAGING_MEMORY>>12)           //分页后的主存页面数
#define MAP_NR(addr) (((addr)-LOW_MEM)>>12)         //指定内存地址映射为页号
#define USED 100            //页面被占用标志

//模拟物理内存，用phy_m[a]表示地址是a，用phy_m[a].word代替*a操作
struct phy_mem
{
    unsigned long word;
}phy_m[HIGH_MEM];

//物理内存占用标志图，值为占用次数，由于本程序没有设计共享内存，故只有1，0，和100
static unsigned char mem_map[PAGING_PAGES] = {0,};


//从主内存中获取空闲物理页面，从后往前遍历mem_map[]
unsigned long get_free_page(void)
{
    int i;
    unsigned long a;
    for(i=PAGING_PAGES-1;i>=0 && mem_map[i]!=0;i--){}   //从后往前找
    if(i==-1)
        return 0;           //没有则返回0
    mem_map[i]=1;       //找到后置1
    a=(((unsigned long)i)<<12)+LOW_MEM;
    for(i=1023;i>=0;i--)        //将本页所有清0
    {
        phy_m[a+i].word=0;
    }
    return a;       //返回空闲页面地址
}


//释放物理地址addr处的一页内存。用于函数free_page_tables()
void free_page(unsigned long addr)
{
	//addr小于LOW_MEM不予处理
	if (addr < LOW_MEM) return;
	if (addr >= HIGH_MEM)
		printf("trying to free nonexistent page");

	//将addr转化为页面号
	addr -= LOW_MEM;
	addr >>= 12;


	//如果页面号对应的页面映射字节不等于0，则减1返回，如果等于0，显示出错并停机
	if (mem_map[addr]--) return;
	mem_map[addr]=0;
	printf("trying to free free page");
}


//释放连续的内存块，以页目录项，也就是页表的整倍数释放
int free_page_tables(unsigned long from,unsigned long m_size)   //from线性地址起始，size要释放内存大小
{
	unsigned long pg_table;
	unsigned long dir, nr;


	//size加上4MB-1再>>22，相当于计算一共多少页目录项数，也即页表数
	m_size = (m_size + 0x3fffff) >> 22;


	dir = ((from>>20) & 0xffc);  //计算目录表项地址

	for ( ; m_size-->0 ; dir++) {
        if (!(1 & phy_m[dir].word))     //如果最后1位有效位为0
			continue;
		pg_table = 0xfffff000 & phy_m[dir].word;   //取页表地址
		for (nr=0 ; nr<1024 ; nr++) {
		    if (1 & phy_m[pg_table].word)
                free_page(0xfffff000 & phy_m[pg_table].word);
			phy_m[pg_table].word = 0;           //该页表项内容清零
			pg_table++;           //指向页表中的下一项
		}
		free_page(0xfffff000 & phy_m[dir].word);      //释放该页面所占内存页面
		phy_m[dir].word = 0;           //对应页表的目录项清零
	}
	return 0;
}

//将一内存页面映射到指定线性地址空间
unsigned long put_page(unsigned long page,unsigned long address)
{
	unsigned long tmp, page_table;
	page_table =((address>>20) & 0xffc);        //页目录项地址
	if (phy_m[page_table].word & 1)         //页目录项有效位为1，说明此页存在
        page_table =(0xfffff000 & phy_m[page_table].word);      //获得页表地址
    else                //若不存在则分配一页空闲物理页面给页表
    {
        if (!(tmp=get_free_page()))
			return 0;
		phy_m[page_table].word = tmp|7;         //将新申请页面填到页目录项并置后三位为1
		page_table =tmp;                //获得页表地址
    }
	phy_m[page_table+((address>>12) & 0x3ff)].word = page|7;    //将物理内存地址填到页表表项并置后三位为1
	return page;
}


//取得一页空闲内存页并映射到物理地址
void get_empty_page(unsigned long address)
{
	unsigned long tmp;

	if (!(tmp=get_free_page()) || !put_page(tmp,address))  //先后调用get_free_page和put_page
    {
		free_page(tmp);                 //如果失败释放以调用页，如果tmp=0也不会出错
		printf("out of momery!");
	}
}


//内存模拟初始化函数
void initi_simulation()
{
    int i;
    for (i=0;i<PAGING_PAGES;i++)        //对所有内存页置used
		mem_map[i] = USED;
    i=(START_MEM-LOW_MEM)>>12;
	while (i<PAGING_PAGES)          //将主存页置0
		mem_map[i++]=0;
}

//计算空闲页数
int calc_mem(void)
{
	int i,free=0;
	long * pg_tbl;

	for(i=0 ; i<PAGING_PAGES ; i++)     //每有一个空闲页，free自加1
		if (!mem_map[i]) free++;
	return free;
}

//模拟线程内存块动作表
int table[1024]={0,};


//模拟获取内存块
unsigned long get_mm(void)
{
    unsigned long i,j;
    unsigned long address,tmp;
    for (i=1;i<1024&&table[i]!=0;i++){}
    if (i==1024)        //到达线性地址上限
        return 0;
    tmp=address= i<<22; //模拟内存块编号i
    table[i]=1;
    srand((unsigned)time(NULL));
    j=(rand()%29)+4;    //随机内存块大小为4~32页
    if(j>calc_mem())    //内存不足分配的情况
    {
        table[i]=0;
        return 0;
    }
    for(;j>0;j--)
    {
        get_empty_page(tmp);
        tmp=tmp+0x1000;        //下一页线性地址
    }
    return i;      //返回内存块编号
}

//查询当前内存页使用状态函数
void status()
{
    int i,j=0;
    //打印mem_cap[]表
    for(i=0;i<PAGING_PAGES;i++)
    {
        printf("%d\t",mem_map[i]);
        j++;
        if(j==9)        //10个1行
        {
            j=0;
            printf("\n");
        }
    }
    //打印页使用情况
    printf("\n%d pages free (of %d)\n\n\r",calc_mem(),PAGING_PAGES);
}

//模拟释放内存块函数
void free_mm(unsigned long i)
{
    unsigned long address;
    address= i<<22;
    free_page_tables(address,0x400000);     //大小设1页页表，远大于内存块大小
    table[i]=0;
}


void memorycontroy(char* d)
{
    int i;

    //while(1)
    //{
    //printf("请输入指令：\ngm：获取一块随机4-32页内存\nfm:选择释放一块内存\nst:当前内存状态\ne:退出\n");
    //scanf("%s",d);
    if(strcmp(d,"gm")==0)   //输入指令gm
    {
        int i=get_mm();
        if(i==0)
            printf("本次内存不足\n");
        else
            printf("内存块%d已申请\n",i);
    }
    else if(strcmp(d,"fm")==0)      //输入指令fm
    {
        int j;
        printf("请输入要释放内存块(0退出)：");

        for(j=1; j<1024; j++)       //判断有哪些内存块
        {
            if(table[j]==1)
                printf("%d ", j);
        }
        printf("\n");
        scanf("%d",&j);         //输入要释放的内存块
        if(j==0)                //0退出
            return;
        else if(table[j]==1)        //判断输入是否有效
        {
            free_mm(j);             //释放该块
            printf("释放成功\n");
        }
        else
            printf("操作无效\n");
    }
    else if(strcmp(d,"st")==0)          //输入指令st
    {
        status();
    }
    else
        printf("输入有误\n");           //输入有误

    //}

}
