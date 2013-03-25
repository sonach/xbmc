
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
/** CNcomment:����Ŀ��������tsý���ļ�����Ŀ�����������5�����򲥷������ֻ�ܴ洢����ĳ5����Ŀ��Ϣ�����������е�ĳһ�� */
#define HI_FORMAT_MAX_PROGRAM_NUM           (5)
/** Length of the uniform resource locator (URL) character */
/** CNcomment:·���ַ����� */
#define HI_FORMAT_MAX_URL_LEN               (1024)
/** Length of the file name */
/** CNcomment:�ļ����Ƴ��� */
#define HI_FORMAT_MAX_FILE_NAME_LEN         (512)
/** Maximum number of characters in the information such as the file title and author */
/** CNcomment:�ļ����⡢���ߵ�����ַ��� */
#define HI_FORMAT_TITLE_MAX_LEN             (512)
/** Number of language characters */
/** CNcomment:�����ַ��� */
#define HI_FORMAT_LANG_LEN                  (4)
/** Length of the subtitle title */
/** CNcomment:��Ļ���ⳤ�� */
#define HI_FORMAT_SUB_TITLE_LEN             (32)
/** Bytes of year */
/** CNcomment:����ֽ��� */
#define HI_FORMAT_TIME_LEN                  (8)
/** Number of streams */
/** CNcomment:������ */
#define HI_FORMAT_MAX_STREAM_NUM            (32)
/** Maximum number of supported languages */
/** CNcomment:֧�ֵ��������������� */
#define HI_FORMAT_MAX_LANG_NUM              (64)



/** Player status */
/** CNcomment:PLAYER״̬ */
typedef enum hiSVR_PLAYER_STATE_E
{
    HI_SVR_PLAYER_STATE_INI = 0,    /**< The player is in the initial state. It changes to the initial state after being created. *//**< CNcomment:��������ǰ���ڳ�ʼ״̬��create�󲥷�������ini״̬ */
    HI_SVR_PLAYER_STATE_DEINI,      /**< The player is deinitialized. *//**< CNcomment:�������Ѿ�ȥ��ʼ״̬ */
    HI_SVR_PLAYER_STATE_PLAY,       /**< The player is in the playing state. *//**< CNcomment:��������ǰ���ڲ���״̬ */
    HI_SVR_PLAYER_STATE_FORWARD,    /**< The player is in the fast forward state. *//**< CNcomment:��������ǰ���ڿ��״̬ */
    HI_SVR_PLAYER_STATE_BACKWARD,   /**< The player is in the rewind state. *//**< CNcomment:��������ǰ���ڿ���״̬ */
    HI_SVR_PLAYER_STATE_PAUSE,      /**< The player is in the pause state. *//**< CNcomment:��������ǰ������ͣ״̬ */
    HI_SVR_PLAYER_STATE_STOP,       /**< The player is in the stop state. *//**< CNcomment:��������ǰ����ֹͣ״̬ */
    HI_SVR_PLAYER_STATE_BUTT
} HI_SVR_PLAYER_STATE_E;


/** Player information */
/** CNcomment:��������Ϣ */
typedef struct hiSVR_PLAYER_INFO_S
{
    HI_U32  u32Progress;                 /**< Playing progress. The value ranges from 0 to 100. *//**< CNcomment:���Űٷֱȣ�0-100 */
    HI_U64  u64TimePlayed;               /**< Elapsed time, in the unit of ms. *//**< CNcomment:�Ѳ���ʱ�䣬��λms */
    HI_S32  s32Speed;                    /**< Playing speed *//**< CNcomment:�������� */
    HI_SVR_PLAYER_STATE_E  eStatus;      /**< Playing status *//**< CNcomment:����״̬ */
} HI_SVR_PLAYER_INFO_S;

/** Subtitle data type */
/** CNcomment:��Ļ�������� */
typedef enum hiFORMAT_SUBTITLE_DATA_TYPE_E
{
    HI_FORMAT_SUBTITLE_DATA_TEXT = 0x0,    /**< Character string *//**< CNcomment:�ַ��� */
    HI_FORMAT_SUBTITLE_DATA_BMP,           /**< BMP picture *//**< CNcomment:bmpͼƬ����������Ч */
    HI_FORMAT_SUBTITLE_DATA_ASS,           /**< ASS subtitle *//**< CNcomment:ass��Ļ */
    HI_FORMAT_SUBTITLE_DATA_HDMV_PGS,      /**< pgs subtitle *//**< CNcomment:pgs��Ļ */
    HI_FORMAT_SUBTITLE_DATA_BMP_SUB,       /**< idx+sub subtitle *//**< CNcomment:sub��Ļ */
    HI_FORMAT_SUBTITLE_DATA_DVB_SUB,       /**< DVB subtitle *//**< CNcomment:DVBsub��Ļ */
    HI_FORMAT_SUBTITLE_DATA_BUTT
} HI_FORMAT_SUBTITLE_DATA_TYPE_E;

