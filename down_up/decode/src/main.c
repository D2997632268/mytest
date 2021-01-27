#include "test_include.h"

struct timeval tpstart,tpend;
float timeuse;
#include <math.h>
//using namespace cv; // 包含 cv 命名空间
//using namespace std;
 int width = 640;
int height = 480;
//extern int width = 1280;
//extern int height = 720;

extern void binaryzation(/*IplImage* image,*/int *y, int * X_position, int *X_width , int *num);
//void slopeCross_point(/*IplImage *image,*/unsigned char* pic,int y,double k_horizontal, double b, double k_v, double b_v,  int *ax, int *pd, int ROW,int *ay, int *ay_width,int  ROW2, int *ay2, int *ay_width2, int* X , int * Y);
//-----------------------寻找垂直1:1:2：2：1-----------------//
int test_vertical(/*IplImage* image,*/ unsigned char* pic,int position, int sum, int* ax,  double slope, int *a,int * ROW, int*ay, int *ay_width)//
{
//    cout<<"---test_vertical-----"<<endl;
    int check[5] = {1, 1, 2, 2, 1};
    int AVG = sum/7*12;
    int start_x;
    int z;
//    uchar temp[1600];
    int data[1600];
    int pd[5];
    int Ay[5];
    int ok=0;
    int mmin = a[0]>a[1]?a[1]:a[0];

//    cout<<"slope="<<slope<<endl;
//    cout<<"mmin="<<mmin<<endl;
//    cout<<"a[0]="<<a[0]<<endl;
//    cout<<"a[1]="<<a[1]<<endl;
//    cout<<"输出="<<(pow(slope,2)+1)<<endl;
    if(slope>0){
        double w = sqrt(pow((sum/7),2)/(pow(slope,2)+1)/1.0)*abs(slope/1.0)*34;
        double wucha = sqrt(pow((sum/7),2)/(pow(slope,2)+1)/1.0);
        double pos = sqrt(pow(w,2)/(pow(slope,2)+1)/1.0)-wucha;
        start_x = (int)(position+pos)-1;
    }
    else if (slope==0 && mmin>=0){
//        cout<<"--in--"<<endl;
        if(a[0]>a[1])
        {
            start_x = (int)(position-a[0]);
            z = 1;
        }else{
            start_x = (int)(position+a[1]);
            z = -1;
//            cout<<"start_x="<<start_x<<endl;
        }
    }
    else if(slope==0 && mmin<0){
        start_x = position;
    }
    else{
        start_x = position+1;
    }

    if(start_x>=width)
    {
        return 0;
    }
    int i;
//    cout<<"start_x="<<start_x<<endl;
    for(int j=0; j<width-start_x; j+=5)
    {
        int n = 0;
        if(slope == 0 && mmin>0)
        {
            i = start_x+5*j*z;
        }
        else if(slope==0 && mmin<0)
        {
            i = start_x+AVG*pow(-1, j)+5*j*pow(-1, j)*(-1);

        }
        else{
            i = start_x+5*j;
        }

        for(int j=0; j<height;j++)
        {
//            temp[j] = *((uchar *)gray->imageData+j*gray->widthStep+i);
            data[j] =pic[j*width+i]+0;
//            cout<<data[j]<<endl;
        }

        int y_col[200];
        int Ywidth[200];
        int LEN_y=0;
//        cvLine(image, CvPoint(i, 0), CvPoint(i, gray->height), CvScalar(0, 0, 250), 1, 8, 0);
        binaryzation(/*image,*/ data, y_col, Ywidth, &LEN_y);
//        printf("y-len=%d\n",LEN_y);
//        for(int i=0; i<LEN_y; i++)
//        {
////                cout<<"y_col"<<i<<"="<<y_col[i]<<endl;
//                cout<<"Ywidth"<<i<<"="<<Ywidth[i]<<endl;
//        }
        for(int j=0; j<LEN_y-4;j++)
        {
            for(int i=0; i<5; i++)
            {
                pd[i] = Ywidth[j+i];
                Ay[i] = y_col[j+i];
            }

            int avg = (pd[0]+pd[1]+pd[4])/3;
            int err = avg*1/2;
            ok = 1;
            for(int j=0; j<5; j++)
            {
                if(pd[j]<check[j] * avg - err || pd[j] > check[j] * avg + err + 1)
                {
                    ok = 0;
                    break;
                }
            }
            if(ok==1)
            {
                n = 1;
//                cout<<"垂直解码位置y="<<i<<endl;
//                cvLine(image, CvPoint(i, 0), CvPoint(i,height), CvScalar(0, 0, 250), 1, 8, 0);
                break;
            }
        }
        if(n>0 && ok == 1)
        {
            *ROW = i;
            for(int i=0; i<5; i++)
            {
                *(ay+i) = Ay[i];
                *(ay_width+i) = pd[i];
            }
//            cout<<"--out--"<<endl;
            return 1;
        }

    }
    return 0;


}

