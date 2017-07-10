#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void print_zhangzhe();//显示封面
void print_help();//显示提示信息

int main()
{
    char input[30], filename[30];
    file_init();//初始化
    initi_simulation();
    print_zhangzhe();
    printf("欢迎来到 RedBoyOS 操作系统。\n\n");
    printf("提示:第一次使用操作系统请输入help。\n");
    while(1)
    {
        scanf("%s", input);//输入命令
        if(strcmp(input, "exit") == 0)//退出
        {
            break;
        }
        else if(strcmp(input, "ls") == 0)//显示当前目录中的文件
        {
            ls();
        }
        else if(strcmp(input, "cd") == 0)//上移或下移当前节点
        {
            scanf("%s", filename);
            cd(filename);
        }
        else if(strcmp(input, "touch") == 0)//新建文件
        {
            scanf("%s", filename);
            create(filename, 0);
        }
        else if(strcmp(input, "mkdir") == 0)//新建目录
        {
            scanf("%s", filename);
            create(filename, 1);
        }
        else if(strcmp(input, "rm") == 0)//删除文件
        {
            scanf("%s", filename);
            del(filename, 0);
        }
        else if(strcmp(input, "rmdir") == 0)//删除目录
        {
            scanf("%s", filename);
            del(filename, 1);
        }
        else if(strcmp(input, "schedule") == 0)//进程调度
        {
            getchar();
            process();
        }
        else if(strcmp(input,"gm")==0||strcmp(input,"fm")==0||strcmp(input,"st")==0)//内存的申请、释放以及显示
        {
            memorycontroy(input);
        }
        else if(strcmp(input, "help") == 0)//显示提示信息
        {
             print_help();
        }
        else
        {
            printf("unknown instruction\n\n");//报错，显示指令错误
        }
    }
    return 0;
}

void print_help()//显示提示信息
{
    printf("本操作系统包含以下函数:\n");
    printf("ls - 显示当前目录文件；cd - 前往上级或子目录；touch - 创建文件\n");
    printf("mkdir - 生成目录；rm - 删除文件；rmdir - 删除目录；\n");
    printf("schedule - 进程调度；gm - 申请内存；fm - 释放内存； st - 显示内存\n");
    printf("注:cd后加..表示返回上级目录\n");
}

void print_zhangzhe()//显示封面，为图片转ascii化
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


