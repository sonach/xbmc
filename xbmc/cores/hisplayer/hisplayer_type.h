
#ifndef __HISPLAYER_TYPE_H__
#define __HISPLAYER_TYPE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef unsigned int        HI_U32;
typedef int                 HI_S32;
typedef unsigned long       HI_SIZE_T;
typedef unsigned long       HI_LENGTH_T;
typedef __int64             HI_U64;
typedef __int64             HI_S64;
typedef char                HI_CHAR;
typedef unsigned char       HI_U8;
typedef unsigned char       HI_UCHAR;
typedef unsigned short      HI_U16;

typedef HI_U32              HI_HANDLE;
typedef void                HI_VOID;


typedef enum {
    HI_FALSE    = 0,
    HI_TRUE     = 1,
} HI_BOOL;

/** Maximum number of programs. For example, if the number of programs in the transport stream (TS) media file is greater than 5, the player stores only five programs at most and plays one of the programs at a time. */
/** CNcomment:最大节目个数，如ts媒体文件，节目个数如果超过5个，则播放器最多只能存储其中某5个节目信息，并播放其中的某一个 */
#define HI_FORMAT_MAX_PROGRAM_NUM           (5)
/** Length of the uniform resource locator (URL) character */
/** CNcomment:路径字符长度 */
#define HI_FORMAT_MAX_URL_LEN               (1024)
/** Length of the file name */
/** CNcomment:文件名称长度 */
#define HI_FORMAT_MAX_FILE_NAME_LEN         (512)
/** Maximum number of characters in the information such as the file title and author */
/** CNcomment:文件标题、作者等最大字符数 */
#define HI_FORMAT_TITLE_MAX_LEN             (512)
/** Number of language characters */
/** CNcomment:语言字符数 */
#define HI_FORMAT_LANG_LEN                  (4)
/** Length of the subtitle title */
/** CNcomment:字幕标题长度 */
#define HI_FORMAT_SUB_TITLE_LEN             (32)
/** Bytes of year */
/** CNcomment:年份字节数 */
#define HI_FORMAT_TIME_LEN                  (8)
/** Number of streams */
/** CNcomment:流个数 */
#define HI_FORMAT_MAX_STREAM_NUM            (32)
/** Maximum number of supported languages */
/** CNcomment:支持的最多语言种类个数 */
#define HI_FORMAT_MAX_LANG_NUM              (64)



/** Player status */
/** CNcomment:PLAYER状态 */
typedef enum hiSVR_PLAYER_STATE_E
{
    HI_SVR_PLAYER_STATE_INI = 0,    /**< The player is in the initial state. It changes to the initial state after being created. *//**< CNcomment:播放器当前处于初始状态，create后播放器处于ini状态 */
    HI_SVR_PLAYER_STATE_DEINI,      /**< The player is deinitialized. *//**< CNcomment:播放器已经去初始状态 */
    HI_SVR_PLAYER_STATE_PLAY,       /**< The player is in the playing state. *//**< CNcomment:播放器当前处于播放状态 */
    HI_SVR_PLAYER_STATE_FORWARD,    /**< The player is in the fast forward state. *//**< CNcomment:播放器当前处于快进状态 */
    HI_SVR_PLAYER_STATE_BACKWARD,   /**< The player is in the rewind state. *//**< CNcomment:播放器当前处于快退状态 */
    HI_SVR_PLAYER_STATE_PAUSE,      /**< The player is in the pause state. *//**< CNcomment:播放器当前处于暂停状态 */
    HI_SVR_PLAYER_STATE_STOP,       /**< The player is in the stop state. *//**< CNcomment:播放器当前处于停止状态 */
    HI_SVR_PLAYER_STATE_BUTT
} HI_SVR_PLAYER_STATE_E;


