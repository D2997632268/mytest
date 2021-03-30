/*
 * Copyright (C) 2020 Ingenic Semiconductor Co., Ltd.
 *
 * Ingenic Media Development Kit(IMDK)
 *
 */

#ifdef __cplusplus             //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <assert.h>
#include <signal.h>
#include <isp.h>
#include <isp_tuning.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "test_include.h" //decode

static int dev_fd;
ISPTuning *tuning = NULL;

static struct camera_info info;

static struct frame_image_format output_fmt = {
    .width              = 1280,
    .height             = 720,
    .pixel_format       = CAMERA_PIX_FMT_NV12,

    .scaler.enable      = 0,
    .scaler.width       = 1920,
    .scaler.height      = 1080,

    .crop.enable        = 0,
    .crop.top           = 0,
    .crop.left          = 0,
    .crop.width         = 1280,
    .crop.height        = 720,

    .frame_nums         = 2,
};

static void usage(const char *prg_name, int status)
{
    fprintf(stderr, "%s usage\n", prg_name);
    fprintf(stderr, "    -h/--help                        : show help info\n");
    fprintf(stderr, "    device_path                      : start frame\n");
    fprintf(stderr, "    Example: %s  /dev/mscaler0-ch0\n", prg_name);
    fprintf(stderr, "\n");

    exit(status);
}

static inline uint64_t boot_time_usecs(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_BOOTTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000 * 1000 + ts.tv_nsec / 1000;
}

/*
函数功能：通过操作像素对NV12格式的一帧图片进行剪切
*/
static int rkNV12_cut_nv12(unsigned char * srcImage, int srcW, int srcH,
                   unsigned char *destImage, int dstw, int dsth){

    int dst_size = dstw * dsth; // 新分辨率像素总数
    int src_size = srcW * srcH; // 旧分辨率像素总数
    int y_dst_size = dst_size;    // Y 分量总数 = 总数
    int i = 0; //表示新分辨率 像素 个数
    int j = 0; //表示旧分辨率 像素 个数
    int line_n = 0; // 表示此时的行数
    int exce_W = srcW - dstw; // 图片对于的像素宽度

    //剪切Y分量

    line_n = dsth ;

    for(i =0; i < line_n; i++ ){
        memcpy(&(destImage[i*dstw]), (srcImage + i*srcW ),dstw);
    }

    //剪切UV分量
    for(i =0; i < line_n/2; i++ ){
        memcpy(&(destImage[i*dstw + dst_size]), (srcImage + i*srcW + src_size),dstw);
    }
    return 0;
}

static int save_frame_to_file(void *mem, int size)
{
    int fd;
    char *file_name = "/tmp/frame.nv12";
    int ret = 0;

    unsigned char  * des_picture=malloc(640*480+640*240);
    rkNV12_cut_nv12(mem,1280,720,des_picture,640,480);

    fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK, 0644);
    if (fd < 0) {
        fprintf(stderr, "open file[%s] failed\n", file_name);
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    ret = write(fd, des_picture,640*480+640*240);
    if (ret < 0) {
        fprintf(stderr, "write to file(%s) failed ret=%d\n", file_name, ret);
        close(fd);
        return -1;
    }

    fsync(fd);
    close(fd);
    system("sync");


    fprintf(stderr, "Save Frame to File(%s) size=%d\n", file_name, 640*480+640*240);


    FILE * fp;
    char buffer[1024];
    fp=popen("/app-user/lsz frame.nv12 > /dev/ttyGS0 <  /dev/ttyGS0","r");

    while (fgets(buffer, 1024, fp) != NULL)
    {
        char *ret_get = strstr(buffer, "Transfer complete");
        if(ret_get!=NULL)
        {
            printf("result= %s\n",ret_get);
        }
        else
        {
            printf("buffer=%s\r\n",buffer);
        }
     }

    pclose(fp);
    system("rm -rf frame.nv12");
    free(des_picture);

    return 0;
}


