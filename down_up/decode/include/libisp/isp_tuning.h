/*
 * isp tuning header file.
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 */

#ifndef __ISPTuning_H__
#define __ISPTuning_H__

#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * @file
 * ISP模块头文件
 */

/**
 * @defgroup IMP_ISP
 * @ingroup imp
 * @brief 图像信号处理单元。主要包含图像效果设置、模式切换以及Sensor的注册添加删除等操作
 *
 * ISP模块与数据流无关，不需要进行Bind，仅作用于效果参数设置及Sensor控制。
 *
 * ISP模块的使能步骤如下：
 * @{
 */

#define MAX_NAME_LEN                32                    /**< Name字符串最大长度 */

typedef enum {
    ISP_STATE_UNKNOW,
    ISP_STATE_CLOSE,
    ISP_STATE_OPEN,
    ISP_STATE_RUN,
} ISPRunState;

/**
 * ISP功能开关
 */
typedef enum {
    ISP_TUNING_OPS_MODE_DISABLE,            /**< 不使能该模块功能 */
    ISP_TUNING_OPS_MODE_ENABLE,            /**< 使能该模块功能 */
    ISP_TUNING_OPS_MODE_BUTT,            /**< 用于判断参数的有效性，参数大小必须小于这个值 */
} ISPTuningOpsMode;

/**
 * ISP功能选用开关
 */
typedef enum {
    ISP_TUNING_OPS_TYPE_AUTO,            /**< 该模块的操作为自动模式 */
    ISP_TUNING_OPS_TYPE_MANUAL,            /**< 该模块的操作为手动模式 */
    ISP_TUNING_OPS_TYPE_BUTT,            /**< 用于判断参数的有效性，参数大小必须小于这个值 */
} ISPTuningOpsType;

/**
 * ISP抗闪频属性参数结构体。
 */
typedef enum {
    ISP_ANTIFLICKER_DISABLE,    /**< 不使能ISP抗闪频功能 */
    ISP_ANTIFLICKER_50HZ,    /**< 使能ISP抗闪频功能, 并设置频率为50HZ */
    ISP_ANTIFLICKER_60HZ,    /**< 使能ISP抗闪频功能，并设置频率为60HZ */
    ISP_ANTIFLICKER_BUTT,    /**< 用于判断参数的有效性，参数大小必须小于这个值 */
} ISPAntiflickerAttr;
typedef struct {
    uint32_t totalGain;
    unsigned char contrast;
    unsigned char sharpness;
    unsigned char saturation;
    unsigned char brightness;

    unsigned int fpsNum;
    unsigned int fpsDen;

    ISPAntiflickerAttr antiflicker;
} ISPTuningData;

#define ISP_TUNING_DEAMON_FUNCS_NUM 10
typedef struct {
    char name[20];
    unsigned int en;
    int (*func)(void *arg);
    void *arg;
} ISPTuningDeamonFunc;
typedef struct {
    int deamonInitFlag;
    pthread_mutex_t deamonMutex;
    pthread_t tid;
    unsigned int threadEn;
    unsigned int funcsEn;
    ISPTuningDeamonFunc funcs[ISP_TUNING_DEAMON_FUNCS_NUM];
} ISPTuningDeamonInfo;


typedef struct {
    /* isp tuning */
    char dev[MAX_NAME_LEN];
    int fd;
    ISPRunState state;

    pthread_mutex_t tuningMutex;
    ISPTuningData tuningData;

    char debugPath[MAX_NAME_LEN];
    ISPTuningDeamonInfo deamonInfo;

    int memFd;
    void *mapAddr;
    uint32_t mapLen;

    void *pri;
} ISPTuning;


/**
 * @fn ISPTuning *ISPTuning_Create(const char *dev)
 *
 * 创建使能ISP效果调试功能
 *
 * @param[in] dev isp tuning设备名
 *
 * @retval 非NULL 成功，返回tuning数据结构
 * @retval NULL 失败
 *
 * @attention 在使用这个函数之前，必须保证ISP stream on。
 */