/** Player information */
/** CNcomment:播放器信息 */
typedef struct hiSVR_PLAYER_INFO_S
{
    HI_U32  u32Progress;                 /**< Playing progress. The value ranges from 0 to 100. *//**< CNcomment:播放百分比，0-100 */
    HI_U64  u64TimePlayed;               /**< Elapsed time, in the unit of ms. *//**< CNcomment:已播放时间，单位ms */
    HI_S32  s32Speed;                    /**< Playing speed *//**< CNcomment:播放速率 */
    HI_SVR_PLAYER_STATE_E  eStatus;      /**< Playing status *//**< CNcomment:播放状态 */
} HI_SVR_PLAYER_INFO_S;

/** Subtitle data type */
/** CNcomment:字幕数据类型 */
typedef enum hiFORMAT_SUBTITLE_DATA_TYPE_E
{
    HI_FORMAT_SUBTITLE_DATA_TEXT = 0x0,    /**< Character string *//**< CNcomment:字符串 */
    HI_FORMAT_SUBTITLE_DATA_BMP,           /**< BMP picture *//**< CNcomment:bmp图片，该类型无效 */
    HI_FORMAT_SUBTITLE_DATA_ASS,           /**< ASS subtitle *//**< CNcomment:ass字幕 */
    HI_FORMAT_SUBTITLE_DATA_HDMV_PGS,      /**< pgs subtitle *//**< CNcomment:pgs字幕 */
    HI_FORMAT_SUBTITLE_DATA_BMP_SUB,       /**< idx+sub subtitle *//**< CNcomment:sub字幕 */
    HI_FORMAT_SUBTITLE_DATA_DVB_SUB,       /**< DVB subtitle *//**< CNcomment:DVBsub字幕 */
    HI_FORMAT_SUBTITLE_DATA_BUTT
} HI_FORMAT_SUBTITLE_DATA_TYPE_E;

/** Type of subtitle */
/** CNcomment: 字幕类型信息 */
#define HI_FORMAT_SUB_TYPE_LEN              (32)
#define HI_FORMAT_SUBTITLE_PGS              "PGS"
#define HI_FORMAT_SUBTITLE_ASS              "ASS"
#define HI_FORMAT_SUBTITLE_SSA              "SSA"
#define HI_FORMAT_SUBTITLE_SMI              "SMI"
#define HI_FORMAT_SUBTITLE_SRT              "SRT"
#define HI_FORMAT_SUBTITLE_LRC              "LRC"
#define HI_FORMAT_SUBTITLE_SUB              "SUB"
#define HI_FORMAT_SUBTITLE_DVDSUB           "DVD_SUB"
#define HI_FORMAT_SUBTITLE_DVBSUB           "DVB_SUB"
#define HI_FORMAT_SUBTITLE_TS               "TS_SUB"
#define HI_FORMAT_SUBTITLE_UTF8_TEXT        "UTF8_TEXT"



