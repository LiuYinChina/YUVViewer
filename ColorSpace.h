
#ifndef _COLORSPACE_H
#define _COLORSPACE_H

typedef unsigned char uint8_t;

/* initialize tables */

void colorspace_init(void);

/* colorspace conversion function (encoder) */

typedef void (packedFunc)
(
	uint8_t * x_ptr,
	int x_stride,
	uint8_t * y_src,
	uint8_t * v_src,
	uint8_t * u_src,
	int y_stride,
	int uv_stride,
	int width,
	int height,
	int vflip
);

/* plain c */
packedFunc rgb555_to_yv12_c;
packedFunc rgb565_to_yv12_c;
packedFunc rgb_to_yv12_c;
packedFunc bgr_to_yv12_c;
packedFunc bgra_to_yv12_c;
packedFunc abgr_to_yv12_c;
packedFunc rgba_to_yv12_c;
packedFunc argb_to_yv12_c;
packedFunc yuyv_to_yv12_c;
packedFunc uyvy_to_yv12_c;

packedFunc rgb555i_to_yv12_c;
packedFunc rgb565i_to_yv12_c;
packedFunc rgbi_to_yv12_c;
packedFunc bgri_to_yv12_c;
packedFunc bgrai_to_yv12_c;
packedFunc abgri_to_yv12_c;
packedFunc rgbai_to_yv12_c;
packedFunc argbi_to_yv12_c;
packedFunc yuyvi_to_yv12_c;
packedFunc uyvyi_to_yv12_c;

#if defined(ARCH_IS_IA32) || defined(ARCH_IS_X86_64)
/* mmx */
packedFunc bgr_to_yv12_mmx;
packedFunc rgb_to_yv12_mmx;
packedFunc bgra_to_yv12_mmx;
packedFunc rgba_to_yv12_mmx;
packedFunc yuyv_to_yv12_mmx;
packedFunc uyvy_to_yv12_mmx;

/* 3dnow */
packedFunc yuyv_to_yv12_3dn;
packedFunc uyvy_to_yv12_3dn;

/* xmm */
packedFunc yuyv_to_yv12_xmm;
packedFunc uyvy_to_yv12_xmm;
#endif

#ifdef ARCH_IS_PPC
packedFunc bgra_to_yv12_altivec_c;
packedFunc abgr_to_yv12_altivec_c;
packedFunc rgba_to_yv12_altivec_c;
packedFunc argb_to_yv12_altivec_c;

packedFunc yuyv_to_yv12_altivec_c;
packedFunc uyvy_to_yv12_altivec_c;
#endif

/* plain c */
packedFunc yv12_to_rgb555_c;
packedFunc yv12_to_rgb565_c;
packedFunc yv12_to_rgb_c;
packedFunc yv12_to_bgr_c;
packedFunc yv12_to_bgra_c;
packedFunc yv12_to_abgr_c;
packedFunc yv12_to_rgba_c;
packedFunc yv12_to_argb_c;
packedFunc yv12_to_yuyv_c;
packedFunc yv12_to_uyvy_c;

packedFunc yv12_to_rgb555i_c;
packedFunc yv12_to_rgb565i_c;
packedFunc yv12_to_rgbi_c;
packedFunc yv12_to_bgri_c;
packedFunc yv12_to_bgrai_c;
packedFunc yv12_to_abgri_c;
packedFunc yv12_to_rgbai_c;
packedFunc yv12_to_argbi_c;
packedFunc yv12_to_yuyvi_c;
packedFunc yv12_to_uyvyi_c;

#if defined(ARCH_IS_IA32) || defined(ARCH_IS_X86_64)
/* mmx */
packedFunc yv12_to_bgr_mmx;
packedFunc yv12_to_bgra_mmx;
packedFunc yv12_to_yuyv_mmx;
packedFunc yv12_to_uyvy_mmx;

packedFunc yv12_to_yuyvi_mmx;
packedFunc yv12_to_uyvyi_mmx;
#endif

#ifdef ARCH_IS_PPC
packedFunc yv12_to_yuyv_altivec_c;
packedFunc yv12_to_uyvy_altivec_c;
#endif

typedef void (planarFunc)
(
	uint8_t *y_dst, 
	uint8_t *u_dst, 
	uint8_t *v_dst,
	int y_dst_stride, 
	int uv_dst_stride,
	uint8_t *y_src, 
	uint8_t *u_src, 
	uint8_t *v_src,
	int y_src_stride, 
	int uv_src_stride,
	int width, 
	int height, 
	int vflip
);

planarFunc yv12_to_yv12_c;

#if defined(ARCH_IS_IA32) || defined(ARCH_IS_X86_64)
planarFunc yv12_to_yv12_mmx;
planarFunc yv12_to_yv12_xmm;
#endif

#endif