int slopeCross_point(/*IplImage *image,*/ unsigned char* pic,int y,double k_horizontal, double b, double k_v, double b_v,  int *ax, int *pd, int ROW, int *ay, int *ay_width,int  ROW2, int *ay2, int *ay_width2, int* X, int * Y)
{
    int sumx=0;
    int sumy=0;
    double bb;
    double bb_v;
    for(int i=0; i<5; i++)
    {
        sumx += pd[i];
        sumy += ay_width[i];
    }
    if(k_v==0 || k_horizontal==0)
    {
        X[0] = ax[0] - pd[0];
        Y[0] = ay[0] - ay_width[0];

        X[1] = (int)(ax[4]+sumx/7);
        Y[1] = (int)(ay[4]+sumy/7);
        if(pic[Y[1]*width+X[1]]+0>127)
        {
            for(int i=1; i<4; i++)
            {
                if(pic[Y[1]*width+X[1]+i]+0<127){
                    X[1] += i;
                    break;
                }
                if(pic[(Y[1]+i)*width+X[1]]+0<127){
                    Y[1] += i;
                    break;
                }
                if(pic[(Y[1]+i)*width+(X[1]+i)]+0<127){
                    X[1] += i;
                    Y[1] += i;
                    break;
                }

            }
        }

        int width = sumx/7*35;
        X[2] = (int)(X[1]+width);
        Y[2] = Y[1];

        int high = sumy/7*28;
        Y[3] = (int)(Y[1]+high);
        X[3] = X[1];
//        cvCircle(image, CvPoint(X[3], Y[3]),3, CvScalar(0, 250, 0), 3, 8, 0);

        X[4] = X[2];
        Y[4] = Y[3];
//        cvCircle(image, CvPoint(X[4], Y[4]),3, CvScalar(0, 250, 0), 3, 8, 0);
    }else{
        X[0] = (int)((b_v-b)/(k_horizontal-k_v));
        Y[0] = (int)(k_v*X[0]+b_v);
//        cvCircle(image, CvPoint(X[0], Y[0]),3, CvScalar(0, 250, 0), 3, 8, 0);
//        cout<<"(x0=)"<<X[0]<<endl;
//        cout<<"(y0=)"<<Y[0]<<endl;

        bb = y-k_horizontal*(ax[4]+pd[4]);
        bb_v = (ay2[4]+ay_width2[4])-k_v*ROW2;

        X[1] = (int)((bb_v-bb)/(k_horizontal-k_v));
        Y[1] = (int)(k_v*X[1]+bb_v);
//        cout<<"(x1=)"<<X[1]<<endl;
//        cout<<"(y1=)"<<Y[1]<<endl;
//        cvCircle(image, CvPoint(X[1], Y[1]),3, CvScalar(0, 250, 0), 3, 8, 0);
        if(pic[Y[1]*width+X[1]]+0>127)
        {
            for(int i=1; i<10; i++)
            {
                if(pic[Y[1]*width+(X[1]+i)]+0<127){
                    X[1] += i;
                    break;
                }
                if(pic[(Y[1]+i)*width+X[1]]+0<127){
                    Y[1] += i;
                    break;
                }
                if(pic[(Y[1]+i)*width+X[1]+i]+0<127){
                    X[1] += i;
                    Y[1] += i;
                    break;
                }

            }
        }
        while(pic[Y[1]*width+X[1]-1]+0<127)
        {
            X[1] -= 1;
        }

//        cvLine(image,CvPoint(X[1],Y[1]),CvPoint(1000,int(k_horizontal*1000+bb)), CvScalar(0,0,250),1,8,0);
//        cvLine(image,CvPoint(X[1],Y[1]),CvPoint(1200,int(k_v*1200+bb_v)), CvScalar(0,0,250),1,8,0);

        //测试：
//        double b_shu = Y[1]-k_horizontal*(X[1]);
//        cvLine(image,CvPoint(X[1],Y[1]),CvPoint(1000,int(k_horizontal*1000+b_shu)), CvScalar(250,0,0),1,8,0);

//        pixel4XY[0] = int(sqrt(pow(dis/7, 2)/(pow(k_horizontal,2)+1))*k_horizontal*4);
//        pixel4XY[1] = int(sqrt(pow(sumyy/7, 2)/(pow(k_v, 2)+1))*4);

//        int juggleX = int(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1)));
//        int juggleY = int(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal));

        double w = sqrt(pow(sumx/7,2)/(pow(k_horizontal,2)+1))*abs(k_horizontal)*35;
        int width = sqrt(pow(w, 2)/((pow(k_horizontal, 2)+1)*1.0))*abs(k_horizontal);

//        cout<<"w="<<w<<endl;
//        cout<<"width="<<width<<endl;

        X[2] = X[1]+width;
        Y[2] = (int)(k_v*X[2]+bb_v);
//        cvCircle(image, CvPoint(X[2], Y[2]),3, CvScalar(0, 250, 0), 3, 8, 0);

        double ww = sqrt(pow(sumy/7, 2)/(pow(k_v, 2)+1))*26;
        int high = sqrt(pow(ww, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal);

        Y[3] = Y[1]+high;
        X[3] = (int)((Y[3]-bb)/k_horizontal);
//        cvCircle(image, CvPoint(X[3], Y[3]),3, CvScalar(0, 250, 0), 3, 8, 0);

        double b_v4 = Y[3] - k_v*X[3];
        double b4 = Y[2] - k_horizontal*X[2];
        X[4] = (int)((b_v4-b4)/(k_horizontal-k_v));
        Y[4] = (int)(k_v*X[4]+b_v4);
//        cvCircle(image, CvPoint(X[4], Y[4]),3, CvScalar(0, 250, 0), 3, 8, 0);
        _Bool button1, button2, button3, button4;
        button1 = X[3]<X[1]?1:0;
        button2 = X[4]<X[2]?1:0;
        button3 = Y[1]<=Y[2]?1:0;
        button4 = Y[3]<=Y[4]?1:0;
        if(button1 == button2 == button3 == button4 == 0)
        {
            return 1;
        }else if(button1 == button2 == button3 == button4 == 1){

            return 1;
        }else{
            return 0;
        }

    }
}



int compare_pixel(unsigned char* pic, int x, int y)
{
    int a = pic[y*width+x]+0;
    int nextx[4] = {1,0,-1,0};
    int nexty[4] = {0,1,0,-1};
    int n=1;
    for(int i =0; i<4; i++)
    {
        int b = abs(a-( pic[(y+nexty[i])*width+(x+nextx[i])]+0))<80?1:0;
        if(b)
        {
            n++;
        }else{
            n--;
        }
    }

    return n;
}


int finder_scan(/*IplImage* image, */ unsigned char* pic,int * pixel4XY)
{
    int N = 0;
    int dis = 0;
    int sumx = 0;
    int n = 1;
    int ok = 0;
    int y = 0;
    int data[1600];

    int x_num=0;//记录可解线条的次数

    int pre_X[5];
    int pd[5];
    int ax[5];
    int pre_y=0;
    double slope;
    double k_horizontal;
    double b;
    double k_v;
    double b_v;
    int va;
    int position;//定位垂直扫描的第一个坐标点X值

    //垂直扫描第一条线的输出
    int ROW;
    int ay[5];
    int ay_width[5];

    //垂直扫描第一条线的输出
    int ROW2;
    int ay2[5];
    int ay_width2[5];

    int X[5] = {-1, -1, -1, -1, -1};//存储五个交点的坐标
    int Y[5] = {-1, -1, -1, -1, -1};

//    cvLine(image,CvPoint(40,40),CvPoint(400,40), CvScalar(0,0,250),1,8,0);
    int check[5] = {1, 1, 3, 1, 1};

    for(int i=1; i<height/5; i++)
    {
//        printf("--i=%d---\n", i);
        if(i%2==0)
        {
            N = i - 1;
        }
        else{
            N = i;
        }
        if(dis == 0)
        {
            y = height/2 + pow(-1, i) * 20*N;
        }
        else{

            y += pow(-1, i)*dis*n;
            n += 1;
        }
        if(y>0 && y<height)
        {
            for(int j=0; j<width;j++)
            {
                data[j] = pic[y*width+j]+0;
            }

        }

        int Xposition[200];
        int Xwidth[200];
        int LEN=1;

        binaryzation(/*image,*/ data, Xposition, Xwidth, &LEN);

        for(int j=0; j<LEN-4; j++)
        {
            for(int k=0; k<5;k++){
                pd[k] = Xwidth[j+k];
                ax[k] = Xposition[j+k];
            }
            int avg = (pd[0]+pd[1]+pd[3]+pd[4])/4;
            int err = avg*1/2;
            ok = 1;
            for(int i=0; i<5; i++)
            {
                if(pd[i] < check[i] * avg - err || pd[i] > check[i] * avg + err + 1)
                {
                    ok = 0;
                    break;
                }
            }
            if(ok==1)
            {
//                cout<<"水平解码位置x="<<y<<endl;
                x_num += 1;
                if(x_num<2)
                {
                    memcpy(pre_X, ax, sizeof(int)*5);
                    pre_y = y;
                    for(int i=0; i<5; i++)
                    {
                        sumx += pd[i];
//                        cout<<"pre_X"<<i<<"="<<pre_X[i]<<endl;

                    }
                    dis = 1.3*sumx;

//                    cout<<"sum="<<dis<<endl;
                    break;
                }else{
                    break;
                }
            }

        }
        if(x_num>=2)
        {
//            printf("--x_num>2---\n");
            if(ax[1]-pre_X[1]==0)
            {
                slope=1;
                k_horizontal=0;
                b = 0;
            }
            else{
                k_horizontal = (y-pre_y)/((ax[1]-pre_X[1])*1.0);
                slope = -k_horizontal;
                b = y - k_horizontal*(ax[0]-pd[0]);
            }
            position = pre_X[4]>ax[4]?ax[4]:pre_X[4];
            //垂直扫描第一条线
            int autoXY[2] = {-1, -1};
//            cout<<"垂直扫描第一条线"<<endl;
            va = test_vertical(/*image,*/  pic, position, sumx, ax,  slope, autoXY, &ROW, ay, ay_width);
            if(va==0){
                return 0;
            }
            if(k_horizontal>0)
            {
                int mminy = pre_y>y?y:pre_y;
                int distance = mminy - ay[4];
//                cvLine(image, CvPoint(pre_X[4], pre_y), CvPoint(ROW, ay[4]), CvScalar(0, 250,0), 2,8,0);
                double a = sqrt(pow(distance, 2)/(pow(k_horizontal, 2)+1)/1.0);
//                cout<<k_horizontal<<endl;
//                cout<<"double->a="<<(pow(k_horizontal, 2)+1)<<endl;
                int left_distance = (int)(sqrt(pow(a, 2)/(pow(k_horizontal, 2)+1)/1.0)*abs(k_horizontal));
//                cout<<"left_distance="<<left_distance<<endl;

                double w = sqrt(pow(sumx/7, 2)/(pow(k_horizontal,2)+1)/1.0)*abs(k_horizontal)*32;
                double width = sqrt(pow(w, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal);
                int right_distance = width - left_distance - sqrt(pow(sumx, 2)/(pow(k_horizontal, 2)+1));
//                cout<<"w="<<w<<endl;
//                cout<<"width="<<width<<endl;
//                cout<<"right_distance="<<right_distance<<endl;
                autoXY[0] = left_distance;
                autoXY[1] = right_distance;
            }else{
                autoXY[0] = -1;
                autoXY[1] = -1;
            }
//            cout<<"垂直扫描第二条线"<<endl;
            va = test_vertical(/*image, */ pic, ROW, sumx, ax,  0, autoXY, &ROW2, ay2, ay_width2);
            if(va==0){
                return 0;
            }
//            cvLine(image, CvPoint(ROW2, 0), CvPoint(ROW2, 300), CvScalar(0, 250,0), 2,8,0);
            int sumyy=0;
            for(int i=0; i<5; i++)
            {
                sumyy += ay_width2[i];
            }

            if(ay[1]-ay2[1]==0)
            {
                k_v = 0.0;
                b_v = 0.0;
            }
            else{
                k_v = (ay[1]-ay2[1])/((ROW-ROW2)*1.0);
                b_v = ay2[0] - ay_width2[0] - k_v*ROW2;
            }
//            cout<<"k_v="<<k_v<<endl;
//            cout<<"b_v="<<b_v<<endl;
            int re = slopeCross_point(/*image,*/pic, y, k_horizontal, b, k_v, b_v, ax, pd, ROW, ay, ay_width, ROW2, ay2, ay_width2, X, Y);
//            cvLine(image,CvPoint(X[2],Y[2]),CvPoint(X[4],Y[4]), CvScalar(0,0,250),1,8,0);
//            cvLine(image,CvPoint(X[3],Y[3]),CvPoint(X[4],Y[4]), CvScalar(0,0,250),1,8,0);
//            cout<<"re="<<re<<endl;
//            printf("\nre= %d\n", re);
            printf("\nTop_Left_Corner=[%d, %d]\n", X[0], Y[0]);



// cout<<"----------------------------------------------------------------------------------------------"<<endl;

            if(re)
            {
                int v_X1;
                int v_Y1;
                int h_X1;
                int h_Y1;
//                cout<<"k_hor="<<k_horizontal<<endl;
                pixel4XY[0] = (int)(sqrt(pow(sumx/7, 2)/(pow(k_horizontal,2)+1))*abs(k_horizontal)*4);
                pixel4XY[1] = (int)(sqrt(pow(sumyy/7, 2)/(pow(k_v, 2)+1))*4);

//                cout<<"pixel4XY[0]="<<pixel4XY[0]<<endl;
//                cout<<"pixel4XY[1]="<<pixel4XY[1]<<endl;
                //水平四像素粗略宽度的X投影
                int upX = (int)(sqrt(pow(pixel4XY[0], 2)/(pow(k_v, 2)+1)));
                //水平四像素粗略宽度的Y投影
                int upY = (int)(sqrt(pow(pixel4XY[0], 2)/(pow(k_v, 2)+1))*abs(k_v));
                //垂直四像素粗略宽度的X投影
                int leftX = (int)(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1)));
                //垂直四像素粗略宽度的Y投影
                int leftY = (int)(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal));

                double midV_b = Y[1] - k_v*X[1];
                double midh_b = Y[1] - k_horizontal*X[1];

                //水平位置中点;
                int hmidX1 = X[1] + upX/2;
                int hmidY1 = k_v*hmidX1+midV_b;
//                cout<<"hmidX1="<<hmidX1<<endl;
//                cout<<"hmidY1="<<hmidY1<<endl;
//                cvCircle(image, CvPoint(hmidX1,hmidY1), 2, CvScalar(0,0,250),2,8,0);

                //竖直位置中点
                int vmidY1 = Y[1] + leftY/2;
                int vmidX1 = (vmidY1-midh_b)/k_horizontal;
//                cout<<"vmidY1="<<vmidY1<<endl;
//                cout<<"vmidX1="<<vmidX1<<endl;
//                cvCircle(image, CvPoint(vmidX1,vmidY1), 2, CvScalar(0,0,250),2,8,0);

                if(k_horizontal>0)
                {
                    //水平
                    v_X1 = vmidX1+upX;
                    v_Y1 = vmidY1-upY;
//                    cvCircle(image, CvPoint(v_X1,v_Y1), 2, CvScalar(0,0,250),2,8,0);
//                    cout<<"newv_X="<<v_X1<<endl;
//                    cout<<"newv_Y="<<v_Y1<<endl;

                    //垂直
                    h_X1= hmidX1+leftX;
                    h_Y1 = hmidY1 + leftY;
//                    cvCircle(image, CvPoint(h_X1,h_Y1), 2, CvScalar(0,0,250),2,8,0);

                }else{
//                    cout<<"k_hor<0"<<endl;
                    v_X1 = vmidX1+upX;
                    v_Y1 = vmidY1+upY;
//                    cvCircle(image, CvPoint(v_X1,v_Y1), 2, CvScalar(0,0,250),2,8,0);
//                    cout<<"newv_X="<<v_X1<<endl;
//                    cout<<"newv_Y="<<v_Y1<<endl;

                    //垂直
                    h_X1= hmidX1-leftX;
                    h_Y1 = hmidY1 + leftY;
//                    cvCircle(image, CvPoint(h_X1,h_Y1), 2, CvScalar(0,0,250),2,8,0);
                }
//                cout<<"newv_X="<<v_X1<<endl;
//                cout<<"newv_Y="<<v_Y1<<endl;

                //水平位置寻找4像素单位边缘
                int c1;
                double B1 = v_Y1 - k_v*v_X1;
                if(pic[v_Y1*width+v_X1]+0>170)
                {
                    c1 = -1;
                }else{
                    c1 = 1;
                }
//                cout<<"c1="<<c1<<endl;
                if(c1==1)
                {
                    for(int i=0; i<10; i++)
                    {
                        if(pic[v_Y1*width+v_X1]+0<170)
                        {
                            v_X1 += c1;
                            v_Y1 = k_v*v_X1+B1;
                        }else{
                            break;
                        }

                    }
                }else{
                    for(int i=0; i<10; i++)
                    {
                        if(pic[v_Y1*width+v_X1]+0>110)
                        {

                            v_X1 += c1;
                            v_Y1 = k_v*v_X1+B1;
                        }else{
//                            v_X1 -= c1;
                            break;
                        }


                    }
                }
                //水平4像素精调宽度
                pixel4XY[0] = sqrt(pow(v_X1-vmidX1,2)+pow(v_Y1-vmidY1, 2));


                //垂直寻找4像素精准边缘
                double B2 = h_Y1 - k_horizontal*h_X1;
                int c2;
                if(pic[h_Y1*width+h_X1]+0<170)
                {
                    c2 = 1;
                }else{
                    c2 = -1;
                }

                if(c2==1)
                {
                    for(int i=0; i<10; i++)
                    {
                        if(pic[h_Y1*width+ h_X1]+0<170)
                        {
                            h_Y1 += c2;
                            h_X1 = (h_Y1-B2)/k_horizontal;
                        }else{
                            break;
                        }

                    }
                }else{
                    for(int i=0; i<10; i++)
                    {
                        if(pic[h_Y1*width+ h_X1]+0>110)
                        {
                            h_Y1 += c2;
                            h_X1 = (h_Y1-B2)/k_horizontal;
                        }else{
//                            h_Y1 -= c2;
                            break;
                        }


                    }
                }
                //竖直4像素精调高度
                pixel4XY[1] = sqrt(pow(h_X1-hmidX1,2)+pow(h_Y1-hmidY1, 2));


                double pixelX = 0;
                double pixelY = 0;
                printf("\ndecode data:\n");
                for(int i=1; i<7;i++)
                {
                    pixelY = pixel4XY[1];//竖直
//                    cout<<"1pixelY="<<pixelY<<endl;
                    pixelY = (i-1+0.5)*pixelY;
//                    cout<<"2pixelY="<<pixelY<<endl;

                    //垂直四像素粗略精确的X投影
                    int leftX = (int)(sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1)));
                    //垂直四像素粗略精确的Y投影
                    int leftY = (int)(sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal));

                    for(int j=1; j<9; j++)
                    {
                        pixelX = pixel4XY[0];//水平
//                        cout<<"1pixelX="<<pixelX<<endl;
                        pixelX = (j-1+0.5)*pixelX;
//                        cout<<"2pixelX="<<pixelX<<endl;

                        //水平四像素精确宽度的X投影
                        int upX = (int)(sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)));
                        //水平四像素精确宽度的Y投影
                        int upY = (int)(sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1))*abs(k_v));

                        if(k_horizontal>0)
                        {

                            v_X1 = upX+X[1];
                            v_Y1 = Y[1] - upY;
//                            cvCircle(image, CvPoint(v_X1,v_Y1), 3, CvScalar(250,0,0),3,8,0);
                            //计算垂直B
                            double B_h = v_Y1 - k_horizontal*v_X1;
//                            cvLine(image,CvPoint(v_X1,v_Y1),CvPoint(1,k_horizontal*1+B_h), CvScalar(0,250,0),1,8,0);

                            h_X1= leftX+X[1];
                            h_Y1 = Y[1] + leftY;

                            double B_v = h_Y1 -k_v*h_X1;
//                            cvLine(image,CvPoint(h_X1,h_Y1),CvPoint(1000,k_v*1000+B_v), CvScalar(0,250,0),1,8,0);

                            int midx = (int)((B_v-B_h)/(k_horizontal-k_v)/1.0);
                            int midy = (int)(midx*k_v/1.0+B_v);
                            int re = compare_pixel(pic, midx, midy);
//                            cout<<"result="<<re<<endl;
                            if(re>0)
                            {
                                if(pic[midy*width+midx]+0<127)
                                {
                                    printf("0\t");
                                }else{
                                    printf("1\t");
                                }
//                                printf("pixel=%d\n",pic[midy*width+midx]+0);
//                                cout<<"解码成功！！";
                            }
                            else{
                                return 0;
                            }


//                            cvCircle(image, CvPoint(h_X1,h_Y1), 3, CvScalar(0,0,250),3,8,0);
//                            cvCircle(image, CvPoint(midx,midy), 3, CvScalar(0,0,250),3,8,0);


                        }else{
                            v_X1 = upX+X[1];
                            v_Y1 = Y[1] + upY;
//                            cvCircle(image, CvPoint(v_X1,v_Y1), 3, CvScalar(250,0,0),3,8,0);
                            //计算垂直B
                            double B_h = v_Y1 - k_horizontal*v_X1;
//                            cvLine(image,CvPoint(v_X1,v_Y1),CvPoint(1,k_horizontal*1+B_h), CvScalar(0,250,0),1,8,0);

                            h_X1= X[1]-leftX;
                            h_Y1 = Y[1] + leftY;
                            double B_v = h_Y1 -k_v*h_X1;
//                            cvLine(image,CvPoint(h_X1,h_Y1),CvPoint(1000,k_v*1000+B_v), CvScalar(0,250,0),1,8,0);

                            int midx = (int)((B_v-B_h)/(k_horizontal-k_v)/1.0);
                            int midy = (int)(midx*k_v/1.0+B_v);

//                            cvCircle(image, CvPoint(h_X1,h_Y1), 3, CvScalar(0,0,250),3,8,0);
//                            cvCircle(image, CvPoint(midx,midy), 3, CvScalar(0,0,250),3,8,0);
                            int re = compare_pixel(pic, midx, midy);
//                            printf("pixel=%d\t", pic[midy*width+midx]+0);

//                            cout<<"result="<<re<<endl;

                            if(re>0)
                            {
                                if(pic[midy*width+midx]+0<127)
                                {
                                    printf("0\t");
                                }else{
                                    printf("1\t");
                                }
//                                printf("pixel=%d\n",pic[midy*width+midx]+0);
//                                cout<<"解码成功！！";
                            }else{
                                return 0;
                            }
//                            printf("*");

                        }

                    }
                    printf("\n");
                }

                return re;


            }


            break;
        }

    }
