#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*进程控制块数据结构*/
typedef struct node
{
    char name[10];      //进程名
    int prio;          //进程优先级
    int round;        //进程分配的时间片
    int cpu_time;     //进程消耗的CPU时间
    int need_time;   //进程需要的CPU时间
    int count;     //进程运行时间
    char state;   //进程的状态：'R':运行,'W'：等待,'F'：结束
    struct node *next;//指向下一个进程的指针
} PCB;
PCB *finish,*ready,*tail,*run;//指向三个队列的队首的指针，tail为就绪队列的队尾指针
int N;//定义进程的数目


/*各个函数声明 */
void firstin(void);          //函数功能： 将进程就绪队列中第一个放进运行队列
void prt1(char a);          //函数功能：输出进程信息的标题函数
void prt2(char a,PCB *p);  //函数功能：输出单个进程信息的函数
void prt(char algo);      //函数功能：输出所有进程信息的函数
void insert_prio(PCB *q);           //函数功能：优先级法调度将进程插入到就绪队列算法
void insert_round(PCB *q);         //函数功能：时间片轮转算法调度将进程插入到就绪队列算法
void pcreate_task(char algo);     //函数功能：采用优先级进程调度法时，进程初始化函数
void rcreate_task(char algo);    //函数功能：采用时间片轮转法进程调度法时，进程初始化函数
void priority(char algo);       //函数功能：采用优先级进程调度法时，进程调度函数
void roundrun(char algo);      //函数功能：采用时间片轮转法进程调度法时，进程调度函数


/*main 函数*/
void process()
{
    char algo;
    printf("Choose the type of attemper P:priority R:timeround\n");   //输入调度类型，p：优先级    r：时间片
    scanf("%c",&algo);
    if((algo!='P')&&(algo!='p')&&(algo!='r')&&(algo!='R'))
    {
        printf("Unkonwn type of attemper\n");
        return;
    }
    printf("Please enter the number of processes N:\n");           //输入进程数
    scanf("%d",&N);
    if(N<=0)
    {
        printf("Illegal input\n");
        return;
    }
    if((algo=='P')||(algo=='p'))
    {
        pcreate_task(algo);     //调用优先级初始化调度函数
        priority(algo);        //调用优先级调度函数
    }
    else if((algo=='r')||(algo=='R'))
    {
        rcreate_task(algo);     //调用时间片初始化调度函数
        roundrun(algo);        //调用时间片调度函数
    }
}


void firstin(void)        //将就绪队列中的第一个进程放进运行队列
{
    if(ready!=NULL)    //就绪队列不空
    {
        run=ready;              //执行队列指向就绪队列首指针
        ready=ready->next;     //就绪队列首指针下移
        run->state='R';       //状态变为执行
        run->next=NULL;
    }
    else                //就绪队列为空
    {
        run=NULL;    //没有可执行的进程
    }
}

void prt1(char a)     //打印进程的标题信息
{
    if(toupper(a)=='P')
    {
        printf("name\tcputime\tneedtime priority state \n");
    }
    else
    {
        printf("name\tcputime\tneedtime count\tround\tstate \n");
    }
}



void prt2(char a,PCB *p)     //打印单个进程信息
{
    if(toupper(a)=='P')
    {
        printf("%s,\t%d,\t%d,\t %d,\t  %c\n",p->name,p->cpu_time,p->need_time,p->prio,p->state);
    }
    else
    {
        printf("%s,\t%d,\t%d,\t %d,\t%d,\t%c\n",p->name,p->cpu_time,p->need_time,p->count,p->round,p->state);
    }
}



void prt(char algo)    //打印所有的进程信息
{
    PCB *p;
    prt1(algo);
    if(run!=NULL)    //打印执行的进程
    {
        prt2(algo,run);
    }
    p=ready;
    while(p!=NULL)    //循环打印就绪队列的进程信息，直到没有进程
    {
        prt2(algo,p);
        p=p->next;
    }
    p=finish;
    while(p!=NULL)   //循环打印终止队列的进程信息，知道没有进程
    {
        prt2(algo,p);
        p=p->next;
    }
    //getchar();
}



void insert_prio(PCB *q)     //优先级调度算法插入就绪队列
{
    PCB *p,*s,*r; /*p,r用来控制就绪队列滚动，S指向插入的队列*/
    int b; /*b作为插入控制标志的*/
    s=q;
    p=ready;
    r=p;
    b=1;
    if(s->prio>=ready->prio)     //若当前进程优先级高于就绪队列的收割优先级
    {
        s->next=ready;    //将当前进程设为就绪队列的首个进程
        ready=s;
    }
    else
    {
        while((p!=NULL)&&b)      //从就绪队列遍历，找到第一个优先级低于当前进程的指针
        {
            if(p->prio>=s->prio)
            {
                r=p;
                p=p->next;
            }
            else     //若没有找到，则表示当前进程优先级最低 ，并改变标志位
            {
                b=0;
            }
        }
        s->next=p;
        r->next=s;
    }
}




