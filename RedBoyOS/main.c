#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void print_zhangzhe();//��ʾ����
void print_help();//��ʾ��ʾ��Ϣ

int main()
{
    char input[30], filename[30];
    file_init();//��ʼ��
    initi_simulation();
    print_zhangzhe();
    printf("��ӭ���� RedBoyOS ����ϵͳ��\n\n");
    printf("��ʾ:��һ��ʹ�ò���ϵͳ������help��\n");
    while(1)
    {
        scanf("%s", input);//��������
        if(strcmp(input, "exit") == 0)//�˳�
        {
            break;
        }
        else if(strcmp(input, "ls") == 0)//��ʾ��ǰĿ¼�е��ļ�
        {
            ls();
        }
        else if(strcmp(input, "cd") == 0)//���ƻ����Ƶ�ǰ�ڵ�
        {
            scanf("%s", filename);
            cd(filename);
        }
        else if(strcmp(input, "touch") == 0)//�½��ļ�
        {
            scanf("%s", filename);
            create(filename, 0);
        }
        else if(strcmp(input, "mkdir") == 0)//�½�Ŀ¼
        {
            scanf("%s", filename);
            create(filename, 1);
        }
        else if(strcmp(input, "rm") == 0)//ɾ���ļ�
        {
            scanf("%s", filename);
            del(filename, 0);
        }
        else if(strcmp(input, "rmdir") == 0)//ɾ��Ŀ¼
        {
            scanf("%s", filename);
            del(filename, 1);
        }
        else if(strcmp(input, "schedule") == 0)//���̵���
        {
            getchar();
            process();
        }
        else if(strcmp(input,"gm")==0||strcmp(input,"fm")==0||strcmp(input,"st")==0)//�ڴ�����롢�ͷ��Լ���ʾ
        {
            memorycontroy(input);
        }
        else if(strcmp(input, "help") == 0)//��ʾ��ʾ��Ϣ
        {
             print_help();
        }
        else
        {
            printf("unknown instruction\n\n");//������ʾָ�����
        }
    }
    return 0;
}

void print_help()//��ʾ��ʾ��Ϣ
{
    printf("������ϵͳ�������º���:\n");
    printf("ls - ��ʾ��ǰĿ¼�ļ���cd - ǰ���ϼ�����Ŀ¼��touch - �����ļ�\n");
    printf("mkdir - ����Ŀ¼��rm - ɾ���ļ���rmdir - ɾ��Ŀ¼��\n");
    printf("schedule - ���̵��ȣ�gm - �����ڴ棻fm - �ͷ��ڴ棻 st - ��ʾ�ڴ�\n");
    printf("ע:cd���..��ʾ�����ϼ�Ŀ¼\n");
}

