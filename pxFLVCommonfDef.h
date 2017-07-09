#ifndef _PX_FLV_COMMON_DEF_H
#define _PX_FLV_COMMON_DEF_H

/* 
 * 
 * Zhaoliang Guo
 * 1799580752@qq.com
 * http://www.jianshu.com/u/b971a1d12a6f
 * 
 * FLV Parser 
 * Base on Adobe Flash Video File Format Specification Version 10.1
 * 2017-01-14
 * 
 */

#pragma pack(1)

const unsigned int FLV_HEADER_LEN     = 9;
const unsigned int FLV_TAG_HEADER_LEN = 11;

const unsigned int FLV_VIDEO_CODEC_ID_AVC = 7;
const unsigned int FLV_AUDIO_CODEC_ID_AAC = 10;

// Tag Type
typedef enum EPxTagType
{
	kePxTagType_Invalid = -1,
	kePxTagType_Audio   = 8,   // AUDIODATA
	kePxTagType_Video   = 9,   // VIDEODATA
	kePxTagType_Script  = 18,  // SCRIPTDATAOBJECT
	kePxTagType_Cnt
}EPxTagType;

// FLV File Header (9 Bytes)
/*
	Field             Type      Comment
	Signature         UI8       Signature byte always 'F' (0x46)
	Signature         UI8       Signature byte always 'L' (0x4C)
	Signature         UI8       Signature byte always 'V' (0x56)
	Version           UI8       File version (for example, 0x01 for FLV version 1)
	TypeFlagsReserved UB[5]     Must be 0
	TypeFlagsAudio    UB[1]     Audio tags are present
	TypeFlagsReserved UB[1]     Must be 0
	TypeFlagsVideo    UB[1]     Video tags are present
	DataOffset        UI32      The length of this header in bytes
*/

typedef struct SPxFLVFileHeader
{
	unsigned char uchSignature1;             // Signature byte always 'F' (0x46)
	unsigned char uchSignature2;             // Signature byte always 'L' (0x4C)
	unsigned char uchSignature3;             // Signature byte always 'V' (0x56)
	unsigned char uchVersion;                // File version (for example, 0x01 for FLV version 1)
	unsigned char uchTypeFlagsVideo     : 1; // 1 = Video tags are present 
	unsigned char uchTypeFlagsReserved1 : 1; // Shall be 0
	unsigned char uchTypeFlagsAudio     : 1; // 1 = Audio tags are present 
	unsigned char uchTypeFlagsReserved0 : 5; // Shall be 0
	unsigned int  uiDataOffset;              // The length of this header in bytes
}SPxFLVFileHeader;

// FLV Tag Header (11 Bytes)
typedef struct SPxFLVTagHeader
{
	unsigned char uchTagType;           // Type of contents in this tag. 8 = audio; 9 = video; 18 = script data.
	unsigned char uchDataSize[3];       // Length of the message. Number of bytes after StreamID to
	                                    // end of tag (Equal to length of the tag ¨C 11)
	unsigned char uchTimestamp[3];      // Time in milliseconds at which the data in this tag applies.
	                                    // This value is relative to the first tag in the FLV file, which always has a timestamp of 0.
	unsigned char uchTimestampExtended; // Extension of the Timestamp field to form a SI32 value. This
	                                    // field represents the upper 8 bits, while the previous
		                                // Timestamp field represents the lower 24 bits of the time in milliseconds.
	unsigned char uchStreamID[3];       // Always 0. 
}SPxFLVTagHeader;

typedef struct SPxVideoTagHeader
{
	/*
		Codec Identifier. 

		The following values are defined:
		2 = Sorenson H.263
		3 = Screen video
		4 = On2 VP6
		5 = On2 VP6 with alpha channel
		6 = Screen video version 2
		7 = AVC
	*/
	unsigned char uchCodecID   : 4; 

	/*  
		Type of video frame.

		The following values are defined:
	    1 = key frame (for AVC, a seekable frame)
		2 = inter frame (for AVC, a non-seekable frame)
		3 = disposable inter frame (H.263 only)
		4 = generated key frame (reserved for server use only)
		5 = video info/command frame
	*/
	unsigned char uchFrameType : 4;


	/* 
		Exist IF CodecID == 7

	    0 = AVC sequence header
	    1 = AVC NALU
	    2 = AVC end of sequence (lower level NALU sequence ender is not required or supported)
	*/
	unsigned char uchAVCPacketType;  

	/*
	    Exist IF CodecID == 7

		IF AVCPacketType == 1
		Composition time offset
		ELSE
		0
	*/
	char          chCompositionTime[3]; 
}SPxVideoTagHeader;