ISPTuning *ISPTuning_Create(const char *dev);

/**
 * @fn int ISPTuning_Destroy(ISPTuning *tuning)
 *
 * 销毁ISP效果调试功能
 *
 * @param[in] tuning 要释放的tuning
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 */
int ISPTuning_Destroy(ISPTuning *tuning);

/**
 * @fn int ISPTuning_SetSensorFPS(ISPTuning *tuning, uint32_t fps_num, uint32_t fps_den)
 *
 * 设置摄像头输出帧率
 *
 * @param[in] fps_num 设定帧率的分子参数
 * @param[in] fps_den 设定帧率的分母参数
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetSensorFPS(ISPTuning *tuning, uint32_t fps_num, uint32_t fps_den);

/**
 * @fn int ISPTuning_GetSensorFPS(ISPTuning *tuning, uint32_t *fps_num, uint32_t *fps_den)
 *
 * 获取摄像头输出帧率
 *
 * @param[in] fps_num 获取帧率分子参数的指针
 * @param[in] fps_den 获取帧率分母参数的指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetSensorFPS(ISPTuning *tuning, uint32_t *fps_num, uint32_t *fps_den);

/**
 * @fn int ISPTuning_SetAntiFlickerAttr(ISPTuning *tuning, ISPAntiflickerAttr attr)
 *
 * 设置ISP抗闪频属性
 *
 * @param[in] attr 设置参数值
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_SetAntiFlickerAttr(ISPTuning *tuning, ISPAntiflickerAttr attr);

/**
 * @fn int ISPTuning_GetAntiFlickerAttr(ISPTuning *tuning, ISPAntiflickerAttr *pattr)
 *
 * 获得ISP抗闪频属性
 *
 * @param[in] pattr 获取参数值指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_GetAntiFlickerAttr(ISPTuning *tuning, ISPAntiflickerAttr *pattr);

/**
 * @fn int ISPTuning_SetBrightness(ISPTuning *tuning, unsigned char bright)
 *
 * 设置ISP 综合效果图片亮度
 *
 * @param[in] bright 图片亮度参数
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加亮度，小于128降低亮度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_SetBrightness(ISPTuning *tuning, unsigned char bright);

/**
 * @fn int ISPTuning_GetBrightness(ISPTuning *tuning, unsigned char *pbright)
 *
 * 获取ISP 综合效果图片亮度
 *
 * @param[in] bright 图片亮度参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加亮度，小于128降低亮度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_GetBrightness(ISPTuning *tuning, unsigned char *pbright);

/**
 * @fn int ISPTuning_SetContrast(ISPTuning *tuning, unsigned char contrast)
 *
 * 设置ISP 综合效果图片对比度
 *
 * @param[in] contrast 图片对比度参数
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加对比度，小于128降低对比度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_SetContrast(ISPTuning *tuning, unsigned char contrast);

/**
 * @fn int ISPTuning_GetContrast(ISPTuning *tuning, unsigned char *pcontrast)
 *
 * 获取ISP 综合效果图片对比度
 *
 * @param[in] contrast 图片对比度参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加对比度，小于128降低对比度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_GetContrast(ISPTuning *tuning, unsigned char *pcontrast);

 /**
 * @fn int ISPTuning_SetSharpness(ISPTuning *tuning, unsigned char sharpness)
 *
 * 设置ISP 综合效果图片锐度
 *
 * @param[in] sharpness 图片锐度参数值
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加锐度，小于128降低锐度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_SetSharpness(ISPTuning *tuning, unsigned char sharpness);

/**
 * @fn int ISPTuning_GetSharpness(ISPTuning *tuning, unsigned char *psharpness)
 *
 * 获取ISP 综合效果图片锐度
 *
 * @param[in] sharpness 图片锐度参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加锐度，小于128降低锐度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_GetSharpness(ISPTuning *tuning, unsigned char *psharpness);

/**
 * @fn int ISPTuning_SetSaturation(ISPTuning *tuning, unsigned char sat)
 *
 * 设置ISP 综合效果图片饱和度
 *
 * @param[in] sat 图片饱和度参数值
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加饱和度，小于128降低饱和度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_SetSaturation(ISPTuning *tuning, unsigned char sat);

/**
 * @fn int ISPTuning_GetSaturation(ISPTuning *tuning, unsigned char *psat)
 *
 * 获取ISP 综合效果图片饱和度
 *
 * @param[in] sat 图片饱和度参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @remark 默认值为128，大于128增加饱和度，小于128降低饱和度。
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能.
 */