void print_zhangzhe()//��ʾ���棬ΪͼƬתascii��
{
    printf("           rYiirvYujSLri       LYrrrriiii  iuYi  iiiviiiirrriivjuiiYojJjUji\n");
    printf("viLvvji    rLriiiiivooi      i  iiiiiiiri        i iiiiirvrJrirvi  iYuvLvi \n");
    printf("JvLLqXkFFFoLLvvrrirJri  iUuiiiiiuuuuVYvvLii    ruviiiiiirrrvouJLv   rrvvYLv\n");
    printf("   iLYvokUriivrrviii   iirirLUoFFuvvYJLUkNuuriiiiriiiiiivvi ivLJvivuvrrvLJi\n");
    printf("      iiiiiiirvvv ii  rjokENOqkooVojoLokXPOOGEVvi iiiiriiii  rUviivrirvrjvi\n");
    printf("  ri  iriiiirrvLiirLvUPMOBBBEuJLuSVNkLLkVXGBMBMMkFIr ivvrviiiiiiii     iJUi\n");
    printf("LuMMPrrirriiiivrvJukZGBBBBBMGFouLvqNIjYuXXXXMBBBOBMriJOGXVriri iiiiiiiiiuIi\n");
    printf("ouvJiiiiiriiir vBBNBBBBBBBBOGNuvrvjJJuirrLvirUSqXMBBkujBuiiriiiiirruPkYuji \n");
    printf("oLLi  iiiiiivi PBqGGBBqJLLLLuYviiiiiii         irjGOZSoOE   iiiiii  LSVvui \n");
    printf(" rFFrirjUJii  LBXJjVIv     iiii                  iLkNNuVMi   i  i       ikY\n");
    printf(" iqqkPGNEkUi  BBkoVILi                            iirJNEBU    iii       iUI\n");
    printf("irrvLviii ii rBMNkuvii                           iirLPqGBB   iiiivL     rFi\n");
    printf("iivLiiii  iiiPBGEkUrriiiiiiiiii i i i             irLuNNGBJiiivYri         \n");
    printf("   i  iui    OBNSGFUriiiiiiiirrriiiiiiiiiiii i i  irrLUNNBv iirkFri        \n");
    printf("Fi    iui iiiZBZGOEUriiiiiiiiiiiirriiiiiiii  ii   irLLIkqBi    iFIFUSi     \n");
    printf("vUu   iLri iiNBGOMMPriiiiiii                      ivvjokBM       irJuY     \n");
    printf("UFui   ii  i iBBMMBEiiiii                         ivuoMBB   iii  iiiiii    \n");
    printf("ivkGkL     i  FBBBGviiiiiiiiiiii iiiii iiiiiii     iuGBk      ivii ii      \n");
    printf("  ivvvi ii    iEGBBiiivviiiivuuvriviiirrvvviiii i   uMo           ivNF     \n");
    printf("i  iii  jv  i  Nu vjiLvi       iiiiiirvii           kZ  Mr       iLoYPjvi  \n");
    printf("    riiiri  rviiPi FBvirvYoNBBvJOF   OqLXSSXoiiiLuiiYi  jvi    rji    iqMi \n");
    printf("i     iiri  iLLrUj BS iiiPEUuPUjLMOJMJiUqPXBGjiiirBr ivi LVi   uFvi    iuv \n");
    printf("i     irviiiii  viik    iiiiiri  MBMBi ivii       PO  uiivi      IGYvi    i\n");
    printf("      iiiiiiii  ii ki i iiivivi uB  IB  vvriii    LF  iiiU       rrYNPi    \n");
    printf("   iiii   i  ii    LLiiii   iiiIBi   Mq  iiii  i  Ji    JL ii      LP      \n");
    printf("iivvirr  iriiivvi iirriiiiiiirjSi     VVriiiiii iLr i  ivi iii     ijFi    \n");
    printf("iirrLvi   iiiirvi iivuvvuLIUkNJiVqviiiiijYuvLLviiii i ii             NU    \n");
    printf("   vrii   iriiiriiiiiLrvvjuIvivNMOONoBGi iJGNoriiii      i     iii   ii    \n");
    printf("  iLi i    iii  iiiiiirvvvri  i iivYi      rriiiii       ii                \n");
    printf("   rr      rri   Iiiiiivrviiii   iii       iiiiiii     iiiiiiiii           \n");
    printf("iiJLi     ivi   LBXiiiiiiivLviirriiii ii iiiiri iiiiiii   iiiiiiiii        \n");
    printf("iii        ii iGBMBqiiriiiroNMMBBBBBBBBBBqrLvrii vvii                 i    \n");
    printf("i   iivLFkNNOOMMOOBMMvrLvivvrivJLLVkviiii  iLii ivvii  iiiiii i            \n");
    printf("i    rVJvOBBMBOOOMOBBBPujJriiiiiiiiii      iii  iiirriiiiiiiiiiiiiii  i    \n");
    printf("iivvLYULjJSMBOMOOZOOMMBMESr iirrLoFLuvii    ii  iiiivri    iiiiirriiiiLvvii\n");
    printf("uuVFuUuUouoOOOOOOOGOGGMBBBELi  iii       iiii  iiiiiii             vovruoIq\n");
    printf("PXNNEPPVSouoEMMGMOOOMOMOMMBBOSuvririrriiLvi   iiiiiiiiiiiii i      iJVorrvL\n");
    printf("qkSEGMMBMMNPXMMMOMOMOMOMOMOBBBBBBBBBBEuJri   iiiiiiiiLLLvvrii    iuLVBGJvuu\n");
    printf("BGNSkSGMMMBOGGOMMOMMMOMOMOOGBMMBBkVSviPB   iiiiiiiirrvjYviiiiYFuVIOENGOOMMB\n");
    printf("OOMOPFIkNOGMOOGOOOOMOMMMOMOMOMOMMNi  rBB   i iiiiiirirrvjji ikMMBMOONEOBBBB\n");
    printf("BMMMBBGNNqGOMOMGOGMOMOMOMOMOMMMOBBFrZBBM  i iiiiiirirrvvLji LUjXZNZZOOGiirr\n");
    printf("GOOOGMMMZNqNZMOOOMGMGOGMOMMMMMMMZOOOOBBB  iiiiiiiiirivriiiiuXVSqPPXEGMBi   \n");
    printf("GGGqNZOOMGOZGGMOOOMOMMMOMMMMMOONNNGGOGMMv  riiiiirLYUui iLOMGNZOGNEPEGBEi  \n");
    printf("MMOMOMOMOMOMGOGMOMOMOMMMOOEGNENNEOGOOOOBM  rjiiiriiiuONXoqZMMGPNqZEEEOMBEkX\n");
    printf("MGMMBMMMMMMMMMOGMGOMOOMOMGZNZNZGMMMOMMMBB   vviiii   LBBOSXNGGGqEGGNGOMOOOB\n");
    printf("OZNGZZZOOOGMMMOOGOOMZOGOOOOOOMOMOMOOOOOBBP   ii i  V  iBBujFkGGZNZNEEOGOEGN\n");
    Sleep(5000);
    system("CLS");
}


