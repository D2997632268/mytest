#include "test_include.h"
//#include "math.h"
struct timeval tpstart,tpend;
float timeuse;


int width = 640;
int height = 480;

int decode_result[49];
int pixelMax=0, pixelMin=255;
//int width = 1920;
//int height = 1080;

/***************junlin*******************/
struct Decode_Struct_Date decode_struct_date;
/***************junlin*******************/

extern void binaryzation(int *y, int * X_position, int *X_width , int *num);

int test_vertical(unsigned char* pic,int position, int sum, int* ax,  double slope, int *a,int * ROW, int*ay, int *ay_width, int limit, int limit_width_3);//
int slopeCross_point( unsigned char* pic,int y,double k_horizontal, double b,
                      double k_v, double b_v,  int *ax, int *pd, int ROW, int *ay, int *ay_width,int  ROW2,
                      int *ay2, int *ay_width2, int* X, int * Y, int pixelMax, int pixelMin);
int plotPoint(unsigned char* pic ,int pixel4XYX, int pixel4XYY, double k_horizontal, double k_v, int vmidX1, int vmidY1, int hmidX1, int hmidY1, int *X, int *Y);
int accurateSearchPixel4XY(unsigned char* pic ,double k_horizontal, double k_v, int sumx, int sumyy, int *X, int *Y);
int flood_fill_seed(unsigned char* pic, int x, int y, int *nextX, int *nextY, int *book);
int compare_pixel(unsigned char* pic, int x, int y);
int finder_scan(unsigned char* pic);



