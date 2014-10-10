//---------------------------------------------------------------------------------------
// rsp_opcodes.c
//
// Copyright © 2004 Gerrit Goossen, <gerrit@mac.com>
//---------------------------------------------------------------------------------------

#include "rsp_prefix.h"
#include "rsp_opcodes.h"

//---------------------------------------------------------------------------------------

extern t_state		state;

//---------------------------------------------------------------------------------------

#define kVectorSignedMin		-32768
#define kVectorSignedMax		32767

//---------------------------------------------------------------------------------------

#define Vector_ClampedOperation(destValue, operation) {						\
	_s32		__result = (operation);										\
	if (__result < kVectorSignedMin) __result = kVectorSignedMin;			\
	else if (__result > kVectorSignedMax) __result = kVectorSignedMax;		\
	destValue = __result;													\
}

//---------------------------------------------------------------------------------------

#define Vector_ClampValue(__value) {										\
	if (__value < kVectorSignedMin) __value = kVectorSignedMin;				\
	else if (__value > kVectorSignedMax) __value = kVectorSignedMax;		\
}

//---------------------------------------------------------------------------------------

#define Vector_ClearHiAccumulator {											\
	state.accum[0].S32[1] = 0;												\
	state.accum[1].S32[1] = 0;												\
	state.accum[2].S32[1] = 0;												\
	state.accum[3].S32[1] = 0;												\
	state.accum[4].S32[1] = 0;												\
	state.accum[5].S32[1] = 0;												\
	state.accum[6].S32[1] = 0;												\
	state.accum[7].S32[1] = 0;												\
}

//---------------------------------------------------------------------------------------

#define Vector_ClearLoAccumulator {											\
	state.accum[0].U16[1] = 0;												\
	state.accum[1].U16[1] = 0;												\
	state.accum[2].U16[1] = 0;												\
	state.accum[3].U16[1] = 0;												\
	state.accum[4].U16[1] = 0;												\
	state.accum[5].U16[1] = 0;												\
	state.accum[6].U16[1] = 0;												\
	state.accum[7].U16[1] = 0;												\
}

//---------------------------------------------------------------------------------------

#ifdef IGNORE_THIS

//-----------------------------------------------------------------
//| VABS      | Vector Absolute Value                             |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  010011   |
//------6------1----4--------5---------5---------5----------6------

void rsp_cop2_vabs(_s16 *s2, _s16 *s1, _s16 *dest)
{
	_u32		c;

	for (c = 0; c < 8; c++) {
		if (s1[c] > 0) {
			dest[c] = s2[c];
		} else if (s1[c] < 0) {
			if ((_u16)s2[c] == 0x8000) {
				dest[c] = 0x7FFF;
			} else {
				dest[c] = -s2[c];
			}
		} else {
			dest[c] = 0;
		}
	}
}

#endif // IGNORE_THIS

//-----------------------------------------------------------------
//| VAND      | Vector Logical AND                                |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  101000   |
//------6------1----4--------5---------5---------5----------6------

void rsp_cop2_vand(_u32 *dest, _u32 *s1, _u32 *s2)
{
	// note: should be safe to do this 32-bits at a time
	dest[0] = (s1[0] & s2[0]);
	dest[1] = (s1[1] & s2[1]);
	dest[2] = (s1[2] & s2[2]);
	dest[3] = (s1[3] & s2[3]);
}

//-----------------------------------------------------------------

void rsp_cop2_vand_element(_u32 *dest, _u32 *s1, _u16 *s2)
{
	_u32		andValue = (((*s2) << 16) | (*s2));

	dest[0] = (s1[0] & andValue);
	dest[1] = (s1[1] & andValue);
	dest[2] = (s1[2] & andValue);
	dest[3] = (s1[3] & andValue);
}

//-----------------------------------------------------------------

void rsp_cop2_vand_h(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index)
{
	_u32		andValue;

	andValue = ((s2[(7 - (s2index + 4))] << 16) | s2[(7 - (s2index + 4))]);
	dest[0] = (s1[0] & andValue);
	dest[1] = (s1[1] & andValue);
	andValue = ((s2[(7 - (s2index + 0))] << 16) | s2[(7 - (s2index + 0))]);
	dest[2] = (s1[2] & andValue);
	dest[3] = (s1[3] & andValue);
}

//-----------------------------------------------------------------