/** Type of subtitle */
/** CNcomment: ��Ļ������Ϣ */
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
/** CNcomment:��Ļ��Ϣ */
typedef struct hiFORMAT_SUBTITLE_INFO_S
{
    HI_S32  s32StreamIndex;                            /**< Stream index. The value is the stream PID for TS streams. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:������������ts������ֵΪ��pid���Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID */
    HI_BOOL bExtSubTitle;                              /**< Whether subtitles are external subtitles. When bExtSubTitle is HI_TRUE, the subtitles are external. When bExtSubTitle is HI_FALSE, the subtitles are internal. *//**< CNcomment:�Ƿ�Ϊ�����Ļ, HI_TRUEΪ�����Ļ��HI_FALSEΪ������Ļ */
    HI_FORMAT_SUBTITLE_DATA_TYPE_E eSubtitileType;     /**< Subtitle format, such as BMP and string *//**< CNcomment:��Ļ��ʽ����bmp,string */
    HI_CHAR aszSubTitleName[HI_FORMAT_SUB_TITLE_LEN];  /**< Subtitle title *//**< CNcomment:��Ļ���� */
    HI_U16  u16OriginalFrameWidth;                     /**< Width of the original image *//**< CNcomment:ԭʼͼ���� */
    HI_U16  u16OriginalFrameHeight;                    /**< Height of the original image *//**< CNcomment:ԭʼͼ��߶� */
    HI_U16  u16HorScale;                               /**< Horizontal scaling ratio. The value ranges from 0 to 100. *//**< CNcomment:ˮƽ���űȣ�0-100 */
    HI_U16  u16VerScale;                               /**< Vertical scaling ratio. The value ranges from 0 to 100. *//**< CNcomment:��ֱ���űȣ�0-100 */
    HI_U32  u32ExtradataSize;                          /**< Length of the extended data *//**< CNcomment:��չ���ݳ��� */
    HI_U8   *pu8Extradata;                             /**< Extended data *//**< CNcomment:��չ���� */
    HI_U32  u32SubTitleCodeType;                       /**< Encoding type of the subtitle, the value range is as follows:
                                                            1. The default value is 0.
                                                            2. The value of the u32SubTitleCodeType is the identified byte encoding value if the IdentStream byte encoding function (for details about the definition, see hi_charset_common.h) is set.
                                                            3. If the ConvStream function (for details about the definition, see hi_charset_common.h) is set and the invoke interface is called to set the encoding type to be converted by implementing HI_FORMAT_INVOKE_SUB_SET_CODETYPE, the value of the u32SubTitleCodeType is the configured encoding type */
                                                       /**< CNcomment:��Ļ�������ͣ�ȡֵ��Χ����:
                                                            1. Ĭ��ֵΪ0
                                                            2. ���������IdentStream�ַ�����ʶ����
                                                               (�ο�hi_charset_common.h)���������ֵΪʶ������ַ�����ֵ
                                                            3. ���������ConvStreamת�뺯�� (�ο�hi_charset_common.h)��
                                                              ���ҵ���Invoke�ӿ�ִ��HI_FORMAT_INVOKE_SUB_SET_CODETYPE��
                                                              ��Ҫת���ɵı������ͣ���ֵΪ���õı�������*/
    HI_CHAR aszSubtType[HI_FORMAT_SUB_TYPE_LEN];       /**< Type of subtitle. e.g. pgs,srt etc. *//**< CNcomment:��Ļ����,��pgs,srt�ȵ� */
} HI_FORMAT_SUBTITLE_INFO_S;