//-----------------------寻找垂直1:1:2：2：1-----------------//
int test_vertical(unsigned char* pic,int position, int sum, int* ax,  double slope, int *a,int * ROW, int*ay, int *ay_width, int limit, int limit_width_3)//
{
    //    cout<<"---test_vertical-----"<<endl;
    int check[5] = {1, 1, 2, 2, 1};
    int start_x;
    int data[1600];
    int pd[5];
    int Ay[5];
    int ok=0;
    if(slope>0){
        double w = (sqrt(pow((sum),2)/(pow(slope,2)+1)/1.0)/7)*abs(slope/1.0)*34;
        double wucha = (sqrt(pow((sum),2)/(pow(slope,2)+1)/1.0)/7);
        double pos = sqrt(pow(w,2)/(pow(slope,2)+1)/1.0)-wucha;
        start_x = (int)(position+pos)+3;
    }
    else if (slope==0 ){
        start_x = a[0];
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
        if(slope == 0 )
        {
            i = start_x + a[1]*j;
        }
        else{
            i = start_x+j*2;
        }

        for(int jd=0; jd<height;jd++)
        {
            data[jd] =pic[jd*width+i]+0;
            //            cout<<data[j]<<endl;
        }

        int y_col[200];
        int Ywidth[200];
        int LEN_y=0;
        binaryzation(/*image,*/ data, y_col, Ywidth, &LEN_y);
        for(int jh=0; jh<LEN_y-4;jh++)
        {
            for(int iw=0; iw<5; iw++)
            {
                pd[iw] = Ywidth[jh+iw];
                Ay[iw] = y_col[jh+iw];
            }

            int avg = (pd[0]+pd[1]+pd[4])/3;
            int err = avg*1/2;
            ok = 1;
            if(slope == 0)
            {
                //                printf("----------%d, %d\n",Ay[4]-limit, sum );
                //                printf("----------%d, %d, %d\n",pd[3], limit_width_3 , sum/7);
                if(abs(Ay[4]-limit)>sum*2/3 || abs(pd[3]-limit_width_3)>sum/7){
                    ok=0;
                }
            }else{
                if((Ay[4]-limit)>sum*3.5 && (Ay[4]-limit)<0)
                {
                    ok=0;
                }
            }
            for(int jp=0; jp<5; jp++)
            {
                if(pd[jp]<check[jp] * avg - err || pd[jp] > check[jp] * avg + err + 1)
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
            for(int ir=0; ir<5; ir++)
            {
                *(ay+ir) = Ay[ir];
                *(ay_width+ir) = pd[ir];
            }
            return 1;
        }

    }
    return 0;


}

int slopeCross_point( unsigned char* pic,int y,double k_horizontal, double b,
                      double k_v, double b_v,  int *ax, int *pd, int ROW, int *ay, int *ay_width,int  ROW2,
                      int *ay2, int *ay_width2, int* X, int * Y, int pixelMax, int pixelMin)
{
    int sumx=0;
    int sumy=0;
    double bb;
    double bb_v;
    bool button1, button2, button3, button4;
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
            for(int i=1; i<10; i++)
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

        X[4] = X[2];
        Y[4] = Y[3];
        button1 = X[3]<X[2]?1:0;
        button2 = X[1]<X[4]?1:0;
        button3 = Y[1]<=Y[3]?1:0;
        button4 = Y[2]<=Y[4]?1:0;
        //if(button1 == button2 == button3 == button4 ==1 )
        if(button1 == 1 &&button2 ==1 && button3 ==1&& button4 ==1 ) //junlin change
        {
            return 1;
        } else{
            return 0;
        }


    }else{
        X[0] = (int)((b_v-b)/(k_horizontal-k_v));
        Y[0] = (int)(k_v*X[0]+b_v);
        bb = y-k_horizontal*(ax[4]+pd[4]);
        bb_v = (ay2[4]+ay_width2[4])-k_v*ROW2;

        X[1] = (int)((bb_v-bb)/(k_horizontal-k_v));
        Y[1] = (int)(k_v*X[1]+bb_v);
        if(pic[Y[1]*width+X[1]]+0>pixelMax)
        {
            for(int i=1; i<10; i++)
            {
                if(pic[Y[1]*width+(X[1]+i)]+0<pixelMax){
                    X[1] += i;
                    break;
                }
                if(pic[(Y[1]+i)*width+X[1]]+0<pixelMax){
                    Y[1] += i;
                    break;
                }
                if(pic[(Y[1]+i)*width+X[1]+i]+0<pixelMax){
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

        double w = (sqrt(pow(sumx,2)/(pow(k_horizontal,2)+1))/7)*abs(k_horizontal)*35;
        int width = sqrt(pow(w, 2)/((pow(k_horizontal, 2)+1)*1.0))*abs(k_horizontal);
        X[2] = X[1]+width;
        Y[2] = (int)(k_v*X[2]+bb_v);

        double ww = (sqrt(pow(sumy, 2)/(pow(k_v, 2)+1))/7)*26;
        int high = sqrt(pow(ww, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal);

        Y[3] = Y[1]+high;
        X[3] = (int)((Y[3]-bb)/k_horizontal);

        double b_v4 = Y[3] - k_v*X[3];
        double b4 = Y[2] - k_horizontal*X[2];
        X[4] = (int)((b_v4-b4)/(k_horizontal-k_v));
        Y[4] = (int)(k_v*X[4]+b_v4);

        button1 = X[3]<X[2]?1:0;
        button2 = X[1]<X[4]?1:0;
        button3 = Y[1]<=Y[3]?1:0;
        button4 = Y[2]<=Y[4]?1:0;
        //if(button1 == button2 == button3 == button4 ==1 )
        if(button1 == 1 &&button2 ==1 && button3 ==1&& button4 ==1 ) //junlin change
        {
            return 1;
        } else{
            return 0;
        }


    }
}

int pos[2];
int key=1;
int flood_fill_seed(unsigned char* pic, int x, int y, int *nextX, int *nextY, int *book)
{
    int tx;
    int ty;
    //    printf("原像素=%d\n", pic[y*width+x]);

    for(int i=0; i<3; i++)
    {
        tx = x + nextX[i];
        ty = y + nextY[i];
        //        printf("next-x=%d, next-y=%d\n", nextX[i], nextY[i]);
        //        printf("x=%d, y=%d\n", x, y);
        //        printf("tx=%d, ty=%d\n", tx, ty);
        if(tx <1 || tx>width || ty<1 || ty>height)
        {
            continue;
        }
        //        printf("像素=%d\n", pic[ty*width+tx]);
        if(book[ty*width+tx] == 0 && ((pixelMax+pixelMin)/2 >= pic[ty*width+tx]+0))
        {
            book[ty*width+tx] = 1;
            flood_fill_seed(pic, tx,  ty, nextX, nextY, book);
        }
        //        printf("---------key=%d\n", key);
        if(key==0)
        {
            return 1;
        }
        if(i==2 && book[ty*width+tx] == 0)
        {
            pos[0] = x;
            pos[1] = y;
            key = 0;
            //            printf("-----------pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
            return 1;
        }
    }
    return 0;
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
int plotPoint(unsigned char* pic ,int pixel4XYX, int pixel4XYY, double k_horizontal, double k_v, int vmidX1, int vmidY1, int hmidX1, int hmidY1, int *X, int *Y)
{

    double pixelX = 0;
    double pixelY = 0;
    int leftX;
    int leftY;
    int upX;
    int upY;
    int num=0;
    for(int i=1; i<7;i++)
    {
        num = (i-1)*8;
        pixelY = pixel4XYY;//竖直
        pixelY = (i-1+0.5)*pixelY;

        if(k_v==0)
        {
            //垂直四像素粗略宽度的X投影
            leftX = 0;
            //垂直四像素粗略宽度的Y投影
            leftY = pixelY;
        }else{
            //垂直四像素粗略精确的X投影
            leftX = sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1));
            //垂直四像素粗略精确的Y投影
            leftY = sqrt(pow(pixelY, 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal);
        }

        for(int j=1; j<9; j++)
        {

            num += 1;
            pixelX = pixel4XYX;//水平
            pixelX = (j-1+0.5)*pixelX;
            if(k_horizontal==0)
            {
                //水平四像素粗略宽度的X投影
                upX = pixelX;
                //水平四像素粗略宽度的Y投影
                upY = 0;
            }else{
                //水平四像素精确宽度的X投影
                upX = sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)/1.0);
                //水平四像素精确宽度的Y投影
                upY = sqrt(pow(pixelX, 2)/(pow(k_v, 2)+1)/1.0)*abs(k_v);
            }


            if(k_horizontal>=0)
            {

                double v_X11 = upX+X[1];
                double v_Y11 = Y[1] - upY;
                //计算垂直B
                double B_h = v_Y11 - k_horizontal*v_X11;
                double h_X11= leftX+X[1];
                double h_Y11 = Y[1] + leftY;
                double B_v = h_Y11 -k_v*h_X11;
                int midx ;
                int midy ;
                if(k_horizontal==0 && k_v==0)
                {
                    midx = abs(v_X11);
                }else{
                    midx = (int)((B_v-B_h)/(k_horizontal-k_v)/1.0);
                }
                if(k_horizontal==0 && k_v==0)
                {
                    midy = abs(h_Y11);
                }else
                {
                    midy = (int)(midx*k_v/1.0+B_v);
                }


                //printf("像素=%d\t",*(uchar *)(gray->imageData+midy*gray->widthStep+midx)+0);
                int re = compare_pixel(pic, midx, midy);
                if(re>0)
                {
                    if(pic[midy*width+midx]+0>(pixelMin+pixelMax)/2)
                    {
                        //                                    printf("%d/n", num);
                        decode_result[num] = 0;
                        printf("pixel=0\t");
                    }
                    else{
                        decode_result[num] = 1;
                        printf("pixel=1\t");
                    }

                }else{
                    return 0;
                }


            }else{
                double v_X11 = upX+vmidX1;
                double v_Y11 = vmidY1 + upY;
                //计算垂直B
                double B_h = v_Y11 - k_horizontal*v_X11;
                double h_X11= hmidX1-leftX;
                double h_Y11 = hmidY1 + leftY;
                double B_v = h_Y11 -k_v*h_X11;
                int midx ;
                int midy ;
                if(k_horizontal==0 && k_v==0)
                {
                    midx = abs(v_X11);
                }else{
                    midx = (int)((B_v-B_h)/(k_horizontal-k_v)/1.0);
                }
                if(k_horizontal==0 && k_v==0)
                {
                    midy = abs(h_Y11);
                }else
                {
                    midy = (int)(midx*k_v/1.0+B_v);
                }

                //              printf("像素=%d\t",*(uchar *)(gray->imageData+midy*gray->widthStep+midx)+0);
                int re = compare_pixel(pic, midx, midy);
                if(re>0)
                {
                    if(pic[midy*width+midx]+0>(pixelMin+pixelMax)/2)
                    {
                        decode_result[num] = 0;
                        printf("pixel=0\t");
                    }
                    else{
                        decode_result[num] = 1;
                        printf("pixel=1\t");
                    }
                }


            }
        }

        printf("--\n");
    }
    //                    printf("=%d-\n",decode_result[0]);
    //    printf("%d, %d, %d, %d, %d, %d\n", decode_result[43], decode_result[44], decode_result[45], decode_result[46], decode_result[47], decode_result[48]);
    int t=0;
    for(int k=1 ; k<41; k++)
    {
        if(decode_result[k]==1)
        {
            t+=1;
        }
        //                        printf("code=%d\t", decode_result[k]);
    }
    //    printf("t=%d\n", t);
    if((t-4)%2==0 && (decode_result[42]==1))
    {
        int all_num=decode_result[48]*1+decode_result[47]*2+decode_result[46]*4+decode_result[45]*8+decode_result[44]*16;
        //        printf("all_num=%d\n", all_num);
        if(t-4==all_num)
        {
            printf("decode success!!\n");
            return 1;
        }else{
            printf("decode fail!!\n");
            return 0;
        }
    }else if((t-4)%2==1 && (decode_result[42]==0))
    {

        int all_num=decode_result[48]*1+decode_result[47]*2+decode_result[46]*4+decode_result[45]*8+decode_result[44]*16;
        //        printf("all_num=%d\n", all_num);
        if(t-4==all_num)
        {
            //            printf("decode success!!\n");
            return 1;
        }else{
            //            printf("decode fail!!\n");
            return 0;
        }
    }else{
        //        printf("decode fail!!\n");
        return 0;
    }


}


int accurateSearchPixel4XY(unsigned char* pic ,double k_horizontal, double k_v, int sumx, int sumyy, int *X, int *Y)
{
    int v_X1;
    int v_Y1;
    int h_X1;
    int h_Y1;
    int leftX;
    int leftY;
    int upX;
    int upY;


    double pixel4XY[2] = {0, 0};
    if(k_horizontal==0)
    {
        pixel4XY[0]=sumx/7*8;
        //水平四像素粗略宽度的X投影
        upX = pixel4XY[0];
        //水平四像素粗略宽度的Y投影
        upY = 0;
    }else{
        pixel4XY[0] = (sqrt(pow(sumx, 2)/(pow(k_horizontal,2)+1))/7)*abs(k_horizontal)*8;
        //水平四像素粗略宽度的X投影
        upX = (int)(sqrt(pow(pixel4XY[0], 2)/(pow(k_v, 2)+1)));
        //水平四像素粗略宽度的Y投影
        upY = (int)(sqrt(pow(pixel4XY[0], 2)/(pow(k_v, 2)+1))*abs(k_v));
    }
    if(k_v==0)
    {
        pixel4XY[1]=sumyy/7*8;
        //垂直四像素粗略宽度的X投影
        leftX = 0;
        //垂直四像素粗略宽度的Y投影
        leftY = pixel4XY[1];
    }else{
        pixel4XY[1] = (sqrt(pow(sumyy, 2)/(pow(k_v, 2)+1))/7)*8;
        //垂直四像素粗略宽度的X投影
        leftX = (int)(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1)));
        //垂直四像素粗略宽度的Y投影
        leftY = (int)(sqrt(pow(pixel4XY[1], 2)/(pow(k_horizontal, 2)+1))*abs(k_horizontal));
    }
    double midV_b = Y[1] - k_v*X[1];
    double midh_b = Y[1] - k_horizontal*X[1];

    //水平位置中点;
    int hmidX1 = X[1] + upX/4;
    int hmidY1 = k_v*hmidX1+midV_b;

    int BOOL=1;
    if(pic[hmidY1*width+hmidX1]+0<pixelMax && pic[(hmidY1-1)*width+hmidX1]+0<pixelMin)
    {
        //                    printf("从下往上找边缘");
        BOOL = 0;
        for(int i=0; i<10; i++)
        {
            if(pic[hmidY1*width+hmidX1]+0<pixelMax)
            {
                hmidY1 -= 1;
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

        if(pic[hmidY1*width+hmidX1]+0>pixelMin && pic[(hmidY1+1)*width+hmidX1]+0>pixelMin )
        {
            for(int i=0; i<10; i++)
            {
                if(pic[hmidY1*width+hmidX1]+0>pixelMin)
                {
                    hmidY1 += 1;
                }else{
                    break;
                }
            }
        }
    }



    //竖直位置中点
    int vmidY1 = Y[1] + leftY/4;
    int vmidX1;
    if(k_horizontal==0)
    {
        vmidX1 = X[1];
    }else{
        vmidX1 = (vmidY1-midh_b)/k_horizontal;
    }

    if(pic[vmidY1*width+vmidX1]+0<pixelMax)//当前点是黑色
    {
        if(pic[vmidY1*width+vmidX1]+0<pixelMax)
        {
            for(int i=0; i<10; i++)
            {
                if(pic[vmidY1*width+vmidX1]+0<pixelMin)
                {
                    vmidX1 -= 1;
                }else{
                    vmidX1 += 1;
                    break;
                }
            }
        }
    }
    else if(pic[vmidY1*width+vmidX1]+0>pixelMin)//当前点是白色
    {
        //                    printf("从左往右找边缘");
        for(int i=0; i<10; i++)
        {
            if(pic[vmidY1*width+vmidX1]+0>pixelMin)
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

    }else{
        v_X1 = vmidX1+upX;
        v_Y1 = vmidY1+upY;

        //垂直
        h_X1= hmidX1-leftX;
        h_Y1 = hmidY1 + leftY;
    }

    //水平位置寻找4像素单位边缘
    //  printf("*************************测试水平加载一个白像素点开始********************************\n");
    int c1;
    double B1 = v_Y1 - k_v*v_X1;
    if(pic[v_Y1*width+v_X1]+0<pixelMin)
    {
        c1 = -1;
    }else{
        c1 = 1;
    }
    //                printf("now 像素=%d\n", *(uchar *)(gray->imageData+(v_Y1-1)*gray->widthStep+v_X1)+0);
    //                printf("now 像素=%d\n", *(uchar *)(gray->imageData+Y[1]*gray->widthStep+X[1])+0);
    //                printf("now 像素=%d\n", *(uchar *)(gray->imageData+(v_Y1+1)*gray->widthStep+v_X1)+0);
    if(c1==1)
    {
        //                    printf("c1==1\n");
        for(int i=0; i<40; i++)
        {
            if(pic[v_Y1*width+v_X1]+0>(pixelMax+pixelMin)/2)
            {
                //                            printf("v_X1+1\n");
                v_X1 += c1;
                v_Y1 = k_v*v_X1+B1;
                //                            printf("if水平边缘点像素=%d\n", *(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0);
            }else{
                //                            printf("else水平边缘点像素=%d\n", *(uchar *)(gray->imageData+v_Y1*gray->widthStep+v_X1)+0);
                break;
            }

        }
    }else{
        //                    printf("c1==-1\n");
        for(int i=0; i<10; i++)
        {
            if(pic[v_Y1*width+v_X1]+0<(pixelMax+pixelMin)/2)
            {

                v_X1 += c1;
                v_Y1 = k_v*v_X1+B1;
            }else{
                break;
            }


        }
    }

    //水平4像素精调宽度
    //                printf("水平中间像素-----%d\n", *(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1-1)+0);
    //                printf("水平中间像素-----%d\n", *(uchar *)(gray->imageData+vmidY1*gray->widthStep+vmidX1)+0);
    pixel4XY[0] = sqrt(pow(v_X1-vmidX1,2)+pow(v_Y1-vmidY1, 2))/2;
    //    printf("----------pixel4XY[0]=%f\n", pixel4XY[0]);
    //                printf("----------v_X1=%d,  v_Y1=%d",v_X1,  v_Y1);
    //                printf("*************************测试水平加载一个白像素点完毕********************************\n");

    //垂直寻找4像素精准边缘
    double B2 = h_Y1 - k_horizontal*h_X1;
    int c2;
    if(pic[h_Y1*width+h_X1]+0>pixelMax)
    {
        c2 = 1;
    }else{
        c2 = -1;
    }
    //                printf("c2=%d\n", c2);
    if(c2==1)
    {
        for(int i=0; i<10; i++)
        {
            if(pic[h_Y1*width+h_X1]+0>pixelMax)
            {
                h_Y1 += c2;
                if(k_horizontal==0)
                {
                    h_X1 = h_X1;
                }else{
                    h_X1 = (h_Y1-B2)/k_horizontal;
                }

            }else{
                break;
            }

        }
    }else{
        for(int i=0; i<10; i++)
        {
            if(pic[h_Y1*width+h_X1]+0<(pixelMax+pixelMin)/2)
            {
                h_Y1 += c2;
                if(k_horizontal==0)
                {
                    h_X1 = h_X1;
                }else{
                    h_X1 = (h_Y1-B2)/k_horizontal;
                }
            }else{
                break;
            }


        }
    }

    //竖直4像素精调高度
    pixel4XY[1] = sqrt(pow(h_X1-hmidX1,2)+pow(h_Y1-hmidY1, 2))/2;
    //    printf("----------pixel4XY[1]=%f\n", pixel4XY[1]);
    int result = plotPoint(pic,pixel4XY[0], pixel4XY[1], k_horizontal,  k_v, vmidX1, vmidY1, hmidX1, hmidY1, X, Y);

    return result;

}


int finder_scan(unsigned char* pic)
{
    int N = 0;
    int dis = 0;
    int dis_i = 0;
    int sumx = 0;
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

    int book[width*height];
    memset(book, 0, sizeof(book));

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
        //        printf("N=%d\n", N);
        if(dis == 0)
        {
            //            printf("dis==0\n");
            y = height/2 + pow(-1, i) * 10*N;

            //            printf("--%f--%f\n", height/2, pow(-1, i) * 10*N);
        }
        else{
            //            printf("dis!=0\n");
            y = dis+dis_i*i;
        }
        //        printf("--------y=%d\n", y);
        if(y>0 && y<height)
        {
            //            printf("--------y=%d\n", y);
            for(int j=0; j<width;j++)
            {
                //                printf("--------j=%d\n", j);
                //                printf("--------index=%d\n", y*width+j);
                //                printf("--------len=%d\n", sizeof(pic)/sizeof(pic[0]));
                //                printf("--------pic=%d\n", pic[y*width+j]+0);
                data[j] = pic[y*width+j]+0;
                //                printf("%d\t", data[j]);


            }

        }


        int Xposition[200];
        int Xwidth[200];
        int LEN=1;

        binaryzation(data, Xposition, Xwidth, &LEN);
        //        printf("length=%d\n",LEN);
        //        for(int i=0; i<LEN; i++)
        //        {
        //            printf("%d ", Xwidth[i]);
        //        }
        //        printf("/n");

        //        return 0;
        for(int j=0; j<LEN-4; j++)
        {
            for(int k=0; k<5;k++){
                pd[k] = Xwidth[j+k];
                ax[k] = Xposition[j+k];
            }
            int avg = (pd[0]+pd[1]+pd[3]+pd[4])/4;
            int err = avg*1/2;
            ok = 1;
            for(int ip=0; ip<5; ip++)
            {
                if(pd[ip] < check[ip] * avg - err || pd[ip] > check[ip] * avg + err + 1)
                {
                    ok = 0;
                    break;
                }
            }
            if(ok==1)
            {
                //                printf("%d该条直线解码成功\n", y);
                int black[5] = {pic[y*width+ax[0]-1]+0, pic[y*width+ax[1]+1]+0,
                                pic[y*width+ax[2]-1]+0,pic[y*width+ax[3]+1]+0,
                                pic[y*width+ax[4]-1]+0};
                int white[5] = {pic[y*width+ax[0]+1]+0, pic[y*width+ax[1]-1]+0,
                                pic[y*width+ax[2]+1]+0, pic[y*width+ax[3]-1]+0,
                                pic[y*width+ax[4]+1]+0};
                //                printf("%d, %d, %d, %d,%d\n", pic[y*width+ax[0]+1]+0, pic[y*width+ax[1]-1]+0,
                //                       pic[y*width+ax[2]+1]+0, pic[y*width+ax[3]-1]+0,
                //                       pic[y*width+ax[4]+1]+0);
                int maxx, mmin;
                for(int im=0; im<5; im++)
                {
                    //                    printf("black1=%d, white1=%d\n", black[im], white[im]);
                    if(pixelMin>pixelMax){
                        mmin = white[im]<pixelMin?white[im]:pixelMin;
                        maxx = black[im]>pixelMax?black[im]:pixelMax;
                        if(mmin>maxx)
                        {
                            pixelMin=mmin;
                            pixelMax=maxx;
                        }else{
                            break;
                        }
                    }
                }

                //                printf("pixelMin=%d, pixelMax=%d\n", pixelMin,pixelMax);

                x_num += 1;
                if(x_num<2)
                {
                    //                    printf("**x_num<2**\n");
                    memcpy(pre_X, ax, sizeof(int)*5);
                    pre_y = y;
                    for(int is=0; is<5; is++)
                    {
                        sumx += pd[is];

                    }
                    int nextXd[3] = {0, 1, -1};
                    int nextYd[3] = {1, 1, 1};//向下找
                    //                    printf("ax[0]-ay_width[0]=%d, y=%d\n", ax[0]-pd[0], y);
                    int a = flood_fill_seed(pic, ax[1]+pd[2]/2, y, nextXd, nextYd, book);
                    //                    printf("a=%d\n", a);
                    if(a)
                    {
                        //                        printf("flood!!!");
                        //                       printf("pos像素=%d\n", *(uchar *)(gray->imageData+pos[1]*gray->widthStep+pos[0])+0);
                        double pw = sqrt(pow(sumx, 2)/(pow(1,2)+1));
                        double h = sqrt(pow(pw, 2)/(pow(1,2)+1));
                        int poss_down=pos[1]-h;
                        //                       printf("sum=%d, pw=%f, h=%f\n", sumx, pw, h);
                        //                       printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
                        dis = poss_down;
                        dis_i = -1;
                        //                       printf("down__dis=%d\n", dis);
                        //                       printf("down__dis-y=%d\n", dis-y);
                        if(dis-y<sumx)
                        {
                            key = 1;//打开key递归开关
                            memset(book, 0, sizeof(book));
                            int nextXu[4] = {0, 1, -1, 0};
                            int nextYu[4] = {-1, -1, -1, -2};//向上找
                            int a = flood_fill_seed(pic, ax[1]+pd[2]/2, y, nextXu, nextYu, book);
                            //                           printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
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
                            }

                        }

                    }

                    break;
                }else{
                    break;
                }

            }

        }
        if(x_num>=2)
        {
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
            int autoXY[2] = {0, 0};

            //            cout<<"垂直扫描第一条线"<<endl;
            va = test_vertical( pic, position, sumx, ax,  slope, autoXY, &ROW, ay, ay_width, y, pd[3]);
            if(va==0){
                return 0;
            }
            //            printf("ROW, ay[0]=%d, %d\n", ROW, ay[0]);
            //            printf("垂直扫描第一条线\n");
            int sumy=0;
            for(int i=0; i<5; i++)
            {
                sumy += ay_width[i];
            }

            int black1[5] = {pic[(ay[0]-1)*width+ROW]+0, pic[(ay[1]+1)*width+ROW]+0,
                             pic[(ay[2]-1)*width+ROW]+0,pic[(ay[3]+1)*width+ROW]+0,
                             pic[(ay[4]-1)*width+ROW]+0};
            int white1[5] = {pic[(ay[0]+1)*width+ROW]+0, pic[(ay[1]-1)*width+ROW]+0,
                             pic[(ay[2]+1)*width+ROW]+0, pic[(ay[3]-1)*width+ROW]+0,
                             pic[(ay[4]+1)*width+ROW]+0};
            int maxx, mmin;
            for(int im=0; im<5; im++)
            {
                if(pixelMin>pixelMax){
                    mmin = white1[im]<pixelMin?white1[im]:pixelMin;
                    maxx = black1[im]>pixelMax?black1[im]:pixelMax;
                    if(mmin>maxx)
                    {
                        pixelMin=mmin;
                        pixelMax=maxx;
                    }else{
                        break;
                    }
                }
            }
            //测试洪水遍历法找最右边的点开始
            int pos_x;
            int pos_xi;
            int action=1;
            key = 1;//打开key递归开关
            memset(book, 0, sizeof(book));
            int nextXu[4] = {1, 1, 1, 2};
            int nextYu[4] = {0, 1, -1, 0};//向右找
            int a = flood_fill_seed(pic, ROW, ay[1]+ay_width[2]/2, nextXu, nextYu, book);
            //            printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
            pos_x = pos[0]- sqrt(pow(sumy, 2)/2);
            pos_xi = -1;

            if(pos_x<ROW)
            {
                memset(book, 0, sizeof(book));
                int nextXu[4] = {2, 2, 2, 3};
                int nextYu[4] = {0, 1, -1, 0};//向右找
                int a = flood_fill_seed(pic, ROW, ay[1]+ay_width[2]/2, nextXu, nextYu, book);
                //                printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
                pos_x = pos[0]- sqrt(pow(sumy, 2)/2);
                pos_xi = -1;
            }
            //            printf("%f\n", k_horizontal);
            if(pos_x<ROW)
            {
                action=0;
                pos_x = ROW+1.5*sumx;
                pos_xi = 1;
            }
            if(k_horizontal>=0 && action)
            {
                //                printf("%d, %d, %d\n", pos_x, ROW, sumy);
                if(pos_x-ROW<sumy)
                {
                    key = 1;//打开key递归开关
                    memset(book, 0, sizeof(book));
                    int nextXu[3] = {-1, -1, -1};
                    int nextYu[3] = {0, 1, -1};//向左找
                    int a = flood_fill_seed(pic, ROW, ay[4]+ay_width[4]/2, nextXu, nextYu, book);
                    //                    printf("pos[0]=%d, pos[1]=%d\n", pos[0], pos[1]);
                    pos_x = pos[0] + sumx/7;
                    pos_xi = 1;
                }
            }
            autoXY[0] = pos_x;
            autoXY[1] = pos_xi;
            int n=1;
            va = test_vertical( pic, ROW, sumx, ax,  0, autoXY, &ROW2, ay2, ay_width2, ay[4], ay_width[3]);
            k_v = (ay[1]-ay2[1])/((ROW-ROW2)*1.0);
            while(abs(ay2[0]-ay[0])>2*sumx )
            {
                //                printf("进入垂直扫描第二次循环中！！\n");
                ROW2 += n;
                autoXY[0] = ROW2;
                autoXY[1] = pos_xi;
                va = test_vertical(pic, ROW2+n, sumx, ax,  0, autoXY, &ROW2, ay2, ay_width2, ay[4], ay_width[3]);
                //                printf("------k_hor*k_V-->%f*%f=%f\n", k_horizontal,k_v, k_horizontal*k_v);
                if(va==0)
                {
                    return 0;
                }

                n += 10;

                if(n>100)
                {
                    return 0;
                }
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

            int black[5] = {pic[(ay2[0]-1)*width+ROW2]+0, pic[(ay2[1]+1)*width+ROW2]+0,
                            pic[(ay2[2]-1)*width+ROW2]+0,pic[(ay2[3]+1)*width+ROW2]+0,
                            pic[(ay2[4]-1)*width+ROW2]+0};
            int white[5] = {pic[(ay2[0]+1)*width+ROW2]+0, pic[(ay2[1]-1)*width+ROW2]+0,
                            pic[(ay2[2]+1)*width+ROW2]+0, pic[(ay2[3]-1)*width+ROW2]+0,
                            pic[(ay2[4]+1)*width+ROW2]+0};
            for(int i=0; i<5; i++)
            {
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
            int sumyy=0;
            for(int iy=0; iy<5; iy++)
            {
                sumyy += ay_width2[iy];
            }

            b_v = ay2[0] - ay_width2[0] - k_v*ROW2;


            int re = slopeCross_point(pic, y, k_horizontal, b, k_v, b_v, ax, pd, ROW, ay, ay_width, ROW2, ay2, ay_width2, X, Y, pixelMax, pixelMin);

            printf("\nTop_Left_Corner=[%d, %d]\n", X[0], Y[0]);

            if(re)
            {
                int result = accurateSearchPixel4XY(pic, k_horizontal, k_v, sumx, sumyy, X, Y);
                return result;
            }

            break;
        }



    }

    /****************junlin***************/
    for(int update_count=0;update_count<5;update_count++)
    {
    decode_struct_date.X[update_count]=X[update_count];
    decode_struct_date.Y[update_count]=Y[update_count];
    }
    /***************junlin****************/
    return 0;
}

#ifdef dongxiu
int main(int argc, char *argv[])
#endif

  /***************junlin****************/
int decode_function(unsigned char* pic,int picture_hight,int picture_width)
{

    width = picture_width;
    height = picture_hight;
    memset(&decode_struct_date,0,sizeof (decode_struct_date));
   /***************junlin****************/

    printf("\n-------start------\n");
    gettimeofday(&tpstart,NULL);//计时开始
#ifdef dongxiu
    if(argv[1]!=NULL)
    {
        printf("send_File FALSE\r\n");
    }


    FILE *fp = fopen(argv[1], "rb++"); //pic/70.yuv   frame.nv12   nv12/6.nv12
    //    FILE *fp = fopen("text1.txt", "r"); //pic/70.yuv   frame.nv12
    if(fp==NULL) {printf("图像地址空"); return 0;}

    //    unsigned char* pic = (unsigned char*)malloc(width*height*3/2);
    //    fread(pic, 1, width*height*3/2, fp);

    unsigned char* pic = (unsigned char*)malloc(width*height);
    fread(pic, 1, width*height, fp);
#endif


    //    FILE *fp1;
    //    if((fp1=fopen("text.txt","wb"))==NULL)
    //    {
    //        printf("cant open the file!\n");
    //        exit(0);
    //    }
    //    int i;
    //    for(i=0;i<height;i++)
    //    {
    //        for(int j=0; j<width; j++)
    //        {
    //            fprintf(fp1, "%d\n", pic[i*640+j]+0);
    ////            fwrite((char *)gray->imageData,gray->width*gray->height,1,fp);
    //        }
    //    }

    //    for(int i=0;i<480;i++)
    //    {
    //        for(int j=0; j<640; j++)
    //        {
    //            fscanf(fp, "%d", &pic[i*640+j]);
    //        }
    //    }
    //    printf("pic=%d\t", pic[0]+0);

    int result = finder_scan(pic);
    if(result)
    {
        printf("\n**decode success**\n");
    }else{
        printf("\n**decode fail**\n");
    }

#ifdef dongxiu
    free(pic);


    fclose(fp);
#endif

    gettimeofday(&tpend,NULL);//计时结束，计算代码运行花费时间
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;

    printf("\ntime =%f",timeuse);
    printf("\n-------end------\n");


    /****************junlin***************/
    memcpy(decode_struct_date.decode_result,decode_result,sizeof (decode_result));
    return result;
    /****************junlin***************/
}