void rsp_cop2_vand_q(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index)
{
	_u32		andValue;

	andValue = ((s2[(7 - (s2index + 6))] << 16) | s2[(7 - (s2index + 6))]);
	dest[0] = (s1[0] & andValue);
	andValue = ((s2[(7 - (s2index + 4))] << 16) | s2[(7 - (s2index + 4))]);
	dest[1] = (s1[1] & andValue);
	andValue = ((s2[(7 - (s2index + 2))] << 16) | s2[(7 - (s2index + 2))]);
	dest[2] = (s1[2] & andValue);
	andValue = ((s2[(7 - (s2index + 0))] << 16) | s2[(7 - (s2index + 0))]);
	dest[3] = (s1[3] & andValue);
}

//-----------------------------------------------------------------
//| VCL       | Vector Clip Low                                   |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  100100   |
//------6------1----4--------5---------5---------5----------6------

#define VCL_operation(index, _s1, _s2, _dest) {				\
	if ((flag0 & (1 << (7 - index))) != 0) {				\
		if ((flag0 & (1 << ((7 - index) + 8))) != 0) {		\
			if ((flag1 & (1 << (7 - index))) != 0) {		\
				state.accum[index].U16[1] = -(_s2);			\
			} else {										\
				state.accum[index].U16[1] = (_s1);			\
			}												\
		} else {											\
			if ((flag2 & (1 << (7 - index)))) {				\
				if (((_s1) + (_s2)) > 0x10000) {			\
					state.accum[index].U16[1] = (_s1);		\
					flag1 &= (~(1 << (7 - index)));			\
				} else {									\
					state.accum[index].U16[1] = -(_s2);		\
					flag1 |= (1 << (7 - index));			\
				}											\
			} else {										\
				if (((_s2) + (_s1)) != 0) {					\
					state.accum[index].U16[1] = (_s1);		\
					flag1 &= (~(1 << (7 - index)));			\
				} else {									\
					state.accum[index].U16[1] = -(_s2);		\
					flag1 |= (1 << (7 - index));			\
				}											\
			}												\
		}													\
	} else {												\
		if ((flag0 & (1 << ((7 - index) + 8))) != 0) {		\
			if ((flag1 & (1 << ((7 - index) + 8))) != 0) {	\
				state.accum[index].U16[1] = (_s2);			\
			} else {										\
				state.accum[index].U16[1] = (_s1);			\
			}												\
		} else {											\
			if (((_s1) - (_s2)) >= 0) {						\
				state.accum[index].U16[1] = (_s2);			\
				flag1 |= (1 << ((7 - index) + 8));			\
			} else {										\
				state.accum[index].U16[1] = (_s1);			\
				flag1 &= (~(1 << ((7 - index) + 8)));		\
			}												\
		}													\
	}														\
	(_dest) = state.accum[index].U16[1];					\
}

//-----------------------------------------------------------------

void rsp_cop2_vcl(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = state.flag[1];
	_u16		flag2 = state.flag[2];

	VCL_operation(0, s1[0], s2[0], dest[0]);
	VCL_operation(1, s1[1], s2[1], dest[1]);
	VCL_operation(2, s1[2], s2[2], dest[2]);
	VCL_operation(3, s1[3], s2[3], dest[3]);
	VCL_operation(4, s1[4], s2[4], dest[4]);
	VCL_operation(5, s1[5], s2[5], dest[5]);
	VCL_operation(6, s1[6], s2[6], dest[6]);
	VCL_operation(7, s1[7], s2[7], dest[7]);

	state.flag[0] = 0x0000;
	state.flag[1] = flag1;
	state.flag[2] = 0x0000;
}

//-----------------------------------------------------------------

void rsp_cop2_vcl_element(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = state.flag[1];
	_u16		flag2 = state.flag[2];
	_u16		s2element = (*s2);

	VCL_operation(0, s1[0], s2element, dest[0]);
	VCL_operation(1, s1[1], s2element, dest[1]);
	VCL_operation(2, s1[2], s2element, dest[2]);
	VCL_operation(3, s1[3], s2element, dest[3]);
	VCL_operation(4, s1[4], s2element, dest[4]);
	VCL_operation(5, s1[5], s2element, dest[5]);
	VCL_operation(6, s1[6], s2element, dest[6]);
	VCL_operation(7, s1[7], s2element, dest[7]);

	state.flag[0] = 0x0000;
	state.flag[1] = flag1;
	state.flag[2] = 0x0000;
}

//-----------------------------------------------------------------

