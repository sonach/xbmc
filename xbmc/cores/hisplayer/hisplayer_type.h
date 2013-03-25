
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

#define HI_FORMAT_MAX_PROGRAM_NUM           (5)

#define HI_FORMAT_MAX_URL_LEN               (1024)

#define HI_FORMAT_MAX_FILE_NAME_LEN         (512)

#define HI_FORMAT_TITLE_MAX_LEN             (512)

#define HI_FORMAT_LANG_LEN                  (4)

#define HI_FORMAT_SUB_TITLE_LEN             (32)

#define HI_FORMAT_TIME_LEN                  (8)

#define HI_FORMAT_MAX_STREAM_NUM            (32)

#define HI_FORMAT_MAX_LANG_NUM              (64)

typedef enum hiSVR_PLAYER_STATE_E
{
    HI_SVR_PLAYER_STATE_INI = 0,   
    HI_SVR_PLAYER_STATE_DEINI,      
    HI_SVR_PLAYER_STATE_PLAY,       
    HI_SVR_PLAYER_STATE_FORWARD,    
    HI_SVR_PLAYER_STATE_BACKWARD,   
    HI_SVR_PLAYER_STATE_PAUSE,     
    HI_SVR_PLAYER_STATE_STOP,       
    HI_SVR_PLAYER_STATE_BUTT
} HI_SVR_PLAYER_STATE_E;

typedef struct hiSVR_PLAYER_INFO_S
{
    HI_U32  u32Progress;                 
    HI_U64  u64TimePlayed;               
    HI_S32  s32Speed;                    
    HI_SVR_PLAYER_STATE_E  eStatus;     
} HI_SVR_PLAYER_INFO_S;

typedef enum hiFORMAT_SUBTITLE_DATA_TYPE_E
{
    HI_FORMAT_SUBTITLE_DATA_TEXT = 0x0,    
    HI_FORMAT_SUBTITLE_DATA_BMP,           
    HI_FORMAT_SUBTITLE_DATA_ASS,           
    HI_FORMAT_SUBTITLE_DATA_HDMV_PGS,      
    HI_FORMAT_SUBTITLE_DATA_BMP_SUB,       
    HI_FORMAT_SUBTITLE_DATA_DVB_SUB,       
    HI_FORMAT_SUBTITLE_DATA_BUTT
} HI_FORMAT_SUBTITLE_DATA_TYPE_E;

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


typedef struct hiFORMAT_SUBTITLE_INFO_S
{
    HI_S32  s32StreamIndex;                           
    HI_BOOL bExtSubTitle;                             
    HI_FORMAT_SUBTITLE_DATA_TYPE_E eSubtitileType;    
    HI_CHAR aszSubTitleName[HI_FORMAT_SUB_TITLE_LEN];  
    HI_U16  u16OriginalFrameWidth;                     
    HI_U16  u16OriginalFrameHeight;                    
    HI_U16  u16HorScale;                              
    HI_U16  u16VerScale;                               
    HI_U32  u32ExtradataSize;                          
    HI_U8   *pu8Extradata;                             
    HI_U32  u32SubTitleCodeType;                      
                                                         
    HI_CHAR aszSubtType[HI_FORMAT_SUB_TYPE_LEN]; 
} HI_FORMAT_SUBTITLE_INFO_S;

typedef struct hiFORMAT_AUD_INFO_S
{
    HI_S32 s32StreamIndex;                   
    HI_U32 u32Format;                        
    HI_U32 u32Version;                       
    HI_U32 u32SampleRate;                    
    HI_U16 u16BitPerSample;                  
    HI_U16 u16Channels;                      
    HI_U32 u32BlockAlign;                    
    HI_U32 u32Bps;                          
    HI_BOOL bBigEndian;                      
    HI_CHAR aszAudLang[HI_FORMAT_LANG_LEN];  
    HI_U32 u32ExtradataSize;                
    HI_U8  *pu8Extradata;                    
    HI_VOID  *pCodecContext;                 
    HI_S32 s32SubStreamID;                   
} HI_FORMAT_AUD_INFO_S;