typedef struct SPxAudioTagHeader
{
	/*
		SoundType 
		UB [1] 

		Mono or stereo sound
		0 = Mono sound
		1 = Stereo sound
	*/
	unsigned char uchSoundType : 1;

	/*
		SoundSize 
		UB [1] 

		Size of each audio sample. 
		This parameter only pertains to uncompressed formats. 
		Compressed formats always decode to 16 bits internally.
		0 = 8-bit samples
		1 = 16-bit samples
	*/
	unsigned char uchSoundSize : 1;

	/*
		SoundRate 
		UB [2] 

		Sampling rate. 
		The following values are defined:
		0 = 5.5 kHz
		1 = 11 kHz
		2 = 22 kHz
		3 = 44 kHz
	*/
	unsigned char uchSoundRate : 2;

	/*
		SoundFormat
		UB [4] 
		Format of SoundData. 
		The following values are defined:
		0 = Linear PCM, platform endian
		1 = ADPCM
		2 = MP3
		3 = Linear PCM, little endian
		4 = Nellymoser 16 kHz mono
		5 = Nellymoser 8 kHz mono
		6 = Nellymoser
		7 = G.711 A-law logarithmic PCM
		8 = G.711 mu-law logarithmic PCM
		9 = reserved
		10 = AAC
		11 = Speex
		14 = MP3 8 kHz
		15 = Device-specific sound

		Formats 7, 8, 14, and 15 are reserved.

		AAC is supported in Flash Player 9,0,115,0 and higher.
		Speex is supported in Flash Player 10 and higher.
	*/
	unsigned char uchSoundFormat : 4;

	/*
		AACPacketType 
		Exist IF SoundFormat == 10
		UI8
		
		The following values are defined:
		0 = AAC sequence header
		1 = AAC raw
    */
	unsigned char uchAACPacketType;
}SPxAudioTagHeader;

typedef struct SPxFLVTagHeaderEx
{
	SPxFLVTagHeader   sPxFLVTagHeader;
	SPxVideoTagHeader sPxVideoTagHeader;
    SPxAudioTagHeader sPxAudioTagHeader;
}SPxFLVTagHeaderEx;

typedef enum EPxAVC_NALU_Type
{
	kePxAVC_NALU_Type_Invalid    = -1,
	kePxAVC_NALU_Type_NAL        = 0,
	kePxAVC_NALU_Type_SLICE      = 1,
	kePxAVC_NALU_Type_SLICE_DPA  = 2,
	kePxAVC_NALU_Type_SLICE_DPB  = 3,
	kePxAVC_NALU_Type_SLICE_DPC  = 4,
	kePxAVC_NALU_Type_SLICE_IDR  = 5,
	kePxAVC_NALU_Type_SEI        = 6,
	kePxAVC_NALU_Type_SPS        = 7,
	kePxAVC_NALU_Type_PPS        = 8,
	kePxAVC_NALU_Type_Cnt
}EPxAVC_NALU_Type;

enum EPxAMFDataType
{ 
	AMF_NUMBER  = 0, 
	AMF_BOOLEAN = 1, 
	AMF_STRING  = 2, 
	AMF_OBJECT,
	AMF_MOVIECLIP,		/* reserved, not used */
	AMF_NULL, 
	AMF_UNDEFINED, 
	AMF_REFERENCE, 
	AMF_ECMA_ARRAY = 8, 
	AMF_OBJECT_END = 9,
	AMF_STRICT_ARRAY, 
	AMF_DATE, 
	AMF_LONG_STRING, 
	AMF_UNSUPPORTED,
	AMF_RECORDSET,		/* reserved, not used */
	AMF_XML_DOC, 
	AMF_TYPED_OBJECT,
	AMF_AVMPLUS,		/* switch to AMF3 */
	AMF_INVALID = 0xFF
};

/*
	Type of video frame. 
	
	The following values are defined:
	1 = key frame (for AVC, a seekable frame)
	2 = inter frame (for AVC, a non-seekable frame)
	3 = disposable inter frame (H.263 only)
	4 = generated key frame (reserved for server use only)
	5 = video info/command frame
*/
typedef enum SPxVideoFrameType
{
	kePxVideoFrameType_Invalid               = -1,
	kePxVideoFrameType_KeyFrame              = 1, // I frame
	kePxVideoFrameType_InterFrame            = 2, // P frame or B frame
	kePxVideoFrameType_Disposable_InterFrame = 3, 
	kePxVideoFrameType_GeneratedKeyFram      = 4,
	kePxVideoFrameType_VideoInfo             = 5,
	kePxVideoFrameType_Cnt
}SPxVideoFrameType;

