#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*���̿��ƿ����ݽṹ*/
typedef struct node
{
    char name[10];      //������
    int prio;          //�������ȼ�
    int round;        //���̷����ʱ��Ƭ
    int cpu_time;     //�������ĵ�CPUʱ��
    int need_time;   //������Ҫ��CPUʱ��
    int count;     //��������ʱ��
    char state;   //���̵�״̬��'R':����,'W'���ȴ�,'F'������
    struct node *next;//ָ����һ�����̵�ָ��
} PCB;
PCB *finish,*ready,*tail,*run;//ָ���������еĶ��׵�ָ�룬tailΪ�������еĶ�βָ��
int N;//������̵���Ŀ


/*������������ */
void firstin(void);          //�������ܣ� �����̾��������е�һ���Ž����ж���
void prt1(char a);          //�������ܣ����������Ϣ�ı��⺯��
void prt2(char a,PCB *p);  //�������ܣ��������������Ϣ�ĺ���
void prt(char algo);      //�������ܣ�������н�����Ϣ�ĺ���
void insert_prio(PCB *q);           //�������ܣ����ȼ������Ƚ����̲��뵽���������㷨
void insert_round(PCB *q);         //�������ܣ�ʱ��Ƭ��ת�㷨���Ƚ����̲��뵽���������㷨
void pcreate_task(char algo);     //�������ܣ��������ȼ����̵��ȷ�ʱ�����̳�ʼ������
void rcreate_task(char algo);    //�������ܣ�����ʱ��Ƭ��ת�����̵��ȷ�ʱ�����̳�ʼ������
void priority(char algo);       //�������ܣ��������ȼ����̵��ȷ�ʱ�����̵��Ⱥ���
void roundrun(char algo);      //�������ܣ�����ʱ��Ƭ��ת�����̵��ȷ�ʱ�����̵��Ⱥ���


/*main ����*/
void process()
{
    char algo;
    printf("Choose the type of attemper P:priority R:timeround\n");   //����������ͣ�p�����ȼ�    r��ʱ��Ƭ
    scanf("%c",&algo);
    if((algo!='P')&&(algo!='p')&&(algo!='r')&&(algo!='R'))
    {
        printf("Unkonwn type of attemper\n");
        return;
    }
    printf("Please enter the number of processes N:\n");           //���������
    scanf("%d",&N);
    if(N<=0)
    {
        printf("Illegal input\n");
        return;
    }
    if((algo=='P')||(algo=='p'))
    {
        pcreate_task(algo);     //�������ȼ���ʼ�����Ⱥ���
        priority(algo);        //�������ȼ����Ⱥ���
    }
    else if((algo=='r')||(algo=='R'))
    {
        rcreate_task(algo);     //����ʱ��Ƭ��ʼ�����Ⱥ���
        roundrun(algo);        //����ʱ��Ƭ���Ⱥ���
    }
}


void firstin(void)        //�����������еĵ�һ�����̷Ž����ж���
{
    if(ready!=NULL)    //�������в���
    {
        run=ready;              //ִ�ж���ָ�����������ָ��
        ready=ready->next;     //����������ָ������
        run->state='R';       //״̬��Ϊִ��
        run->next=NULL;
    }
    else                //��������Ϊ��
    {
        run=NULL;    //û�п�ִ�еĽ���
    }
}

void prt1(char a)     //��ӡ���̵ı�����Ϣ
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



void prt2(char a,PCB *p)     //��ӡ����������Ϣ
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



void prt(char algo)    //��ӡ���еĽ�����Ϣ
{
    PCB *p;
    prt1(algo);
    if(run!=NULL)    //��ӡִ�еĽ���
    {
        prt2(algo,run);
    }
    p=ready;
    while(p!=NULL)    //ѭ����ӡ�������еĽ�����Ϣ��ֱ��û�н���
    {
        prt2(algo,p);
        p=p->next;
    }
    p=finish;
    while(p!=NULL)   //ѭ����ӡ��ֹ���еĽ�����Ϣ��֪��û�н���
    {
        prt2(algo,p);
        p=p->next;
    }
    //getchar();
}



void insert_prio(PCB *q)     //���ȼ������㷨�����������
{
    PCB *p,*s,*r; /*p,r�������ƾ������й�����Sָ�����Ķ���*/
    int b; /*b��Ϊ������Ʊ�־��*/
    s=q;
    p=ready;
    r=p;
    b=1;
    if(s->prio>=ready->prio)     //����ǰ�������ȼ����ھ������е��ո����ȼ�
    {
        s->next=ready;    //����ǰ������Ϊ�������е��׸�����
        ready=s;
    }
    else
    {
        while((p!=NULL)&&b)      //�Ӿ������б������ҵ���һ�����ȼ����ڵ�ǰ���̵�ָ��
        {
            if(p->prio>=s->prio)
            {
                r=p;
                p=p->next;
            }
            else     //��û���ҵ������ʾ��ǰ�������ȼ���� �����ı��־λ
            {
                b=0;
            }
        }
        s->next=p;
        r->next=s;
    }
}




