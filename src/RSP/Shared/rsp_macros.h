//---------------------------------------------------------------------------------------
// rsp_macros.h
//
// Copyright © 2004 Gerrit Goossen, <gerrit@mac.com>
//---------------------------------------------------------------------------------------

#ifndef __RSP_MACROS__
#define __RSP_MACROS__

//---------------------------------------------------------------------------------------

#if !defined(__MWERKS__)
#pragma warning( disable: 4127 )
#endif // __MWERKS__

//---------------------------------------------------------------------------------------

#define kVectorSignedMin		-32768
#define kVectorSignedMax		32767

#define Vector_ClampValue(value) {												\
	if ((value) < kVectorSignedMin) (value) = kVectorSignedMin;					\
	else if ((value) > kVectorSignedMax) (value) = kVectorSignedMax;			\
}

#ifdef BIG_ENDIAN
	#define FlagIndex(element)		(element)
#else
	#define FlagIndex(element)		(7 - element)
#endif // BIG_ENDIAN

//---------------------------------------------------------------------------------------


#define VADD_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {		\
	if (wrElement) {																		\
		_s32	result = (((_s16)(s1) + (_s16)(s2)) + ((flag0 >> FlagIndex(index)) & 0x01));\
		Vector_ClampValue(result);															\
		(*(_s16*)&(dest)) = (_s16)result;													\
	}																						\
}


#define VADDC_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrFlag0) {																\
		_u32	result = ((_u16)(s1) + (_u16)(s2));											\
		if (wrFlag0) if (result > 0xFFFF) flag0 |= (1 << FlagIndex(index));					\
		if (wrElement) (*(_u16*)&(dest)) = (_u16)result;									\
	}																						\
}


#define VMACF_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_s32)(_s16)(s1) * (_s32)(_s16)(s2));								\
		_s32	hiValue = state.accum[index].S32[1];										\
		_u32	loValue = ((_u16)(result << 1) + state.accum[index].U16[1]);				\
		if (wrLoAccum) state.accum[index].U16[1] = ((_u16)loValue);							\
		if (wrHiAccum || wrElement) hiValue += ((loValue >> 16) + (result >> 15));			\
		if (wrHiAccum) state.accum[index].S32[1] = hiValue;									\
		if (wrElement) {																	\
			Vector_ClampValue(hiValue);														\
			(*(_s16*)&(dest)) = (_s16)hiValue;												\
		}																					\
	}																						\
}


#define VMADH_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum) {															\
		_s32	result = (((_s16)(s1) * (_s16)(s2)) + state.accum[index].S32[1]);			\
		if (wrHiAccum) state.accum[index].S32[1] = result;									\
		if (wrElement) {																	\
			Vector_ClampValue(result);														\
			(*(_s16*)&(dest)) = (_s16)result;												\
		}																					\
	}																						\
}


#define VMADM_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s64	result = (_s32)((_s32)(_s16)(s1) * (_u32)(_u16)(s2));						\
		_s64	accum = (((_s64)state.accum[index].S32[1] << 16) | state.accum[index].U16[1]);	\
		accum += result;																	\
		if (wrLoAccum) state.accum[index].U16[1] = (_u16)accum;								\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = (_s32)(accum >> 16);											\
			if (wrHiAccum) state.accum[index].S32[1] = hiValue;								\
			if (wrElement) {																\
				Vector_ClampValue(hiValue);													\
				(*(_u16*)&(dest)) = (_s16)hiValue;											\
			}																				\
		}																					\
	}																						\
}


#define VMADN_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_u16)(s1) * (_s16)(s2));											\
		_u32	loValue = state.accum[index].U16[1];										\
		loValue += (_u16)result;															\
		if (wrLoAccum) state.accum[index].U16[1] = (_u16)loValue;							\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = state.accum[index].S32[1];									\
			hiValue += (loValue >> 16);														\
			hiValue += (result >> 16);														\
			if (wrHiAccum) state.accum[index].S32[1] = hiValue;								\
			if (wrElement) {																\
				(*(_u16*)&(dest)) = (_u16)loValue;											\
				if (hiValue > 32767) {														\
					(*(_u16*)&(dest)) = 0xFFFF;												\
				} else if (hiValue < -32768) {												\
					(*(_u16*)&(dest)) = 0x0000;												\
				}																			\
			}																				\
		}																					\
	}																						\
}


#define VMUDH_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum) {															\
		_s32	result = ((_s16)(s1) * (_s16)(s2));											\
		if (wrHiAccum) state.accum[index].S32[1] = result;									\
		if (wrElement) {																	\
			Vector_ClampValue(result);														\
			(*(_s16*)&(dest)) = (_s16)result;												\
		}																					\
	}																						\
	if (wrLoAccum) state.accum[index].U16[1] = 0;											\
}


#define VMUDL_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrLoAccum) {															\
		_u16	result = (_u16)(((_u32)(_u16)(s1) * (_u32)(_u16)(s2)) >> 16);				\
		if (wrElement) (*(_u16*)&(dest)) = result;											\
		if (wrLoAccum) state.accum[index].U16[1] = result;									\
	}																						\
	if (wrHiAccum) state.accum[index].S32[1] = 0;											\
}


#define VMUDM_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_s32)(_s16)(s1) * (_u32)(_u16)(s2));								\
		if (wrLoAccum) state.accum[index].U16[1] = (_u16)result;							\
		if (wrHiAccum) state.accum[index].S32[1] = (result >> 16);							\
		if (wrElement) (*(_u16*)&(dest)) = (_u16)(result >> 16);							\
	}																						\
}

#define VMUDN_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_u32)(_u16)(s1) * (_s32)(_s16)(s2));								\
		if (wrElement) (*(_u16*)&(dest)) = (_u16)result;									\
		if (wrLoAccum) state.accum[index].U16[1] = (_u16)result;							\
		if (wrHiAccum) state.accum[index].S32[1] = (result >> 16);							\
	}																						\
}


#define VMULF_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_s32)(_s16)(s1) * (_s32)(_s16)(s2));								\
		if (wrLoAccum) state.accum[index].U16[1] = (((_u16)(result << 1)) ^ 0x8000);		\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = (result >> 15);												\
			if (result & 0x4000) hiValue++;													\
			if (wrHiAccum) state.accum[index].S32[1] = hiValue;								\
			if (wrElement) {																\
				Vector_ClampValue(hiValue);													\
				(*(_u16*)&(dest)) = (_u16)hiValue;											\
			}																				\
		}																					\
	}																						\
}


#define VSUB_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {		\
	if (wrElement) {																		\
		_s32	result = (((_s16)(s1) - (_s16)(s2)) - ((flag0 >> FlagIndex(index)) & 0x01));\
		Vector_ClampValue(result);															\
		(*(_s16*)&(dest)) = (_s16)result;													\
	}																						\
}


#define VSUBC_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrFlag0) {																\
		_s32	result = ((_s32)(_u16)(s1) - (_s32)(_u16)(s2));								\
		if (wrElement) (*(_u16*)&(dest)) = (_u16)result;									\
		if (wrFlag0) {																		\
			if (result != 0) flag0 |= (1 << (FlagIndex(index) + 8));						\
			if ((result & 0xFFFF0000) != 0) flag0 |= (1 << FlagIndex(index));				\
		}																					\
	}																						\
}


#define VXOR_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {		\
	if (wrElement) (*(_u16*)&(dest)) = ((_u16)(s1) ^ (_u16)(s2));							\
}


//---------------------------------------------------------------------------------------

#endif // __RSP_MACROS__
