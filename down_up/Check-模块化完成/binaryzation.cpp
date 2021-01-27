
#include <opencv2/highgui/highgui.hpp>//OpenCV highgui ģ��ͷ�ļ� ~
#include <opencv2/imgproc/imgproc.hpp>//OpenCV ͼ����ͷ�ļ�
//#include<iostream>
#include "binaryzation.h"
using namespace cv; // ���� cv �����ռ�
using namespace std;

//******************��˹��������ɺ���*************************

//��һ������date_test�Ǿ���˵�����ָ�룻

//�ڶ�������size�Ǹ�˹����˵ĳߴ��С��

//����������sigma�Ǿ���˵ı�׼��

//*************************************************************

void GetGaussianKernel( double *date_test,int size,double sigma)
{
    int N=size;
    const double pi=3.1415;
    for (int i=1;i<2*N;i++)
    {
        date_test[i]=exp(-(i-N)*(i-N)/(2*sigma*sigma))/(sigma*sqrt(2*pi));
//        printf("GaussTmyemp[%d]=%f\n",i,date_test[i]);
    }
}

//**********************��˹�˲�*****************************

//��һ������y��ԭʼ���Σ�

//�ڶ�������date_test�Ǿ���ˣ�

//����������size�Ǿ���˵Ĵ�С��

//���ĸ�����lengthԭʼ���εĳ���

//*************************************************************
void GaussianFilter(int *y,double *date_test,int size,int len)

{
    int N=size;
    double date_1=0.0;
    for (int i=1;i<6;i++) {
//        printf("date_test=%f\n",date_test[i]);
    }
    for (int i=N/2+1;i<len-N/2-1;i++) {
        date_1=0.0;
        for (int j=1;j<=5;j++) {
             date_1=date_1+y[i-3+j]*date_test[j];
//             printf("(%d)%f ",j,date_1);
        }
        y[i]=(int)date_1;
//        printf("%d ",y[i]);
    }
}


//******************һ�ײ�� �������� ���ײ��*************************

//��һ������y��ԭʼ���Σ�

//�ڶ�������fxһ�ײ�֣�

//����������fxx���ײ�֣�

//���ĸ�����lengthԭʼ���εĳ���

//*************************************************************
void Differential(int *y,int *fx,int *fxx,int len)
{
//    printf("\nһ�ײ��\n");
    for (int i=0;i<len;i++) {
        fx[i]=y[i+1]-y[i];
//        printf("%d-%d=%d\t",y[i+1],y[i],fx[i]);
    }

//      printf("\n���ײ��\n");
    for (int i=0;i<len-1;i++) {
        fxx[i]=fx[i+1]-fx[i];
//        printf("%d-%d=%d\t",fx[i+1],fx[i],fxx[i]);
    }
//    int fxx_size=sizeof (fxx)/sizeof (fxx[0]);
}

//**************************������*****************************

//��һ������baoluoupDate�Ǵ洢�ϰ����ߵ����ݣ�

//��һ������baoluodownDate�Ǵ洢�°����ߵ����ݣ�

//����������wav��ԭ���ε�����

//���ĸ�����fx_size��һ�ײ�ֵĳ���

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
      rate=(baoluoupDate[i]-baoluodownDate[i])/64.0;
//        printf("rate(%d)=%f\t",i,rate);
    }
}
//*************************��Ե����������*****************************

//���һ������result_2�洢��Ե��Ϣ

//*************************************************************
int SearchDecodeTwo(int *fx,int *fxx,int len,double *baoluoupDate,double *baoluodownDate,double *wav,double *result_2,int *fx_result)
{
    int falsevalue_2=0,risevalue_2=0,risestate_2=0,point_2=0,up_2=0,down_2=0,j_2=0,n=0;
    double dx_2=0,blance_2=0,p_2=0;
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
            blance_2=16*dx_2;

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
    //���С��11�Ľ��кϲ�
    for (int i=1;i<w-1;i++) {
        int c = * (result+i+2)-* (result+i+1);
        if(c>1)
        {
            m++;
            *(edgeWidth+m)=c;
            *(x_position+m)=* (result+i+2);
        }
//        cout<<* (result+i+2)<< "-"<< * (result+i+1)<<"="<<* (result+i+2)-* (result+i+1)<<endl;
//        printf("\nedgeWidth=%d\n ",* (edgeWidth+i));
    }
    return m;
}
//int *return_date=(int *) malloc((sizeof (int))*400);


void binaryzation(IplImage* image,int *y, int * X_position, int *X_width , int *num)
{
//    memset(return_date,0,sizeof(return_date));
//    cout<<"binaryzation!!"<<endl;
    void GetGaussianKernel( double *date_test,int size,double sigma);
    void GaussianFilter(int *y,double *date_test,int size,int length);
    void Differential(int *y,int *fx,int *fxx,int len);
    void BaoLuoXian(double *baoluoupDate,double *baoluodownDate,double *wav,int fx_size);
    int SearchDecodeTwo(int *fx,int *fxx,int len,double *baoluoupDate,double *baoluodownDate,double *wav,double *result_2,int *fx_result);
    int ScreeningMethod(int w,double *result,int * edgeWidth,int * x_position, int *fx_re);

    unsigned char date[1601];
    double baoluoupDate[1601];
    double baoluodownDate[1601];
    double wav[1601];
    int fx[1601];
    int fxx[1601];
    int fx_result[1601];
    int fx_re[1601];
    int len = image->width;
    int j=0;
    double sigma=1;
    const int N=2;
    double date_test[2*N];
    double result[1600];
    int w=0;
    int m=0;
    int *edgeWidth=NULL;
    int *x_position=NULL;
    edgeWidth=(int *) malloc((sizeof (int))*200);
    x_position=(int *) malloc((sizeof (int))*200);


//      printf("\n gaussian image\n");
//**************************���ɸ�˹�����********************************//
     GetGaussianKernel(date_test,N,sigma);
//     printf("y.size()=%d",len);

//******************************��˹�˲�*********************************//
      GaussianFilter(y,date_test,N,len);

      for (int i=0;i<len;i++) {
          baoluoupDate[i]=y[i];
          baoluodownDate[i]=y[i];
          wav[i]=y[i];
      }
//**********************һ�ײ�� �������� ���ײ��***************************//
      Differential(y,fx,fxx,len);
     int fx_size=sizeof (fx)/sizeof (fx[0]);
//******************************������*********************************//
      BaoLuoXian(baoluoupDate,baoluodownDate,wav,fx_size);

//*****************************��Ե����������***************************//
    j=SearchDecodeTwo(fx,fxx,len,baoluoupDate,baoluodownDate,wav,result,fx_result);
//    printf("j=%d",j);

    m=ScreeningMethod(j,result,edgeWidth,x_position,fx_re);
    *num = m;
//    printf("num=%d\n", *num);
    for(int i=0; i<m; i++)
    {
        *(X_position+i) = x_position[i];
        *(X_width+i) = edgeWidth[i];
//        cout<<"Xposition"<<i<<"="<<*(Xposition+i)<<endl;
//        cout<<"Xwidth"<<i<<"="<<*(Xwidth+i)<<endl;
    }

//    for (int i=1;i<=m;i++) {
//        cvCircle(image, CvPoint(round(*(x_position+i)),259), 2, CvScalar(0,250,0),1,8,0);
//        printf("%d ",*(x_position+i));
//        printf("%d ",*(edgeWidth+i));
//    }

//    printf("m=%d\n ",m);


}
