/*
 * Copyright (C) 2020 Ingenic Semiconductor Co., Ltd.
 *
 * Ingenic Media Development Kit(IMDK)
 *
 *
 */

#ifndef _LIBMEDIA_ISP_H_
#define _LIBMEDIA_ISP_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <libisp/camera_pixel_format.h>

/* 裁剪 */
struct channel_crop {
    int enable;
    unsigned int top;   /* 起始坐标 */
    unsigned int left;
    unsigned int width;
    unsigned int height;
};

/* 缩放 */
struct channel_scaler {
    int enable;
    unsigned int width;
    unsigned int height;
};

struct frame_image_format {
    unsigned int width;             /* ISP MScaler 输出分宽 */
    unsigned int height;            /* ISP MScaler 输出分高 */
    int pixel_format;               /* ISP MScaler 输出格式 */
    unsigned int frame_size;        /* ISP MScaler 帧大小 */

    struct channel_scaler scaler;   /* ISP MScaler 缩放属性 */
    struct channel_crop crop;       /* ISP MScaler 裁剪属性 */

    int frame_nums;                 /* ISP MScaler 缓存个数 */
};

struct sensor_dbg_register {
    unsigned long long reg;
    unsigned long long val;
    unsigned int size;      /* val size, unit:byte */
};


struct camera_info {
    char name[16];

    /* 每行像素数 */
    unsigned int width;

    /* 行数 */
    unsigned int height;

    /* camera 帧率 */
    unsigned int fps;

    /* camera 帧数据格式 */
    unsigned int data_fmt;

    /* 一行的长度,单位字节
     * 对于 nv12,nv21, 表示y数据一行的长度
     * 另外由此可以算出uv数据偏移 line_length*height
     */
    unsigned int line_length;

    /* 一帧数据经过对齐之后的大小 */
    unsigned int frame_size;

    /* 帧缓冲总数 */
    unsigned int frame_nums;

    /* 帧缓冲的物理基地址 */
    unsigned long phys_mem;

    /* mmap 后的帧缓冲基地址 */
    void *mapped_mem;
    
    /*帧对齐大小 */
    unsigned int align_size;
};

/**
 * @brief 获得camera设备句柄
 * @param device_path 设备节点
 * @return 成功返回设备句柄,失败返回-1
 */
int isp_open(const char* device_path);

/**
 * @brief 获得输出camera信息,映射地址等
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param info 用于存放camera的信息,不能为NULL
 * @return 成功返回0,失败返回负数
 */
int isp_mmap(int fd, struct camera_info *info);

/**
 * @brief 关闭camera设备句柄
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param info camera的信息,由isp_open()函数初始化
 * @return 成功返回0,失败返回-1
 */
int isp_close(int fd, struct camera_info *info);

/**
 * @brief 使能camera设备,上电
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_power_on(int fd);

/**
 * @brief 关闭camera设备,掉电
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_power_off(int fd);

/**
 * @brief 开始camera图像录制
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_stream_on(int fd);

/**
 * @brief 结束camera图像录制
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_stream_off(int fd);

/**
 * @brief 获取一帧录制的图像数据(未获取有效数据继续等待3s)
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回图像缓冲区指针,失败返回NULL
 */
void *isp_wait_frame(int fd);

/**
 * @brief 获取一帧录制的图像数据(不等待)
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回图像缓冲区指针,失败返回NULL
 */
void *isp_get_frame(int fd);

/**
 * @brief 释放一帧图像缓冲区
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param mem 缓冲区指针,由isp_wait_frame()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_put_frame(int fd, void *mem);

/**
 * @brief 获取已录制的图像数据帧数
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回帧数,失败返回负数
 */
int isp_get_avaliable_frame_count(int fd);

/**
 * @brief 丢弃已录制的图像数据帧
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param frames 需要丢弃的帧数
 * @return 成功返回帧数,失败返回负数
 */
int isp_drop_frames(int fd, unsigned int frames);

/**
 * @brief 在sensor帧率的基础上降低mscaler帧率
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param rate 需要降低的百分比(100 90 80)
 * @return 成功返回0,失败返回负数
 */
int isp_set_frame_rate(int fd, unsigned char rate);

/**
 * @brief 获得输出camera信息,映射地址等
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param info 用于存放camera的信息,不能为NULL
 * @return 成功返回0,失败返回负数
 */
int isp_get_info(int fd, struct camera_info *info);

/**
 * @brief 获得输出camera未经过ISP的原始sensor信息等
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param info 用于存放camera的信息,不能为NULL
 * @return 成功返回0,失败返回负数
 */
int isp_get_sensor_info(int fd, struct camera_info *info);

/**
 * @brief 获得ISP scaler的最大尺寸
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param width 用于存放ISP的宽
 * @param height 用于存放ISP的高
 * @return 成功返回0,失败返回负数
 */
int isp_get_max_scaler_size(int fd, int *width, int *height);

/**
 * @brief 获得ISP 行对齐大小,单位是像素
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param align_size 用于存放ISP的行对齐大小
 * @return 成功返回0,失败返回负数
 */
int isp_get_line_align_size(int fd, int *align_size);

/**
 * @brief 设置输出信息,分辨率,buffer个数等
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param fmt 用于传递输出信息,不能为NULL
 * @return 成功返回0,失败返回负数
 */
int isp_set_format(int fd, struct frame_image_format *fmt);

/**
 * @brief 获取输出信息,frame_size(未PageSize对齐)等
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param fmt 用于存放调整后的输出的信息,不能为NULL
 * @return 成功返回0,失败返回负数
 */
int isp_get_format(int fd, struct frame_image_format *fmt);

/**
 * @brief 申请帧buffer
 * @param fd camera设备句柄,由isp_open()函数获得
 * @param fmt 用于存放输出的信息,不能为NULL
 * @return 成功返回0,失败返回负数
 */
int isp_requset_buffer(int fd, struct frame_image_format *fmt);

/**
 * @brief 释放帧buffer
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_free_buffer(int fd);

/**
 * @brief 获取sensor寄存器
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_get_sensor_reg(int fd, struct sensor_dbg_register *reg);

/**
 * @brief 设置sensor寄存器
 * @param fd camera设备句柄,由isp_open()函数获得
 * @return 成功返回0,失败返回负数
 */
int isp_set_sensor_reg(int fd, struct sensor_dbg_register *reg);



#ifdef  __cplusplus
}
#endif

#endif /* _LIBMEDIA_ISP_H_ */
