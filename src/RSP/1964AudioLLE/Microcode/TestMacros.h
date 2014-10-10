
_u16 accumulator[8];

typedef union {
	_s32			S32;
	_u32			U32;
	_u16			U16[2];
} HiAccumType;

HiAccumType accumulator_hi[8];

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
		_s32	hiValue = accumulator_hi[index].S32;										\
		_u32	loValue = ((_u16)(result << 1) + accumulator[index]);				\
		if (wrLoAccum) accumulator[index] = ((_u16)loValue);							\
		if (wrHiAccum || wrElement) hiValue += ((loValue >> 16) + (result >> 15));			\
		if (wrHiAccum) accumulator_hi[index].S32 = hiValue;									\
		if (wrElement) {																	\
			Vector_ClampValue(hiValue);														\
			(*(_s16*)&(dest)) = (_s16)hiValue;												\
		}																					\
	}																						\
}


#define VMADH_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum) {															\
		_s32	result = (((_s16)(s1) * (_s16)(s2)) + accumulator_hi[index].S32);			\
		if (wrHiAccum) accumulator_hi[index].S32 = result;									\
		if (wrElement) {																	\
			Vector_ClampValue(result);														\
			(*(_s16*)&(dest)) = (_s16)result;												\
		}																					\
	}																						\
}

//modified by schibo
//#define VMADM_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s64	result = (_s32)((_s32)(_s16)(s1) * (_u32)(_u16)(s2));						\
		_s64	accum = (((_s64)accumulator_hi[index].S32 << 16) | accumulator[index]);	\
		accum += result;																	\
		if (wrLoAccum) accumulator[index] = (_u16)accum;								\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = (_s32)(accum >> 16);											\
			if (wrHiAccum) accumulator_hi[index].S32 = hiValue;								\
			if (wrElement) {																\
				Vector_ClampValue(hiValue);													\
				(*(_u16*)&(dest)) = (_s16)hiValue;											\
			}																				\
		}																					\
	}																						\
}

#define VMADM_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_s32)(_s16)(s1) * (_u32)(_u16)(s2));								\
		_s32	loValue = accumulator[index];												\
		loValue += (_u16)result;															\
		if (wrLoAccum) accumulator[index] = (_u16)loValue;									\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = accumulator_hi[index].S32;									\
			hiValue += (loValue >> 16);														\
			hiValue += (result >> 16);														\
			if (wrHiAccum) accumulator_hi[index].S32 = hiValue;								\
			if (wrElement) {																\
				Vector_ClampValue(hiValue);													\
				(*(_u16*)&(dest)) = (_u16)hiValue;											\
			}																				\
		}																					\
	}																						\
}


#define VMADN_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		/*_s32	result = ((_u16)(s1) * (_s16)(s2));	changed by schibo*/						\
		_s32	result = ((_u32)(_u16)(s1) * (_s32)(_s16)(s2));								\
		_u32	loValue = accumulator[index];										\
		loValue += (_u16)result;															\
		if (wrLoAccum) accumulator[index] = (_u16)loValue;							\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = accumulator_hi[index].S32;									\
			hiValue += (loValue >> 16);														\
			hiValue += (result >> 16);														\
			if (wrHiAccum) accumulator_hi[index].S32 = hiValue;								\
			if (wrElement) {																\
				if (hiValue > 32767) {														\
					(*(_u16*)&(dest)) = 0xFFFF;												\
				} else if (hiValue < -32768) {												\
					(*(_u16*)&(dest)) = 0x0000;												\
				} else																		\
					(*(_u16*)&(dest)) = (_u16)loValue;										\
			}																				\
		}																					\
	}																						\
}


#define VMUDH_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum) {															\
		_s32	result = ((_s16)(s1) * (_s16)(s2));											\
		if (wrHiAccum) accumulator_hi[index].S32 = result;									\
		if (wrElement) {																	\
			Vector_ClampValue(result);														\
			(*(_s16*)&(dest)) = (_s16)result;												\
		}																					\
	}																						\
	if (wrLoAccum) accumulator[index] = 0;											\
}


#define VMUDL_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrLoAccum) {															\
		_u16	result = (_u16)(((_u32)(_u16)(s1) * (_u32)(_u16)(s2)) >> 16);				\
		if (wrElement) (*(_u16*)&(dest)) = result;											\
		if (wrLoAccum) accumulator[index] = result;									\
	}																						\
	if (wrHiAccum) accumulator_hi[index].S32 = 0;											\
}


#define VMUDM_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_s32)(_s16)(s1) * (_u32)(_u16)(s2));								\
		if (wrLoAccum) accumulator[index] = (_u16)result;							\
		if (wrHiAccum) accumulator_hi[index].S32 = (result >> 16);							\
		if (wrElement) (*(_u16*)&(dest)) = (_u16)(result >> 16);							\
	}																						\
}

#define VMUDN_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_u32)(_u16)(s1) * (_s32)(_s16)(s2));								\
		if (wrElement) (*(_u16*)&(dest)) = (_u16)result;									\
		if (wrLoAccum) accumulator[index] = (_u16)result;							\
		if (wrHiAccum) accumulator_hi[index].S32 = (result >> 16);							\
	}																						\
}


#define VMULF_operation(index, dest, s1, s2, wrElement, wrHiAccum, wrLoAccum, wrFlag0) {	\
	if (wrElement || wrHiAccum || wrLoAccum) {												\
		_s32	result = ((_s32)(_s16)(s1) * (_s32)(_s16)(s2));								\
		if (wrLoAccum) accumulator[index] = (((_u16)(result << 1)) ^ 0x8000);		\
		if (wrElement || wrHiAccum) {														\
			_s32	hiValue = (result >> 15);												\
			if (result & 0x4000) hiValue++;													\
			if (wrHiAccum) accumulator_hi[index].S32 = hiValue;								\
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