//

void mount_lsz_function()
{
    FILE * fp;
    char buffer[1024];
    fp=popen("/app-user/serial_config.sh  start","r");
    fgets(buffer, 1024, fp);
    printf("buffer=%s\r\n",buffer);
    pclose(fp);

}

static void signal_handler(int signum)
{
    int fd = dev_fd;

    isp_power_off(fd);

    isp_free_buffer(fd);

    ISPTuning_Destroy(tuning);

    isp_close(fd, &info);

    dev_fd = -1;
    exit(-1);
}

int main(int argc, char *argv[])
{
    int ret = 0;

    mount_lsz_function();//
    /* 参数检查 */
    if (argc < 2)
        usage(argv[0], -1);

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        usage(argv[0], argc == 2 ? 0 : -1);

    signal(SIGINT, signal_handler);

    int fd = isp_open(argv[1]);
    if (fd == -ENODEV)
        return -ENODEV;

    dev_fd = fd;

    if (!output_fmt.scaler.enable && !output_fmt.crop.enable) {
        if (isp_get_sensor_info(fd, &info) == 0) {
            output_fmt.width = info.width;
            output_fmt.height = info.height;
        }
    }

    ret = isp_set_format(fd, &output_fmt);
    if (ret < 0) {
        fprintf(stderr, "isp set format failed\n");
        close(fd);
        return ret;
    }

    ret = isp_requset_buffer(fd, &output_fmt);
    if (ret < 0) {
        fprintf(stderr, "isp set request buffer failed\n");
        close(fd);
        return ret;
    }

    isp_get_info(fd, &info);

    ret = isp_mmap(fd, &info);
    if (ret < 0) {
        isp_free_buffer(fd);
        close(fd);
        return ret;
    }

    ret = isp_power_on(fd);
    if (ret)
        goto close_fd;

    ret = isp_stream_on(fd);
    if (ret)
        goto close_fd;

    char fmt_a = (char)(info.data_fmt >> 0);
    char fmt_b = (char)(info.data_fmt >> 8);
    char fmt_c = (char)(info.data_fmt >> 16);
    char fmt_d = (char)(info.data_fmt >> 24);
    fprintf(stderr, "name        : %s\n", info.name);
    fprintf(stderr, "width       : %d\n", info.width);
    fprintf(stderr, "height      : %d\n", info.height);
    fprintf(stderr, "fps         : %d\n", info.fps);
    fprintf(stderr, "data_fmt    : %C%C%C%C\n", fmt_a, fmt_b, fmt_c, fmt_d);
    fprintf(stderr, "line_length : %d\n", info.line_length);
    fprintf(stderr, "frame_size  : %d\n", info.frame_size);
    fprintf(stderr, "frame_nums  : %d\n", info.frame_nums);
    fprintf(stderr, "phys_mem    : %08lx\n", info.phys_mem);
    fprintf(stderr, "mapped_mem  : %p\n", info.mapped_mem);

    tuning = ISPTuning_Create("/dev/isp-tuning0");
    if (!tuning)
        fprintf(stderr, "isp tuning create fail\n");

    /* 获取frame_size(非对齐大小) */
    struct frame_image_format fmt;
    isp_get_format(fd, &fmt);

    int count = 0;
    while (1) {
        count++;
        void *mem = isp_wait_frame(fd);
        if (mem) {
            decode_function((unsigned char*)(mem),480,640);
            save_frame_to_file(mem, fmt.frame_size);
            isp_put_frame(fd, mem);

        }
        //sleep(5);
    }

    isp_drop_frames(fd, info.frame_nums);

    ISPTuning_Destroy(tuning);

    isp_stream_off(fd);

    isp_power_off(fd);

close_fd:
    isp_free_buffer(fd);
    isp_close(fd, &info);
    dev_fd = -1;

    return ret;
}

#ifdef __cplusplus
}

#endif
