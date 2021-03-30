
#include "test_include.h"

void GetGaussianKernel( double *date_test );
void GaussianFilter(int *y,int *yy,double *date_test,int size,int length);
void Differential(int *y,int *fx,int *fxx,int len);
void BaoLuoXian(double *baoluoupDate,double *baoluodownDate,double *wav,int fx_size);
int SearchDecodeTwo(int *fx,int *fxx,int len,double *baoluoupDate,double *baoluodownDate,double *wav,double *result_2,int *fx_result);
int ScreeningMethod(int w,double *result,int * edgeWidth,int * x_position, int *fx_re);



extern int width ;
extern int height ;

//******************高斯卷积核生成函数*************************

//第一个参数date_test是卷积核的数组指针；

//第二个参数size是高斯卷积核的尺寸大小；

//第三个参数sigma是卷积核的标准差

//*************************************************************

void GetGaussianKernel( double *date_test)
{
    int n=2;
    double p=3.1415;
    double sigma=1;

    for (int i=1;i<2*n;i++)
    {
//        double tmp = -(i-n)*(i-n);
//        double temp = exp(tmp/2);
//        date_test[i] = temp/(1/sqrt(2*p));
//         printf("%f, %f\n",tmp,temp);
        date_test[i]=exp(-(i-n)*(i-n)/(2*sigma*sigma))  /(sigma*sqrt(2*p));
//        printf("GaussTmyemp[%d]=%f\n",i,date_test[i]);
    }
}

//**********************高斯滤波*****************************

//第一个参数y是原始波形；

//第二个参数date_test是卷积核；

//第三个参数size是卷积核的大小；

//第四个参数length原始波形的长度

//*************************************************************
void GaussianFilter(int *y,int *yy,double *date_test,int size,int len)

{
    yy[0] = 0;
    yy[len-1] = 0;
    int N=size;
    double date_1=0.0;
//    for (int i=1;i<6;i++) {
////        printf("date_test=%f\n",date_test[i]);
//    }
    for (int i=N/2+1;i<len-N/2-1;i++) {
        date_1=0.0;
        for (int j=1;j<=3;j++) {
             date_1=date_1+y[i-2+j]*date_test[j];
//             printf("(%d)%f ",j,date_1);
        }
        yy[i]=(int)date_1;
//        printf("%d ",yy[i]);
    }
}


//******************一阶差分 ———— 二阶差分*************************

//第一个参数y是原始波形；

//第二个参数fx一阶差分；

//第三个参数fxx二阶差分；

//第四个参数length原始波形的长度

//*************************************************************
void Differential(int *y,int *fx,int *fxx,int len)
{
//    printf("\n一阶差分\n");
    for (int i=0;i<len-1;i++) {
        fx[i]=y[i+1]-y[i];
//        printf("%d-%d=%d\t",y[i+1],y[i],fx[i]);
    }

//      printf("\n二阶差分\n");
    for (int i=0;i<len-2;i++) {
        fxx[i]=fx[i+1]-fx[i];
//        printf("%d-%d=%d\t",fx[i+1],fx[i],fxx[i]);
//        printf("fxx(%d)=%d\t",i,fxx[i]);
    }
//    int fxx_size=sizeof (fxx)/sizeof (fxx[0]);
}

//**************************包络线*****************************

//第一个参数baoluoupDate是存储上包络线的数据；

//第一个参数baoluodownDate是存储下包络线的数据；

//第三个参数wav是原波形的数据

//第四个参数fx_size是一阶差分的长度

//*************************************************************
void BaoLuoXian(double *baoluoupDate,double *baoluodownDate,double *wav,int fx_size)
{
     double lastup=0.0,lastdown=3000.0,rate=0.5;
    for (int i=0;i<fx_size;i++)
    {
//           printf("wav(%d)=%f\t",i,wav[i]);
      if(wav[i]>=lastup)
          baoluoupDate[i]=wav[i];
      else {
          if(wav[i]>=lastup-rate)
              baoluoupDate[i]=wav[i];
          else {
              baoluoupDate[i]=lastup-rate;

          }
      }
      lastup=baoluoupDate[i];
//      printf("baoluoupDate(%d)=%f\n",i,baoluoupDate[i]);
//        printf("lastup(%d)=%f\t",i,lastup);
      if(wav[i]<=lastdown)
          baoluodownDate[i]=wav[i];
      else {
          if(wav[i]<=lastdown+rate)
              baoluodownDate[i]=wav[i];
          else {
              baoluodownDate[i]=lastdown+rate;
          }
      }
      lastdown=baoluodownDate[i];
//        printf("baoluodownDate(%d)=%f\t",i,baoluodownDate[i]);
//        printf("%f\t",baoluodownDate[i]);
      rate=(baoluoupDate[i]-baoluodownDate[i])/64.0;
//        printf("rate(%d)=%f\t",i,rate);
    }
}
//*************************边缘搜索方法二*****************************

//最后一个参数result_2存储边缘信息