void rsp_cop2_vcl_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = state.flag[1];
	_u16		flag2 = state.flag[2];
	_u16		s2value;

	s2value = s2[(7 - (s2index + 4))];
	VCL_operation(0, s1[0], s2value, dest[0]);
	VCL_operation(1, s1[1], s2value, dest[1]);
	VCL_operation(2, s1[2], s2value, dest[2]);
	VCL_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 0))];
	VCL_operation(4, s1[4], s2value, dest[4]);
	VCL_operation(5, s1[5], s2value, dest[5]);
	VCL_operation(6, s1[6], s2value, dest[6]);
	VCL_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0x0000;
	state.flag[1] = flag1;
	state.flag[2] = 0x0000;
}

//-----------------------------------------------------------------

void rsp_cop2_vcl_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = state.flag[1];
	_u16		flag2 = state.flag[2];
	_u16		s2value;

	s2value = s2[(7 - (s2index + 6))];
	VCL_operation(0, s1[0], s2value, dest[0]);
	VCL_operation(1, s1[1], s2value, dest[1]);
	s2value = s2[(7 - (s2index + 4))];
	VCL_operation(2, s1[2], s2value, dest[2]);
	VCL_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 2))];
	VCL_operation(4, s1[4], s2value, dest[4]);
	VCL_operation(5, s1[5], s2value, dest[5]);
	s2value = s2[(7 - (s2index + 0))];
	VCL_operation(6, s1[6], s2value, dest[6]);
	VCL_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0x0000;
	state.flag[1] = flag1;
	state.flag[2] = 0x0000;
}

//-----------------------------------------------------------------
//| VEQ       | Vector EQual comparison                           |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  100001   |
//------6------1----4--------5---------5---------5----------6------

#define VEQ_operation(index, s1, s2) {					\
	if (s1 == s2) resultFlag |= (1 << (7 - index));		\
	dest[index] = s2;									\
}