/** Subtitle information */
/** CNcomment:字幕信息 */
typedef struct hiFORMAT_SUBTITLE_INFO_S
{
    HI_S32  s32StreamIndex;                            /**< Stream index. The value is the stream PID for TS streams. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:流索引，对于ts流，该值为流pid，非法值为::HI_FORMAT_INVALID_STREAM_ID */
    HI_BOOL bExtSubTitle;                              /**< Whether subtitles are external subtitles. When bExtSubTitle is HI_TRUE, the subtitles are external. When bExtSubTitle is HI_FALSE, the subtitles are internal. *//**< CNcomment:是否为外挂字幕, HI_TRUE为外挂字幕，HI_FALSE为内置字幕 */
    HI_FORMAT_SUBTITLE_DATA_TYPE_E eSubtitileType;     /**< Subtitle format, such as BMP and string *//**< CNcomment:字幕格式，如bmp,string */
    HI_CHAR aszSubTitleName[HI_FORMAT_SUB_TITLE_LEN];  /**< Subtitle title *//**< CNcomment:字幕标题 */
    HI_U16  u16OriginalFrameWidth;                     /**< Width of the original image *//**< CNcomment:原始图像宽度 */
    HI_U16  u16OriginalFrameHeight;                    /**< Height of the original image *//**< CNcomment:原始图像高度 */
    HI_U16  u16HorScale;                               /**< Horizontal scaling ratio. The value ranges from 0 to 100. *//**< CNcomment:水平缩放比，0-100 */
    HI_U16  u16VerScale;                               /**< Vertical scaling ratio. The value ranges from 0 to 100. *//**< CNcomment:垂直缩放比，0-100 */
    HI_U32  u32ExtradataSize;                          /**< Length of the extended data *//**< CNcomment:扩展数据长度 */
    HI_U8   *pu8Extradata;                             /**< Extended data *//**< CNcomment:扩展数据 */
    HI_U32  u32SubTitleCodeType;                       /**< Encoding type of the subtitle, the value range is as follows:
                                                            1. The default value is 0.
                                                            2. The value of the u32SubTitleCodeType is the identified byte encoding value if the IdentStream byte encoding function (for details about the definition, see hi_charset_common.h) is set.
                                                            3. If the ConvStream function (for details about the definition, see hi_charset_common.h) is set and the invoke interface is called to set the encoding type to be converted by implementing HI_FORMAT_INVOKE_SUB_SET_CODETYPE, the value of the u32SubTitleCodeType is the configured encoding type */
                                                       /**< CNcomment:字幕编码类型，取值范围如下:
                                                            1. 默认值为0
                                                            2. 如果设置了IdentStream字符编码识别函数
                                                               (参考hi_charset_common.h)，则该属性值为识别出的字符编码值
                                                            3. 如果设置了ConvStream转码函数 (参考hi_charset_common.h)，
                                                              并且调用Invoke接口执行HI_FORMAT_INVOKE_SUB_SET_CODETYPE设
                                                              置要转换成的编码类型，该值为设置的编码类型*/
    HI_CHAR aszSubtType[HI_FORMAT_SUB_TYPE_LEN];       /**< Type of subtitle. e.g. pgs,srt etc. *//**< CNcomment:字幕类型,如pgs,srt等等 */
} HI_FORMAT_SUBTITLE_INFO_S;

/** Audio stream information */
/** CNcomment:音频流信息 */
typedef struct hiFORMAT_AUD_INFO_S
{
    HI_S32 s32StreamIndex;                   /**< Stream index. The value is the stream PID for TS streams. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:流索引，对于ts流，该值为流pid，非法值为::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;                        /**< Audio encoding format. For details about the value definition, see ::HA_FORMAT_E. *//**< CNcomment:音频编码格式，值定义参考::HA_FORMAT_E */
    HI_U32 u32Version;                       /**< Audio encoding version, such as 0x160(WMAV1) and 0x161 (WMAV2). It is valid only for WMA encoding. *//**< CNcomment:音频编码版本，仅对wma编码，0x160(WMAV1), 0x161 (WMAV2) */
    HI_U32 u32SampleRate;                    /**< 8000,11025,441000,... */
    HI_U16 u16BitPerSample;                  /**< Number of bits occupied by each audio sampling point such as 8 bits or 16 bits. *//**< CNcomment:音频每个采样点所占的比特数，8bit,16bit */
    HI_U16 u16Channels;                      /**< Number of channels, 1 or 2. *//**< CNcomment:声道数, 1 or 2 */
    HI_U32 u32BlockAlign;                    /**< Number of bytes contained in a packet *//**< CNcomment:packet包含的字节数 */
    HI_U32 u32Bps;                           /**< Audio bit rate, in the unit of bit/s. *//**< CNcomment:音频码率，bit/s */
    HI_BOOL bBigEndian;                      /**< Big endian or little endian. It is valid only for the PCM format *//**< CNcomment:大小端，仅pcm格式有效 */
    HI_CHAR aszAudLang[HI_FORMAT_LANG_LEN];  /**< Audio stream language *//**< CNcomment:音频流语言 */
    HI_U32 u32ExtradataSize;                 /**< Length of the extended data *//**< CNcomment:扩展数据长度 */
    HI_U8  *pu8Extradata;                    /**< Extended data *//**< CNcomment:扩展数据 */
    HI_VOID  *pCodecContext;                 /**< Audio decode context *//**< CNcomment:音频解码上下文 */
    HI_S32 s32SubStreamID;                   /**< Sub audio stream ID *//**< CNcomment:子音频流ID,如果为TrueHD音频，则可能含有一路ac3子流 ，当前处理策略为将两条流分离开来，并用此字段来记录关系 */
} HI_FORMAT_AUD_INFO_S;