int ISPTuning_GetSaturation(ISPTuning *tuning, unsigned char *psat);

/**
 * @fn int ISPTuning_GetTotalGain(ISPTuning *tuning, uint32_t *gain)
 *
 * 获取ISP输出图像的整体增益值
 *
 * @param[in] gain 获取增益值参数的指针,其数据存放格式为[24.8]，高24bit为整数，低8bit为小数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetTotalGain(ISPTuning *tuning, uint32_t *gain);

/**
 * 设置ISP图像镜面效果功能是否使能
 *
 * @fn int ISPTuning_SetISPHflip(ISPTuning *tuning, ISPTuningOpsMode mode)
 *
 * @param[in] mode 是否使能镜面效果
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetISPHflip(ISPTuning *tuning, ISPTuningOpsMode mode);

/**
 * @fn int ISPTuning_GetISPHflip(ISPTuning *tuning, ISPTuningOpsMode *pmode)
 *
 * 获取ISP图像镜面效果功能的操作状态
 *
 * @param[in] pmode 操作参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetISPHflip(ISPTuning *tuning, ISPTuningOpsMode *pmode);

/**
 * @fn int ISPTuning_SetISPVflip(ISPTuning *tuning, ISPTuningOpsMode mode)
 *
 * 设置ISP图像上下反转效果功能是否使能
 *
 * @param[in] mode 是否使能图像上下反转
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetISPVflip(ISPTuning *tuning, ISPTuningOpsMode mode);

/**
 * @fn int ISPTuning_GetISPVflip(ISPTuningOpsMode *pmode)
 *
 * 获取ISP图像上下反转效果功能的操作状态
 *
 * @param[in] pmode 操作参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetISPVflip(ISPTuning *tuning, ISPTuningOpsMode *pmode);

/**
 * ISP 工作模式配置，正常模式或夜视模式。
 */
typedef enum {
    ISP_RUNNING_MODE_DAY = 0,                /**< 正常模式 */
    ISP_RUNNING_MODE_NIGHT = 1,                /**< 夜视模式 */
    ISP_RUNNING_MODE_BUTT,                    /**< 最大值 */
} ISPRunningMode;

/**
 * @fn int ISPTuning_SetISPRunningMode(ISPTuning *tuning, ISPRunningMode mode)
 *
 * 设置ISP工作模式，正常模式或夜视模式；默认为正常模式。
 *
 * @param[in] mode运行模式参数
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * 示例：
 * @code
 * ISPRunningMode mode;
 *
 *    if( it is during a night now){
        mode = ISP_RUNNING_MODE_NIGHT
    }else{
        mode = ISP_RUNNING_MODE_DAY;
    }
    ret = ISPTuning_SetISPRunningMode(mode);
    if(ret){
        IMP_LOG_ERR(TAG, "ISPTuning_SetISPRunningMode error !\n");
        return -1;
    }
 *
 * @endcode
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetISPRunningMode(ISPTuning *tuning, ISPRunningMode mode);

/**
 * @fn int ISPTuning_GetISPRunningMode(ISPTuning *tuning, ISPRunningMode *pmode)
 *
 * 获取ISP工作模式，正常模式或夜视模式。
 *
 * @param[in] pmode操作参数指针
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetISPRunningMode(ISPTuning *tuning, ISPRunningMode *pmode);

/**
 * gamma
 */
typedef struct {
    uint16_t gamma[129];        /**< gamma参数数组，有129个点 */
} ISPGamma;