void rsp_cop2_veq(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;

	VEQ_operation(0, s1[0], s2[0]);
	VEQ_operation(1, s1[1], s2[1]);
	VEQ_operation(2, s1[2], s2[2]);
	VEQ_operation(3, s1[3], s2[3]);
	VEQ_operation(4, s1[4], s2[4]);
	VEQ_operation(5, s1[5], s2[5]);
	VEQ_operation(6, s1[6], s2[6]);
	VEQ_operation(7, s1[7], s2[7]);

	// clear elements set by zero bit
	resultFlag &= (~(flag0 >> 8));

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_veq_element(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_s32		s2element = (*s2);
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;

	VEQ_operation(0, s1[0], s2element);
	VEQ_operation(1, s1[1], s2element);
	VEQ_operation(2, s1[2], s2element);
	VEQ_operation(3, s1[3], s2element);
	VEQ_operation(4, s1[4], s2element);
	VEQ_operation(5, s1[5], s2element);
	VEQ_operation(6, s1[6], s2element);
	VEQ_operation(7, s1[7], s2element);

	// clear elements set by zero bit
	resultFlag &= (~(flag0 >> 8));

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_veq_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;
	_s32		s2value;

	s2value = s2[(7 - (s2index + 4))];
	VEQ_operation(0, s1[0], s2value);
	VEQ_operation(1, s1[1], s2value);
	VEQ_operation(2, s1[2], s2value);
	VEQ_operation(3, s1[3], s2value);
	s2value = s2[(7 - (s2index + 0))];
	VEQ_operation(4, s1[4], s2value);
	VEQ_operation(5, s1[5], s2value);
	VEQ_operation(6, s1[6], s2value);
	VEQ_operation(7, s1[7], s2value);

	// clear elements set by zero bit
	resultFlag &= (~(flag0 >> 8));

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_veq_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;
	_s32		s2value;

	s2value = s2[(7 - (s2index + 6))];
	VEQ_operation(0, s1[0], s2value);
	VEQ_operation(1, s1[1], s2value);
	s2value = s2[(7 - (s2index + 4))];
	VEQ_operation(2, s1[2], s2value);
	VEQ_operation(3, s1[3], s2value);
	s2value = s2[(7 - (s2index + 2))];
	VEQ_operation(4, s1[4], s2value);
	VEQ_operation(5, s1[5], s2value);
	s2value = s2[(7 - (s2index + 0))];
	VEQ_operation(6, s1[6], s2value);
	VEQ_operation(7, s1[7], s2value);

	// clear elements set by zero bit
	resultFlag &= (~(flag0 >> 8));

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------
//| VGE       | Vector Greater than or Equal comparison           |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  100011   |
//------6------1----4--------5---------5---------5----------6------

#define VGE_operation(index, s1, s2, dest) {								\
	if (s1 == s2) {															\
		dest = s1;															\
		if ((flag0 & (0x101 << (7 - index))) == (0x101 << (7 - index))) {	\
			resultFlag &= ~(1 << (7 - index));								\
		} else {															\
			resultFlag |= (1 << (7 - index));								\
		}																	\
	} else if (s1 > s2) {													\
		dest = s1;															\
		resultFlag |= (1 << (7 - index));									\
	} else {																\
		dest = s2;															\
		resultFlag &= ~(1 << (7 - index));									\
	}																		\
}

void rsp_cop2_vge(_s16 *dest, _s16 *s1, _s16 *s2)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;

	VGE_operation(0, s1[0], s2[0], dest[0]);
	VGE_operation(1, s1[1], s2[1], dest[1]);
	VGE_operation(2, s1[2], s2[2], dest[2]);
	VGE_operation(3, s1[3], s2[3], dest[3]);
	VGE_operation(4, s1[4], s2[4], dest[4]);
	VGE_operation(5, s1[5], s2[5], dest[5]);
	VGE_operation(6, s1[6], s2[6], dest[6]);
	VGE_operation(7, s1[7], s2[7], dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_vge_element(_s16 *dest, _s16 *s1, _s16 *s2)
{
	_s32		s2element = (*s2);
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;

	VGE_operation(0, s1[0], s2element, dest[0]);
	VGE_operation(1, s1[1], s2element, dest[1]);
	VGE_operation(2, s1[2], s2element, dest[2]);
	VGE_operation(3, s1[3], s2element, dest[3]);
	VGE_operation(4, s1[4], s2element, dest[4]);
	VGE_operation(5, s1[5], s2element, dest[5]);
	VGE_operation(6, s1[6], s2element, dest[6]);
	VGE_operation(7, s1[7], s2element, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_vge_h(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;
	_s32		s2value;

	s2value = s2[(7 - (s2index + 4))];
	VGE_operation(0, s1[0], s2value, dest[0]);
	VGE_operation(1, s1[1], s2value, dest[1]);
	VGE_operation(2, s1[2], s2value, dest[2]);
	VGE_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 0))];
	VGE_operation(4, s1[4], s2value, dest[4]);
	VGE_operation(5, s1[5], s2value, dest[5]);
	VGE_operation(6, s1[6], s2value, dest[6]);
	VGE_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_vge_q(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;
	_s32		s2value;

	s2value = s2[(7 - (s2index + 6))];
	VGE_operation(0, s1[0], s2value, dest[0]);
	VGE_operation(1, s1[1], s2value, dest[1]);
	s2value = s2[(7 - (s2index + 4))];
	VGE_operation(2, s1[2], s2value, dest[2]);
	VGE_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 2))];
	VGE_operation(4, s1[4], s2value, dest[4]);
	VGE_operation(5, s1[5], s2value, dest[5]);
	s2value = s2[(7 - (s2index + 0))];
	VGE_operation(6, s1[6], s2value, dest[6]);
	VGE_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------
//| VLT       | Vector Less Than comparison                       |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  100000   |
//------6------1----4--------5---------5---------5----------6------

#define VLT_operation(index, s1, s2, dest) {								\
	if (s1 == s2) {															\
		dest = s1;															\
		if ((flag0 & (0x101 << (7 - index))) == (0x101 << (7 - index))) {	\
			resultFlag |= (1 << (7 - index));								\
		}																	\
	} else if (s1 < s2) {													\
		dest = s1;															\
		resultFlag |= (1 << (7 - index));									\
	} else {																\
		dest = s2;															\
	}																		\
}