/** Video stream information */
/** CNcomment:视频流信息 */
typedef struct hiFORMAT_VID_INFO_S
{
    HI_S32 s32StreamIndex;        /**< Stream index. The value is the stream PID for TS streams. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:流索引，对于ts流，该值为流pid，非法值为::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;             /**< Video encoding format. For details about the value definition, see ::HI_UNF_VCODEC_TYPE_E. *//**< CNcomment:视频编码格式，值定义参考::HI_UNF_VCODEC_TYPE_E */
    HI_U16 u16Width;              /**< Width, in the unit of pixel. *//**< CNcomment:宽度，单位像素 */
    HI_U16 u16Height;             /**< Height, in the unit of pixel. *//**< CNcomment:高度，单位像素 */
    HI_U16 u16FpsInteger;         /**< Integer part of the frame rate *//**< CNcomment:帧率，整数部分 */
    HI_U16 u16FpsDecimal;         /**< Decimal part of the frame rate *//**< CNcomment:帧率，小数部分 */
    HI_U32 u32Bps;                /**< Video bit rate, in the unit of bit/s. *//**< CNcomment:视频码率，bit/s */
    HI_U32 u32ExtradataSize;      /**< Length of the extended data *//**< CNcomment:扩展数据长度 */
    HI_U8  *pu8Extradata;         /**< Extended data *//**< CNcomment:扩展数据 */
    HI_VOID  *pCodecContext;      /**< video decode context *//**< CNcomment:视频解码上下文 */
} HI_FORMAT_VID_INFO_S;



/** Program information. If a file contains internal subtitles and external subtitles, add the external subtitles to the end of the internal subtitles.
    Note: If a media file does not contain video streams, set videstreamindex to HI_FORMAT_INVALID_STREAM_ID. */
/** CNcomment:节目信息，如果文件既带内置字幕也带外置字幕，则外置字幕信息追加在内置字幕后，
    注: 如果媒体文件不带视频流，需将videstreamindex设置为HI_FORMAT_INVALID_STREAM_ID */
typedef struct hiFORMAT_PROGRAM_INFO_S
{
    HI_FORMAT_VID_INFO_S stVidStream;                                  /**< Video stream information *//**< CNcomment:视频流信息 */
    HI_U32 u32AudStreamNum;                                            /**< Number of audio streams *//**< CNcomment:音频流个数 */
    HI_FORMAT_AUD_INFO_S astAudStream[HI_FORMAT_MAX_STREAM_NUM];       /**< Audio stream information *//**< CNcomment:音频流信息 */
    HI_U32 u32SubTitleNum;                                             /**< Number of subtitles *//**< CNcomment:字幕个数 */
    HI_FORMAT_SUBTITLE_INFO_S astSubTitle[HI_FORMAT_MAX_LANG_NUM];     /**< Subtitle information *//**< CNcomment:字幕信息 */
} HI_FORMAT_PROGRAM_INFO_S;


/** Stream type of the file to be played */
/** CNcomment:文件流类型，播放的文件类型 */
typedef enum hiFORMAT_STREAM_TYPE_E
{
    HI_FORMAT_STREAM_ES = 0x0,    /**< Element stream (ES) file *//**< CNcomment:es流文件 */
    HI_FORMAT_STREAM_TS,          /**< TS file *//**< CNcomment:ts流文件 */
    HI_FORMAT_STREAM_NORMAL,      /**< Common files *//**< CNcomment:普通文件，如idx,lrc,srt等字幕文件 */
    HI_FORMAT_STREAM_NET,         /**< network stream file *//**< CNcomment:网络流文件 */
    HI_FORMAT_STREAM_LIVE,        /**< live streams *//**< CNcomment:直播流，无文件结束信息 */
    HI_FPRMAT_STREAM_BUTT
} HI_FORMAT_STREAM_TYPE_E;