/** Audio stream information */
/** CNcomment:��Ƶ����Ϣ */
typedef struct hiFORMAT_AUD_INFO_S
{
    HI_S32 s32StreamIndex;                   /**< Stream index. The value is the stream PID for TS streams. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:������������ts������ֵΪ��pid���Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;                        /**< Audio encoding format. For details about the value definition, see ::HA_FORMAT_E. *//**< CNcomment:��Ƶ�����ʽ��ֵ����ο�::HA_FORMAT_E */
    HI_U32 u32Version;                       /**< Audio encoding version, such as 0x160(WMAV1) and 0x161 (WMAV2). It is valid only for WMA encoding. *//**< CNcomment:��Ƶ����汾������wma���룬0x160(WMAV1), 0x161 (WMAV2) */
    HI_U32 u32SampleRate;                    /**< 8000,11025,441000,... */
    HI_U16 u16BitPerSample;                  /**< Number of bits occupied by each audio sampling point such as 8 bits or 16 bits. *//**< CNcomment:��Ƶÿ����������ռ�ı�������8bit,16bit */
    HI_U16 u16Channels;                      /**< Number of channels, 1 or 2. *//**< CNcomment:������, 1 or 2 */
    HI_U32 u32BlockAlign;                    /**< Number of bytes contained in a packet *//**< CNcomment:packet�������ֽ��� */
    HI_U32 u32Bps;                           /**< Audio bit rate, in the unit of bit/s. *//**< CNcomment:��Ƶ���ʣ�bit/s */
    HI_BOOL bBigEndian;                      /**< Big endian or little endian. It is valid only for the PCM format *//**< CNcomment:��С�ˣ���pcm��ʽ��Ч */
    HI_CHAR aszAudLang[HI_FORMAT_LANG_LEN];  /**< Audio stream language *//**< CNcomment:��Ƶ������ */
    HI_U32 u32ExtradataSize;                 /**< Length of the extended data *//**< CNcomment:��չ���ݳ��� */
    HI_U8  *pu8Extradata;                    /**< Extended data *//**< CNcomment:��չ���� */
    HI_VOID  *pCodecContext;                 /**< Audio decode context *//**< CNcomment:��Ƶ���������� */
    HI_S32 s32SubStreamID;                   /**< Sub audio stream ID *//**< CNcomment:����Ƶ��ID,���ΪTrueHD��Ƶ������ܺ���һ·ac3���� ����ǰ�������Ϊ�����������뿪�������ô��ֶ�����¼��ϵ */
} HI_FORMAT_AUD_INFO_S;

/** Video stream information */
/** CNcomment:��Ƶ����Ϣ */
typedef struct hiFORMAT_VID_INFO_S
{
    HI_S32 s32StreamIndex;        /**< Stream index. The value is the stream PID for TS streams. The invalid value is ::HI_FORMAT_INVALID_STREAM_ID. *//**< CNcomment:������������ts������ֵΪ��pid���Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID */
    HI_U32 u32Format;             /**< Video encoding format. For details about the value definition, see ::HI_UNF_VCODEC_TYPE_E. *//**< CNcomment:��Ƶ�����ʽ��ֵ����ο�::HI_UNF_VCODEC_TYPE_E */
    HI_U16 u16Width;              /**< Width, in the unit of pixel. *//**< CNcomment:��ȣ���λ���� */
    HI_U16 u16Height;             /**< Height, in the unit of pixel. *//**< CNcomment:�߶ȣ���λ���� */
    HI_U16 u16FpsInteger;         /**< Integer part of the frame rate *//**< CNcomment:֡�ʣ��������� */
    HI_U16 u16FpsDecimal;         /**< Decimal part of the frame rate *//**< CNcomment:֡�ʣ�С������ */
    HI_U32 u32Bps;                /**< Video bit rate, in the unit of bit/s. *//**< CNcomment:��Ƶ���ʣ�bit/s */
    HI_U32 u32ExtradataSize;      /**< Length of the extended data *//**< CNcomment:��չ���ݳ��� */
    HI_U8  *pu8Extradata;         /**< Extended data *//**< CNcomment:��չ���� */
    HI_VOID  *pCodecContext;      /**< video decode context *//**< CNcomment:��Ƶ���������� */
} HI_FORMAT_VID_INFO_S;



/** Program information. If a file contains internal subtitles and external subtitles, add the external subtitles to the end of the internal subtitles.
    Note: If a media file does not contain video streams, set videstreamindex to HI_FORMAT_INVALID_STREAM_ID. */
/** CNcomment:��Ŀ��Ϣ������ļ��ȴ�������ĻҲ��������Ļ����������Ļ��Ϣ׷����������Ļ��
    ע: ���ý���ļ�������Ƶ�����轫videstreamindex����ΪHI_FORMAT_INVALID_STREAM_ID */
typedef struct hiFORMAT_PROGRAM_INFO_S
{
    HI_FORMAT_VID_INFO_S stVidStream;                                  /**< Video stream information *//**< CNcomment:��Ƶ����Ϣ */
    HI_U32 u32AudStreamNum;                                            /**< Number of audio streams *//**< CNcomment:��Ƶ������ */
    HI_FORMAT_AUD_INFO_S astAudStream[HI_FORMAT_MAX_STREAM_NUM];       /**< Audio stream information *//**< CNcomment:��Ƶ����Ϣ */
    HI_U32 u32SubTitleNum;                                             /**< Number of subtitles *//**< CNcomment:��Ļ���� */
    HI_FORMAT_SUBTITLE_INFO_S astSubTitle[HI_FORMAT_MAX_LANG_NUM];     /**< Subtitle information *//**< CNcomment:��Ļ��Ϣ */
} HI_FORMAT_PROGRAM_INFO_S;