//    cout<<"解码失败！"<<endl;
//    printf("decode fail！！");
    return 0;

}

int test_vertical(/*IplImage* image,*/ unsigned char* pic,int position, int sum, int* ax,  double slope, int* a, int * ROW, int*ay, int *ay_width);//
int  slopeCross_point(/*IplImage *image,*/unsigned char* pic,int y,double k_horizontal, double b, double k_v, double b_v,  int *ax, int *pd, int ROW,int *ay, int *ay_width,int  ROW2, int *ay2, int *ay_width2, int* X , int * Y);

int decode(void *picture_cache)
{
    printf("\n-------start------\n");
    gettimeofday(&tpstart,NULL);
    int compare_pixel(unsigned char* pic, int x, int y);
    int finder_scan(/*IplImage* image,*/  unsigned char* pic, int* pixel4XY);
//    void binaryzation(IplImage* image,int *y, int * Xposition, int *Xwidth , int *num);

//    void four_point_transform( Mat* src, Mat *dst, int *X, int *Y);// Mat *dst,
//    Mat src = imread("120.jpg");
    //cout<<"\n-------start------\n"<<endl;

//    simplest_yuv420_gray("frame.nv12", w, h, 1);
//    FILE *fp = fopen("test.yuv", "rb++");
//    FILE *fp = *picture_cache;
//    if(fp==nullptr) {printf("error"); return 0;}
//    FILE * fp1 = fopen("outopt_gray.yuv", "wb+");
//    unsigned char* pic = (unsigned char*)malloc(width*height*3/2);
//    fread(pic, 1, width*height*3/2, fp);
      unsigned char* pic = (unsigned char*)picture_cache;

//    memset(pic+width*height, 128, width*height/2);
//    fwrite(pic, 1, width*height*3/2, fp1);

    //读取图像
//    IplImage* image = cvLoadImage("2.jpg");
////    Mat src = cvarrToMat(image);
//    if(!image)
//    {
////        cout<<"can not load image\n"<<endl;
//        printf("can not load image\n");
//    }
    //转换成灰度图像
//    IplImage* gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
//    cvCvtColor(image, gray, CV_BGR2GRAY);

//    cout<<"gray.width="<<gray->width<<";"<<"gray.height="<<gray->height<<endl;
//    uchar a[1600];
//    cvLine(image,CvPoint(40,40),CvPoint(400,40), CvScalar(0,0,250),1,8,0);//绘制直线
    int pixel4XY[2] = {0,0};

    int result = finder_scan(/*image,*/pic,pixel4XY);
    if(result)
    {
        printf("\n**decode success**\n");
    }else{
        printf("\n**decode fail**\n");
    }


//    free(pic);
//    fclose(fp);
//    fclose(fp1);


    //计算代码运行花费时间
    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
    printf("\ntime =%f",timeuse);
    printf("\n-------end------\n");
//    cvNamedWindow( "code", CV_WINDOW_AUTOSIZE );
//    cvShowImage( "code", image );
//    cvShowImage("gray", gray);

//    waitKey();//等待按键
    return 0;
}