/** Media file information */
/** CNcomment:媒体文件信息 */
typedef struct hiFORMAT_FILE_INFO_S
{
    HI_FORMAT_STREAM_TYPE_E eStreamType;                /**< File stream type *//**< CNcomment:文件流类型 */
    HI_S64  s64FileSize;                                /**< File size, in the unit of byte. *//**< CNcomment:文件大小，单位字节 */
    HI_S64  s64StartTime;                               /**< Start time of playing a file, in the unit is ms. *//**< CNcomment:文件播放起始时间，单位ms */
    HI_S64  s64Duration;                                /**< Total duration of a file, in the unit of ms. *//**< CNcomment:文件总时长，单位ms */
    HI_U32  u32Bps;                                     /**< File bit rate, in the unit of bit/s. *//**< CNcomment:文件码率，bit/s */
    HI_CHAR aszFileName[HI_FORMAT_MAX_FILE_NAME_LEN];   /**< File name *//**< CNcomment:文件名称 */
    HI_CHAR aszAlbum[HI_FORMAT_TITLE_MAX_LEN];          /**< Album *//**< CNcomment:专辑 */
    HI_CHAR aszTitle[HI_FORMAT_TITLE_MAX_LEN];          /**< Title *//**< CNcomment:标题 */
    HI_CHAR aszArtist[HI_FORMAT_TITLE_MAX_LEN];         /**< Artist, author *//**< CNcomment:艺术家，作者 */
    HI_CHAR aszGenre[HI_FORMAT_TITLE_MAX_LEN];          /**< Genre *//**< CNcomment:风格 */
    HI_CHAR aszComments[HI_FORMAT_TITLE_MAX_LEN];       /**< Comments *//**< CNcomment:附注 */
    HI_CHAR aszTime[HI_FORMAT_TIME_LEN];                /**< Created time *//**< CNcomment:创建年份 */
    HI_U32  u32ProgramNum;                              /**< Actual number of programs *//**< CNcomment:实际节目个数 */
    HI_FORMAT_PROGRAM_INFO_S astProgramInfo[HI_FORMAT_MAX_PROGRAM_NUM];    /**< Program information *//**< CNcomment:节目信息 */
} HI_FORMAT_FILE_INFO_S;