/** Stream type of the file to be played */
/** CNcomment:�ļ������ͣ����ŵ��ļ����� */
typedef enum hiFORMAT_STREAM_TYPE_E
{
    HI_FORMAT_STREAM_ES = 0x0,    /**< Element stream (ES) file *//**< CNcomment:es���ļ� */
    HI_FORMAT_STREAM_TS,          /**< TS file *//**< CNcomment:ts���ļ� */
    HI_FORMAT_STREAM_NORMAL,      /**< Common files *//**< CNcomment:��ͨ�ļ�����idx,lrc,srt����Ļ�ļ� */
    HI_FORMAT_STREAM_NET,         /**< network stream file *//**< CNcomment:�������ļ� */
    HI_FORMAT_STREAM_LIVE,        /**< live streams *//**< CNcomment:ֱ���������ļ�������Ϣ */
    HI_FPRMAT_STREAM_BUTT
} HI_FORMAT_STREAM_TYPE_E;


/** Media file information */
/** CNcomment:ý���ļ���Ϣ */
typedef struct hiFORMAT_FILE_INFO_S
{
    HI_FORMAT_STREAM_TYPE_E eStreamType;                /**< File stream type *//**< CNcomment:�ļ������� */
    HI_S64  s64FileSize;                                /**< File size, in the unit of byte. *//**< CNcomment:�ļ���С����λ�ֽ� */
    HI_S64  s64StartTime;                               /**< Start time of playing a file, in the unit is ms. *//**< CNcomment:�ļ�������ʼʱ�䣬��λms */
    HI_S64  s64Duration;                                /**< Total duration of a file, in the unit of ms. *//**< CNcomment:�ļ���ʱ������λms */
    HI_U32  u32Bps;                                     /**< File bit rate, in the unit of bit/s. *//**< CNcomment:�ļ����ʣ�bit/s */
    HI_CHAR aszFileName[HI_FORMAT_MAX_FILE_NAME_LEN];   /**< File name *//**< CNcomment:�ļ����� */
    HI_CHAR aszAlbum[HI_FORMAT_TITLE_MAX_LEN];          /**< Album *//**< CNcomment:ר�� */
    HI_CHAR aszTitle[HI_FORMAT_TITLE_MAX_LEN];          /**< Title *//**< CNcomment:���� */
    HI_CHAR aszArtist[HI_FORMAT_TITLE_MAX_LEN];         /**< Artist, author *//**< CNcomment:�����ң����� */
    HI_CHAR aszGenre[HI_FORMAT_TITLE_MAX_LEN];          /**< Genre *//**< CNcomment:��� */
    HI_CHAR aszComments[HI_FORMAT_TITLE_MAX_LEN];       /**< Comments *//**< CNcomment:��ע */
    HI_CHAR aszTime[HI_FORMAT_TIME_LEN];                /**< Created time *//**< CNcomment:������� */
    HI_U32  u32ProgramNum;                              /**< Actual number of programs *//**< CNcomment:ʵ�ʽ�Ŀ���� */
    HI_FORMAT_PROGRAM_INFO_S astProgramInfo[HI_FORMAT_MAX_PROGRAM_NUM];    /**< Program information *//**< CNcomment:��Ŀ��Ϣ */
} HI_FORMAT_FILE_INFO_S;