//*************************************************************
int SearchDecodeTwo(int *fx,int *fxx,int len,double *baoluoupDate,double *baoluodownDate,double *wav,double *result_2,int *fx_result)
{
    int falsevalue_2=0,risevalue_2=0,risestate_2=0,point_2=0,up_2=0,down_2=0,j_2=0,n=0;
    double dx_2=0,blance_2=0,p_2=0;
//    for(int i=5; i<len-5; i++)
//    {
//        printf("fx(%d)=%d,fxx(%d)=%d\n",i,fx[i], i,fxx[i]);
//        printf("BU(%d)=%f,BD(%d)=%f\n",i,baoluoupDate[i], i,baoluodownDate[i]);

//    }
    if(fx[0]>0)
    {
        risestate_2=1;
        risevalue_2=0;
        j_2=1;
    }
    else {
        risestate_2=0;
        falsevalue_2=0;
        j_2=2;
    }

    for (int i=1;i<len-2;i++)
    {
        up_2=wav[i-1]+wav[i+1]-2*wav[i];
        down_2=wav[i]+wav[i+2]-2*wav[i+1];
        if((up_2>=0 && down_2<0) ||(up_2<=0 && down_2>0))
        {
            dx_2=abs((baoluoupDate[i]-wav[i])-(wav[i]-baoluodownDate[i]));
            dx_2=dx_2/(baoluoupDate[i]-baoluodownDate[i]);
//            blance_2=6*dx_2;
            blance_2=14*dx_2;//调试  对步长进行限制

            if(risestate_2)
            {
                if(fx[i]>risevalue_2+blance_2)
                {
                    risevalue_2=fx[i];
                    point_2=1;

                }
                if(fx[i]<risevalue_2-blance_2)
                {
                    risestate_2=0;

                    falsevalue_2=fx[i];
                    j_2=j_2+1;
                    point_2=1;

                }
            }
            else {
                if(fx[i]>falsevalue_2+blance_2)
                {
                     risevalue_2=fx[i];
                     risestate_2=1;
                     point_2=1;
                     j_2=j_2+1;

                }

                if(fx[i]<falsevalue_2-blance_2)
                {
                    falsevalue_2=fx[i];
                    point_2=1;

                }

            }


            if(point_2)
            {

                point_2=0;

                p_2=abs(fxx[i-1])/(double)abs(fxx[i-1]-fxx[i]);
//                 printf("p_2[%d]=%f--%d-%d\n",j_2,p_2, fxx[i-1],fxx[i]);
                result_2[j_2]=i+p_2;
                fx_result[j_2]=fx[i];


            }
        }



    }
//    for (int i=1;i<=j_2;i++) {
//        n++;
//        printf("result_2[%d]=%f\n",i,result_2[i]);
//    }
    return j_2;

}
int ScreeningMethod(int w,double *result,int * edgeWidth,int * x_position, int *fx_re)
{
    int m=0;
    //宽度小于11的进行合并
    for (int i=1;i<w-1;i++) {
        int c = * (result+i+2)-* (result+i+1);
        if(c>1)
        {
            m++;
            *(edgeWidth+m)=c;
            *(x_position+m)= *(result+i+2);
        }
//        cout<<* (result+i+2)<< "-"<< * (result+i+1)<<"="<<* (result+i+2)-* (result+i+1)<<endl;

    }

//    for(int i=1; i<=m; i++)
//    {
//        printf("\n i=%d, edgeWidth=%d\n ",i, *(edgeWidth+i));
//    }
    return m;
}
//int *return_date=(int *) malloc((sizeof (int))*400);


int binaryzation(int *y, int * X_position, int *X_width , int *num, int *length)
{
//    memset(return_date,0,sizeof(return_date));
//    cout<<"binaryzation!!"<<endl;
    double baoluoupDate[800];
    double baoluodownDate[800];
    double wav[800];
    int yy[800];
    int fx[800];
    int fxx[800];
    int fx_result[800];
    int fx_re[800];
    int len = *length;
//    printf("--------------binaryzation----len=%d----------------\n", len);
    int j=0;
//    double sigma=1;
    const int N=1;
    double date_test[2*2] = {0, 0.2420, 0.3989, 0.2420};
    double result[800];
    int m=0;
    int *edgeWidth=NULL;
    int *x_position=NULL;
    edgeWidth=(int *) malloc((sizeof (int))*200);
    x_position=(int *) malloc((sizeof (int))*200);


//      printf("\n gaussian image\n");
//**************************生成高斯卷积核********************************//
//     GetGaussianKernel(date_test);

//     printf("y.size()=%d\n",len);

//******************************高斯滤波*********************************//
      GaussianFilter(y,yy,date_test,N,len);


      for (int i=0;i<len;i++) {
//          baoluoupDate[i]=yy[i];
//          baoluodownDate[i]=yy[i];
          wav[i]=(double) yy[i];
//           printf("%f\t",*(wav+i));
      }
//**********************一阶差分 ———— 二阶差分***************************//
      Differential(yy,fx,fxx,len);//调试 尝试使用原始数据一阶差分和二阶差分  对比滤波后的数据一阶差分和二阶差分
//    int fx_size=sizeof (fx)/sizeof (fx[0]);
//     printf("fx_size=%d\n",fx_size);
//******************************包络线*********************************//
      BaoLuoXian(baoluoupDate,baoluodownDate,wav,len);

//*****************************边缘搜索方法二***************************//
    j=SearchDecodeTwo(fx,fxx,len,baoluoupDate,baoluodownDate,wav,result,fx_result);
//    printf("SearchDecodeTwo j=%d\n",j);
    if(j<8)
    {
        return 0;
    }

    m=ScreeningMethod(j,result,edgeWidth,x_position,fx_re);
    *num = m;
//    printf("ScreeningMethod m=%d\n",m);
    if(m<7)
    {
        return 0;
    }
    for(int i=1; i<=m; i++)
    {
        *(X_position+i-1) = x_position[i];
        *(X_width+i-1) = edgeWidth[i];
//        printf("i=%d\n",i-1);
//        printf("x_position=%d\n",*(X_position+i-1));
//        printf("edgeWidth=%d\n",*(X_width+i-1));
    }

    free(edgeWidth);
    free(x_position);

    return 1;


}
