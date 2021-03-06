/*****************************************
 Copyright � 2001-2003  
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/

/**
   @addtogroup basictypes

   These are the Basic Cross Platform (cross-compiler, cross-OS, 
   cross-board, cross-microprocessor) types.
 */

/**
  @file   rmbtypes.h
  @ingroup basictypes
*/


#ifndef __RMBTYPES_H__
#define __RMBTYPES_H__


/** @{ */

/** Generic pointer */
#ifndef NULL
/** pointer not defined */
#define NULL (void *)(0)
#endif // NULL

/** Boolean type */
typedef unsigned char RMbool;
#ifndef TRUE
/** boolean true value. ! some platforms choose `-1' */
#define TRUE (RMbool)1
/** boolean false value.*/
#define FALSE (RMbool)0
#endif // TRUE

/* Per conclusion (11/16/2007 rmdef review) */
#define RMTRUE  TRUE
#define RMFALSE FALSE
#define RMNULL  NULL

#define RMstatus 		RMbool
#define RM_OK 			TRUE
#define RM_ERROR 		FALSE

/** 8 bits long unsigned integer */
typedef unsigned char RMuint8;
/** Maximum value for an 8 bit unsigned integer. */
#define MaxRMuint8 ((RMuint8) 255)
/** Minimum value for an 8 bit unsigned integer. */
#define MinRMuint8 ((RMuint8) 0)

/** 8 bits long integer */
typedef signed char RMint8;
/** Maximum value for an 8 bit signed integer. */
#define MaxRMint8 ((RMint8) 127)
/** Minimum value for an 8 bit signed integer. */
#define MinRMint8 ((RMint8) -128)

/** 16 bits long unsigned integer */
typedef unsigned short RMuint16;
/** Maximum value for a 16 bit unsigned integer. */
#define MaxRMuint16 ((RMuint16)65535)
/** Minimum value for a 16 bit unsigned integer. */
#define MinRMuint16 ((RMuint16)0)

/** 16 bits long integer */
typedef          short RMint16;
/** Maximum value for a 16 bit signed integer. */
#define MaxRMint16 ((RMint16)32767)
/** Minimum value for a 16 bit signed integer. */
#define MinRMint16 ((RMint16)-32768)

/** 32 bits long unsigned integer */
typedef unsigned long RMuint32;
/* constants are in OS dependent headers 
   because big types require postfixes like UL L ULL LL */

/** 32 bits long integer */
typedef          long RMint32;
/* constants are in OS dependent headers 
   because big types require postfixes like UL L ULL LL */

/** floating point number */
typedef double RMreal;

/** 7 bit us-ascii character */
typedef char RMascii;

/** 64 bits long unsigned integer */
typedef unsigned long long RMuint64;
/** maximum 64 bit unsigned integer value. */
#define MaxRMuint64 ((RMuint64)18446744073709551615ULL)
/** minimum 64 bit unsigned integer value. */
#define MinRMuint64 ((RMuint64)0ULL)

/** 64 bits long integer */
typedef long long RMint64;
/** maximum 64 bit signed integer value. */
#define MaxRMint64 ((RMint64)9223372036854775807LL)
/** minimum 64 bit signed integer value. */
#define MinRMint64 ((RMint64)-9223372036854775807LL-1LL)

/** @} */


#endif // __RMBTYPES_H__