typedef struct hiFORMAT_VID_INFO_S
{
    HI_S32 s32StreamIndex;        
    HI_U32 u32Format;             
    HI_U16 u16Width;              
    HI_U16 u16Height;            
    HI_U16 u16FpsInteger;        
    HI_U16 u16FpsDecimal;         
    HI_U32 u32Bps;                
    HI_U32 u32ExtradataSize;      
    HI_U8  *pu8Extradata;         
    HI_VOID  *pCodecContext;      
} HI_FORMAT_VID_INFO_S;


typedef struct hiFORMAT_PROGRAM_INFO_S
{
    HI_FORMAT_VID_INFO_S stVidStream;                                 
    HI_U32 u32AudStreamNum;                                            
    HI_FORMAT_AUD_INFO_S astAudStream[HI_FORMAT_MAX_STREAM_NUM];      
    HI_U32 u32SubTitleNum;                                             
    HI_FORMAT_SUBTITLE_INFO_S astSubTitle[HI_FORMAT_MAX_LANG_NUM];     
} HI_FORMAT_PROGRAM_INFO_S;



typedef enum hiFORMAT_STREAM_TYPE_E
{
    HI_FORMAT_STREAM_ES = 0x0,    
    HI_FORMAT_STREAM_TS,         
    HI_FORMAT_STREAM_NORMAL,      
    HI_FORMAT_STREAM_NET,         
    HI_FORMAT_STREAM_LIVE,       
    HI_FPRMAT_STREAM_BUTT
} HI_FORMAT_STREAM_TYPE_E;


typedef struct hiFORMAT_FILE_INFO_S
{
    HI_FORMAT_STREAM_TYPE_E eStreamType;               
    HI_S64  s64FileSize;                               
    HI_S64  s64StartTime;                               
    HI_S64  s64Duration;                                
    HI_U32  u32Bps;                                     
    HI_CHAR aszFileName[HI_FORMAT_MAX_FILE_NAME_LEN];  
    HI_CHAR aszAlbum[HI_FORMAT_TITLE_MAX_LEN];          
    HI_CHAR aszTitle[HI_FORMAT_TITLE_MAX_LEN];          
    HI_CHAR aszArtist[HI_FORMAT_TITLE_MAX_LEN];        
    HI_CHAR aszGenre[HI_FORMAT_TITLE_MAX_LEN];          
    HI_CHAR aszComments[HI_FORMAT_TITLE_MAX_LEN];       
    HI_CHAR aszTime[HI_FORMAT_TIME_LEN];                
    HI_U32  u32ProgramNum;                              
    HI_FORMAT_PROGRAM_INFO_S astProgramInfo[HI_FORMAT_MAX_PROGRAM_NUM];  
} HI_FORMAT_FILE_INFO_S;


typedef enum hiSVR_PLAYER_EVENT_E
{
    HI_SVR_PLAYER_EVENT_STATE = 0x0,   
    HI_SVR_PLAYER_EVENT_SOF,           
                                       
    HI_SVR_PLAYER_EVENT_EOF,           
    HI_SVR_PLAYER_EVENT_PROGRESS,      
    HI_SVR_PLAYER_EVENT_STREAMID_CHG, 
    HI_SVR_PLAYER_EVENT_SEEK_FIN,      
                                       
    HI_SVR_PLAYER_EVENT_CODETYPE_CHG,  
    HI_SVR_PLAYER_EVENT_DOWNLOAD_PROGRESS,   
    HI_SVR_PLAYER_EVENT_BUFFER,        
    HI_SVR_PLAYER_EVENT_FIRST_FRAME_TIME,    
    HI_SVR_PLAYER_EVENT_ERROR,               
    HI_SVR_PLAYER_EVNET_NETWORK,     
    HI_SVR_PLAYER_EVENT_BUTT
} HI_SVR_PLAYER_EVENT_E;


typedef struct hiSVR_PLAYER_EVENT_S
{
    HI_SVR_PLAYER_EVENT_E  eEvent;    
    HI_U32  u32Len;                   
    HI_U8   *pu8Data;                
} HI_SVR_PLAYER_EVENT_S;


#define HI_FORMAT_INVALID_STREAM_ID         (-1)





#ifdef __cplusplus
}
#endif  

#endif 