/** Event type */
/** �¼����� */
typedef enum hiSVR_PLAYER_EVENT_E
{
    HI_SVR_PLAYER_EVENT_STATE = 0x0,   /**< Player status change event. The parameter value is ::HI_SVR_PLAYER_STATE_E. *//**< CNcomment:������״̬ת���¼�������ֵΪ::HI_SVR_PLAYER_STATE_E */
    HI_SVR_PLAYER_EVENT_SOF,           /**< Event indicating that file playing starts or a file is rewound to the file header. The parameter type is HI_U32. The value ::HI_SVR_PLAYER_STATE_PLAY indicates that file playing starts and the value ::HI_SVR_PLAYER_STATE_BACKWARD indicates that a file is rewound to the file header. */
                                       /**< CNcomment:�ļ���ʼ���Ż���˵��ļ�ͷ�¼�����������ΪHI_U32��ֵΪ::HI_SVR_PLAYER_STATE_PLAY��ʾ��ʼ���ţ�����ֵΪ::HI_SVR_PLAYER_STATE_BACKWARD��ʾ���˵��ļ�ͷ */
    HI_SVR_PLAYER_EVENT_EOF,           /**< Event indicating that a file is played till the end of the file. There is no parameter. *//**< CNcomment:�ļ����ŵ�β�¼����޲��� */
    HI_SVR_PLAYER_EVENT_PROGRESS,      /**< Event indicating the current progress of the player. This event is reported once every 500 ms. The parameter is ::HI_SVR_PLAYER_PROGRESS_S. *//**< CNcomment:��������ǰ���Ž����¼���ÿ��300ms�ϱ�һ�θ��¼�������ֵΪ::HI_SVR_PLAYER_PROGRESS_S */
    HI_SVR_PLAYER_EVENT_STREAMID_CHG,  /**< Stream ID change event. The parameter is ::HI_SVR_PLAYER_STREAMID_S. *//**< CNcomment:stream id �����仯�¼�������Ϊ::HI_SVR_PLAYER_STREAMID_S */
    HI_SVR_PLAYER_EVENT_SEEK_FIN,      /**< Seek operation is complete. The parameter is HI_U32. When the value of the parameter is ::HI_FAILURE, the seek operation fails. When the value of the parameter is ::HI_SUCCESS, the seek operation succeeds. When the value of the parameter is ::HI_FORMAT_ERRO_ENDOFFILE, the file is read to the end. */
                                       /**< CNcomment:Seek������ɣ�����ΪHI_U32��ֵΪ::HI_FAILURE Seekʧ�ܣ�::HI_SUCCESS seek�ɹ���::HI_FORMAT_ERRO_ENDOFFILE �ļ���ȡ���ļ�β */
    HI_SVR_PLAYER_EVENT_CODETYPE_CHG,  /**< Event of indicating the byte encoding configuration is complete. The parameter is ::HI_SVR_PLAYER_SUB_CODETYPE_S. *//**< Ncomment:�����ַ���������¼����¼�����::HI_SVR_PLAYER_SUB_CODETYPE_S */
    HI_SVR_PLAYER_EVENT_DOWNLOAD_PROGRESS,   /**< Current download progress of the player. The event is reported every 300 ms. The parameter is ::HI_SVR_PLAYER_PROGRESS_S. *//**< CNcomment:��������ǰ���ؽ��ȣ�ÿ��300ms�ϱ�һ�θ��¼�������ֵΪ::HI_SVR_PLAYER_PROGRESS_S */
    HI_SVR_PLAYER_EVENT_BUFFER,        /**< Reporting buffer status. The parameter type is ::HI_SVR_PLAYER_BUFFER_S. *//**< CNcomment:����״̬�ϱ�,��������Ϊ::HI_SVR_PLAYER_BUFFER_S */
    HI_SVR_PLAYER_EVENT_FIRST_FRAME_TIME,    /**< The display time of the first frame from setting the setMedia. The parameter is ::HI_U32, in the unit of ms. *//**< CNcomment:������ý��setMedia��ʼ��һ֡��ʾʱ��,����Ϊ::HI_U32,��λΪms */

    HI_SVR_PLAYER_EVENT_ERROR,               /**< Event of indicating that an error occurs in the player. The parameter is ::HI_SVR_PLAYER_ERROR_E.*//**< CNcomment:������������Ϣ�¼�������Ϊ::HI_SVR_PLAYER_ERROR_E */
    HI_SVR_PLAYER_EVNET_NETWORK,       /**< Report the network status. The parameter is ::HI_FORMAT_NET_STATUS_S. *//**< CNcomment:����״̬�ϱ�, ����Ϊ::HI_FORMAT_NET_STATUS_S */
    HI_SVR_PLAYER_EVENT_BUTT
} HI_SVR_PLAYER_EVENT_E;


/** Player event callback parameters */
/** CNcomment:�������¼��ص����� */
typedef struct hiSVR_PLAYER_EVENT_S
{
    HI_SVR_PLAYER_EVENT_E  eEvent;    /**< Event type *//**< CNcomment:�¼����� */
    HI_U32  u32Len;                   /**< Event parameter length, in the unit of byte. *//**< CNcomment:�¼��������ȣ��ֽ�Ϊ��λ */
    HI_U8   *pu8Data;                 /**< Start address of event parameter data *//**< CNcomment:�¼�����������ʼ��ַ */
} HI_SVR_PLAYER_EVENT_S;


#define HI_FORMAT_INVALID_STREAM_ID         (-1)





#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __HI_TYPE_H__ */

