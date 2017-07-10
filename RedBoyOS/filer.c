#include <stdio.h>
#include <string.h>
#define NAME_LEN 14  //文件名长度
#define CHILD_NUM_LIMIT 256  //孩子节点数量上限
#define ROOTNUM 1  //根节点编号
#define FILE_NUM_LIMIT 8000  //文件数量上限

struct m_inode
{
    char name[NAME_LEN];//文件名
    int father;//父节点编号
    int isdir;//是否为目录
    int child_num;//孩子数量
    int child[CHILD_NUM_LIMIT];//孩子编号
} m_node[20010];

int currentdir = ROOTNUM;//当前目录
int i_table[20010];//空闲inode表

//从文件中读取文件系统信息
void file_fpb_read(int inode)
{
    int i;

    char filename[NAME_LEN], filename_1[NAME_LEN];
    sprintf(filename, "%d", inode);//把inode转换为字符串

    freopen(filename, "r", stdin);//重定向文件流
    scanf("%s", m_node[inode].name);
    scanf("%d", &m_node[inode].father);
    scanf("%d", &m_node[inode].isdir);
    if(m_node[inode].isdir == 1)//若是目录
    {
        scanf("%d", &m_node[inode].child_num);
        for(i = 1; i <= m_node[inode].child_num; i++)//每个孩子节点赋值
        {
            scanf("%d", &m_node[inode].child[i]);
        }
        for(i = 1; i <= m_node[inode].child_num; i++)//每个孩子节点赋值
        {
            //scanf("%d", &m_node[inode].child[i]);
            sprintf(filename_1, "%d", m_node[inode].child[i]);
            freopen(filename_1, "r", stdin);//定向子节点
            scanf("%s", m_node[m_node[inode].child[i]].name);
            scanf("%d", &m_node[m_node[inode].child[i]].father);
            scanf("%d", &m_node[m_node[inode].child[i]].isdir);
            freopen(filename, "r", stdin);//文件流定向回去
        }
    }
    freopen("CON","r",stdin);
}

//向文件写入文件系统信息
void file_fpb_write(int inode)
{
    int i;

    char filename[NAME_LEN];
    sprintf(filename, "%d", inode);//当前节点

    freopen(filename, "w", stdout);//输出流重定向
    printf("%s\n", m_node[inode].name);
    printf("%d\n", m_node[inode].father);

    printf("%d\n", m_node[inode].isdir);
    if(m_node[inode].isdir == 1)//若为目录写入更多信息
    {
        printf("%d\n", m_node[inode].child_num);//
        for(i = 1; i <= m_node[inode].child_num; i++)//遍历当前目录孩子节点
        {
            printf("%d ", m_node[inode].child[i]);
        }
    }
    freopen("CON", "w", stdout);//定向回控制台
}

//从文件中读取空闲表信息
void i_table_read()
{
    int i;
    freopen("inode_table", "r", stdin);//输入流重定向
    for(i=1; i<=FILE_NUM_LIMIT; i++)//读入inode空闲表
    {
        scanf("%d",&i_table[i]);
    }
    freopen("CON","r",stdin);//输入流定向回控制台
}

//将空闲表信息写入文件
void i_table_write()
{
    int i;
    freopen("inode_table", "w", stdout);//输出流重定向
    for(i=1; i<=FILE_NUM_LIMIT; i++)//inode空闲表写回
    {
        printf("%d ", i_table[i]);
    }
    freopen("CON", "w", stdout);//输出流定向回控制台
}

//文件系统初始化
void file_init()
{
    i_table_read();//inode空闲表读入
    file_fpb_read(1);//读入根目录信息
}

//显示当前目录中的文件
void ls()
{
    int i;
    printf("%s:\n", m_node[currentdir].name);
    for(i = 1; i <= m_node[currentdir].child_num; i++)
    {
        printf("\t%s\t", m_node[m_node[currentdir].child[i]].name);
        printf("%s\n", m_node[m_node[currentdir].child[i]].isdir == 1 ? "folder":"file");
    }
    printf("\n");
}