void insert_round(PCB *q)    //时间片轮转算法插入就绪队列
{
    tail->next=q;   //将当前进程插入到队尾
    tail=q;
    q->next=NULL;
}



void pcreate_task(char algo)     //优先级调度进程初始化
{
    PCB *p;
    int i,time;
    char na[10];
    ready=NULL;
    finish=NULL;
    run=NULL;
    for(i=0; i<N; i++)
    {
        p=(PCB*)malloc(sizeof(PCB));
        printf("Enter the name of process\n");   //输入进程名
        scanf("%s",na);
        printf("Enter the time of process\n");    //输入进程所用时间
        scanf("%d",&time);
        while(time<=0)
        {
            printf("Wrong input.\n");
            printf("Enter the time of process\n");
            scanf("%d",&time);
        }
        strcpy(p->name,na);   //名字赋值
        p->cpu_time=0;       //cpu时间初始化为0
        p->need_time=time;  //需要时间初始化
        p->state='W';      //状态设为等待
        p->prio=time;
        if(ready==NULL)    //就绪队列空，将当前进程设为首指针
        {
            ready=p;
            ready->next=NULL;
        }
        else
        {
            insert_prio(p);    //插入到就绪队列
        }
        printf("Output the waiting processes information\n");    //打印等待进程的信息
        prt(algo);    //打印整个进程的信息
    }
    firstin();
}



void rcreate_task(char algo)     //时间片调度进程初始化
{
    PCB *p;
    int i,time;
    char na[10];
    ready=NULL;      //就绪队列为空
    finish=NULL;    //终止队列为空
    run=NULL;      //执行队列为空
    for(i=0; i<N; i++)
    {
        p=(PCB*)malloc(sizeof(PCB));    //分配内存
        printf("Enter the name of process\n");    //进程命名
        scanf("%s",na);
        printf("Enter the time of process\n");    //输入进程需要的时间
        scanf("%d",&time);
        while(time<=0)
        {
            printf("Wrong input\n");
            printf("Enter the time of process\n");
            scanf("%d",&time);
        }
        strcpy(p->name,na);    //名字赋值
        p->cpu_time=0;        //占用cpu时间为0
        p->need_time=time;   //需要时间赋值
        p->count=0;        //进程消耗总时间
        p->state='W';    //状态初始化
        p->round=2;     //分配时间片为2
        if(ready!=NULL)     //就绪队列不空
        {
            insert_round(p);    //插入队列
        }
        else     //就绪队列空
        {
            p->next=ready;    //直接将当前进程设为就绪队列首指针
            ready=p;
            tail=p;
        }
        printf("Output the waiting processes information\n");    //打印等待信息
        prt(algo);
    }
    run=ready;      //状态改变
    ready=ready->next;
    run->state='R';
}



void priority(char algo)     //优先级调度
{
    while(run!=NULL)     //执行队列不空
    {
        run->cpu_time+=1;     //执行的进程占用的cpu时间+1
        run->need_time-=1;   //还需要的执行时间-1
        run->prio-=3;       //优先级降低3
        if(run->need_time==0)    //进程执行完毕
        {
            run->next=finish;    //进入终止状态
            finish=run;
            run->state='F';
            run=NULL;     //执行队列空
            firstin();   //将就绪队列的进程加入到执行队列
        }
        else
        {
            if((ready!=NULL)&&(run->prio<ready->prio))    //就绪队列不空  并且当前执行的进程优先级低于就绪队列的收个进程
            {
                run->state='W';     //执行的进程状态变为等待
                insert_prio(run);  //调用优先级调度插入队列
                run=NULL;
                firstin();
            }
        }
        prt(algo);
    }
}




void roundrun(char algo)       //时间片调度
{
    while(run!=NULL)     //执行进程不空
    {
        run->cpu_time=run->cpu_time+1;          //执行进程占用的cpu时间+1
        run->need_time=run->need_time-1;       //执行进程还需要的时间-1
        run->count=run->count+1;              //执行的总时间+1
        if(run->need_time==0)        //执行完毕
        {
            run->next=finish;       //进入终止队列
            finish=run;
            run->state='F';
            run=NULL;
            if(ready!=NULL)     //就绪队列不空
            {
                firstin();
            }
        }
        else       //未执行完毕
        {
            if(run->count==run->round)   //执行总时间等于时间片轮转时间
            {
                run->count=0;    //该程序执行完毕
                if(ready!=NULL)
                {
                    run->state='W';
                    insert_round(run);  //调用时间片插入函数
                    firstin();
                }
            }
        }
        prt(algo);
    }
}