/**
* @fn int ISPTuning_SetGamma(ISPTuning *tuning, ISPGamma *gamma)
*
* 设置GAMMA参数.
* @param[in] gamma gamma参数
*
* @retval 0 成功
* @retval 非0 失败，返回错误码
*
* @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
*/
int ISPTuning_SetGamma(ISPTuning *tuning, ISPGamma *gamma);

/**
* @fn int ISPTuning_GetGamma(ISPTuning *tuning, ISPGamma *gamma)
*
* 获取GAMMA参数.
* @param[out] gamma gamma参数
*
* @retval 0 成功
* @retval 非0 失败，返回错误码
*
* @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
*/
int ISPTuning_GetGamma(ISPTuning *tuning, ISPGamma *gamma);

/**
 * 曝光模式
 */
enum isp_core_expr_mode {
    ISP_CORE_EXPR_MODE_AUTO,            /**< 自动模式 */
    ISP_CORE_EXPR_MODE_MANUAL,            /**< 手动模式 */
};

/**
 * 曝光单位
 */
enum isp_core_expr_unit {
    ISP_CORE_EXPR_UNIT_LINE,            /**< 行 */
    ISP_CORE_EXPR_UNIT_US,                /**< 毫秒 */
};

/**
 * 曝光参数
 */
typedef union isp_core_expr_attr{
    struct {
        enum isp_core_expr_mode mode;        /**< 设置的曝光模式 */
        enum isp_core_expr_unit unit;        /**< 设置的曝光单位 */
        uint16_t time;
    } s_attr;
    struct {
        enum isp_core_expr_mode mode;            /**< 获取的曝光模式 */
        uint16_t integration_time;        /**< 获取的曝光时间，单位为行 */
        uint16_t integration_time_min;    /**< 获取的曝光最小时间，单位为行 */
        uint16_t integration_time_max;    /**< 获取的曝光最大时间，单位为行 */
        uint16_t one_line_expr_in_us;        /**< 获取的一行曝光时间对应的微妙数 */
    } g_attr;
}ISPExpr;


/**
 * @fn int ISPTuning_SetExpr(ISPTuning *tuning, ISPExpr *expr)
 *
 * 设置AE参数。
 *
 * @param[in] expr AE参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetExpr(ISPTuning *tuning, ISPExpr *expr);

/**
 * @fn int ISPTuning_GetExpr(ISPTuning *tuning, ISPExpr *expr)
 *
 * 获取AE参数。
 *
 * @param[out] expr AE参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetExpr(ISPTuning *tuning, ISPExpr *expr);

/**
 * 曝光统计区域选择
 */
typedef union isp_core_ae_roi_select{
    struct {
        unsigned endy :8;                   /**< 结束点y坐标 (0 ~ 255)*/
        unsigned endx :8;                   /**< 结束点x坐标 (0 ~ 255)*/
        unsigned starty :8;                 /**< 起始点y坐标 (0 ~ 255)*/
        unsigned startx :8;                 /**< 起始点x坐标 (0 ~ 255)*/
    };
    uint32_t value;
} ISPAERoi;

/**
 * 白平衡模式
 */
enum isp_core_wb_mode {
    ISP_CORE_WB_MODE_AUTO = 0,            /**< 自动模式 */
    ISP_CORE_WB_MODE_MANUAL,            /**< 手动模式 */
    ISP_CORE_WB_MODE_DAY_LIGHT,            /**< 晴天 */
    ISP_CORE_WB_MODE_CLOUDY,            /**< 阴天 */
    ISP_CORE_WB_MODE_INCANDESCENT,        /**< 白炽灯 */
    ISP_CORE_WB_MODE_FLOURESCENT,        /**< 荧光灯 */
    ISP_CORE_WB_MODE_TWILIGHT,            /**< 黄昏 */
    ISP_CORE_WB_MODE_SHADE,                /**< 阴影 */
    ISP_CORE_WB_MODE_WARM_FLOURESCENT,    /**< 暖色荧光灯 */
    ISP_CORE_WB_MODE_CUSTOM,    /**< 自定义模式 */
};

