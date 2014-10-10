//---------------------------------------------------------------------------------------
// rsp_opcodes.h
//
// Copyright © 2004 Gerrit Goossen, <gerrit@mac.com>
//---------------------------------------------------------------------------------------

#ifndef __RSP_OPCODES__
#define __RSP_OPCODES__

//---------------------------------------------------------------------------------------

#include "state.h"

//---------------------------------------------------------------------------------------

#define RSP_BUILD_FLAG0_RD			(1 << 0)	// must load flag0 for op
#define RSP_BUILD_FLAG0_WR			(1 << 1)	// write flag0 at end of op
#define RSP_BUILD_FLAG0_CLEAR		(1 << 2)	// clear flag0 at end of op

//---------------------------------------------------------------------------------------

void rsp_cop2_vand(_u32 *dest, _u32 *s1, _u32 *s2);
void rsp_cop2_vand_element(_u32 *dest, _u32 *s1, _u16 *s2);
void rsp_cop2_vand_h(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vand_q(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vcl(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_vcl_element(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_vcl_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vcl_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_veq(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_veq_element(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_veq_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_veq_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vge(_s16 *dest, _s16 *s1, _s16 *s2);
void rsp_cop2_vge_element(_s16 *dest, _s16 *s1, _s16 *s2);
void rsp_cop2_vge_h(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index);
void rsp_cop2_vge_q(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index);
void rsp_cop2_vlt(_s16 *dest, _s16 *s1, _s16 *s2);
void rsp_cop2_vlt_element(_s16 *dest, _s16 *s1, _s16 *s2);
void rsp_cop2_vlt_h(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index);
void rsp_cop2_vlt_q(_s16 *dest, _s16 *s1, _s16 *s2, _u32 s2index);
void rsp_cop2_vmrg(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_vmrg_element(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_vmrg_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vmrg_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vne(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_vne_element(_u16 *dest, _u16 *s1, _u16 *s2);
void rsp_cop2_vne_h(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vne_q(_u16 *dest, _u16 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vor(_u32 *dest, _u32 *s1, _u32 *s2);
void rsp_cop2_vor_element(_u32 *dest, _u32 *s1, _u16 *s2);
void rsp_cop2_vor_h(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vor_q(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vxor(_u32 *dest, _u32 *s1, _u32 *s2);
void rsp_cop2_vxor_element(_u32 *dest, _u32 *s1, _u16 *s2);
void rsp_cop2_vxor_h(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index);
void rsp_cop2_vxor_q(_u32 *dest, _u32 *s1, _u16 *s2, _u32 s2index);

//---------------------------------------------------------------------------------------

#endif // __RSP_OPCODES__