void rsp_cop2_vlt(_s16 *dest, _s16 *s1, _s16 *s2)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;

	VLT_operation(0, s1[0], s2[0], dest[0]);
	VLT_operation(1, s1[1], s2[1], dest[1]);
	VLT_operation(2, s1[2], s2[2], dest[2]);
	VLT_operation(3, s1[3], s2[3], dest[3]);
	VLT_operation(4, s1[4], s2[4], dest[4]);
	VLT_operation(5, s1[5], s2[5], dest[5]);
	VLT_operation(6, s1[6], s2[6], dest[6]);
	VLT_operation(7, s1[7], s2[7], dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_vlt_element(_s16 *dest, _s16 *s1, _s16 *s2)
{
	_s32		s2element = (*s2);
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;

	VLT_operation(0, s1[0], s2element, dest[0]);
	VLT_operation(1, s1[1], s2element, dest[1]);
	VLT_operation(2, s1[2], s2element, dest[2]);
	VLT_operation(3, s1[3], s2element, dest[3]);
	VLT_operation(4, s1[4], s2element, dest[4]);
	VLT_operation(5, s1[5], s2element, dest[5]);
	VLT_operation(6, s1[6], s2element, dest[6]);
	VLT_operation(7, s1[7], s2element, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_vlt_h(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;
	_s32		s2value;

	s2value = s2[(7 - (s2index + 4))];
	VLT_operation(0, s1[0], s2value, dest[0]);
	VLT_operation(1, s1[1], s2value, dest[1]);
	VLT_operation(2, s1[2], s2value, dest[2]);
	VLT_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 0))];
	VLT_operation(4, s1[4], s2value, dest[4]);
	VLT_operation(5, s1[5], s2value, dest[5]);
	VLT_operation(6, s1[6], s2value, dest[6]);
	VLT_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------

void rsp_cop2_vlt_q(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		resultFlag = 0;
	_s32		s2value;

	s2value = s2[(7 - (s2index + 6))];
	VLT_operation(0, s1[0], s2value, dest[0]);
	VLT_operation(1, s1[1], s2value, dest[1]);
	s2value = s2[(7 - (s2index + 4))];
	VLT_operation(2, s1[2], s2value, dest[2]);
	VLT_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 2))];
	VLT_operation(4, s1[4], s2value, dest[4]);
	VLT_operation(5, s1[5], s2value, dest[5]);
	s2value = s2[(7 - (s2index + 0))];
	VLT_operation(6, s1[6], s2value, dest[6]);
	VLT_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = resultFlag;
}

//-----------------------------------------------------------------
//| VMRG      | Vector Merge                                      |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  100111   |
//------6------1----4--------5---------5---------5----------6------

#define VMRG_operation(index, value1, value2) {			\
	if ((flag1 & (1 << (7 - index))) != 0) {			\
		dest[index] = value1;							\
	} else {											\
		dest[index] = value2;							\
	}													\
}

void rsp_cop2_vmrg(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_u32		flag1 = state.flag[1];

	VMRG_operation(0, s1[0], s2[0]);
	VMRG_operation(1, s1[1], s2[1]);
	VMRG_operation(2, s1[2], s2[2]);
	VMRG_operation(3, s1[3], s2[3]);
	VMRG_operation(4, s1[4], s2[4]);
	VMRG_operation(5, s1[5], s2[5]);
	VMRG_operation(6, s1[6], s2[6]);
	VMRG_operation(7, s1[7], s2[7]);
}

//-----------------------------------------------------------------

void rsp_cop2_vmrg_element(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_u32		flag1 = state.flag[1];
	_u16		s2element = (*s2);

	VMRG_operation(0, s1[0], s2element);
	VMRG_operation(1, s1[1], s2element);
	VMRG_operation(2, s1[2], s2element);
	VMRG_operation(3, s1[3], s2element);
	VMRG_operation(4, s1[4], s2element);
	VMRG_operation(5, s1[5], s2element);
	VMRG_operation(6, s1[6], s2element);
	VMRG_operation(7, s1[7], s2element);
}

//-----------------------------------------------------------------

void rsp_cop2_vmrg_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u32		flag1 = state.flag[1];
	_u16		s2value;

	s2value = s2[(7 - (s2index + 4))];
	VMRG_operation(0, s1[0], s2value);
	VMRG_operation(1, s1[1], s2value);
	VMRG_operation(2, s1[2], s2value);
	VMRG_operation(3, s1[3], s2value);
	s2value = s2[(7 - (s2index + 0))];
	VMRG_operation(4, s1[4], s2value);
	VMRG_operation(5, s1[5], s2value);
	VMRG_operation(6, s1[6], s2value);
	VMRG_operation(7, s1[7], s2value);
}

//-----------------------------------------------------------------

void rsp_cop2_vmrg_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u32		flag1 = state.flag[1];
	_u16		s2value;

	s2value = s2[(7 - (s2index + 6))];
	VMRG_operation(0, s1[0], s2value);
	VMRG_operation(1, s1[1], s2value);
	s2value = s2[(7 - (s2index + 4))];
	VMRG_operation(2, s1[2], s2value);
	VMRG_operation(3, s1[3], s2value);
	s2value = s2[(7 - (s2index + 2))];
	VMRG_operation(4, s1[4], s2value);
	VMRG_operation(5, s1[5], s2value);
	s2value = s2[(7 - (s2index + 0))];
	VMRG_operation(6, s1[6], s2value);
	VMRG_operation(7, s1[7], s2value);
}