//当前目录转换为上层目录或下层目录
void cd(char* filename)
{
    int i;
    if(strcmp(filename, "..") == 0)//上层目录
    {
        if(currentdir == ROOTNUM)//若为根节点则不处理
            return;
        else
        {
            currentdir = m_node[currentdir].father;//当前目录转换为上层目录
            file_fpb_read(currentdir);//读该目录信息
            return;
        }
    }
    else
    {
        for(i = 1; i <= m_node[currentdir].child_num; i++)//寻找与filename名字相同的孩子节点
        {
            if(strcmp(filename, m_node[m_node[currentdir].child[i]].name) == 0)//匹配下层节点
            {
                if(m_node[m_node[currentdir].child[i]].isdir == 0)//不是目录
                {
                    printf("sorry, %s is not a folder\n\n", filename);
                    return;
                }
                else//是目录
                {
                    currentdir = m_node[currentdir].child[i];//当前目录转换为下层目录
                    file_fpb_read(currentdir);//读该目录信息
                    return;
                }
            }
        }
        printf("sorry, there is no such folder\n\n");//找不到匹配的孩子节点
        return;
    }
}

//创建文件或目录
void create(char* filename, int isdir)
{
    int inode, i;

    if(strlen(filename) >= NAME_LEN)//文件名过长
    {
        printf("sorry, the name is too long\n\n");
        return;
    }

    if(m_node[currentdir].child_num == CHILD_NUM_LIMIT)//目录满
    {
        printf("sorry, the folder is full\n\n");
        return;
    }

    for(i = 1; i <= m_node[currentdir].child_num; i++)//有重名文件
    {
        if(strcmp(filename, m_node[m_node[currentdir].child[i]].name) == 0)
        {
            printf("same name!\n\n");
            return;
        }
    }

    for(inode = 0, i = 1; i <= FILE_NUM_LIMIT; i++)//查找空闲inode
    {
        if(i_table[i] == 0 && inode == 0)//若inode空闲且是第一个
        {
            i_table[i] = 1;
            inode = i;
            break;
        }
    }
    if(inode == 0)//没有空闲
    {
        printf("sorry, the disk is full\n\n");
        return;
    }

    i_table_write();//m_node与空闲表操作
    m_node[currentdir].child_num++;//当前目录孩子节点数增加
    m_node[currentdir].child[m_node[currentdir].child_num] = inode;//当前目录孩子节点中加入新节点
    strcpy(m_node[inode].name, filename);//为新节点赋值
    m_node[inode].father = currentdir;
    m_node[inode].isdir = isdir;
    m_node[inode].child_num = 0;
    printf("create success\n\n");

    file_fpb_write(currentdir);//写回节点信息
    file_fpb_write(inode);//写回节点信息
}
//递归删除inode，用于目录删除
void rm(int inode)
{
    int i;
    if(m_node[inode].isdir == 1)//若inode是目录
    {
        for(i = 1; i <= m_node[inode].child_num; i++)//遍历当前目录孩子节点
        {
            rm(m_node[inode].child[i]);//递归处理
        }
    }
    i_table[inode] = 0;//删除inode
}

//删除文件或目录
void del(char* filename)
{
    int i;

    for(i = 1; i <= m_node[currentdir].child_num; i++)//遍历当前目录孩子节点
    {
        if(strcmp(filename, m_node[m_node[currentdir].child[i]].name) == 0)//遍历当前目录孩子节点
        {
            rm(m_node[currentdir].child[i]);//递归删除inode
            i_table_write();//inode空闲表写回
            for(; i <= m_node[currentdir].child_num - 1; i++)//将孩子节点从删除处前移一位
            {
                m_node[currentdir].child[i] = m_node[currentdir].child[i+1];
            }
            m_node[currentdir].child_num--;//当前目录孩子节点数减1
            file_fpb_write(currentdir);//写回节点信息
            printf("delete success\n\n");//删除成功
            return;
        }
    }
    printf("sorry, there is no such name\n\n");//找不到匹配的孩子节点
}


/*main()
{
    file_init();
    create("asdfadsf", 1);
    create("asdfadsf",0);
    create("qwerqwer",0);
    cd("asdfadsf");
    create("zxcvzxcv",0);
    del("zxcvzxcv");
    cd("..");
    del("asdfadsf");
    del("qwerqwer");

}//²âÊÔ*/