/**
 * 白平衡参数
 */
typedef struct isp_core_wb_attr{
    enum isp_core_wb_mode mode;        /**< 白平衡模式，分为自动与手动模式 */
    uint16_t rgain;            /**< 红色增益，手动模式时有效 */
    uint16_t bgain;            /**< 蓝色增益，手动模式时有效 */
}ISPWB;

/**
 * @fn int ISPTuning_SetWB(ISPTuning *tuning, ISPWB *wb)
 *
 * 设置白平衡功能设置。可以设置自动与手动模式，手动模式主要通过设置rgain、bgain实现。
 *
 * @param[in] wb 设置的白平衡参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetWB(ISPTuning *tuning, ISPWB *wb);

/**
 * @fn int ISPTuning_GetWB(ISPTuning *tuning, ISPWB *wb)
 *
 * 获取白平衡功能设置。
 *
 * @param[out] wb 获取的白平衡参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetWB(ISPTuning *tuning, ISPWB *wb);

/**
 * @fn ISPTuning_GetWB_Statis(ISPTuning *tuning, ISPWB *wb)
 *
 * 获取白平衡统计值。
 *
 * @param[out] wb 获取的白平衡统计值。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetWB_Statis(ISPTuning *tuning, ISPWB *wb);

/**
 * @fn ISPTuning_GetWB_GOL_Statis(ISPTuning *tuning, ISPWB *wb)
 *
 * 获取白平衡全局统计值。
 *
 * @param[out] wb 获取的白平衡全局统计值。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetWB_GOL_Statis(ISPTuning *tuning, ISPWB *wb);

/**
 * @fn int ISPTuning_SetMaxAgain(uint32_t gain)
 *
 * 设置sensor可以设置最大Again。
 *
 * @param[in] gain sensor可以设置的最大again.0表示1x，32表示2x，依次类推。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */

int ISPTuning_SetMaxAgain(ISPTuning *tuning, uint32_t gain);

/**
 * @fn int ISPTuning_GetMaxAgain(ISPTuning *tuning, uint32_t *gain)
 *
 * 获取sensor可以设置最大Again。
 *
 * @param[out] gain sensor可以设置的最大again.0表示1x，32表示2x，依次类推。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetMaxAgain(ISPTuning *tuning, uint32_t *gain);

/**
 * @fn int ISPTuning_SetMaxDgain(ISPTuning *tuning, uint32_t gain)
 *
 * 设置ISP可以设置的最大Dgain。
 *
 * @param[in] ISP Dgain 可以设置的最大dgain.0表示1x，32表示2x，依次类推。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetMaxDgain(ISPTuning *tuning, uint32_t gain);

/**
 * @fn int ISPTuning_GetMaxDgain(ISPTuning *tuning, uint32_t *gain)
 *
 * 获取ISP设置的最大Dgain。
 *
 * @param[out] ISP Dgain 可以得到设置的最大的dgain.0表示1x，32表示2x，依次类推。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetMaxDgain(ISPTuning *tuning, uint32_t *gain);

/**
 * @fn int ISPTuning_SetHiLightDepress(ISPTuning *tuning, uint32_t strength)
 *
 * 设置强光抑制强度。
 *
 * @param[in] strength 强光抑制强度参数.取值范围为［0-10], 0表示关闭功能。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetHiLightDepress(ISPTuning *tuning, uint32_t strength);

/**
 * @fn int ISPTuning_GetHiLightDepress(ISPTuning *tuning, uint32_t *strength)
 *
 * 获取强光抑制的强度。
 *
 * @param[out] strength 可以得到设置的强光抑制的强度.0表示关闭此功能。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetHiLightDepress(ISPTuning *tuning, uint32_t *strength);

/**
 * ISP EV 参数。
 */
