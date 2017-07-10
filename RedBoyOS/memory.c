#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HIGH_MEM (1*1024*1024)    //��������ڴ�
#define LOW_MEM (64*1024)      //�ڴ�Ͷ�
#define START_MEM (256*1024)      //������ʼ
#define PAGING_MEMORY (768*1024)          //��ҳ�ڴ棬���ڴ����ֵ
#define PAGING_PAGES (PAGING_MEMORY>>12)           //��ҳ�������ҳ����
#define MAP_NR(addr) (((addr)-LOW_MEM)>>12)         //ָ���ڴ��ַӳ��Ϊҳ��
#define USED 100            //ҳ�汻ռ�ñ�־

//ģ�������ڴ棬��phy_m[a]��ʾ��ַ��a����phy_m[a].word����*a����
struct phy_mem
{
    unsigned long word;
}phy_m[HIGH_MEM];

//�����ڴ�ռ�ñ�־ͼ��ֵΪռ�ô��������ڱ�����û����ƹ����ڴ棬��ֻ��1��0����100
static unsigned char mem_map[PAGING_PAGES] = {0,};


//�����ڴ��л�ȡ��������ҳ�棬�Ӻ���ǰ����mem_map[]
unsigned long get_free_page(void)
{
    int i;
    unsigned long a;
    for(i=PAGING_PAGES-1;i>=0 && mem_map[i]!=0;i--){}   //�Ӻ���ǰ��
    if(i==-1)
        return 0;           //û���򷵻�0
    mem_map[i]=1;       //�ҵ�����1
    a=(((unsigned long)i)<<12)+LOW_MEM;
    for(i=1023;i>=0;i--)        //����ҳ������0
    {
        phy_m[a+i].word=0;
    }
    return a;       //���ؿ���ҳ���ַ
}


//�ͷ������ַaddr����һҳ�ڴ档���ں���free_page_tables()
void free_page(unsigned long addr)
{
	//addrС��LOW_MEM���账��
	if (addr < LOW_MEM) return;
	if (addr >= HIGH_MEM)
		printf("trying to free nonexistent page");

	//��addrת��Ϊҳ���
	addr -= LOW_MEM;
	addr >>= 12;


	//���ҳ��Ŷ�Ӧ��ҳ��ӳ���ֽڲ�����0�����1���أ��������0����ʾ����ͣ��
	if (mem_map[addr]--) return;
	mem_map[addr]=0;
	printf("trying to free free page");
}


//�ͷ��������ڴ�飬��ҳĿ¼�Ҳ����ҳ����������ͷ�
int free_page_tables(unsigned long from,unsigned long m_size)   //from���Ե�ַ��ʼ��sizeҪ�ͷ��ڴ��С
{
	unsigned long pg_table;
	unsigned long dir, nr;


	//size����4MB-1��>>22���൱�ڼ���һ������ҳĿ¼������Ҳ��ҳ����
	m_size = (m_size + 0x3fffff) >> 22;


	dir = ((from>>20) & 0xffc);  //����Ŀ¼�����ַ

	for ( ; m_size-->0 ; dir++) {
        if (!(1 & phy_m[dir].word))     //������1λ��ЧλΪ0
			continue;
		pg_table = 0xfffff000 & phy_m[dir].word;   //ȡҳ���ַ
		for (nr=0 ; nr<1024 ; nr++) {
		    if (1 & phy_m[pg_table].word)
                free_page(0xfffff000 & phy_m[pg_table].word);
			phy_m[pg_table].word = 0;           //��ҳ������������
			pg_table++;           //ָ��ҳ���е���һ��
		}
		free_page(0xfffff000 & phy_m[dir].word);      //�ͷŸ�ҳ����ռ�ڴ�ҳ��
		phy_m[dir].word = 0;           //��Ӧҳ���Ŀ¼������
	}
	return 0;
}

//��һ�ڴ�ҳ��ӳ�䵽ָ�����Ե�ַ�ռ�
unsigned long put_page(unsigned long page,unsigned long address)
{
	unsigned long tmp, page_table;
	page_table =((address>>20) & 0xffc);        //ҳĿ¼���ַ
	if (phy_m[page_table].word & 1)         //ҳĿ¼����ЧλΪ1��˵����ҳ����
        page_table =(0xfffff000 & phy_m[page_table].word);      //���ҳ���ַ
    else                //�������������һҳ��������ҳ���ҳ��
    {
        if (!(tmp=get_free_page()))
			return 0;
		phy_m[page_table].word = tmp|7;         //��������ҳ���ҳĿ¼��ú���λΪ1
		page_table =tmp;                //���ҳ���ַ
    }
	phy_m[page_table+((address>>12) & 0x3ff)].word = page|7;    //�������ڴ��ַ�ҳ�����ú���λΪ1
	return page;
}