/*
	CodecID 

	Codec Identifier. 

	The following values are defined:
	2 = Sorenson H.263
	3 = Screen video
	4 = On2 VP6
	5 = On2 VP6 with alpha channel
	6 = Screen video version 2
	7 = AVC
*/
typedef enum SPxVideoCodecID
{
	kePxVideoCodecID_Invalid                    = -1,
	kePxVideoCodecID_Sorenson_H_263             = 2, 
	kePxVideoCodecID_Screen_video               = 3,
	kePxVideoCodecID_On2_VP6                    = 4,
	kePxVideoCodecID_On2_VP6_with_alpha_channel = 5,
    kePxVideoCodecID_Screen_video_version_2     = 6, 
	kePxVideoCodecID_AVC                        = 7,
	kePxVideoCodecID_Cnt
}SPxVideoCodecID;

/*
	AVCPacketType IF CodecID == 7

	The following values are defined:
	0 = AVC sequence header
	1 = AVC NALU
	2 = AVC end of sequence (lower level NALU sequence ender is not required or supported)
*/
typedef enum SPxAVCPacketType
{
	kePxAVCPacketType_Invalid             = -1,
	kePxAVCPacketType_AVC_Sequence_Header = 0,
	kePxAVCPacketType_AVC_NALU            = 1,
	kePxAVCPacketType_AVC_End_Of_Sequence = 2,
	kePxAVCPacketType_Cnt
}SPxAVCPacketType;

/*
	SoundType 
	Mono or stereo sound
	0 = Mono sound
	1 = Stereo sound
*/
typedef enum SPxSoundType
{
	kePxSoundType_Invalid = -1,
	kePxSoundType_Mono    = 0,
	kePxSoundType_Stereo  = 1,
	kePxSoundType_Cnt
}SPxSoundType;

/*
	SoundSize  
	Size of each audio sample. 
	
	This parameter only pertains to uncompressed formats. 
	Compressed formats always decode to 16 bits internally.

	0 = 8-bit samples
	1 = 16-bit samples
*/
typedef enum SPxSoundSize
{
	kePxSoundSize_Invalid      = -1,
	kePxSoundSize_Sample_8bit  = 0,
	kePxSoundSize_Sample_16bit = 1,
	kePxSoundSize_Cnt
}SPxSoundSize;

/*
	SoundRate 
	
	Sampling rate. 
	
	The following values are defined:
	0 = 5.5 kHz
	1 = 11 kHz
	2 = 22 kHz
	3 = 44 kHz
*/
typedef enum SPxSoundRate
{
	kePxSoundRate_Invalid = -1,
	kePxSoundRate_5500Hz  = 0,
	kePxSoundRate_11kHz   = 1,
	kePxSoundRate_22kHz   = 2,
	kePxSoundRate_44kHz   = 3,
	kePxSoundRate_Cnt
}SPxSoundRate;

/*
	SoundFormat

	Format of SoundData. 
	
	The following values are defined:
	0 = Linear PCM, platform endian
	1 = ADPCM
	2 = MP3
	3 = Linear PCM, little endian
	4 = Nellymoser 16 kHz mono
	5 = Nellymoser 8 kHz mono
	6 = Nellymoser
	7 = G.711 A-law logarithmic PCM
	8 = G.711 mu-law logarithmic PCM
	9 = reserved
	10 = AAC
	11 = Speex
	14 = MP3 8 kHz
	15 = Device-specific sound
	Formats 7, 8, 14, and 15 are reserved.
	AAC is supported in Flash Player 9,0,115,0 and higher.
	Speex is supported in Flash Player 10 and higher.
*/

typedef enum SPxSoundFormat
{
	kePxSoundFormat_Invalid = -1,
	kePxSoundFormat_Linear_PCM_platform_endian = 0,
	kePxSoundFormat_ADPCM                      = 1,
	kePxSoundFormat_MP3                        = 2,
	kePxSoundFormat_Linear_PCM_little_endian   = 3,
	kePxSoundFormat_Nellymoser_16kHz_Mono      = 4,
	kePxSoundFormat_Nellymoser_8kHz_Mono       = 5,
	kePxSoundFormat_Nellymoser                 = 6,
	kePxSoundFormat_G_711_A_law_PCM            = 7,
	kePxSoundFormat_G_711_mu_law_PCM           = 8,
	kePxSoundFormat_reserved                   = 9,
	kePxSoundFormat_AAC                        = 10,
	kePxSoundFormat_Speex                      = 11,
	kePxSoundFormat_MP3_8kHz                   = 14,
	kePxSoundFormat_Device_Specific            = 15,
	kePxSoundFormat_Cnt,
}SPxSoundFormat;

#endif