//-----------------------------------------------------------------
//| VNE       | Vector Not-Equal comparison                       |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  100010   |
//------6------1----4--------5---------5---------5----------6------

#define VNE_operation(_index, _s1, _s2, _dest) {			\
	if ((_s1) != (_s2)) {									\
		flag1 |= (1 << (7 - _index));						\
	} else {												\
		if ((flag0 & (1 << ((7 - _index) + 8))) != 0)		\
			flag1 |= (1 << (7 - _index));					\
	}														\
	(_dest) = (_s1);										\
}

void rsp_cop2_vne(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = 0;

	VNE_operation(0, s1[0], s2[0], dest[0]);
	VNE_operation(1, s1[1], s2[1], dest[1]);
	VNE_operation(2, s1[2], s2[2], dest[2]);
	VNE_operation(3, s1[3], s2[3], dest[3]);
	VNE_operation(4, s1[4], s2[4], dest[4]);
	VNE_operation(5, s1[5], s2[5], dest[5]);
	VNE_operation(6, s1[6], s2[6], dest[6]);
	VNE_operation(7, s1[7], s2[7], dest[7]);

	state.flag[0] = 0;
	state.flag[1] = flag1;
}

//-----------------------------------------------------------------

void rsp_cop2_vne_element(_u16 *dest, _u16 *s1, _u16 *s2)
{
	_s32		s2element = (*s2);
	_u16		flag0 = state.flag[0];
	_u16		flag1 = 0;

	VNE_operation(0, s1[0], s2element, dest[0]);
	VNE_operation(1, s1[1], s2element, dest[1]);
	VNE_operation(2, s1[2], s2element, dest[2]);
	VNE_operation(3, s1[3], s2element, dest[3]);
	VNE_operation(4, s1[4], s2element, dest[4]);
	VNE_operation(5, s1[5], s2element, dest[5]);
	VNE_operation(6, s1[6], s2element, dest[6]);
	VNE_operation(7, s1[7], s2element, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = flag1;
}

//-----------------------------------------------------------------

void rsp_cop2_vne_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = 0;
	_u16		s2value;

	s2value = s2[(7 - (s2index + 4))];
	VNE_operation(0, s1[0], s2value, dest[0]);
	VNE_operation(1, s1[1], s2value, dest[1]);
	VNE_operation(2, s1[2], s2value, dest[2]);
	VNE_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 0))];
	VNE_operation(4, s1[4], s2value, dest[4]);
	VNE_operation(5, s1[5], s2value, dest[5]);
	VNE_operation(6, s1[6], s2value, dest[6]);
	VNE_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = flag1;
}

//-----------------------------------------------------------------

void rsp_cop2_vne_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index)
{
	_u16		flag0 = state.flag[0];
	_u16		flag1 = 0;
	_u16		s2value;

	s2value = s2[(7 - (s2index + 6))];
	VNE_operation(0, s1[0], s2value, dest[0]);
	VNE_operation(1, s1[1], s2value, dest[1]);
	s2value = s2[(7 - (s2index + 4))];
	VNE_operation(2, s1[2], s2value, dest[2]);
	VNE_operation(3, s1[3], s2value, dest[3]);
	s2value = s2[(7 - (s2index + 2))];
	VNE_operation(4, s1[4], s2value, dest[4]);
	VNE_operation(5, s1[5], s2value, dest[5]);
	s2value = s2[(7 - (s2index + 0))];
	VNE_operation(6, s1[6], s2value, dest[6]);
	VNE_operation(7, s1[7], s2value, dest[7]);

	state.flag[0] = 0;
	state.flag[1] = flag1;
}

//-----------------------------------------------------------------
//| VOR       | Vector OR                                         |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  101010   |
//------6------1----4--------5---------5---------5----------6------

void rsp_cop2_vor(_u32 *dest, _u32 *s1, _u32 *s2)
{
	// note: should be safe to do this 32bits at a time
	dest[0] = (s1[0] | s2[0]);
	dest[1] = (s1[1] | s2[1]);
	dest[2] = (s1[2] | s2[2]);
	dest[3] = (s1[3] | s2[3]);
}