void insert_round(PCB *q)    //ʱ��Ƭ��ת�㷨�����������
{
    tail->next=q;   //����ǰ���̲��뵽��β
    tail=q;
    q->next=NULL;
}



void pcreate_task(char algo)     //���ȼ����Ƚ��̳�ʼ��
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
        printf("Enter the name of process\n");   //���������
        scanf("%s",na);
        printf("Enter the time of process\n");    //�����������ʱ��
        scanf("%d",&time);
        while(time<=0)
        {
            printf("Wrong input.\n");
            printf("Enter the time of process\n");
            scanf("%d",&time);
        }
        strcpy(p->name,na);   //���ָ�ֵ
        p->cpu_time=0;       //cpuʱ���ʼ��Ϊ0
        p->need_time=time;  //��Ҫʱ���ʼ��
        p->state='W';      //״̬��Ϊ�ȴ�
        p->prio=time;
        if(ready==NULL)    //�������пգ�����ǰ������Ϊ��ָ��
        {
            ready=p;
            ready->next=NULL;
        }
        else
        {
            insert_prio(p);    //���뵽��������
        }
        printf("Output the waiting processes information\n");    //��ӡ�ȴ����̵���Ϣ
        prt(algo);    //��ӡ�������̵���Ϣ
    }
    firstin();
}



void rcreate_task(char algo)     //ʱ��Ƭ���Ƚ��̳�ʼ��
{
    PCB *p;
    int i,time;
    char na[10];
    ready=NULL;      //��������Ϊ��
    finish=NULL;    //��ֹ����Ϊ��
    run=NULL;      //ִ�ж���Ϊ��
    for(i=0; i<N; i++)
    {
        p=(PCB*)malloc(sizeof(PCB));    //�����ڴ�
        printf("Enter the name of process\n");    //��������
        scanf("%s",na);
        printf("Enter the time of process\n");    //���������Ҫ��ʱ��
        scanf("%d",&time);
        while(time<=0)
        {
            printf("Wrong input\n");
            printf("Enter the time of process\n");
            scanf("%d",&time);
        }
        strcpy(p->name,na);    //���ָ�ֵ
        p->cpu_time=0;        //ռ��cpuʱ��Ϊ0
        p->need_time=time;   //��Ҫʱ�丳ֵ
        p->count=0;        //����������ʱ��
        p->state='W';    //״̬��ʼ��
        p->round=2;     //����ʱ��ƬΪ2
        if(ready!=NULL)     //�������в���
        {
            insert_round(p);    //�������
        }
        else     //�������п�
        {
            p->next=ready;    //ֱ�ӽ���ǰ������Ϊ����������ָ��
            ready=p;
            tail=p;
        }
        printf("Output the waiting processes information\n");    //��ӡ�ȴ���Ϣ
        prt(algo);
    }
    run=ready;      //״̬�ı�
    ready=ready->next;
    run->state='R';
}



void priority(char algo)     //���ȼ�����
{
    while(run!=NULL)     //ִ�ж��в���
    {
        run->cpu_time+=1;     //ִ�еĽ���ռ�õ�cpuʱ��+1
        run->need_time-=1;   //����Ҫ��ִ��ʱ��-1
        run->prio-=3;       //���ȼ�����3
        if(run->need_time==0)    //����ִ�����
        {
            run->next=finish;    //������ֹ״̬
            finish=run;
            run->state='F';
            run=NULL;     //ִ�ж��п�
            firstin();   //���������еĽ��̼��뵽ִ�ж���
        }
        else
        {
            if((ready!=NULL)&&(run->prio<ready->prio))    //�������в���  ���ҵ�ǰִ�еĽ������ȼ����ھ������е��ո�����
            {
                run->state='W';     //ִ�еĽ���״̬��Ϊ�ȴ�
                insert_prio(run);  //�������ȼ����Ȳ������
                run=NULL;
                firstin();
            }
        }
        prt(algo);
    }
}




void roundrun(char algo)       //ʱ��Ƭ����
{
    while(run!=NULL)     //ִ�н��̲���
    {
        run->cpu_time=run->cpu_time+1;          //ִ�н���ռ�õ�cpuʱ��+1
        run->need_time=run->need_time-1;       //ִ�н��̻���Ҫ��ʱ��-1
        run->count=run->count+1;              //ִ�е���ʱ��+1
        if(run->need_time==0)        //ִ�����
        {
            run->next=finish;       //������ֹ����
            finish=run;
            run->state='F';
            run=NULL;
            if(ready!=NULL)     //�������в���
            {
                firstin();
            }
        }
        else       //δִ�����
        {
            if(run->count==run->round)   //ִ����ʱ�����ʱ��Ƭ��תʱ��
            {
                run->count=0;    //�ó���ִ�����
                if(ready!=NULL)
                {
                    run->state='W';
                    insert_round(run);  //����ʱ��Ƭ���뺯��
                    firstin();
                }
            }
        }
        prt(algo);
    }
}