/** Event type */
/** 事件类型 */
typedef enum hiSVR_PLAYER_EVENT_E
{
    HI_SVR_PLAYER_EVENT_STATE = 0x0,   /**< Player status change event. The parameter value is ::HI_SVR_PLAYER_STATE_E. *//**< CNcomment:播放器状态转换事件，参数值为::HI_SVR_PLAYER_STATE_E */
    HI_SVR_PLAYER_EVENT_SOF,           /**< Event indicating that file playing starts or a file is rewound to the file header. The parameter type is HI_U32. The value ::HI_SVR_PLAYER_STATE_PLAY indicates that file playing starts and the value ::HI_SVR_PLAYER_STATE_BACKWARD indicates that a file is rewound to the file header. */
                                       /**< CNcomment:文件开始播放或快退到文件头事件，参数类型为HI_U32，值为::HI_SVR_PLAYER_STATE_PLAY表示开始播放，参数值为::HI_SVR_PLAYER_STATE_BACKWARD表示快退到文件头 */
    HI_SVR_PLAYER_EVENT_EOF,           /**< Event indicating that a file is played till the end of the file. There is no parameter. *//**< CNcomment:文件播放到尾事件，无参数 */
    HI_SVR_PLAYER_EVENT_PROGRESS,      /**< Event indicating the current progress of the player. This event is reported once every 500 ms. The parameter is ::HI_SVR_PLAYER_PROGRESS_S. *//**< CNcomment:播放器当前播放进度事件，每隔300ms上报一次该事件，参数值为::HI_SVR_PLAYER_PROGRESS_S */
    HI_SVR_PLAYER_EVENT_STREAMID_CHG,  /**< Stream ID change event. The parameter is ::HI_SVR_PLAYER_STREAMID_S. *//**< CNcomment:stream id 发生变化事件，参数为::HI_SVR_PLAYER_STREAMID_S */
    HI_SVR_PLAYER_EVENT_SEEK_FIN,      /**< Seek operation is complete. The parameter is HI_U32. When the value of the parameter is ::HI_FAILURE, the seek operation fails. When the value of the parameter is ::HI_SUCCESS, the seek operation succeeds. When the value of the parameter is ::HI_FORMAT_ERRO_ENDOFFILE, the file is read to the end. */
                                       /**< CNcomment:Seek操作完成，参数为HI_U32，值为::HI_FAILURE Seek失败，::HI_SUCCESS seek成功，::HI_FORMAT_ERRO_ENDOFFILE 文件读取到文件尾 */
    HI_SVR_PLAYER_EVENT_CODETYPE_CHG,  /**< Event of indicating the byte encoding configuration is complete. The parameter is ::HI_SVR_PLAYER_SUB_CODETYPE_S. *//**< Ncomment:设置字符编码完成事件，事件参数::HI_SVR_PLAYER_SUB_CODETYPE_S */
    HI_SVR_PLAYER_EVENT_DOWNLOAD_PROGRESS,   /**< Current download progress of the player. The event is reported every 300 ms. The parameter is ::HI_SVR_PLAYER_PROGRESS_S. *//**< CNcomment:播放器当前下载进度，每隔300ms上报一次该事件，参数值为::HI_SVR_PLAYER_PROGRESS_S */
    HI_SVR_PLAYER_EVENT_BUFFER,        /**< Reporting buffer status. The parameter type is ::HI_SVR_PLAYER_BUFFER_S. *//**< CNcomment:缓冲状态上报,参数类型为::HI_SVR_PLAYER_BUFFER_S */
    HI_SVR_PLAYER_EVENT_FIRST_FRAME_TIME,    /**< The display time of the first frame from setting the setMedia. The parameter is ::HI_U32, in the unit of ms. *//**< CNcomment:从设置媒体setMedia开始第一帧显示时间,参数为::HI_U32,单位为ms */

    HI_SVR_PLAYER_EVENT_ERROR,               /**< Event of indicating that an error occurs in the player. The parameter is ::HI_SVR_PLAYER_ERROR_E.*//**< CNcomment:播放器错误信息事件，参数为::HI_SVR_PLAYER_ERROR_E */
    HI_SVR_PLAYER_EVNET_NETWORK,       /**< Report the network status. The parameter is ::HI_FORMAT_NET_STATUS_S. *//**< CNcomment:网络状态上报, 参数为::HI_FORMAT_NET_STATUS_S */
    HI_SVR_PLAYER_EVENT_BUTT
} HI_SVR_PLAYER_EVENT_E;


/** Player event callback parameters */
/** CNcomment:播放器事件回调参数 */
typedef struct hiSVR_PLAYER_EVENT_S
{
    HI_SVR_PLAYER_EVENT_E  eEvent;    /**< Event type *//**< CNcomment:事件类型 */
    HI_U32  u32Len;                   /**< Event parameter length, in the unit of byte. *//**< CNcomment:事件参数长度，字节为单位 */
    HI_U8   *pu8Data;                 /**< Start address of event parameter data *//**< CNcomment:事件参数数据起始地址 */
} HI_SVR_PLAYER_EVENT_S;


#define HI_FORMAT_INVALID_STREAM_ID         (-1)





#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __HI_TYPE_H__ */