//ȡ��һҳ�����ڴ�ҳ��ӳ�䵽�����ַ
void get_empty_page(unsigned long address)
{
	unsigned long tmp;

	if (!(tmp=get_free_page()) || !put_page(tmp,address))  //�Ⱥ����get_free_page��put_page
    {
		free_page(tmp);                 //���ʧ���ͷ��Ե���ҳ�����tmp=0Ҳ�������
		printf("out of momery!");
	}
}


//�ڴ�ģ���ʼ������
void initi_simulation()
{
    int i;
    for (i=0;i<PAGING_PAGES;i++)        //�������ڴ�ҳ��used
		mem_map[i] = USED;
    i=(START_MEM-LOW_MEM)>>12;
	while (i<PAGING_PAGES)          //������ҳ��0
		mem_map[i++]=0;
}

//�������ҳ��
int calc_mem(void)
{
	int i,free=0;
	long * pg_tbl;

	for(i=0 ; i<PAGING_PAGES ; i++)     //ÿ��һ������ҳ��free�Լ�1
		if (!mem_map[i]) free++;
	return free;
}

//ģ���߳��ڴ�鶯����
int table[1024]={0,};


//ģ���ȡ�ڴ��
unsigned long get_mm(void)
{
    unsigned long i,j;
    unsigned long address,tmp;
    for (i=1;i<1024&&table[i]!=0;i++){}
    if (i==1024)        //�������Ե�ַ����
        return 0;
    tmp=address= i<<22; //ģ���ڴ����i
    table[i]=1;
    srand((unsigned)time(NULL));
    j=(rand()%29)+4;    //����ڴ���СΪ4~32ҳ
    if(j>calc_mem())    //�ڴ治���������
    {
        table[i]=0;
        return 0;
    }
    for(;j>0;j--)
    {
        get_empty_page(tmp);
        tmp=tmp+0x1000;        //��һҳ���Ե�ַ
    }
    return i;      //�����ڴ����
}

//��ѯ��ǰ�ڴ�ҳʹ��״̬����
void status()
{
    int i,j=0;
    //��ӡmem_cap[]��
    for(i=0;i<PAGING_PAGES;i++)
    {
        printf("%d\t",mem_map[i]);
        j++;
        if(j==9)        //10��1��
        {
            j=0;
            printf("\n");
        }
    }
    //��ӡҳʹ�����
    printf("\n%d pages free (of %d)\n\n\r",calc_mem(),PAGING_PAGES);
}

//ģ���ͷ��ڴ�麯��
void free_mm(unsigned long i)
{
    unsigned long address;
    address= i<<22;
    free_page_tables(address,0x400000);     //��С��1ҳҳ��Զ�����ڴ���С
    table[i]=0;
}


void memorycontroy(char* d)
{
    int i;

    //while(1)
    //{
    //printf("������ָ�\ngm����ȡһ�����4-32ҳ�ڴ�\nfm:ѡ���ͷ�һ���ڴ�\nst:��ǰ�ڴ�״̬\ne:�˳�\n");
    //scanf("%s",d);
    if(strcmp(d,"gm")==0)   //����ָ��gm
    {
        int i=get_mm();
        if(i==0)
            printf("�����ڴ治��\n");
        else
            printf("�ڴ��%d������\n",i);
    }
    else if(strcmp(d,"fm")==0)      //����ָ��fm
    {
        int j;
        printf("������Ҫ�ͷ��ڴ��(0�˳�)��");

        for(j=1; j<1024; j++)       //�ж�����Щ�ڴ��
        {
            if(table[j]==1)
                printf("%d ", j);
        }
        printf("\n");
        scanf("%d",&j);         //����Ҫ�ͷŵ��ڴ��
        if(j==0)                //0�˳�
            return;
        else if(table[j]==1)        //�ж������Ƿ���Ч
        {
            free_mm(j);             //�ͷŸÿ�
            printf("�ͷųɹ�\n");
        }
        else
            printf("������Ч\n");
    }
    else if(strcmp(d,"st")==0)          //����ָ��st
    {
        status();
    }
    else
        printf("��������\n");           //��������

    //}

}