typedef struct {
    uint32_t ev;            /**< 曝光值 */
    uint32_t expr_us;        /**< 曝光时间 */
    uint32_t ev_log2;        /**<log格式曝光时间 */
    uint32_t again;            /**< 模拟增益 */
    uint32_t dgain;            /**< 数字增益 */
    uint32_t gain_log2;        /**< log格式增益 */
}ISPEVAttr;

/**
* @fn int ISPTuning_GetEVAttr(ISPTuning *tuning, ISPEVAttr *attr)
*
* 获取EV属性。
* @param[out] attr EV属性参数
*
* @retval 0 成功
* @retval 非0 失败，返回错误码
*
* @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
*/
int ISPTuning_GetEVAttr(ISPTuning *tuning, ISPEVAttr *attr);

/**
* 模式选择选项
*/
typedef enum {
    ISP_TUNING_MODE_AUTO,    /**< 该模块的操作为自动模式 */
    ISP_TUNING_MODE_MANUAL,    /**< 该模块的操作为手动模式 */
    ISP_TUNING_MODE_RANGE,    /**< 该模块的操作为设置范围模式 */
    ISP_TUNING_MODE_BUTT,    /**< 用于判断参数的有效性，参数大小必须小于这个值 */
} ISPTuningMode;

/**
* 曝光参数
*/
typedef struct {
    ISPTuningMode mode;    /**< 曝光模式，分为自动模式、手动模式、设置范围模式 */
    uint16_t integration_time;    /**曝光时间 */
    uint16_t max_integration_time;    /**< 最大曝光时间 */
} ISPITAttr;

/**
 * @fn int ISPTuning_SetIntegrationTime(ISPTuning *tuning, ISPITAttr *itattr)
 *
 * 设置AE参数。
 *
 * @param[in] itattr 曝光参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetIntegrationTime(ISPTuning *tuning, ISPITAttr *itattr);

/**
 * @fn int ISPTuning_GetIntegrationTime(ISPTuning *tuning, ISPITAttr *itattr)
 *
 * 获取曝光相关参数。
 *
 * @param[out] itattr 曝光参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetIntegrationTime(ISPTuning *tuning, ISPITAttr *itattr);

/**
* 权重信息
*/
typedef struct {
    unsigned char weight[15][15];    /**< 各区域权重信息 [0 ~ 8]*/
} ISPWeight;

/**
 * @fn int ISPTuning_SetAeWeight(ISPTuning *tuning, ISPWeight *ae_weight)
 *
 * 设置AE统计区域的权重。
 *
 * @param[in] ae_weight 各区域权重信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetAeWeight(ISPTuning *tuning, ISPWeight *ae_weight);

/**
 * @fn int ISPTuning_GetAeWeight(ISPTuning *tuning, ISPWeight *ae_weight)
 *
 * 获取AE统计区域的权重。
 *
 * @param[out] ae_weight 各区域权重信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAeWeight(ISPTuning *tuning, ISPWeight *ae_weight);

/**
 * @fn int ISPTuning_GetAeROI(ISPTuning *tuning, ISPWeight *roi_weight)
 *
 * 获取AE感兴趣区域，用于场景判断。
 *
 * @param[out] roi_weight AE感兴趣区域权重。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAeROI(ISPTuning *tuning, ISPWeight *roi_weight);

/**
 * @fn int ISPTuning_SetAeROI(ISPTuning *tuning, ISPWeight *roi_weight)
 *
 * 获取AE感兴趣区域，用于场景判断。
 *
 * @param[in] roi_weight AE感兴趣区域权重。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetAeROI(ISPTuning *tuning, ISPWeight *roi_weight);

/**
* AE统计值参数
*/
typedef struct {
    unsigned char ae_histhresh[4];    /**< AE统计直方图bin边界 [0 ~ 255]*/
    unsigned short ae_hist[5];    /**< AE统计直方图bin值 [0 ~ 65535]*/
    unsigned char ae_stat_nodeh;    /**< 水平方向有效统计区域个数 [0 ~ 15]*/
    unsigned char ae_stat_nodev;    /**< 垂直方向有效统计区域个数 [0 ~ 15]*/
} ISPAEHist;

