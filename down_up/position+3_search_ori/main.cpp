#include <opencv2/highgui/highgui.hpp>//OpenCV highgui 模块头文件
#include <opencv2/imgproc/imgproc.hpp>//OpenCV 图像处理头文件
#include<iostream>
#include <sys/time.h>
#include <math.h>
#include "binaryzation.cpp"

struct timeval tpstart,tpend;
float timeuse;

using namespace cv; // 包含 cv 命名空间
using namespace std;
int pixelMax=0, pixelMin=255;

//-----------------------寻找垂直1:1:2：2：1-----------------//
int test_vertical(IplImage* image,IplImage * gray, int position, int sum, int* ax,  double slope, int *a,int * ROW, int*ay, int *ay_width)//
{
    printf("---test_vertical-----");
    int check[5] = {1, 1, 2, 2, 1};
    int AVG = sum/7*12;
    int start_x;
    int z;
    uchar temp[1600];
    int data[1600];
    int pd[5];
    int Ay[5];
    int ok=0;
    int mmin = a[0]>a[1]?a[1]:a[0];

    cout<<"slope="<<slope<<endl;
    cout<<"mmin="<<mmin<<endl;
//    cout<<"a[0]="<<a[0]<<endl;
//    cout<<"a[1]="<<a[1]<<endl;
//    cout<<"输出="<<(pow(slope,2)+1)<<endl;
    if(slope>0){
        double w = sqrt(pow((sum/7),2)/(pow(slope,2)+1)/1.0)*abs(slope/1.0)*34;
        double wucha = sqrt(pow((sum/7),2)/(pow(slope,2)+1)/1.0);
        double pos = sqrt(pow(w,2)/(pow(slope,2)+1)/1.0)-wucha;
        start_x = int(position+pos)-1;
    }
    else if (slope==0 && mmin>=0){
//        cout<<"--in--"<<endl;
        if(a[0]>a[1])
        {
            start_x = int(position-a[0]);
            z = 1;
        }else{
            start_x = int(position+a[1]);
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

    if(start_x>=gray->width)
    {
        return 0;
    }
    int i;
    cout<<"start_x="<<start_x<<endl;
    for(int j=0; j<gray->width-start_x; j+=5)
    {
        int n = 0;
        if(slope == 0 && mmin>0)
        {
            i = start_x+5*j*z;
        }
        else if(slope==0 && mmin<0)
        {
            i = start_x+AVG*pow(-1, j)+10*j*pow(-1, j)*(-1);

        }
        else{
            i = start_x+5*j;
        }
        for(int j=0; j<gray->height;j++)
        {
            temp[j] = *((uchar *)gray->imageData+j*gray->widthStep+i);
            data[j] = temp[j]+0;
//            cout<<data[j]<<endl;
        }

        int y_col[200];
        int Ywidth[200];
        int LEN_y=0;
        cvLine(image, CvPoint(i, 0), CvPoint(i, gray->height), CvScalar(0, 0, 250), 1, 8, 0);
//        cvNamedWindow( "code", CV_WINDOW_AUTOSIZE );
//        cvShowImage( "code", image );
//        waitKey();//等待按键

        binaryzation(image, data, y_col, Ywidth, &LEN_y);
        printf("y-len=%d\n",LEN_y);
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
                if(pd[j]<check[j] * avg - err or pd[j] > check[j] * avg + err + 1)
                {
                    ok = 0;
                    break;
                }
            }
            if(ok==1)
            {
                n = 1;
                cout<<"垂直解码位置y="<<i<<endl;
//                cvLine(image, CvPoint(i, 0), CvPoint(i, gray->height), CvScalar(0, 0, 250), 1, 8, 0);
                break;
            }
        }
        if(n>0 and ok == 1)
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

int slopeCross_point(IplImage *image,IplImage *gray , int y,double k_horizontal, double b,
                     double k_v, double b_v,  int *ax, int *pd, int ROW, int *ay, int *ay_width,int  ROW2,
                     int *ay2, int *ay_width2, int* X, int * Y, int pixelMax, int pixelMin)
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
//    cout<<"sumx="<<sumx<<endl;
//    cout<<"sumy="<<sumy<<endl;
    if(k_v==0 or k_horizontal==0)
    {
        X[0] = ax[0] - pd[0];
        Y[0] = ay[0] - ay_width[0];
//        cout<<"x[0],y[0]="<<(X[0], Y[0])<<endl;
        cvCircle(image, CvPoint(X[0], Y[0]),3, CvScalar(0, 250, 0), 3, 8, 0);

        X[1] = int(ax[4]+sumx/7);
        Y[1] = int(ay[4]+sumy/7);
//        cout<<"x[1],y[1]="<<(X[1], Y[1])<<endl;
//        cout<<"pixel="<<*(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0<<endl;
//        gray->imageData+y*gray->widthStep+j
        cvCircle(image, CvPoint(X[1], Y[1]),3, CvScalar(0, 250, 0), 3, 8, 0);
        if(*(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0>127)
        {
            for(int i=1; i<10; i++)
            {
                if(*(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1]+i)+0<127){
                    X[1] += i;
                    break;
                }
                if(*(uchar *)(gray->imageData+(Y[1]+i)*gray->widthStep+X[1])+0<127){
                    Y[1] += i;
                    break;
                }
                if(*(uchar *)(gray->imageData+(Y[1]+i)*gray->widthStep+X[1]+i)+0<127){
                    X[1] += i;
                    Y[1] += i;
                    break;
                }

            }
        }

        int width = sumx/7*35;
        X[2] = int(X[1]+width);
        Y[2] = Y[1];
        cvCircle(image, CvPoint(X[2], Y[2]),3, CvScalar(0, 250, 0), 3, 8, 0);

        int high = sumy/7*28;
        Y[3] = int(Y[1]+high);
        X[3] = X[1];
        cvCircle(image, CvPoint(X[3], Y[3]),3, CvScalar(0, 250, 0), 3, 8, 0);

        X[4] = X[2];
        Y[4] = Y[3];
        cvCircle(image, CvPoint(X[4], Y[4]),3, CvScalar(0, 250, 0), 3, 8, 0);
    }else{
        X[0] = int((b_v-b)/(k_horizontal-k_v));
        Y[0] = int(k_v*X[0]+b_v);
        if(X[0]>gray->width || Y[0]>gray->height){
            return 0;
        }
        cvCircle(image, CvPoint(X[0], Y[0]),2, CvScalar(0, 250, 0), 2, 8, 0);
//        cout<<"(x0=)"<<X[0]<<endl;
//        cout<<"(y0=)"<<Y[0]<<endl;

        bb = y-k_horizontal*(ax[4]+pd[4]);
        bb_v = (ay2[4]+ay_width2[4])-k_v*ROW2;

        printf("k_hor=%f, k_V=%f\n", k_horizontal, k_v);
        X[1] = int((bb_v-bb)/(k_horizontal-k_v));
        Y[1] = int(k_v*X[1]+bb_v);
        if(X[1]>gray->width || Y[1]>gray->height){
            return 0;
        }
        cout<<"1(x1=)"<<X[1]<<endl;
        cout<<"1(y1=)"<<Y[1]<<endl;
        printf("1像素=%d\n", *(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0);
        cvCircle(image, CvPoint(X[1], Y[1]),2, CvScalar(0, 250, 0), 1, 8, 0);

        if(*(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0>pixelMax)
        {
            for(int i=1; i<10; i++)
            {
                if(*(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1]+i)+0<pixelMax){
                    X[1] += i;
                    break;
                }
                if(*(uchar *)(gray->imageData+(Y[1]+i)*gray->widthStep+X[1])+0<pixelMax){
                    Y[1] += i;
                    break;
                }
                if(*(uchar *)(gray->imageData+(Y[1]+i)*gray->widthStep+X[1]+i)+0<pixelMax){
                    X[1] += i;
                    Y[1] += i;
                    break;
                }

            }
        }

        cout<<"2(x1=)"<<X[1]<<endl;
        cout<<"2(y1=)"<<Y[1]<<endl;
        while(*(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1]-1)+0<127)
        {
            X[1] -= 1;
//            Y[1] = int(k_v*X[1]+bb_v);
        }
        cout<<"3(x1=)"<<X[1]<<endl;
        cout<<"3(y1=)"<<Y[1]<<endl;
        printf("2像素=%d\n", *(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0);
        cvCircle(image, CvPoint(X[1], Y[1]),2, CvScalar(0, 0, 250), 1, 8, 0);


        double w = sqrt(pow(sumx/7,2)/(pow(k_horizontal,2)+1))*abs(k_horizontal)*35;
        int width = sqrt(pow(w, 2)/((pow(k_horizontal, 2)+1)*1.0))*abs(k_horizontal);

//        cout<<"w="<<w<<endl;
//        cout<<"width="<<width<<endl;

        X[2] = X[1]+width;
        Y[2] = int(k_v*X[2]+bb_v);
//        cvCircle(image, CvPoint(X[2], Y[2]),3, CvScalar(0, 250, 0), 3, 8, 0);

        double ww = sqrt(pow(sumy/7, 2)/(pow(k_v, 2)+1))*26;
        int high = sqrt(pow(ww, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal);

        Y[3] = Y[1]+high;
        X[3] = int((Y[3]-bb)/k_horizontal);
//        cvCircle(image, CvPoint(X[3], Y[3]),3, CvScalar(0, 250, 0), 3, 8, 0);

        double b_v4 = Y[3] - k_v*X[3];
        double b4 = Y[2] - k_horizontal*X[2];
        X[4] = int((b_v4-b4)/(k_horizontal-k_v));
        Y[4] = int(k_v*X[4]+b_v4);
//        cvCircle(image, CvPoint(X[4], Y[4]),3, CvScalar(0, 250, 0), 3, 8, 0);
        bool button1, button2, button3, button4;
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

void four_point_transform(Mat* src,Mat* dst, int *X, int *Y)// Mat *dst,
{

    Point2f srcPoints[4];//原图中的四点 ,一个包含三维点（x，y）的数组，其中x、y是浮点型数
    Point2f dstPoints[4];//目标图中的四点

    srcPoints[0] = Point2f(X[1], Y[1]);
    srcPoints[1] = Point2f(X[2], Y[2]);
    srcPoints[2] = Point2f(X[4], Y[4]);
    srcPoints[3] = Point2f(X[3], Y[3]);

    int widthA = sqrt(pow((X[3]-X[4]),2) + pow((Y[3]-Y[4]),2));
    int widthB = sqrt(pow((X[1]-X[2]),2) + pow((Y[1]-Y[2]),2));
    int maxWidth = widthA>widthB?widthA:widthB;

    //计算新图片的高度值，选取垂直差值的最大值
    int heightA = sqrt(pow((X[1]-X[3]),2) + pow((Y[1]-Y[3]),2));
    int heightB = sqrt(pow((X[2]-X[4]),2) + pow((Y[2]-Y[4]),2));
    int maxHeight = heightA>heightB?heightA:heightB;

    cout<<"maxWidth="<<maxWidth<<endl;
    cout<<"maxHeight="<<maxHeight<<endl;
    dstPoints[0] = Point2f(0, 0);
    dstPoints[1] = Point2f(maxWidth-1, 0);
    dstPoints[2] = Point2f(maxWidth-1, maxHeight-1);
    dstPoints[3] = Point2f(0, maxHeight-1);

    int newX[4] = {1, 1, maxHeight, maxHeight};
    int newY[4] = {1, maxWidth, 1,maxWidth};

    double A[8][8] = {X[0], Y[0], 1, 0, 0, 0, -newX[0]*X[0], -newX[0]*Y[0],
                      0, 0, 0, X[0], Y[0], 1,-newY[0]*X[0], -newY[0]*Y[0],
                      X[1], Y[1], 1, 0, 0, 0, -newX[1]*X[1], -newX[1]*Y[1],
                      0, 0, 0, X[1], Y[1], 1,-newY[1]*X[1], -newY[1]*Y[1],
                      X[2], Y[2], 1, 0, 0, 0, -newX[2]*X[2], -newX[2]*Y[2],
                      0, 0, 0, X[2], Y[2], 1,-newY[2]*X[2], -newY[2]*Y[2],
                      X[3], Y[3], 1, 0, 0, 0, -newX[3]*X[3], -newX[3]*Y[3],
                      0, 0, 0, X[3], Y[3], 1,-newY[3]*X[3], -newY[3]*Y[3]};
    int B[8] = {newX[0], newY[0], newX[1], newY[1], newX[2], newY[2], newX[3], newY[3]};
    double C[8];
//    for(int i=0 ; i<8; i++)
//    {
//        for(int j=0; j<8; j++)
//        {
//            C[i] += A[]
//        }
//    }


    for(int i=0 ; i<4; i++ )
    {
        cout<<"X="<<X[i]<<endl;
        cout<<"Y="<<Y[i]<<endl;
        cout<<"newX="<<newX[i]<<endl;
        cout<<"newY="<<newY[i]<<endl;
    }
    for(int i=0 ; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            cout<<A[i][j]<<endl;
        }
    }




    Mat M1 = getPerspectiveTransform(srcPoints, dstPoints);//由四个点对计算透视变换矩阵
//    cout<<M1<<"\t";
    warpPerspective(*src,*dst , M1, Size(maxWidth, maxHeight));//仿射变换
//    cout<<"WH="<<WH<<endl;
//    mywarpPerspective(*src,*dst , M1, maxWidth, maxHeight);//仿射变换
//    mywarpPerspective(*src, *dst, M1, 1);
//    imshow("perspective transformation1（四点法）", dst);
//    return dst;


}

int pos[2];
int key=1;
int flood_fill_seed(IplImage *image, IplImage *gray , int x, int y, int *nextX, int *nextY, int *book)
{
    int tx;
    int ty;
//    printf("原像素=%d\n", *((uchar *)gray->imageData+y*gray->widthStep+x));

    for(int i=0; i<3; i++)
    {
        tx = x + nextX[i];
        ty = y + nextY[i];
//        printf("next-x=%d, next-y=%d\n", nextX[i], nextY[i]);
//        printf("x=%d, y=%d\n", x, y);
//        printf("tx=%d, ty=%d\n", tx, ty);
        if(tx <1 || tx>gray->width || ty<1 || ty>gray->height)
        {
            continue;
        }
//        printf("像素=%d\n", *((uchar *)gray->imageData+ty*gray->widthStep+tx));
        if(book[ty*gray->width+tx] == 0 && ((pixelMax+pixelMin)/2 >= *((uchar *)gray->imageData+ty*gray->widthStep+tx)))
        {
            book[ty*gray->width+tx] = 1;
//            cvCircle(image, CvPoint(tx,  ty), 1, CvScalar(0, 0,250), 2, 8, 0);

            flood_fill_seed(image, gray, tx,  ty, nextX, nextY, book);
        }
//        printf("---------key=%d\n", key);
        if(key==0)
        {
            return 1;
        }
        if(i==2 && book[ty*gray->width+tx] == 0)
        {
            pos[0] = x;
            pos[1] = y;
            key = 0;
            printf("-----------pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
//            cvCircle(image, CvPoint(x,  y), 1, CvScalar(0, 250,250), 2, 8, 0);
            return 1;
        }
    }
    return 0;
}

int finder_scan(IplImage* image, IplImage* gray)
{
    int h=0;
    int sumx=0;
    int N = 0;
    int dis = 0;
    int dis_i = 0;
    int n = 1;
    int ok = 0;
    int y = 0;
    int data[1600];
    uchar a[1600];
    int x_num=0;//记录可解线条的次数

    int pre_X[5];
    int pre_X2[5];
    int pd[5];
    int ax[5];
    int pre_y=0;
    int pre_y2=0;
    double slope;
    double k_horizontal;
    double b;
    double k_v;
    double b_v;
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
    int book[gray->width*gray->height];
    memset(book, 0, sizeof(book));

//    cvLine(image,CvPoint(40,40),CvPoint(400,40), CvScalar(0,0,250),1,8,0);
    int check[5] = {1, 1, 3, 1, 1};

    for(int i=1; i<gray->height/5; i++)
    {
        if(i%2==0)
        {
            N = i - 1;
        }
        else{
            N = i;
        }
        if(dis == 0)
        {   
            y = gray->height/2 + pow(-1, i) * 10*N;
//            cout<<"dis==0-->"<<y<<"i="<<i<<"N"<<N<<endl;
        }
        else{


//            y += pow(-1, i)*dis*n;
//            n += 1;

            y = dis + dis_i*i;
             cvLine(image,CvPoint(0,y),CvPoint(gray->width,y), CvScalar(0,0,250),1,8,0);
//            cout<<"tt="<<tt<<endl;
            cout<<"dis!=0-->"<<y<<",dis_i"<<dis_i<<",dis="<<dis<<",y="<<y<<endl;
        }
//        cout<<"-------------y="<<y<<endl;

        if(y>0 && y<gray->height)
        {
            cout<<"-------------y="<<y<<endl;
            cvLine(image,CvPoint(0,y),CvPoint(gray->width,y), CvScalar(0,250,0),1,8,0);
            for(int j=0; j<gray->width;j++)
            {
                a[j] = *((uchar *)gray->imageData+y*gray->widthStep+j);
                data[j] = a[j]+0;
            }

        }
        int Xposition[200];
        int Xwidth[200];
        int LEN=1;

        binaryzation(image, data, Xposition, Xwidth, &LEN);
        printf("length=%d\n",LEN);

//        for(int i=0; i<=LEN; i++)
//        {
//            cout<<"Xposition"<<i<<"="<<Xposition[i]<<endl;
//            cout<<"Xwidth"<<i<<"="<<Xwidth[i]<<endl;
//        }
//        cvCircle(image, CvPoint(Xposition[3],259), 3, CvScalar(0,250,0),2,8,0);

        for(int j=0; j<LEN-4; j++)
        {
            for(int k=0; k<5;k++){
                pd[k] = Xwidth[j+k];
                ax[k] = Xposition[j+k];
            }
//            int pd[5] = {Xwidth[j], Xwidth[j+1], Xwidth[j+2], Xwidth[j+3], Xwidth[j+4]};
//            int ax[5] = {Xposition[j], Xposition[j+1], Xposition[j+2], Xposition[j+3], Xposition[j+4]};
            int avg = (pd[0]+pd[1]+pd[3]+pd[4])/4;
            int err = avg*1/3;
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
                printf("ax[0]=%d, y=%d\n", ax[0], y);
                cvCircle(image, CvPoint(ax[0], y), 5, CvScalar(0, 250,0), 2, 8, 0);

                printf("gray=%d\n", *(uchar *)(gray->imageData+y*gray->widthStep+ax[0]-1)+0);
                int black[5] = {*(uchar *)(gray->imageData+y*gray->widthStep+ax[0]-1)+0, *(uchar *)(gray->imageData+y*gray->widthStep+ax[1]+1)+0,
                               *(uchar *)(gray->imageData+y*gray->widthStep+ax[2]-1)+0,*(uchar *)(gray->imageData+y*gray->widthStep+ax[3]+1)+0,
                               *(uchar *)(gray->imageData+y*gray->widthStep+ax[4]-1)+0};
                int white[5] = {*(uchar *)(gray->imageData+y*gray->widthStep+ax[0]+1)+0, *(uchar *)(gray->imageData+y*gray->widthStep+ax[1]-1)+0,
                               *(uchar *)(gray->imageData+y*gray->widthStep+ax[2]+1)+0, *(uchar *)(gray->imageData+y*gray->widthStep+ax[3]-1)+0,
                               *(uchar *)(gray->imageData+y*gray->widthStep+ax[4]+1)+0};
                int maxx, mmin;
                for(int i=0; i<5; i++)
                {
                    printf("black1=%d, white1=%d\n", black[i], white[i]);
                    if(pixelMin>pixelMax){
                        mmin = white[i]<pixelMin?white[i]:pixelMin;
                        maxx = black[i]>pixelMax?black[i]:pixelMax;
                        if(mmin>maxx)
                        {
                            pixelMin=mmin;
                            pixelMax=maxx;
                        }else{
                            break;
                        }
                    }
                }
                printf("pixelMax=%d, pixelMin=%d\n", pixelMax, pixelMin);
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

                    //增加点搜索功能开始
                    cvCircle(image, CvPoint(ax[1]+pd[2]/2, y), 1, CvScalar(0, 0,250), 2, 8, 0);


                    int nextXd[3] = {0, 1, -1};
                    int nextYd[3] = {1, 1, 1};//向下找
                    printf("ax[0]-ay_width[0]=%d, y=%d\n", ax[0]-pd[0], y);
                    int a = flood_fill_seed(image, gray, ax[1]+pd[2]/2, y, nextXd, nextYd, book);
                    if(a)
                    {
                       cvCircle(image, CvPoint(pos[0], pos[1]), 1, CvScalar(0, 250,0), 2, 8, 0);
                       printf("pos像素=%d\n", *(uchar *)(gray->imageData+pos[1]*gray->widthStep+pos[0])+0);
                       double pw = sqrt(pow(sumx, 2)/(pow(1,2)+1));
                       double h = sqrt(pow(pw, 2)/(pow(1,2)+1));
                       int poss_down=pos[1]-h;
                       printf("sum=%d, pw=%f, h=%f\n", sumx, pw, h);
                       printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
                       dis = poss_down;
                       dis_i = -1;
                       printf("down__dis=%d\n", dis);
                       printf("down__dis-y=%d\n", dis-y);
                       cvCircle(image, CvPoint(pos[0], pos[1]-h), 1, CvScalar(0, 250,250), 2, 8, 0);//向下
                       if(dis-y<sumx)
                       {
                           key = 1;//打开key递归开关
                           memset(book, 0, sizeof(book));
                           int nextXu[3] = {0, 1, -1};
                           int nextYu[3] = {-1, -1, -1};//向上找
                           int a = flood_fill_seed(image, gray, ax[1]+pd[2]/2, y, nextXu, nextYu, book);
                           printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
                           if(a)
                           {
                               int poss_up = pos[1]+sumx;
                               dis = abs(poss_up-y)>abs(poss_down-y)?poss_up:poss_down;
                               if(abs(poss_up-y)>abs(poss_down-y))
                               {
                                   dis = poss_up;
                                   dis_i = 1;
                               }else{
                                    dis = poss_down;
                                    dis_i = -1;
                               }
                                printf("up-----dis=%d, %d, %d----------\n",dis, poss_up, poss_down);
                               cvCircle(image, CvPoint(pos[0], pos[1]+sumx), 1, CvScalar(0, 0,250), 2, 8, 0);//向上
                           }

                       }

                    }
                    cvLine(image,CvPoint(0,dis),CvPoint(600,dis), CvScalar(250,0,0),1,8,0);
                    //增加点搜索功能完毕
                    break;
                }else{
                    break;
                }
            }

        }
        if(x_num>=2)
        {

            cout<<"######num>=2"<<endl;
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
//                cout<<"k_horizontal:"<<k_horizontal<<"slope:"<<slope<<"b:"<<b<<endl;
            }

//            cvLine(image, CvPoint(409,188-5), CvPoint(1,k_horizontal*1+(188-k_horizontal*409)-5),CvScalar(250,0,0),1,8,0);
            position = pre_X[4]>ax[4]?ax[4]:pre_X[4];
//            printf("%d, %d, %d",position, ax[4], pre_X[4]);
            //垂直扫描第一条线
            int autoXY[2] = {-1, -1};
            cout<<"垂直扫描第一条线"<<endl;
            int va = test_vertical(image, gray, position, sumx, ax,  slope, autoXY, &ROW, ay, ay_width);//
            cout<<"垂直扫描第一条线va="<<va<<endl;
            int sumy=0;
            for(int i=0; i<5; i++)
            {
                sumy += ay_width[i];
            }
            if(va==0){
                return 0;
            }
            int black1[5] = {*(uchar *)(gray->imageData+(ay[0]-1)*gray->widthStep+ROW)+0, *(uchar *)(gray->imageData+(ay[1]+1)*gray->widthStep+ROW)+0,
                           *(uchar *)(gray->imageData+(ay[2]-1)*gray->widthStep+ROW)+0,*(uchar *)(gray->imageData+(ay[3]+1)*gray->widthStep+ROW)+0,
                           *(uchar *)(gray->imageData+(ay[4]-1)*gray->widthStep+ROW)+0};
            int white1[5] = {*(uchar *)(gray->imageData+(ay[0]+1)*gray->widthStep+ROW)+0, *(uchar *)(gray->imageData+(ay[1]-1)*gray->widthStep+ROW)+0,
                           *(uchar *)(gray->imageData+(ay[2]+1)*gray->widthStep+ROW)+0, *(uchar *)(gray->imageData+(ay[3]-1)*gray->widthStep+ROW)+0,
                           *(uchar *)(gray->imageData+(ay[4]+1)*gray->widthStep+ROW)+0};
            int maxx, mmin;
//            int sumyy
            for(int i=0; i<5; i++)
            {
                printf("black1=%d, white1=%d\n", black1[i], white1[i]);
                if(pixelMin>pixelMax){
                    mmin = white1[i]<pixelMin?white1[i]:pixelMin;
                    maxx = black1[i]>pixelMax?black1[i]:pixelMax;
                    if(mmin>maxx)
                    {
                        pixelMin=mmin;
                        pixelMax=maxx;
                    }else{
                        break;
                    }
                }
            }
            printf("pixelMax=%d, pixelMin=%d\n", pixelMax, pixelMin);
//            cout<<"ROW"<<ROW<<endl;
//            cout<<"k_horizontal"<<k_horizontal<<endl;

            if(k_horizontal>0)
            {
                int mminy = pre_y>y?y:pre_y;
                int distance = mminy - ay[4];
//                cvLine(image, CvPoint(pre_X[4], pre_y), CvPoint(ROW, ay[4]), CvScalar(0, 250,0), 2,8,0);
                double a = sqrt(pow(distance, 2)/(pow(k_horizontal, 2)+1)/1.0);
//                cout<<k_horizontal<<endl;
//                cout<<"double->a="<<(pow(k_horizontal, 2)+1)<<endl;
                int left_distance = int(sqrt(pow(a, 2)/(pow(k_horizontal, 2)+1)/1.0)*abs(k_horizontal));
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

            //测试洪水遍历法找最右边的点开始
            key = 1;//打开key递归开关
            memset(book, 0, sizeof(book));
            int nextXu[3] = {1, 1, 1};
            int nextYu[3] = {0, 1, -1};//向上找
            int a = flood_fill_seed(image, gray, ROW, ay[1]+ay_width[2]/2, nextXu, nextYu, book);
            printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
            int pos_y = pos[0]- sqrt(pow(sumy, 2)/2);
            cvCircle(image, CvPoint(pos[0],pos[1]), 2, CvScalar(0,0,250),2 ,8,0);
            cvCircle(image, CvPoint(pos_y,pos[1]), 2, CvScalar(0,250,0),2 ,8,0);


            //测试洪水遍历法找最右边的点开始


            cout<<"垂直扫描第二条线"<<endl;
            int n=0;
            k_v =k_horizontal;
            printf("k_horizontal=%f, k_v=%f\n", k_horizontal, k_v);
            while(k_horizontal*k_v>0)
            {
                printf("k_horizontal*k_v>0");
                va = test_vertical(image, gray, ROW+n, sumx, ax,  0, autoXY, &ROW2, ay2, ay_width2);
                printf("------k_hor*k_V-->%f*%f=%f\n", k_horizontal,k_v, k_horizontal*k_v);
                if(va==0)
                {
                    return 0;
                }
                if(ay[1]-ay2[1]==0)
                {
                    k_v = 0.0;
                    b_v = 0.0;
                }
                else{
                        k_v = (ay[1]-ay2[1])/((ROW-ROW2)*1.0);
                        b_v = ay2[0] - ay_width2[0] - k_v*ROW2;

                        cvCircle(image, CvPoint(ROW,ay[1]), 2, CvScalar(0,250,250),2 ,8,0);
                        cvCircle(image, CvPoint(ROW2,ay2[1]), 2, CvScalar(0,250,250),2 ,8,0);
                        printf("ROW=%d, ROW2=%d, ay=%d, ay2=%d\n", ROW, ROW2, ay[1], ay2[1]);

                }
                n += 10;

                if(n>100)
                {
                    return 0;
                }
            }

            int black[5] = {*(uchar *)(gray->imageData+(ay2[0]-1)*gray->widthStep+ROW2)+0, *(uchar *)(gray->imageData+(ay2[1]+1)*gray->widthStep+ROW2)+0,
                           *(uchar *)(gray->imageData+(ay2[2]-1)*gray->widthStep+ROW2)+0,*(uchar *)(gray->imageData+(ay2[3]+1)*gray->widthStep+ROW2)+0,
                           *(uchar *)(gray->imageData+(ay2[4]-1)*gray->widthStep+ROW2)+0};
            int white[5] = {*(uchar *)(gray->imageData+(ay2[0]+1)*gray->widthStep+ROW2)+0, *(uchar *)(gray->imageData+(ay2[1]-1)*gray->widthStep+ROW2)+0,
                           *(uchar *)(gray->imageData+(ay2[2]+1)*gray->widthStep+ROW2)+0, *(uchar *)(gray->imageData+(ay2[3]-1)*gray->widthStep+ROW2)+0,
                           *(uchar *)(gray->imageData+(ay2[4]+1)*gray->widthStep+ROW2)+0};
//            int maxx=0, mmin=255;
            for(int i=0; i<5; i++)
            {
                printf("black1=%d, white1=%d\n", black[i], white[i]);
                if(pixelMin>pixelMax){
                    mmin = white[i]<pixelMin?white[i]:pixelMin;
                    maxx = black[i]>pixelMax?black[i]:pixelMax;
                    if(mmin>maxx)
                    {
                        pixelMin=mmin;
                        pixelMax=maxx;
                    }else{
                        break;
                    }
                }
//                cout<<"ay"<<i<<"="<<ay[i]<<endl;
//                cout<<"ay_width"<<i<<"="<<ay_width[i]<<endl;
            }
            printf("pixelMax=%d, pixelMin=%d\n", pixelMax, pixelMin);
//            cvLine(image, CvPoint(ROW2, 0), CvPoint(ROW2, 300), CvScalar(0, 250,0), 2,8,0);
            int sumyy=0;
            for(int i=0; i<5; i++)
            {
                sumyy += ay_width2[i];
//                cout<<"ay2"<<i<<"="<<ay2[i]<<endl;
//                cout<<"ay_width2"<<i<<"="<<ay_width2[i]<<endl;
            }

            b_v = ay2[0] - ay_width2[0] - k_v*ROW2;
            printf("------k_hor*k_V-->%f*%f=%f\n", k_horizontal,k_v, k_horizontal*k_v);
            if(k_horizontal*k_v>0){
                return 0;
            }
            int re = slopeCross_point(image, gray, y, k_horizontal, b, k_v, b_v, ax, pd, ROW, ay, ay_width, ROW2, ay2, ay_width2, X, Y, pixelMax, pixelMin);
//            cvLine(image,CvPoint(X[1],Y[1]),CvPoint(X[3],Y[3]), CvScalar(0,0,250),1,8,0);
//            cvLine(image,CvPoint(X[3],Y[3]),CvPoint(X[4],Y[4]), CvScalar(250,0,0),1,8,0);
//            cvLine(image,CvPoint(X[2],Y[2]),CvPoint(X[4],Y[4]), CvScalar(240,0,0),1,8,0);
            printf("X[0]=%d, Y[0]=%d\n", X[0], Y[0]);
            cout<<"re="<<re<<endl;
            if(re)
            {
//                cvCircle(image, CvPoint(X[1],Y[1]), 2, CvScalar(0,0,250),1,8,0);
//                printf("pic[X[1], Y[1]]=%d\n", pic[Y[1]*width+X[1]]);
                int v_X1;
                int v_Y1;
                int h_X1;
                int h_Y1;
                double pixel4XY[2] = {0, 0};
                cout<<"k_v="<<k_v<<endl;
                cout<<"k_horizontal="<<k_horizontal<<endl;
                pixel4XY[0] = sqrt(pow(sumx/7, 2)/(pow(k_horizontal,2)+1))*abs(k_horizontal)*8;
                pixel4XY[1] = sqrt(pow(sumyy/7, 2)/(pow(k_v, 2)+1))*8;

                cout<<"pixel4XY[0]="<<pixel4XY[0]<<endl;
//                cout<<"pixel4XY[1]="<<pixel4XY[1]<<endl;
                //水平四像素粗略宽度的X投影
                int upX = int(sqrt(pow(pixel4XY[0], 2)/(pow(k_v, 2)+1)));
                //水平四像素粗略宽度的Y投影
                int upY = int(sqrt(pow(pixel4XY[0], 2)/(pow(k_v, 2)+1))*abs(k_v));
                //垂直四像素粗略宽度的X投影
                int leftX = int(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1)));
                //垂直四像素粗略宽度的Y投影
                int leftY = int(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal));

                cout<<"upX="<<upX<<endl;
                cout<<"upY="<<upY<<endl;
//                cout<<"leftX="<<leftX<<endl;
//                cout<<"leftY="<<leftY<<endl;

                double midV_b = Y[1] - k_v*X[1];
                double midh_b = Y[1] - k_horizontal*X[1];

                //水平位置中点;
                int hmidX1 = X[1] + upX/4;
                int hmidY1 = k_v*hmidX1+midV_b;
                cout<<"hmidX1="<<hmidX1<<endl;
                cout<<"hmidY1="<<hmidY1<<endl;
                cvCircle(image, CvPoint(hmidX1,hmidY1), 2, CvScalar(250,0,0),2,8,0);
                int BOOL=1;
                if(*(uchar *)(gray->imageData+hmidY1*gray->widthStep+hmidX1)+0<pixelMax && *(uchar *)(gray->imageData+(hmidY1-1)*gray->widthStep+hmidX1)+0<pixelMin)
                {
                    printf("从下往上找边缘");
                    BOOL = 0;
                        for(int i=0; i<10; i++)
                        {
                            if(*(uchar *)(gray->imageData+hmidY1*gray->widthStep+hmidX1)+0<pixelMax)
                            {
                                hmidY1 -= 1;
    //                            hmidX1 = (hmidY1-midV_b)/k_v;
                            }else{
                                hmidY1 += 1;//保证从黑色像素点起始
                                break;
                            }
                        }

                }
                if(BOOL)
                {
//                    printf("从上往下找边缘");
//                    printf("当前点像素：%d\n", *(uchar *)(gray->imageData+hmidY1*gray->widthStep+hmidX1)+0);
//                    printf("当前点像素：%d\n", *(uchar *)(gray->imageData+(hmidY1+1)*gray->widthStep+hmidX1)+0);
//                    printf("%d, %d\n", pixelMin, pixelMax);

                    if(*(uchar *)(gray->imageData+hmidY1*gray->widthStep+hmidX1)+0>pixelMin && *(uchar *)(gray->imageData+(hmidY1+1)*gray->widthStep+hmidX1)+0>pixelMin )
                    {
                        for(int i=0; i<10; i++)
                        {
                            if(*(uchar *)(gray->imageData+hmidY1*gray->widthStep+hmidX1)+0>pixelMin)
                            {
                                hmidY1 += 1;
//                                hmidX1 = (hmidY1-midV_b)/k_v;
                            }else{
                                break;
                            }
                        }
                    }
                }

                cout<<"hmidX1="<<hmidX1<<endl;
                cout<<"hmidY1="<<hmidY1<<endl;
                cvCircle(image, CvPoint(hmidX1,hmidY1), 2, CvScalar(0,250,0),2,8,0);


                //竖直位置中点
                int vmidY1 = Y[1] + leftY/4;
                int vmidX1 = (vmidY1-midh_b)/k_horizontal;
                cvCircle(image, CvPoint(vmidX1,vmidY1), 2, CvScalar(0,250,250),2,8,0);

                if(*(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0<pixelMax)//当前点是黑色
                {
                    if(*(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0<pixelMax)
                    {
                        for(int i=0; i<10; i++)
                        {
                            if(*(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0<pixelMin)
                            {
                                vmidX1 -= 1;
                            }else{
                                vmidX1 += 1;
                                break;
                            }
                        }
                    }else{
                        break;
                    }
                }
                else if(*(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0>pixelMin)//当前点是白色
                {
                    printf("从左往右找边缘");
                    for(int i=0; i<10; i++)
                    {
                        if(*(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0>pixelMin)
                        {
                            vmidX1 += 1;
                        }else{
                            break;
                        }
                    }
                }
                if(k_horizontal>0)
                {
                    //水平
                    v_X1 = vmidX1+upX;
                    v_Y1 = vmidY1-upY;
                    //垂直
                    h_X1= hmidX1+leftX;
                    h_Y1 = hmidY1 + leftY;
//                    cvCircle(image, CvPoint(h_X1,h_Y1), 2, CvScalar(0,250,0),2,8,0);

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
                cout<<"newv_X="<<v_X1<<endl;
                cout<<"newv_Y="<<v_Y1<<endl;

                //水平位置寻找4像素单位边缘
                printf("*************************测试水平加载一个白像素点开始********************************\n");
                int c1;
                double B1 = v_Y1 - k_v*v_X1;
                if(*(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0<pixelMin)
                {
                    c1 = -1;
                }else{
                    c1 = 1;
                }
//                cout<<"c1="<<c1<<endl;
//                printf("now 像素=%d\n", *(uchar *)(gray->imageData+(v_Y1-1)*gray->widthStep+v_X1)+0);
                printf("now 像素=%d\n", *(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0);
//                printf("now 像素=%d\n", *(uchar *)(gray->imageData+(v_Y1+1)*gray->widthStep+v_X1)+0);
//                cvCircle(image, CvPoint(v_X1,v_Y1), 2, CvScalar(0,250,250),2,8,0);
                if(c1==1)
                {
                    printf("c1==1\n");
                    for(int i=0; i<40; i++)
                    {
                        if(*(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0>(pixelMax+pixelMin)/2)
                        {
                            printf("v_X1+1\n");
                            v_X1 += c1;
                            v_Y1 = k_v*v_X1+B1;
                            printf("if水平边缘点像素=%d\n", *(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0);
                        }else{
//                            v_X1 -= c1;
//                            v_Y1 = k_v*v_X1+B1;
                            printf("else水平边缘点像素=%d\n", *(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0);
                            break;
                        }

                    }
                }else{
                    printf("c1==-1\n");
                    for(int i=0; i<10; i++)
                    {
                        cout<<"c=-1"<<endl;
                        cout<<"水平位置："<<*(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0<<endl;
                        if(*(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0<(pixelMax+pixelMin)/2)
                        {

                            v_X1 += c1;
                            v_Y1 = k_v*v_X1+B1;
                            cout<<"--newv_X="<<v_X1<<endl;
                            cout<<"--newv_Y="<<v_Y1<<endl;
                        }else{
                            cout<<"测试。。水平位置："<<*(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0<<endl;
                            break;
                        }


                    }
                }

                cout<<"newv_X="<<v_X1<<endl;
                cout<<"newv_Y="<<v_Y1<<endl;
//                v_X1 = vmidX1+upX;
//                v_Y1 = vmidY1-upY;
//                cvCircle(image, CvPoint(v_X1,v_Y1), 2, CvScalar(0,0,250),2,8,0);
//                cvLine(image, CvPoint(v_X1,v_Y1), CvPoint(10,k_horizontal*10+(v_Y1-k_horizontal*v_X1)),CvScalar(250,0,0),1,8,0);

                //水平4像素精调宽度
//                printf("水平中间像素-----%d\n", *(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1-1)+0);
//                printf("水平中间像素-----%d\n", *(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0);
                pixel4XY[0] = sqrt(pow(v_X1-vmidX1,2)+pow(v_Y1-vmidY1, 2))/2;
//                double a = sqrt(pow(v_X1-vmidX1,2)+pow(v_Y1-vmidY1, 2))/2/1.0;
//                printf("----------pixel4XY[0]=%f\n", pixel4XY[0]);
//                printf("----------v_X1=%d,  v_Y1=%d",v_X1,  v_Y1);
//                cvLine(image, CvPoint(vmidX1, vmidY1), CvPoint(v_X1, v_Y1), CvScalar(0,250,255),1,8,0);

//                printf("original=%d\n",*(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0);
//                printf("next+1=%d\n",*(uchar *)(gray->imageData+int(k_v*(vmidX1+1)+B1)*gray->widthStep+vmidX1+1)+0);
//                printf("next+2=%d\n",*(uchar *)(gray->imageData+int(k_v*(vmidX1+2)+B1)*gray->widthStep+vmidX1+2)+0);
//                printf("next+3=%d\n",*(uchar *)(gray->imageData+int(vmidY1-2*upY)*gray->widthStep+vmidX1+2*upX)+0);
//                cvCircle(image, CvPoint(vmidX1+2*upX,int(vmidY1-2*upY)), 2, CvScalar(0,0,250),2,8,0);
                printf("*************************测试水平加载一个白像素点完毕********************************\n");
                //垂直寻找4像素精准边缘
//                cout<<"垂直位置："<<pic[h_Y1*width +h_X1]+0<<endl;
                double B2 = h_Y1 - k_horizontal*h_X1;
                int c2;
                if(*(uchar *)(gray->imageData+h_Y1*gray->widthStep+h_X1)+0>pixelMax)
                {
                    c2 = 1;
                }else{
                    c2 = -1;
                }
                printf("c2=%d\n", c2);
                if(c2==1)
                {
                    for(int i=0; i<10; i++)
                    {
                        if(*(uchar *)(gray->imageData+h_Y1*gray->widthStep+h_X1)+0>pixelMax)
                        {
                            h_Y1 += c2;
                            h_X1 = (h_Y1-B2)/k_horizontal;
                        }else{
//                            h_Y1 -= c2;
//                            h_X1 = (h_Y1-B2)/k_horizontal;
                            printf("水平边缘点像素=%d\n", *(uchar *)(gray->imageData+h_Y1*gray->widthStep+h_X1)+0);
//                            cvCircle(image, CvPoint(h_X1,h_Y1), 2, CvScalar(0,250,250),2,8,0);


                            break;
                        }

                    }
                }else{
                    for(int i=0; i<10; i++)
                    {
                        if(*(uchar *)(gray->imageData+h_Y1*gray->widthStep+h_X1)+0<(pixelMax+pixelMin)/2)
                        {
                            h_Y1 += c2;
                            h_X1 = (h_Y1-B2)/k_horizontal;
                        }else{
//                            h_Y1 -= c2;
                            break;
                        }


                    }
                }

//                cout<<"new h_X="<<h_X1<<endl;
//                cout<<"new h_Y="<<h_Y1<<endl;
//                cvCircle(image, CvPoint(h_X1,h_Y1), 2, CvScalar(0,250,0),2,8,0);

                //竖直4像素精调高度
                pixel4XY[1] = sqrt(pow(h_X1-hmidX1,2)+pow(h_Y1-hmidY1, 2))/2;
                printf("----------pixel4XY[1]=%f\n", pixel4XY[1]);
//                cvLine(image, CvPoint(hmidX1, hmidY1), CvPoint(h_X1, h_Y1), CvScalar(0,0,255),1,8,0);
//                cvLine(image, CvPoint(h_X1,h_Y1), CvPoint(1000,k_v*1000+(h_Y1-k_v*h_X1)),CvScalar(250,0,0),1,8,0);

                double pixelX = 0;
                double pixelY = 0;

                printf("k_horizontal=%f, k_v=%f\n", k_horizontal, k_v);

//                for(int i=0; i<8; i++)
//                {
//                    pixelY = i*pixel4XY[1];
//                    //垂直四像素粗略精确的X投影
//                    int leftX = int(sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1)));
//                    //垂直四像素粗略精确的Y投影
//                    int leftY = int(sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal));

//                    pixelX = i*pixel4XY[0];
//                    //水平四像素精确宽度的X投影
//                    int upX = int(sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)));
//                    printf("-------------upX=%d\n", upX);
//                    printf("-------------upX=%f\n", sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)/1.0));
//                    //水平四像素精确宽度的Y投影
//                    int upY = int(sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1))*abs(k_v));

//                    if(k_horizontal>0)
//                    {
//                        printf("大于零\n");
//                        int h_X, h_Y, v_X, v_Y;
//                        h_X= X[1] +leftX;
//                        h_Y = Y[1] + leftY;
////                        cvCircle(image, CvPoint(h_X,h_Y), 3, CvScalar(0,0,250),1,8,0);
//                        cvLine(image, CvPoint(h_X,h_Y), CvPoint(2000,k_v*2000+(h_Y-k_v*h_X)),CvScalar(0,0,250),1,8,0);

//                        v_X = upX+X[1];
//                        v_Y = Y[1] - upY;
////                        cvCircle(image, CvPoint(v_X,v_Y), 3, CvScalar(0,0,250),1,8,0);
//                        cvLine(image, CvPoint(v_X,v_Y), CvPoint(1000,k_horizontal*1000+(v_Y-k_horizontal*v_X)),CvScalar(0,0,250),1,8,0);

//                    }
//                    else if(k_horizontal<0)
//                    {
//                        printf("小于零\n");
//                        int h_X2, h_Y2, v_X2, v_Y2;
////                        h_X2= X[1] -leftX;
////                        h_Y2 = Y[1] + leftY;
//                        h_X2= hmidX1 -leftX;
//                        h_Y2 =hmidY1+ leftY;
////                        cvCircle(image, CvPoint(h_X,h_Y), 3, CvScalar(0,0,250),1,8,0);
//                        cvLine(image, CvPoint(h_X2,h_Y2), CvPoint(2000,k_v*2000+(h_Y2-k_v*h_X2)),CvScalar(0,0,250),1,8,0);

////                        v_X2 = upX+X[1];
////                        v_Y2 = Y[1] + upY;
//                        v_X2 = upX+vmidX1;
//                        v_Y2 = upY + vmidY1;
//                        printf("v_X=%d, v_Y=%d\n", v_X2, v_Y2);
////                        cvCircle(image, CvPoint(v_X2,v_Y2), 3, CvScalar(0,0,250),1,8,0);
//                        cvLine(image, CvPoint(v_X2,v_Y2), CvPoint(1,k_horizontal*1+(v_Y2-k_horizontal*v_X2)),CvScalar(0,0,250),1,8,0);

//                    }


//                }


                if(1)
                {

                    for(int i=1; i<7;i++)
                    {
                        pixelY = pixel4XY[1];//竖直
    //                    cout<<"1pixelY="<<pixelY<<endl;
                        pixelY = (i-1+0.5)*pixelY;
    //                    cout<<"2pixelY="<<pixelY<<endl;

                        //垂直四像素粗略精确的X投影
                        double leftX = sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1));
                        //垂直四像素粗略精确的Y投影
                        double leftY = sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal);

                        for(int j=1; j<9; j++)
                        {
                            pixelX = pixel4XY[0];//水平
    //                        cout<<"1pixelX="<<pixelX<<endl;
                            pixelX = (j-1+0.5)*pixelX;
    //                        cout<<"2pixelX="<<pixelX<<endl;

                            //水平四像素精确宽度的X投影
                            double upX = sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)/1.0);
                            //水平四像素精确宽度的Y投影
                            double upY = sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)/1.0)*abs(k_v);

                            if(k_horizontal>0)
                            {

                                double v_X11 = upX+X[1];
                                double v_Y11 = Y[1] - upY;
    //                            cvCircle(image, CvPoint(v_X11,v_Y11), 3, CvScalar(250,0,0),3,8,0);
                                //计算垂直B
                                double B_h = v_Y11 - k_horizontal*v_X11;
    //                            cvLine(image,CvPoint(v_X1,v_Y1),CvPoint(1,k_horizontal*1+B_h), CvScalar(0,250,0),1,8,0);

                                double h_X11= leftX+X[1];
                                double h_Y11 = Y[1] + leftY;

                                double B_v = h_Y11 -k_v*h_X11;
    //                            cvLine(image,CvPoint(h_X11,h_Y11),CvPoint(1000,k_v*1000+B_v), CvScalar(0,250,0),1,8,0);

                                int midx = int((B_v-B_h)/(k_horizontal-k_v)/1.0);
                                int midy = int(midx*k_v/1.0+B_v);
//                                printf("像素=%d\t",*(uchar *)(gray->imageData+midy*gray->widthStep+midx)+0);
    //                            int re = compare_pixel(pic, midx, midy);
    //                            cout<<"result="<<re<<endl;
                                if(*(uchar *)(gray->imageData+midy*gray->widthStep+midx)+0>(pixelMin+pixelMax)/2)
                                {
                                    printf("pixel=0\t");
                                }
                                else{
                                    printf("pixel=1\t");
                                }

    //                            cvCircle(image, CvPoint(h_X1,h_Y1), 3, CvScalar(0,0,250),3,8,0);
                                cvCircle(image, CvPoint(midx,midy), 1, CvScalar(0,0,250),3,8,0);


                            }else{
//                                double v_X11 = upX+X[1];
//                                double v_Y11 = Y[1] + upY;
                                double v_X11 = upX+vmidX1;
                                double v_Y11 = vmidY1 + upY;
    //                            cvCircle(image, CvPoint(v_X1,v_Y1), 3, CvScalar(250,0,0),3,8,0);
                                //计算垂直B
                                double B_h = v_Y11 - k_horizontal*v_X11;
    //                            cvLine(image,CvPoint(v_X1,v_Y1),CvPoint(1,k_horizontal*1+B_h), CvScalar(0,250,0),1,8,0);

                                double h_X11= hmidX1-leftX;
                                double h_Y11 = hmidY1 + leftY;
                                double B_v = h_Y11 -k_v*h_X11;
    //                            cvLine(image,CvPoint(h_X1,h_Y1),CvPoint(1000,k_v*1000+B_v), CvScalar(0,250,0),1,8,0);

                                int midx = int((B_v-B_h)/(k_horizontal-k_v)/1.0);
                                int midy = int(midx*k_v/1.0+B_v);

    //                            cvCircle(image, CvPoint(h_X1,h_Y1), 3, CvScalar(0,0,250),3,8,0);
                                cvCircle(image, CvPoint(midx,midy), 1, CvScalar(0,0,250),3,8,0);
//                                printf("像素=%d\t",*(uchar *)(gray->imageData+midy*gray->widthStep+midx)+0);
    //                            int re = compare_pixel(pic, midx, midy);

    ////                            cout<<"result="<<re<<endl;

                                if(*(uchar *)(gray->imageData+midy*gray->widthStep+midx)+0>(pixelMin+pixelMax)/2)
                                {
                                    printf("pixel=0\t");
                                }
                                else{
                                    printf("pixel=1\t");
                                }

                            }

                        }
                        printf("--\n");
                    }
                }

                return re;


            }


            break;
        }




    }
    cout<<"解码失败！"<<endl;
    return 0;

}

int main()
{
    int finder_scan(IplImage* image, IplImage* gray);
//    void binaryzation(IplImage* image,int *y, int * Xposition, int *Xwidth , int *num);
    int test_vertical(IplImage* image,IplImage * gray, int position, int sum, int* ax,  double slope, int* a, int * ROW, int*ay, int *ay_width);//
    void slopeCross_point(IplImage *image,IplImage *gray, int y,double k_horizontal, double b, double k_v, double b_v,  int *ax, int *pd, int ROW,int *ay, int *ay_width,int  ROW2, int *ay2, int *ay_width2, int* X , int * Y);
    void four_point_transform( Mat* src, Mat *dst, int *X, int *Y);// Mat *dst,
//    Mat src = imread("1-20.jpg");
    cout<<"\n-------start------\n"<<endl;
    //读取图像
    IplImage* image = cvLoadImage("image_c3/c29.jpg");//image/c900
//    Mat src = cvarrToMat(image);
    if(!image)
    {
        cout<<"can not load image\n"<<endl;
    }
    //转换成灰度图像
    IplImage* gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
    cvCvtColor(image, gray, CV_BGR2GRAY);
    gettimeofday(&tpstart,NULL);
    cout<<"gray.width="<<gray->width<<";"<<"gray.height="<<gray->height<<endl;

    int result = finder_scan(image, gray);


    //计算代码运行花费时间
    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
    cout<<"\n用时="<<timeuse<<"s"<<endl;
    cout<<"\n-------end------\n"<<endl;
    cvNamedWindow( "code", CV_WINDOW_AUTOSIZE );
    cvShowImage( "code", image );
//    cvShowImage("gray", gray);

    waitKey();//等待按键
    return 0;
}