//-----------------------------------------------------------------

void rsp_cop2_vor_element(_u32 *dest, _u32 *s1, _u16 *s2)
{
	_u32		s2element = ((*s2) | ((*s2) << 16));

	// note: should be safe to to do this 32-bits at a time
	dest[0] = (s1[0] | s2element);
	dest[1] = (s1[1] | s2element);
	dest[2] = (s1[2] | s2element);
	dest[3] = (s1[3] | s2element);
}

//-----------------------------------------------------------------

void rsp_cop2_vor_h(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index)
{
	_u32		s2value;

	// note: should be safe to to do this 32-bits at a time
	s2value = ((s2[(7 - (s2index + 4))] << 16) | s2[(7 - (s2index + 4))]);
	dest[0] = (s1[0] | s2value);
	dest[1] = (s1[1] | s2value);
	s2value = ((s2[(7 - (s2index + 0))] << 16) | s2[(7 - (s2index + 0))]);
	dest[2] = (s1[2] | s2value);
	dest[3] = (s1[3] | s2value);
}

//-----------------------------------------------------------------

void rsp_cop2_vor_q(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index)
{
	_u32		s2value;

	// note: should be safe to to do this 32-bits at a time
	s2value = ((s2[(7 - (s2index + 6))] << 16) | s2[(7 - (s2index + 6))]);
	dest[0] = (s1[0] | s2value);
	s2value = ((s2[(7 - (s2index + 4))] << 16) | s2[(7 - (s2index + 4))]);
	dest[1] = (s1[1] | s2value);
	s2value = ((s2[(7 - (s2index + 2))] << 16) | s2[(7 - (s2index + 2))]);
	dest[2] = (s1[2] | s2value);
	s2value = ((s2[(7 - (s2index + 0))] << 16) | s2[(7 - (s2index + 0))]);
	dest[3] = (s1[3] | s2value);
}

#ifdef IGNORE_THIS

//-----------------------------------------------------------------
//| VRCP      | Vector Reciprocal                                 |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  110000   |
//------6------1----4--------5---------5---------5----------6------

void rsp_cop2_vrcp(_s16 *src, _u16 *dest)
{
	_s32		value = (_s32)(*src);
	_s32		count, neg;

	if (value == 0) {
		value = 0x7FFFFFFF;
	} else {
		if (value < 0) {
			neg = true;
			value = (~value + 1);
		} else {
			neg = false;
		}
		for (count = 15; count > 0; count--) {
			if ((value & (1 << count))) {
				value &= (0xFFC0 >> (15 - count));
				count = 0;
			}
		}
		value = (0x7FFFFFFF / value);
		for (count = 31; count > 0; count--) {
			if ((value & (1 << count))) {
				value &= (0xFFFF8000 >> (31 - count));
				count = 0;
			}
		}
		if (neg == true) {
			value = (~value);
		}
	}

//	for (count = 0; count < 8; count++) {
//		RSP_ACCUM[count].HW[1] = RSP_Vect[RSPOpC.rt].UHW[EleSpec[RSPOpC.rs].B[count]];
//	}

	(*dest) = (_u16)value;
	state->cop2highValue = (_u16)(value >> 16);
}

//-----------------------------------------------------------------
//| VRCPH     | Vector Reciprocal High                            |
//|-----------|---------------------------------------------------|
//|  010010   |1|  sel  |   src   |0|  del  |  dest   |  110000   |
//------6------1----4--------5-----1----4--------5----------6------

void rsp_cop2_vrcph(_u16 *src, _u16 *dest)
{
	(*dest) = state->cop2highValue;
	state->cop2highSource = (*src);
}

//-----------------------------------------------------------------
//| VRCPL     | Vector Reciprocal Low                             |
//|-----------|---------------------------------------------------|
//|  010010   |1|  sel  |   src   |0|  del  |  dest   |  110001   |
//------6------1----4--------5-----1----4--------5----------6------