/**
 * @fn int ISPTuning_SetAeHist(ISPTuning *tuning, ISPAEHist *ae_hist)
 *
 * 设置AE统计相关参数。
 *
 * @param[in] ae_hist AE统计相关参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetAeHist(ISPTuning *tuning, ISPAEHist *ae_hist);

/**
 * @fn int ISPTuning_GetAeHist(ISPTuning *tuning, ISPAEHist *ae_hist)
 *
 * 获取AE统计值。
 *
 * @param[out] ae_hist AE统计值信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAeHist(ISPTuning *tuning, ISPAEHist *ae_hist);

/**
* AF统计值
*/
struct isp_core_af_sta_info{
    unsigned int af_metrics;/**< AF主统计值*/
    unsigned int af_metrics_alt;/**< AF次统计值*/
};
/**
* AF统计值参数
*/
typedef struct {
    struct isp_core_af_sta_info af_stat;    /**< AF统计值信息 */
    unsigned char af_enable;    /**< AF功能开关*/
    unsigned char af_metrics_shift;    /**< AF统计值缩小参数 默认是0，1代表缩小2倍*/
    unsigned short af_delta;    /**< AF主统计值阈值 [0 ~ 64]*/
    unsigned short af_theta;    /**< AF主统计值阈值 [0 ~ 64]*/
    unsigned short af_hilight_th;    /**< AF高亮点统计阈值 [0 ~ 255]*/
    unsigned short af_alpha_alt;    /**< AF次统计值阈值 [0 ~ 64]*/
    unsigned char  af_hstart;    /**< AF统计值横向起始点 [0 ~ 15]*/
    unsigned char  af_vstart;    /**< AF统计值竖向起始点 [0 ~ 15]*/
    unsigned char  af_stat_nodeh;    /**< 水平方向有效统计区域个数 [0 ~ 15]*/
    unsigned char  af_stat_nodev;    /**< 垂直方向有效统计区域个数 [0 ~ 15]*/
} ISPAFHist;

/**
 * @fn ISPTuning_GetAFMetrices(ISPTuning *tuning, unsigned int *metric);
 *
 * 获取AF统计值。
 *
 * @param[out] metric AF统计值信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
    int ISPTuning_GetAFMetrices(ISPTuning *tuning, unsigned int *metric);

/**
 * @fn int ISPTuning_GetAfHist(ISPTuning *tuning, ISPAFHist *af_hist);
 *
 * 获取AF统计值。
 *
 * @param[out] af_hist AF统计值信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAfHist(ISPTuning *tuning, ISPAFHist *af_hist);

/**
 * @fn int ISPTuning_SetAfHist(ISPTuning *tuning, ISPAFHist *af_hist)
 *
 * 设置AF统计相关参数。
 *
 * @param[in] af_hist AF统计相关参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetAfHist(ISPTuning *tuning, ISPAFHist *af_hist);
/**
 * @fn int ISPTuning_SetAfWeight(ISPTuning *tuning, ISPWeight *af_weight)
 *
 * 设置AF统计区域的权重。
 *
 * @param[in] af_weight 各区域权重信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetAfWeight(ISPTuning *tuning, ISPWeight *af_weigh);
/**
 * @fn int ISPTuning_GetAfWeight(ISPTuning *tuning, ISPWeight *af_weight)
 *
 * 获取AF统计区域的权重。
 *
 * @param[out] af_weight 各区域权重信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAfWeight(ISPTuning *tuning, ISPWeight *af_weight);

/**
 * AE Min
 */
typedef struct {
    unsigned int min_it;  /**< AE最小曝光 */
    unsigned int min_again;     /**< AE 最小模拟增益 */
} ISPAEMin;

