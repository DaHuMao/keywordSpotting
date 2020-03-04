/**************************************************************************//*
* @file  hisf_alg.h
*
* @date  2017-12-29
*
* @brief  Copyright (c) 2017 - 2022. Horizon Robotics. All rights reserved.	
*	
*****************************************************************************/

#ifndef HISF_ALG
#define HISF_ALG

#ifndef NULL
#define NULL 0
#endif
#if defined(__cplusplus)
extern "C"
{
#endif
/**********************************************************************************************//**
 * @def MAX_EXT_VAD_NUM.
 *
 * @brief   A macro that defines maximum ext vad number.
 *          
 * @seealso 
 **************************************************************************************************/
#define MAX_EXT_VAD_NUM (2)

/**********************************************************************************************//**
 * @def	HISF_DATA_SIZE.
 *
 * @brief	A macro that defines HISF maximum buffer size(Byte).
 **************************************************************************************************/
#define HISF_DATA_SIZE    10712544     
/**********************************************************************************************//**
 * @def	HISF_FRAME_INC.
 *
 * @brief	A macro that defines HISF frame increment.
 **************************************************************************************************/
#define HISF_FRAME_INC    256
/**********************************************************************************************//**
 * @def	MAX_HISF_MIC_NUM.
 *
 * @brief	A macro that defines maximum HISF Mic number.
 **************************************************************************************************/
#define MAX_HISF_MIC_NUM      2

/**********************************************************************************************//**
 * @def	HISF_PARAM_RESERVE_SIZE
 *
 * @brief	A macro that defines HISF parameter reserve size.
 **************************************************************************************************/
#define HISF_PARAM_RESERVE_SIZE  1024

#define HISF_ASR_BACKTRACKING_FRAME_NUM 60

#define HISF_WAKE_WORD_MAX_NUM 4

/*****************return value of HISF*************************/ 
#define HISF_SUCCESS        0                    /// success
#define HISF_POINT_NULL     1                    /// null point
#define HISF_BUFF_SIZE_ERR  2                    /// buffer size error 
#define HISF_PARAM_SIZE_ERR 3                    /// param error
#define HISF_AGC_FLAG_ERR   4                    /// AGC flag error
#define HISF_AEC_FLAG_ERR   5                    /// AEC flag error
#define HISF_DOA_FLAG_ERR   6                    /// DOA error
#define HISF_NS_FLAG_ERR    7                    /// NS error
#define HISF_BF_FLAG_ERR    8                    /// BF error
#define HISF_BF_AZIMUTH_ERR 9                    /// bf_azimuth error
#define HISF_VAD_MODE_ERR   10                   /// VAD error
#define HISF_READ_CONF_ERR  11                   /// config file error
#define HISF_MODE_ERR       12                   /// HISF mode error
#define HISF_DRB_FLAG_ERR   13                   /// DRB flag error
#define HISF_WAVE_TYPE_ERR  14                   /// The type of wave error
#define HISF_PARAM_ERR		15					 /// parameter err
#define HISF_INIT_ERR       100                  /// init error in module
#define HISF_DOA_ERR        201                  /// HisfProc is not called before DOA is called

/*****************DOA updata mode*************************/
#define EXT_DOA_UPDATE_OFF 0                     /// prevent to update DOA 
#define EXT_DOA_UPDATE_ON  1                     /// allow to update DOA

/**************** HISF mode ******************************/
#define HISF_MODE            0     /// wake up mode
#define HISF_TRAIN           3     /// training mode

/**********************************************************************************************//**
 * @enum    THisfWkpChan.
 *
 * @brief   Values that represent the system is woken up by which channel data. It is used when
 *          the system has been woken and hisf_mode was from 'HISF_WKP' to 'HISF_ASR'.
 *
 * @seealso HisfOut
 **************************************************************************************************/
typedef enum _THisfWkpChan
{
    /** disable the function */
    HISF_DEC_DISABLE = 0,
    /** The system is woken up by enhanced_out in struct T_HisfOut*/
    HISF_DEC_CH1,
    /** The system is woken up by enhanced_out1 in struct T_HisfOut*/
    HISF_DEC_CH2,
    /** The system is woken up by both of enhanced_out and enhanced_out1 in struct T_HisfOut*/
    HISF_DEC_TWO_CH,
    /** End flag of this enum */
    HISF_DEC_END
}T_HisfWkpChan;

typedef enum HisfWaveType
{
	/** 16 bits of wave data are valid */
	HISF_WAVE_INT16 = 0,
	/**  24 bits of wave data are valid */
	HISF_WAVE_INT24,
	/**  32 bits of wave data are valid */
	HISF_WAVE_INT32
}T_HisfWaveType;

typedef enum _THisfExtVad
{
	/** unknown ext vad info */
	kExtVadNull = -1,
	/**  vad is inactive */
	kExtVadOff = 0,
	/** vad is active */
	kExtVadOn = 1
}THisfExtVad;

typedef enum _HisfGetRefEngThreshold
{
	/** no ref singnal exist */
	HISF_REF_ENG_NULL = 0,
	/** ref energe at low level */
	HISF_REF_ENG_LOW,
	/** ref energe at middle level */
	HISF_REF_ENG_MIDDLE,
	/** ref energe at high level */
	HISF_REF_ENG_HIGH
}T_HisfGetRefEngThreshold;

 /**********************************************************************************************//**
  * @typedef	struct _HisfOut
  *
  * @brief	Defines an alias representing the HISF out.
  *
  * @seealso	.
  **************************************************************************************************/
 typedef struct _HisfOut
 {
	 /** The enhanced out[ HISF frame inc]. */
	 short enhanced_out[2 * HISF_FRAME_INC];
	 /** The enhanced out 1[ HISF frame inc]. */
	 short enhanced_out1[2 * HISF_FRAME_INC];
     /** The audio for the recognition online*/
     short asr_audio[2 * HISF_FRAME_INC];
	 /** The doa data[ maximum hisf mic num*hisf frame inc]. */
	 short doa_data[MAX_HISF_MIC_NUM * HISF_FRAME_INC];
	 /** The VAD flag. */
	 int vad_flag;
	 /** The first VAD flag. */
	 int vad_flag1;
	 /** The update flag. */
	 int update_flag;
	 /** The VAD for ASR */
	 int asr_vad;
     /** The azimuth from HISF */
     int azimuth;
	 /** The ref eng threshold for decoder */
	 T_HisfGetRefEngThreshold ref_eng_threshold;
 }T_HisfOut;

 /**********************************************************************************************//**
  * Defines an alias representing the decoder timestamp.
  *
  * @seealso	.
  **************************************************************************************************/
 typedef struct _DecTimestamp
 {
	 int timestamp_num;
	 int dec_frame_mil_sec; // decoder one frame for the number of millisecond
	 long long dec_wkp_idx[HISF_WAKE_WORD_MAX_NUM]; // contain the first word of the wake-up word, if has more, then is the second word of the wake-up word, and so on
	 int timestamp_check[2]; // data
 }T_DecTimestamp;

/**********************************************************************************************//**
 * @typedef	struct _HisfParam
 *
 * @brief	Defines an alias representing the HISF parameter.
 *
 * @seealso	.
 **************************************************************************************************/
typedef struct _HisfParam
{
	/** The point of HISF buffer */
	void *p_data;
	/** The size to p_data */
	int buff_size;
	/** sizeof(T_HisfParam) */
	int param_size;
	/** The DOA update flag.see DOA update mode */
	int doa_update_flag;
	/** The azimuth where speech will be enhanced from */
	int bf_azimuth;
	/** The HISF mode.see HISF mode */
	int hisf_mode;
    /** See T_HisfWkpChan. It must be HISF_DEC_DISABLE if hisf_mode is HISF_WKP.
    It is set value from T_HisfWkpChan if hisf_mode is HISF_ASR */
    int wkp_ch;
	/** Volume of the speaker set by user */
	int spk_volume;
	/** Input is 16bit or 24bit or 32bit */
	T_HisfWaveType in_wave_type;
	/** Output is 16bit or 24bit or 32bit */
	T_HisfWaveType out_wave_type;
	/** ASR model active */
	int asr_active;
	/** asr word get set 1, else default set 0 **/
	int wake_rewake;
	// for ASR VAD start
	T_DecTimestamp dec_timestamp;
	/** @brief   The ext vad active[ maximum ext vad num].*/
	THisfExtVad ext_vad_active_type[MAX_EXT_VAD_NUM];
    /** The HISF parameter reserve buff. */
	char hisf_param_reserve_buff[HISF_PARAM_RESERVE_SIZE];
}T_HisfParam;


typedef enum _HisfExtDataTag
{
    /** long snr for two channel  */
    HISF_EXT_LONG_SNR = 0,

    HISF_EXT_TAG_END
}T_THisfExtDataTag;
/**********************************************************************************************//**
 * HISF get version.
 *
 * @author	chengguangwei.
 * @date	2018/01/20.
 *
 * @return	null if it fails, else a pointer to a const char.
 *
 * @seealso	.
 **************************************************************************************************/
const char * HisfGetVersion();

/**********************************************************************************************//**
 * HISF initial.
 *
 * @author	zhuchangbao.
 * @date	2016/10/27.
 *
 * @param	in_wave_type	Type of the in wave.
 *
 * @param [in,out]	pm 	If non-null, the pm.
 * @param	config_file	The configuration file.
 *
 * @return	An int.
 *
 * @seealso	.
 *
 * @param	out_wave_type	Type of the out wave.
 **************************************************************************************************/
int  HisfInit(T_HisfParam *pm, const char *config_file);

/**********************************************************************************************//**
 * HISF Delay Estimation. It must be called after HisfInit.
 *
 * @author	chengguangwei.
 * @date	2018/5/15.
 *
 * @param [in,out]	pm	HISF parameter.
 *
 * @return	An int delay time in point.
 *
 * @seealso	.
 **************************************************************************************************/
int  HisfDelayEst(T_HisfParam *pm);

/**********************************************************************************************//**
 * HISF process.
 *
 * @author	zhuchangbao.
 * @date	2017/10/18.
 *
 * @param [in,out]	pm			HISF parameter.
 * @param [in ]	inp				input audio as 16bit.
 * @param [in ]	aec_ref			AEC reference audio as 16bit.
 * @param [in,out]	hisf_out	the output from HISF.
 *
 * @return	An int. see "return value of HISF".
 *
 * @seealso	.
 **************************************************************************************************/
int  HisfProc(T_HisfParam *pm, void *inp, T_HisfOut *hisf_out);

/**********************************************************************************************//**
 * Get the buffer size for p_data.
 *
 * @author	chengguangwei.
 * @date	2018/5/15.
 *
 * @param [in,out]	pm   	- HISF parameter.
 * @param [in]	conf_file	- config file.
 *
 * @return	An int. The maximum size is returned if conf_file is null. The size is returned if
 * 			input is normal. An negative value is returned if config file error.
 *
 * @seealso	.
 **************************************************************************************************/
int HisfGetBuffSize(T_HisfParam *pm, const char *conf_file);

/**********************************************************************************************//**
 * HISF destroy.
 *
 * @author	zhuchangbao/chengguangwei.
 * @date	2018/1/6.
 *
 * @param [in,out]	pm	If non-null, the pm.
 *
 * @seealso	.
 **************************************************************************************************/
void HisfDestroy(T_HisfParam *pm);

/**********************************************************************************************//**
 * HISF update. It must run in an independent thread on-line, and it can be run in a thread
 * offline. The update signal is form HisfProc.
 *
 * @author	chengguangwei.
 * @date	2018/5/15.
 *
 * @param [in,out]	pm	HISF parameter.
 *
 * @return	An int. see "return value of HISF".
 *
 * @seealso	.
 **************************************************************************************************/
int HisfUpdate(T_HisfParam * pm);

/**********************************************************************************************//**
 * HISF Get call_frame_num frame ASR Data. when wakeup and asr_vad is true, call back
 * call_frame_num's data.
 *
 * @author	chengguangwei.
 * @date	2018/9/10.
 *
 * @param [in,out]	pm			HISF parameter.
 * @param [in,out]	out_wave	If non-null, the ASR out wave.
 * @param	call_frame_num  	The call frame numbers.
 * @param	force_get			The force get flag. non-zero force get data.
 *
 * @return	An int32. 0 succed; -1:argument error; -2:buffer less call_frame_num or buffer is
 * 			empty;
 *
 * @seealso	.
 **************************************************************************************************/
int  HisfGetAsr(T_HisfParam *pm, void *out_wave, int call_frame_num, int force_get);

/**********************************************************************************************//**
 * @fn  int HisfGetDataToExt(T_HisfParam *pm, T_THisfExtDataTag tag, void *data, int byte_num);
 *
 * @brief   Hisf get data to extent.
 *
 * @author  Zhifei.zhang.
 * @date    2018/11/13.
 *
 * @param [in,out]  pm      If non-null, the pm.
 * @param   tag             The tag.
 * @param [in,out]  data    If non-null, the data. In case for SNR , the range is 0-20. Normally value above 10
 *                  indicates a high possibility for good env. And value between 4 is normally for bad env.
 * @param   byte_num        The whole input size in bytes
 *
 * @return  An int. 0 indeicates success
 *
 * @seealso .
 **************************************************************************************************/

int  HisfGetDataToExt(T_HisfParam *pm, T_THisfExtDataTag tag, void *data, int byte_num);

#if defined(__cplusplus)
}
#endif

#endif