void rsp_cop2_vrcpl(_u16 *src, _u16 *dest)
{
	_s32		value = (_s32)(((_u32)state->cop2highSource << 16) | (*src));
	_s32		count, neg;

	if (value == 0) {
		value = 0x7FFFFFFF;
	} else if (value == 0x80000000) {
		value = 0xFFFFFFFE;
//		value = 0xFFFF0080;
	} else {
		if (value < 0) {
			neg = true;
//			if (((_s16)value == 0xFFFF) && ((value >> 16) < 0)) {
//			if (((value >> 16) == 0xFFFF) && ((_s16)value < 0)) {
//			if (RecpResult.UHW[1] == 0xFFFF && RecpResult.HW[0] < 0) {
				value = (~value + 1);
//			} else {
//				value = (~value);
//			}
		} else {
			neg = false;
		}
		for (count = 31; count > 0; count--) {
			if ((value & (1 << count))) {
				value &= (0xFFC00000 >> (31 - count));
				count = 0;
			}
		}
		value = (0x7FFFFFFF / value);
		for (count = 31; count > 0; count--) {
			if ((value & (1 << count))) {
				value &= (0xFFFF8000 >> (31 - count));
				count = 0;
			}
		}
		if (neg == true) {
			value = (~value);
		}
	}

//	for ( count = 0; count < 8; count++ ) {
//		RSP_ACCUM[count].HW[1] = RSP_Vect[RSPOpC.rt].UHW[EleSpec[RSPOpC.rs].B[count]];
//	}
//	RSP_Vect[RSPOpC.sa].HW[7 - (RSPOpC.rd & 0x7)] = RecpResult.UHW[0];

	(*dest) = (_u16)value;
	state->cop2highValue = (_u16)(value >> 16);
}
/*
{
	_s32		value = (_s32)(((_u32)state->cop2highSource << 16) | (*src));
	double		doubleValue;

	doubleValue = (float)doubleValue;
	doubleValue /= 65535.0;
	doubleValue = (0.4999963 / doubleValue);
	doubleValue *= 65535.0;
	value = (_s32)doubleValue;

	(*dest) = (_u16)value;
	state->cop2highValue = (_u16)(value >> 16);
}
*/

#endif // IGNORE_THIS

//-----------------------------------------------------------------
//| VXOR      | Vector Logical Exclusive OR                       |
//|-----------|---------------------------------------------------|
//|  010010   |1|  el   |   s2    |   s1    |  dest   |  101100   |
//------6------1----4--------5---------5---------5----------6------

void rsp_cop2_vxor(_u32 *dest, _u32 *s1, _u32 *s2)
{
	// note: should be safe to to do this 32-bits at a time
	if (s1 == s2)
	{
		dest[0] = 0;
		dest[1] = 0;
		dest[2] = 0;
		dest[3] = 0;
	}
	else
	{
		dest[0] = (s1[0] ^ s2[0]);
		dest[1] = (s1[1] ^ s2[1]);
		dest[2] = (s1[2] ^ s2[2]);
		dest[3] = (s1[3] ^ s2[3]);
	}
}

//-----------------------------------------------------------------

void rsp_cop2_vxor_element(_u32 *dest, _u32 *s1, _u16 *s2)
{
	_u32		s2element = ((*s2) | ((*s2) << 16));

	// note: should be safe to to do this 32-bits at a time
	dest[0] = (s1[0] ^ s2element);
	dest[1] = (s1[1] ^ s2element);
	dest[2] = (s1[2] ^ s2element);
	dest[3] = (s1[3] ^ s2element);
}

//-----------------------------------------------------------------

void rsp_cop2_vxor_h(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index)
{
	_u32		s2value;

	// note: should be safe to to do this 32-bits at a time
	s2value = ((s2[(7 - (s2index + 4))] << 16) | s2[(7 - (s2index + 4))]);
	dest[0] = (s1[0] ^ s2value);
	dest[1] = (s1[1] ^ s2value);
	s2value = ((s2[(7 - (s2index + 0))] << 16) | s2[(7 - (s2index + 0))]);
	dest[2] = (s1[2] ^ s2value);
	dest[3] = (s1[3] ^ s2value);
}

//-----------------------------------------------------------------

void rsp_cop2_vxor_q(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index)
{
	_u32		s2value;

	// note: should be safe to to do this 32-bits at a time
	s2value = ((s2[(7 - (s2index + 6))] << 16) | s2[(7 - (s2index + 6))]);
	dest[0] = (s1[0] ^ s2value);
	s2value = ((s2[(7 - (s2index + 4))] << 16) | s2[(7 - (s2index + 4))]);
	dest[1] = (s1[1] ^ s2value);
	s2value = ((s2[(7 - (s2index + 2))] << 16) | s2[(7 - (s2index + 2))]);
	dest[2] = (s1[2] ^ s2value);
	s2value = ((s2[(7 - (s2index + 0))] << 16) | s2[(7 - (s2index + 0))]);
	dest[3] = (s1[3] ^ s2value);
}