/**
 * @fn int ISPTuning_SetAeMin(ISPTuning *tuning, ISPAEMin *ae_min)
 *
 * 设置AE最小值参数。
 *
 * @param[in] ae_min AE最小值参数。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetAeMin(ISPTuning *tuning, ISPAEMin *ae_min);

/**
 * @fn int ISPTuning_GetAeMin(ISPTuning *tuning, ISPAEMin *ae_min)
 *
 * 获取AE最小值参数。
 *
 * @param[out] ae_min AE最小值信息。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAeMin(ISPTuning *tuning, ISPAEMin *ae_min);

typedef struct {
    unsigned int zone[15][15];    /**< 各区域信息*/
} ISPZone;

/**
 * @fn int ISPTuning_GetAeZone(ISPTuning *tuning, ISPZone *ae_zone)
 *
 * 获取AE各个zone的Y值。
 *
 * @param[out] ae_zone AE各个区域的Y值。
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetAeZone(ISPTuning *tuning, ISPZone *ae_zone);

/**
* @fn int ISPTuning_GetAeLuma(ISPTuning *tuning, int *luma)
*
* 获取画面平均亮度。
*
* @param[out] luma AE亮度参数
*
* @retval 0 成功
* @retval 非0 失败，返回错误码
*
* @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
*/
int ISPTuning_GetAeLuma(ISPTuning *tuning, int *luma);

/**
 * ISP Module Control
 */
typedef union {
    unsigned int key;
    struct {
        unsigned int bitBypassDPC : 1; /* [0]  */
        unsigned int bitBypassGIB : 1; /* [1]  */
        unsigned int bitBypassLSC : 1; /* [2]  */
        unsigned int bitBypassAWB : 1; /* [3]  */
        unsigned int bitBypassADR : 1; /* [4]  */
        unsigned int bitBypassDMSC : 1; /* [5]    */
        unsigned int bitBypassCCM : 1; /* [6]  */
        unsigned int bitBypassGAMMA : 1; /* [7]     */
        unsigned int bitBypassDEFOG : 1; /* [8]     */
        unsigned int bitBypassCLM : 1; /* [9]  */
        unsigned int bitBypassYSHARPEN : 1; /* [10]  */
        unsigned int bitBypassMDNS : 1; /* [11]     */
        unsigned int bitBypassSDNS : 1; /* [12]     */
        unsigned int bitBypassHLDC : 1; /* [13]     */
        unsigned int bitBypassTP : 1; /* [14]  */
        unsigned int bitBypassFONT : 1; /* [15]     */
        unsigned int bitRsv : 15; /* [16 ~ 30]    */
        unsigned int bitRsv2 : 1; /* [31]  */
    };
} ISPModuleCtl;

/**
 * @fn int ISPTuning_SetModuleControl(ISPTuning *tuning, ISPModuleCtl *ispmodule)
 *
 * 设置ISP各个模块bypass功能
 *
 * @param[in] ispmodule ISP各个模块bypass功能.
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetModuleControl(ISPTuning *tuning, ISPModuleCtl *ispmodule);

/**
 * @fn int ISPTuning_GetModuleControl(ISPTuning *tuning, ISPModuleCtl *ispmodule)
 *
 * 获取ISP各个模块bypass功能.
 *
 * @param[out] ispmodule ISP各个模块bypass功能
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetModuleControl(ISPTuning *tuning, ISPModuleCtl *ispmodule);

/**
 * @fn int ISPTuning_SetDRC_Strength(ISPTuning *tuning, unsigned int strength)
 *
 * 设置DRC强度值.
 *
 * @param[in] strength DRC强度.
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_SetDRC_Strength(ISPTuning *tuning, unsigned int strength);

/**
 * @fn int ISPTuning_GetDRC_Strength(ISPTuning *tuning, unsigned int *strength)
 *
 * 获取DRC强度值.
 *
 * @param[out] strength DRC强度.
 *
 * @retval 0 成功
 * @retval 非0 失败，返回错误码
 *
 * @attention 在使用这个函数之前，必须保证ISP效果调试功能已使能。
 */
int ISPTuning_GetDRC_Strength(ISPTuning *tuning, unsigned int *strength);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

/**
 * @}
 */

#endif /* __IMP_ISP_H__ */