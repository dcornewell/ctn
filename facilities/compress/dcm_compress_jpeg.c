// dcm_compress_jpeg.c
//
// Based on code from dcm_map_to_8.c 
// Contains code from dcm_ctnto10.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "dicom_uids.h"

#ifndef JPEGBITDEPTH
#define JPEGBITDEPTH 12
#endif


#if (JPEGBITDEPTH == 12)
#include "/usr/local/src/jpeg-6b-12bit/jpeglib.h"
#include "/usr/local/src/jpeg-6b-12bit/jversion.h"
#else
#include "jpeglib.h"
#include "jversion.h"
#endif

#if (BITS_IN_JSAMPLE != JPEGBITDEPTH)
#error Inconsistent JPEG bit depth
#endif


#if (JPEGBITDEPTH == 12)
#define jpegx_std_error           jpeg12_std_error
#define jpegx_create_compress     jpeg12_create_compress
#define jpegx_stdio_dest          jpeg12_stdio_dest
#define jpegx_set_defaults        jpeg12_set_defaults
#define jpegx_set_quality         jpeg12_set_quality
#define jpegx_start_compress      jpeg12_start_compress
#define jpegx_write_scanlines     jpeg12_write_scanlines
#define jpegx_finish_compress     jpeg12_finish_compress
#define jpegx_destroy_compress    jpeg12_destroy_compress
#define jpegx_create_decompress   jpeg12_create_decompress
#define jpegx_read_header         jpeg12_read_header
#define jpegx_destroy_decompress  jpeg12_destroy_decompress
#define jpegx_resync_to_restart   jpeg12_resync_to_restart
#define jpegx_start_decompress    jpeg12_start_decompress
#define jpegx_read_scanlines      jpeg12_read_scanlines
#define jpegx_finish_decompress   jpeg12_finish_decompress
#else
#define jpegx_std_error           jpeg_std_error
#define jpegx_create_compress     jpeg_create_compress
#define jpegx_stdio_dest          jpeg_stdio_dest
#define jpegx_set_defaults        jpeg_set_defaults
#define jpegx_set_quality         jpeg_set_quality
#define jpegx_start_compress      jpeg_start_compress
#define jpegx_write_scanlines     jpeg_write_scanlines
#define jpegx_finish_compress     jpeg_finish_compress
#define jpegx_destroy_compress    jpeg_destroy_compress
#define jpegx_create_decompress   jpeg_create_decompress
#define jpegx_read_header         jpeg_read_header
#define jpegx_destroy_decompress  jpeg_destroy_decompress
#define jpegx_resync_to_restart   jpeg_resync_to_restart
#define jpegx_start_decompress    jpeg_start_decompress
#define jpegx_read_scanlines      jpeg_read_scanlines
#define jpegx_finish_decompress   jpeg_finish_decompress
#endif


//#define DEBUGFILE
//#define DEBUGMAIN

struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

static void my_error_exit_encode (j_common_ptr cinfo)
{
	struct my_error_mgr * myerr = (struct my_error_mgr *) cinfo->err;
	longjmp(myerr->setjmp_buffer, 1);
}

static void my_error_exit_decode (j_common_ptr cinfo)
{
	struct my_error_mgr * myerr = (struct my_error_mgr *) cinfo->err;
	longjmp(myerr->setjmp_buffer, 1);
}

struct ucontext {
	int jpeg_image_data_len;
	unsigned char *jpeg_image_data;
	DCM_OBJECT *object;
	void *ctx;  // context pointer for DCM_GetSequenceList
	int source_data_pos;
	JSAMPLE *unc_data;
	int unc_data_len; // measured in bytes
	int nsamples;
};

struct memblk_struct {
  unsigned char *memblk;
  unsigned int memblk_alloc;
  unsigned int used;
  int error_flag;
};

struct my_destmgr_struct {
  struct jpeg_destination_mgr pub; /* public fields */
  JOCTET * buffer;      /* start of buffer */
  struct memblk_struct *mbs;
};

struct my_srcmgr_struct {
  struct jpeg_source_mgr pub; /* public fields */
  JOCTET * buffer;      /* start of buffer */
  struct ucontext *uc;
};

#define OUTPUT_BUF_SIZE  4096
int seg_header[] = {0xe000fffe, 0x00000000, 0xe000fffe, 0x00000000};
int seg_footer[] = {0xe0ddfffe, 0x00000000};

#define MY_JPEG_INPUT_BUF_SIZE 16384


static size_t strlcat(char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

static void
init_destination (j_compress_ptr cinfo)
{
	struct my_destmgr_struct * dest = (struct my_destmgr_struct *) cinfo->dest;

	/* Allocate the output buffer --- it will be released when done with image */
	dest->buffer = (JOCTET *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
		          OUTPUT_BUF_SIZE * sizeof(JOCTET));

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	dest->mbs->memblk_alloc = 1048576;
	dest->mbs->memblk = malloc(dest->mbs->memblk_alloc);
	dest->mbs->used = 0;
	if (dest->mbs->memblk) {
		dest->mbs->error_flag = 0;
		memcpy(dest->mbs->memblk,seg_header,sizeof(seg_header));
		dest->mbs->used = sizeof(seg_header);
	} else
		dest->mbs->error_flag = 1;
}


static int mbs_write_data(struct memblk_struct *mbs, void *buffer, int size)
{
	unsigned int space_needed;

	if(size<1) return TRUE;

	space_needed = mbs->used + size;

	if(space_needed > mbs->memblk_alloc) {
		// need to alloc more memory.

		if(mbs->memblk_alloc >= 256*1048576) {
			// file too big
			mbs->error_flag = 1;
			return 0;
		}

		// Assuming OUTPUT_BUF_SIZE < the initial size of the memory block, doubling
		// the size of the memory block will be sufficient.
		mbs->memblk_alloc *= 2;

		//printf("reallocing to %u\n",mbs->memblk_alloc);
		mbs->memblk = realloc(mbs->memblk, mbs->memblk_alloc);
		if(!mbs->memblk) {
			mbs->error_flag = 1;
			return 0;
		}

	}
	//printf("writing %u\n",size);
	memcpy(&mbs->memblk[mbs->used], buffer, size);
	mbs->used += size;
	return 1;
}

static boolean
empty_output_buffer (j_compress_ptr cinfo)
{

	struct my_destmgr_struct * dest = (struct my_destmgr_struct *) cinfo->dest;

	if(!dest->mbs->memblk) {
		dest->mbs->error_flag = 1;
		return FALSE;
	}
	if(dest->mbs->error_flag) return FALSE;

	if(!mbs_write_data(dest->mbs, dest->buffer, OUTPUT_BUF_SIZE)) {
		return FALSE;
	}

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	return TRUE;
}


static void
term_destination (j_compress_ptr cinfo)
{
	struct my_destmgr_struct * dest = (struct my_destmgr_struct *) cinfo->dest;
	int *header;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

	/* Write any data remaining in the buffer */
	if (datacount > 0 && !dest->mbs->error_flag) {
		mbs_write_data(dest->mbs, dest->buffer, datacount);
	}
	if ((dest->mbs->used & 1)!=0) {
		char zero = 0;
		mbs_write_data(dest->mbs, &zero, 1);
	}
	header = (int *)dest->mbs->memblk;
	header[3] = dest->mbs->used - sizeof(seg_header);
	mbs_write_data(dest->mbs, seg_footer, sizeof(seg_footer));
}



static int init_dest_mgr(struct jpeg_compress_struct *cinfo,  struct memblk_struct *mbs)
{
	struct my_destmgr_struct * dest;


	cinfo->dest = (struct jpeg_destination_mgr *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
		          sizeof(struct my_destmgr_struct));

	dest = (struct my_destmgr_struct *) cinfo->dest;
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
	//dest->outfile = outfile;

	dest->mbs = mbs;

	return 1;
}

// signed 16-bit int
static CONDITION get_ss_value(DCM_OBJECT *object, DCM_TAG tag, int *pvalue)
{
	signed short v;
	void *ctx;
	U32 len;
	DCM_ELEMENT p2 = { 0, DCM_OT, "", 1, 0, {(void*)&v} };
	CONDITION cond;

	ctx=NULL;
	*pvalue = 0;

	p2.tag = tag;
	p2.representation = DCM_SS;
	p2.length = sizeof(v);
	len= sizeof(v);

	if ((cond = DCM_GetElementValue(&object, &p2, &len, &ctx)) != DCM_NORMAL) {
		COND_PopCondition(FALSE);
		return cond;
	}

	*pvalue = (int)v;
	return DCM_NORMAL;
}

// unsigned 16-bit int
static CONDITION get_us_value(DCM_OBJECT *object, DCM_TAG tag, int *pvalue)
{
	signed short v;
	void *ctx;
	U32 len;
	DCM_ELEMENT p2 = { 0, DCM_OT, "", 1, 0, {(void*)&v} };
	CONDITION cond;

	ctx=NULL;
	*pvalue = 0;

	p2.tag = tag;
	p2.representation = DCM_US;
	p2.length = sizeof(v);
	len= sizeof(v);

	if ((cond = DCM_GetElementValue(&object, &p2, &len, &ctx)) != DCM_NORMAL) {
		COND_PopCondition(FALSE);
		return cond;
	}

	*pvalue = (int)v;
	return DCM_NORMAL;
}

static CONDITION get_ds_value(DCM_OBJECT *object, DCM_TAG tag, double *value)
{
	char stringval[32];
	void *ctx;
	U32 len;
	DCM_ELEMENT p2 = { 0, DCM_OT, "", 1, 0, {(void*)stringval} };
	CONDITION cond;

	ctx=NULL;
	*value = 0;
	memset(stringval,0,sizeof(stringval));

	p2.tag = tag;
	p2.representation = DCM_DS;
	p2.length = sizeof(stringval)-1;
	len=sizeof(stringval)-1;

	if ((cond = DCM_GetElementValue(&object, &p2, &len, &ctx)) != DCM_NORMAL) {
//		fprintf(stderr, "Error retrieving element from object\n");
//		COND_DumpConditions();
		COND_PopCondition(FALSE);
		return cond;
	}

	*value = atof(stringval);
	return DCM_NORMAL;
}


static CONDITION set_ds_value(DCM_OBJECT *object, DCM_TAG tag, double value)
{
	char stringval[32];
	U32 len;
	DCM_ELEMENT p2 = { 0, DCM_OT, "", 1, 0, {(void*)stringval} };
	CONDITION cond;

	sprintf(stringval,"%.4f",value);
	len=strlen(stringval);

	p2.tag = tag;
	p2.representation = DCM_DS;
	p2.length = len;

	if ((cond = DCM_AddElement(&object, &p2)) != DCM_NORMAL) {
		fprintf(stderr, "Error adding element to object\n");
		COND_DumpConditions();
		return cond;
	}
	return DCM_NORMAL;
}

static CONDITION get_ui_value(DCM_OBJECT *object, DCM_TAG tag, char *value, int value_len)
{
	DCM_ELEMENT ele;
	CONDITION cond;
	U32 elementLength = 0;
	void *ctx;

	memset(&ele,0,sizeof(DCM_ELEMENT));

	ele.tag = tag;
	ele.length = value_len - 1;
	ele.d.ot = value;
	ctx = NULL;

	cond = DCM_GetElementValue(&object, &ele, &elementLength, &ctx);
	if(cond != DCM_NORMAL) {
		return cond;
	}
	if(elementLength>(value_len-2)) return DCM_ELEMENTNOTFOUND;
	value[elementLength] = '\0';
	return DCM_NORMAL;
}

static CONDITION set_cs_value(DCM_OBJECT *object, DCM_TAG tag, char *value)
{
	DCM_ELEMENT ele;
	CONDITION cond;
	char tmpstring[DICOM_CS_LENGTH + 1];
	int len;


	memset(&ele,0,sizeof(DCM_ELEMENT));
	memset(tmpstring,' ',sizeof(tmpstring));

	len = strlen(value);
	if(len>DICOM_CS_LENGTH) len=DICOM_CS_LENGTH;
	memcpy(tmpstring,value,len);
	if(len%2) len++;

	ele.tag = tag;
	ele.representation = DCM_CS;
	ele.multiplicity = 1;
	ele.length = len;

	ele.d.string = tmpstring;

	DCM_RemoveElement(&object,tag);
	cond = DCM_AddElement(&object, &ele);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error adding element to object\n");
		COND_DumpConditions();
	}
	return cond;
}

static CONDITION set_ui_value(DCM_OBJECT *object, DCM_TAG tag, char *value)
{
	DCM_ELEMENT ele;
	CONDITION cond;
	int len;

	memset(&ele,0,sizeof(DCM_ELEMENT));

	len = strlen(value);

	// Include the trailing NUL if and only if it's necessary to pad the
	// length to an even number.
	if(len%2) len++;

	ele.tag = tag;
	ele.representation = DCM_UI;
	ele.multiplicity = 1;
	ele.length = len;

	ele.d.string = value;

	DCM_RemoveElement(&object,tag);
	cond = DCM_AddElement(&object, &ele);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error adding element to object\n");
		COND_DumpConditions();
	}
	return cond;
}

static void find_min_max_samples_16unsigned(unsigned char *pixels, int npixels,
  int *pminval, int *pmaxval, int has_padding, int paddingValue)
{
	int i;
	unsigned short *px;
	unsigned short v;
	int minval, maxval;

	minval = 65536;
	maxval = 0;
	px = (unsigned short*)pixels;

	for(i=0;i<npixels;i++) {
		v = *px; px+=1;
		if(has_padding && v==paddingValue) continue;
		if(v<minval) minval = v;
		if(v>maxval) maxval = v;
	}
	if(minval>maxval) minval=maxval;
	*pminval = minval;
	*pmaxval = maxval;
}

static void find_min_max_samples_16signed(unsigned char *pixels, int npixels,
  int *pminval, int *pmaxval, int has_padding, int paddingValue)
{
	int i;
	signed short *px;
	signed short v;
	int minval, maxval;

	minval = 32767;
	maxval = -32768;
	px = (signed short*)pixels;

	for(i=0;i<npixels;i++) {
		v = *px; px+=1;
		if(has_padding && v==paddingValue) continue;
		if(v<minval) minval = v;
		if(v>maxval) maxval = v;
	}
	if(minval>maxval) minval=maxval;
	*pminval = minval;
	*pmaxval = maxval;
}


static void find_min_max_samples(unsigned char *pixels, int npixels,
  int bitsAllocated, int bitsStored, int highBit, int pixelRepresentation,
  int *pminval, int *pmaxval, int has_padding, int paddingValue)
{

	// start with some default values
	if(pixelRepresentation) {
		*pmaxval = (1<<highBit)-1;
		*pminval = -(1<<highBit);
	}
	else {
		*pmaxval = (1<<(highBit+1))-1;
		*pminval = 0;
	}

	// call the appropriate scan function, if available
	if(pixelRepresentation && bitsAllocated==16 && (highBit==bitsStored-1)) {
		find_min_max_samples_16signed(pixels,npixels,pminval,pmaxval,has_padding,paddingValue);
	}
	else if(pixelRepresentation==0 && bitsAllocated==16 && (highBit==bitsStored-1)) {
		find_min_max_samples_16unsigned(pixels,npixels,pminval,pmaxval,has_padding,paddingValue);
	}
}

/* compression level */
#if (JPEGBITDEPTH == 12)
CONDITION DCM_jpeg_compress_12(DCM_OBJECT *object, int quality)
#else
CONDITION DCM_jpeg_compress_8(DCM_OBJECT *object, int quality)
#endif
{
	CONDITION cond;
	unsigned long i;
	unsigned long j;
	U32 pixelLength, elementLength, frameLength;
	unsigned char *pixels;
	unsigned short bitsAllocated, bitsStored, highBit, pixelRepresentation, samplesPerPixel, rows, columns;
	unsigned short planarConfig, new_planarConfig;
	int pixelCount;
	int sampleCount;
	char photometricInterpretation[DICOM_CS_LENGTH + 1];
	char lossy_compression[DICOM_CS_LENGTH + 1];
	char sop_inst_id[80], series_uid[80];
	void *ctx;
	CONDITION retval;
	static char qual[100];
	struct memblk_struct mbs;
	int has_ww, has_wc, has_int, has_slp;
	double orig_ww, orig_wc, orig_int, orig_slp;
	int minsampleval, maxsampleval;
	int handle_signed_values=0;
	int has_padding, paddingValue, convert_padding, new_paddingValue;
	unsigned short ori_pv_as_us;
	unsigned short new_pv_as_us;
	int is_rgb;
	int left_shift, right_shift;
	DCM_FILE_META *fileMeta;

	DCM_ELEMENT p2 = { DCM_PXLPIXELDATA, DCM_OT, "", 1, 0, { NULL } };


	DCM_ELEMENT list[] = {

		{DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, DICOM_UI_LENGTH, {(void *) &sop_inst_id}},
		{DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1, DICOM_UI_LENGTH, {(void *) &series_uid}},
		{DCM_IMGBITSALLOCATED, DCM_US, "", 1, sizeof(bitsAllocated), {(void *) &bitsAllocated}},
		{DCM_IMGBITSSTORED, DCM_US, "", 1, sizeof(bitsStored), {(void *) &bitsStored}},
		{DCM_IMGHIGHBIT, DCM_US, "", 1, sizeof(highBit), {(void *) &highBit}},
		{DCM_IMGPIXELREPRESENTATION, DCM_US, "", 1, sizeof(pixelRepresentation), {(void *) &pixelRepresentation}},
		{DCM_IMGSAMPLESPERPIXEL, DCM_US, "", 1, sizeof(samplesPerPixel), {(void *) &samplesPerPixel}},
		{DCM_IMGROWS, DCM_US, "", 1, sizeof(rows), {(void *) &rows}},
		{DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(columns), {(void *) &columns}},
		{DCM_IMGPHOTOMETRICINTERP, DCM_CS, "", 1, sizeof(photometricInterpretation), {photometricInterpretation}},
		//{DCM_IMGLOSSYIMAGECOMPRESSION, DCM_CS, "", 1, sizeof(lossy_compression), {lossy_compression}}
		//{DCM_MAKETAG(DCM_GROUPIMAGE,0x2110), DCM_??, "", 1, sizeof(derivative_description) ... }
	};

	DCM_ELEMENT listnew[] = {
		{DCM_IMGLOSSYIMAGECOMPRESSION, DCM_CS, "", 1, sizeof(lossy_compression), {lossy_compression}},
		{DCM_IDDERIVATIONDESCR, DCM_ST, "", 1, sizeof(qual), {qual} },
		{DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, sizeof(sop_inst_id), {(void *) &sop_inst_id}}
	};

	DCM_ELEMENT list_rgbonly[] = {
		{DCM_IMGPLANARCONFIGURATION, DCM_US, "", 1, sizeof(new_planarConfig), {(void *) &new_planarConfig}}
	};


	static DCM_TAG deleteTags[] = {
		DCM_IMGRESCALEINTERCEPT,
		DCM_IMGRESCALESLOPE,
		//DCM_IMGWINDOWCENTER,
		//DCM_IMGWINDOWWIDTH,
		DCM_IMGPLANARCONFIGURATION,
		DCM_IMGPIXELPADDINGVALUE
	};

	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	int compress_created, compress_started;
	//J_COLOR_SPACE jpeg_colortype;
	JSAMPROW tmprow;
	//int jpeg_cmpts;
	int orig_range, new_range;
	int orig_minval, new_minval;

	unsigned short adjusted_highBit;
	unsigned short adjusted_pixelRepresentation;
	int adjusted_orig_minval;
	int adjusted_orig_range;
	int in_bytes_per_row;
	int out_bytes_per_row;
	int samples_per_row;
	int tmp_int;

	//DCM_Debug(verbose);
	retval = DCM_ELEMENTCREATEFAILED;
	pixels = NULL;
	compress_created=0;
	compress_started=0;
	tmprow = NULL;
	mbs.memblk = NULL;

	memset(&cinfo,0,sizeof(struct jpeg_compress_struct));
	memset(&jerr,0,sizeof(struct my_error_mgr));

	has_wc=0;
	has_ww=0;
	has_int=0;
	has_slp=0;
	orig_int=0.0; orig_slp=1.0;

	orig_range=1; new_range=1;
	orig_minval=0; new_minval=0;

	series_uid[0] = 0;

	cond = DCM_ParseObject(&object, list, (int) DIM_OF(list), NULL, 0, NULL);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error retrieving data elements\n");
		COND_DumpConditions();
		return cond;
	}

	cond = get_ds_value(object, DCM_IMGWINDOWCENTER, &orig_wc);
	if(cond==DCM_NORMAL) {
		has_wc=1;
		//printf("WC: %f\n",orig_wc);
	}
	cond = get_ds_value(object, DCM_IMGWINDOWWIDTH, &orig_ww);
	if(cond==DCM_NORMAL) {
		has_ww=1;
		//printf("WW: %f\n",orig_ww);
	}
	cond = get_ds_value(object, DCM_IMGRESCALEINTERCEPT, &orig_int);
	if(cond==DCM_NORMAL) {
		has_int=1;
		//printf("INT: %f\n",orig_int);
	}
	else {
		has_int=1;
		orig_int=0.0;
	}
	cond = get_ds_value(object, DCM_IMGRESCALESLOPE, &orig_slp);
	if(cond==DCM_NORMAL) {
		has_slp=1;
		//printf("SLP: %f\n",orig_slp);
	}
	else {
		orig_slp = 1.0;
	}

	planarConfig=0;
	cond = get_us_value(object, DCM_IMGPLANARCONFIGURATION, &tmp_int);
	if(cond==DCM_NORMAL) {
		planarConfig=(unsigned short)tmp_int;
	}

	has_padding=0;
	paddingValue=0;
	if(bitsAllocated==16 && pixelRepresentation) {
		cond = get_ss_value(object, DCM_IMGPIXELPADDINGVALUE, &paddingValue);
		if(cond==DCM_NORMAL) has_padding=1;
	}
	else if(bitsAllocated==16 && !pixelRepresentation) {
		cond = get_us_value(object, DCM_IMGPIXELPADDINGVALUE, &paddingValue);
		if(cond==DCM_NORMAL) has_padding=1;
	}

	if(paddingValue<0)
		ori_pv_as_us = (unsigned short)(paddingValue+65536);
	else
		ori_pv_as_us = (unsigned short)paddingValue;

	// find size of old pixels
	cond = DCM_GetElementSize(&object, p2.tag, &pixelLength);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error finding length of pixel data\n");
		COND_DumpConditions();
		return cond;
	}
	frameLength = (U32) samplesPerPixel *(U32) rows *(U32) columns *(U32) (bitsAllocated / 8);
	if (pixelLength != frameLength) {
		fprintf(stderr, "Computed pixel length differs from actual.\n");
		exit(2);
	}

#ifdef DEBUGFILE
	printf("bitsAllocated       %hu\n",  bitsAllocated       );
	printf("bitsStored          %hu\n",  bitsStored          );
	printf("highBit             %hu\n",  highBit             );
	printf("pixelRepresentation %hu\n",  pixelRepresentation );
	printf("samplesPerPixel     %hu\n",  samplesPerPixel     );
	printf("rows                %hu\n",  rows                );
	printf("columns             %hu\n",  columns             );
	printf("photometricInt.     %s\n",   photometricInterpretation);
	printf("planarConfig        %hu\n",  planarConfig);
	if(has_padding) printf("padding value:      %d\n",paddingValue);
#endif

	is_rgb=0;
	if(!strcmp(photometricInterpretation, "RGB")) {
		is_rgb=1;
	}
	else if(!strcmp(photometricInterpretation, "MONOCHROME2")) {
		;
	}
	else if(!strcmp(photometricInterpretation, "MONOCHROME1")) {
		;
	}
	else {
		fprintf(stderr, "Unsupported photometricInterpretation \"%s\".\n",photometricInterpretation);
		goto abort;
	}

	if(bitsAllocated!=8 && bitsAllocated!=16) {
		fprintf(stderr,"Unsupported bitsAllocated (%hu)\n",bitsAllocated);
	}

	if(is_rgb && (samplesPerPixel!=3)) {
		fprintf(stderr,"Unsupported samples/pixel (%hu)\n",samplesPerPixel);
		goto abort;
	}
	if((!is_rgb) && (samplesPerPixel!=1)) {
		fprintf(stderr,"Unsupported samples/pixel (%hu)\n",samplesPerPixel);
		goto abort;
	}

	if(!is_rgb) {
		planarConfig=0;
	}

	if(!is_rgb) {
		; // planarConfig is meaningless for grayscale images
	}
	else if(is_rgb && planarConfig==0) {
		; // RGB planarConfig=contiguous images are supported.
	}
	else if(is_rgb && planarConfig==1 && bitsAllocated==8) {
		; // 8-bit RGB planarConfig=separated images are supported.
	}
	else {
		fprintf(stderr,"Unsupported planarConfig (%hu)\n",planarConfig);
		goto abort;
	}

	orig_range = 1<<(bitsStored);
	if(pixelRepresentation) {
		orig_minval = 0-(1<<(bitsStored-1));
	}
	else {
		orig_minval = 0;
	}

#if (JPEGBITDEPTH == 12)
	new_range = 4096;
#else
	new_range = 256;
#endif
	new_minval = 0;

#ifdef DEBUGFILE
	printf("orig_range: %d\n",orig_range);
	printf("orig_minval: %d\n",orig_minval);
	printf("new_range: %d\n",new_range);
	printf("new_minval: %d\n",new_minval);
#endif

	// prepare a place for the old pixels in memory
	pixels = malloc(pixelLength);
	if (pixels == NULL) {
		perror("Malloc of pixel data");
		exit(1);
	}
	pixelCount = (int) rows *(int) columns;
	sampleCount = pixelCount * samplesPerPixel;


	// read old pixels into memory

	p2.length = pixelLength;
	p2.d.ot = pixels;
	ctx = NULL;

	if ((cond = DCM_GetElementValue(&object, &p2, &elementLength, &ctx)) != DCM_NORMAL) {
		fprintf(stderr, "Error retrieving pixels from image\n");
		COND_DumpConditions();
		return cond;
	}

	// remove old pixels element
	if ((cond = DCM_RemoveElement(&object, p2.tag)) != DCM_NORMAL) {
		fprintf(stderr, "Error removing old pixel data from object\n");
		COND_DumpConditions();
		return cond;
	}

	// Zero out unused bits
	if(bitsAllocated==16 && highBit<15) {
		unsigned short *tmp_pix;
		unsigned short tmp_mask;
		tmp_mask = 0xffff >> (bitsAllocated-highBit-1);
		//printf("mask: %d\n",(int)tmp_mask);
		tmp_pix=(unsigned short*)pixels;
		for(i=0;i<sampleCount;i++) {
			*tmp_pix = (*tmp_pix) & tmp_mask;
			tmp_pix++;
		}
	}
	else if(bitsAllocated==8 && highBit<7) {
		unsigned char tmp_mask;
		tmp_mask = 0xff >> (bitsAllocated-highBit-1);
		for(i=0;i<sampleCount;i++) {
			pixels[i] = pixels[i] & tmp_mask;
		}
	}

	/////////////////////////////////
	// Optimize use of new bits.

	find_min_max_samples(pixels, sampleCount,
	  bitsAllocated, bitsStored, highBit, pixelRepresentation,
  	  &minsampleval, &maxsampleval, has_padding, paddingValue);
#ifdef DEBUGFILE
	printf("minsampleval:       %d\n",  minsampleval);
	printf("maxsampleval:       %d\n",  maxsampleval);
#endif

	adjusted_orig_minval = orig_minval;
	adjusted_orig_range = orig_range;
	adjusted_highBit = highBit;
	adjusted_pixelRepresentation = pixelRepresentation;

	if(bitsAllocated==16) {
		// Find the maximum amount that we can shift the bits
		// without losing the most significant bits.

		if(minsampleval>=0) {
			// All samples are >=0.
			// Treat as unsigned, whether or not the original was signed.
			for(i=8;i<=15;i++) {
				if(maxsampleval<(1<<i)) {
					adjusted_orig_minval = 0;
					adjusted_orig_range = 1<<i;
					adjusted_highBit = i-1;
					adjusted_pixelRepresentation = 0;
					break;
				}
			}
		}
		else {
			// Some samples are negative.
			for(i=8;i<=15;i++) {
				if( (maxsampleval<(1<<(i-1))) &&  (minsampleval>= -(int)(1<<(i-1)))) {
					adjusted_orig_minval = -(int)(1<<(i-1));
					adjusted_orig_range = 1<<i;
					adjusted_highBit = i-1;
					adjusted_pixelRepresentation = 1;
					handle_signed_values=1;
					break;
				}
			}
		}
	}

	orig_minval = adjusted_orig_minval;
	orig_range  = adjusted_orig_range;
	highBit     = adjusted_highBit;
	pixelRepresentation = adjusted_pixelRepresentation;

	convert_padding=0; new_paddingValue=0; new_pv_as_us = 0;
	if(paddingValue < adjusted_orig_minval) {
		convert_padding = 1;
		new_paddingValue = adjusted_orig_minval;
		if(pixelRepresentation)
			new_pv_as_us = 2048;
		else
			new_pv_as_us = 0;
	}
	else if(paddingValue > (adjusted_orig_minval+adjusted_orig_range-1)) {
		convert_padding = 1;
		new_paddingValue = adjusted_orig_minval+adjusted_orig_range-1;
		if(pixelRepresentation)
			new_pv_as_us = 2047;
		else
			new_pv_as_us = 4095;
	}

#ifdef DEBUGFILE
	printf("adj_orig_range: %d\n",orig_range);
	printf("adj_orig_minval: %d\n",orig_minval);
	printf("adj_highBit: %d\n",(int)highBit);
	printf("adj_pixRep: %d\n",(int)pixelRepresentation);
	printf("fix_signed_values: %d\n",handle_signed_values);
	if(convert_padding) printf("new_paddingValue: %d\n",new_paddingValue);
	if(convert_padding) printf("    ori PV as US: %d\n",(int)ori_pv_as_us);
	if(convert_padding) printf("    new PV as US: %d\n",(int)new_pv_as_us);
#endif

	///////////////////// jpeg

	cinfo.err = jpegx_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit_encode;

	if (setjmp(jerr.setjmp_buffer)) {
		fprintf(stderr,"JPEG compression error\n");
		goto abort;
	}

	jpegx_create_compress(&cinfo); compress_created=1;

	init_dest_mgr(&cinfo,&mbs);
	//	jpegx_stdio_dest(&cinfo, outf);

	cinfo.image_width = columns;
	cinfo.image_height = rows;
	if(is_rgb) {
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
	}
	else {
		cinfo.input_components = 1;
		cinfo.in_color_space = JCS_GRAYSCALE;
	}

	jpegx_set_defaults(&cinfo);

	if(quality>=0) {
		jpegx_set_quality(&cinfo,quality,0);
	}

	jpegx_start_compress(&cinfo, TRUE); compress_started=1;

	samples_per_row = cinfo.image_width*samplesPerPixel;
	in_bytes_per_row = samples_per_row * ((bitsAllocated>8)?2:1);
	out_bytes_per_row = samples_per_row*sizeof(JSAMPLE);

	tmprow = (JSAMPROW)calloc(out_bytes_per_row,1);
	if(!tmprow) {
		retval = DCM_MALLOCFAILURE;
		goto abort;
	}

	left_shift=0; right_shift=0;
	if(highBit>(JPEGBITDEPTH-1) && bitsAllocated==16) {
		right_shift = highBit-(JPEGBITDEPTH-1);
	}
	else if(highBit<(JPEGBITDEPTH-1)) {
		left_shift = (JPEGBITDEPTH-1)-highBit;
	}

#ifdef DEBUGFILE
	if(highBit>(JPEGBITDEPTH-1)) {
		printf("bit-shift: right %d\n",right_shift);
	}
	else if(highBit<(JPEGBITDEPTH-1)) {
		printf("bit-shift: left %d\n",left_shift);
	}
    else {
		printf("bit-shift: 0\n");
	}
#endif

	j=0;
	while(cinfo.next_scanline < cinfo.image_height) {
		if(bitsAllocated==16) {
			unsigned short *x2;
			unsigned short v;
			int tmp1;

			x2 = (unsigned short*)&pixels[j*in_bytes_per_row];
			for(i=0;i<samples_per_row;i++) {
				v = *x2;

				if(v==ori_pv_as_us && convert_padding) {
					tmprow[i]=new_pv_as_us;
					goto nextpix;
				}

				if(handle_signed_values && v>=32768) {
					tmp1 = ((int)v)-65536;
					tmp1 += 1<<((int)highBit+1);
					v = (unsigned short)tmp1;
				}

				if(right_shift>0) tmprow[i] = v>>right_shift;
				else if(left_shift>0) tmprow[i] = v<<left_shift;
				else tmprow[i] = v;
nextpix:
				x2++;
			}
		}
		else if(bitsAllocated==8 && planarConfig==1) {
			unsigned char v;
			// color-separated RGB
			for(i=0;i<cinfo.image_width;i++) {
				// red
				v = pixels[j*cinfo.image_width+i];
				tmprow[i*3]=v;
				if(left_shift>0) tmprow[i*3]<<=left_shift;

				// green
				v = pixels[cinfo.image_width*(cinfo.image_height+j)+i];
				tmprow[i*3+1]=v;
				if(left_shift>0) tmprow[i*3+1]<<=left_shift;

				// blue
				v = pixels[cinfo.image_width*(2*cinfo.image_height+j)+i];
				tmprow[i*3+2]=v;
				if(left_shift>0) tmprow[i*3+2]<<=left_shift;
			}
		}
		else if(bitsAllocated==8) {
			unsigned char *x1;
			unsigned short tmps1;
			x1 = (unsigned char*)&pixels[j*in_bytes_per_row];

			for(i=0;i<samples_per_row;i++) {
				tmps1 = (unsigned short) (*x1);
				if(left_shift>0) tmprow[i] = tmps1<<left_shift;
				else tmprow[i] = tmps1;
				x1++;
			}

		}

		if(pixelRepresentation) {  // convert signed samples to unsigned
			for(i=0;i<samples_per_row;i++) {
#if (JPEGBITDEPTH == 12)
				if(tmprow[i]<2048) tmprow[i]+=2048;
				else tmprow[i]-=2048;
#else
				if(tmprow[i]<128) tmprow[i]+=128;
				else tmprow[i]-=128;
#endif
			}
		}

		jpegx_write_scanlines(&cinfo, &tmprow, 1);
		j++;
	}

	jpegx_finish_compress(&cinfo); compress_started=0;
	jpegx_destroy_compress(&cinfo); compress_created=0;

	/////////////////////

	//printf("mbs.memblk        %p\n",mbs.memblk);
	//printf("mbs.memblk_alloc  %u\n",mbs.memblk_alloc);
	//printf("mbs.used          %u\n",mbs.used);
	//printf("mbs.error_flag    %d\n",mbs.error_flag);

	if(mbs.error_flag) {
		fprintf(stderr,"Failure in JPEG generation or memory allocation");
		goto abort;
	}

	// define new pixels element
	p2.length = mbs.used; //fsize;
	if (p2.length & 1)
		p2.length++;

	//p2.d.ot = newPixels;
	//p2.representation = DCM_OT;
	p2.d.ob = mbs.memblk; //newPixels;
	p2.representation = DCM_OB;

	//printf("== newPixels: %p  size: %u\n", newPixels, fsize);
	//printf("== p2.length: %u\n", (unsigned int)p2.length);

	// add new pixels
	if ((cond = DCM_AddEncapElement(&object, &p2)) != DCM_NORMAL) {
		fprintf(stderr, "Error adding new pixel data to object\n");
		COND_DumpConditions();
		return cond;
	}

	// modify other elements
//	bitsAllocated = JPEGBITDEPTH;
#if (JPEGBITDEPTH == 12)
	bitsAllocated = 16;
#else
	bitsAllocated = 8;
#endif
	bitsStored = JPEGBITDEPTH;
	highBit = JPEGBITDEPTH-1;
	pixelRepresentation = 0;
	if(is_rgb) {
		samplesPerPixel = 3;
		strcpy(photometricInterpretation,"YBR_FULL_422");
	}
	else {
		samplesPerPixel = 1;
	}
	strcpy(lossy_compression,"01");
	sprintf(qual, "JPEG %.1f:1 Q=%d (lossy)",(float)pixelLength/(float)mbs.used,quality);
	{
		char temp[200];
		int l;

		sprintf(temp, ".%d", quality);
		strlcat(sop_inst_id, temp, DICOM_UI_LENGTH-1);
		l=strlen(sop_inst_id);
		if (l) {
			if (sop_inst_id[l-1]=='.') sop_inst_id[l-1]=0;  //truncate off the period if it was the last char.
		}

		if (strlen(series_uid))
			strlcat(series_uid, temp, DICOM_UI_LENGTH-1);
	}
//printf("QUALITY: %s\n", qual);fflush(stdout);
	if (DCM_ModifyElements(&object, list, (int) DIM_OF(list), NULL, 0, NULL) !=
		 DCM_NORMAL) {
		fprintf(stderr, "Error adding new pixel data to object\n");
		COND_DumpConditions();
	}

	DCM_ModifyElements(&object, listnew, (int) DIM_OF(listnew), NULL, 0, NULL);

	// delete some elements
	for (i = 0; i < DIM_OF(deleteTags); i++) {
		(void) DCM_RemoveElement(&object, deleteTags[i]);
	}

	if(is_rgb) {
		new_planarConfig=0; // always consider JPEG images to be planarConfig=contiguous
		DCM_ModifyElements(&object, list_rgbonly, (int) DIM_OF(list_rgbonly), NULL, 0, NULL);
	}

	if(new_minval!=orig_minval || new_range!=orig_range) {
		DCM_RemoveElement(&object, DCM_IMGSMALLESTPIXELVALUE);
		DCM_RemoveElement(&object, DCM_IMGLARGESTPIXELVALUE);
		DCM_RemoveElement(&object, DCM_IMGSMALLESTIMAGEPIXELVALUE);
		DCM_RemoveElement(&object, DCM_IMGLARGESTIMAGEPIXELVALUE);
		DCM_RemoveElement(&object, DCM_IMGSMALLESTPIXELVALUESERIES);
		DCM_RemoveElement(&object, DCM_IMGLARGESTPIXELVALUESERIES);
		DCM_RemoveElement(&object, DCM_IMGSMALLESTIMAGEPIXELVALUEPLANE);
		DCM_RemoveElement(&object, DCM_IMGLARGESTIMAGEPIXELVALUEPLANE);
	}
	DCM_RemoveElement(&object, DCM_MEDIAICONIMAGE);//0088:0200
	if(has_ww && has_wc) {
		// Try to keep the displayed window/level the same, by adjusting
		// the slope and intercept.
		double new_slp;
		double new_int;

		new_int = orig_int;
		new_slp = orig_slp;

		// If the raw sample values are increased by X, INTERCEPT needs to be decreased
		// by SLOPE*X.

		new_int -= new_slp * (double)(new_minval - orig_minval);

		// If the raw sample values are multipled by X, SLOPE needs to be divided by X.
		new_slp /= (double)new_range / (double)orig_range;


		//printf("orig int: %f   new int: %f\n",orig_int,new_int);
		//printf("orig slope: %f   new slope: %f\n",orig_slp, new_slp);
		if ((new_int!=0.0)||(new_slp!=1.0)) {
			set_ds_value(object,DCM_IMGRESCALEINTERCEPT,new_int);
			set_ds_value(object,DCM_IMGRESCALESLOPE,new_slp);
		}

	}

	DCM_GetFileMeta(&object, &fileMeta);
	(void) DCM_RemoveGroup(&object, DCM_GROUPFILEMETA);
	DCM_DefaultFileMeta(&object, &fileMeta);
	strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERJPEGEXTENDEDPROC2AND4);
	DCM_SetFileMeta(&object, fileMeta);
	DCM_FreeFileMeta(&fileMeta);

	//////////////////////
//	if (DCM_WriteFile(&object, DCM_ORDERLITTLEENDIAN, "out.dcm") != DCM_NORMAL) {
//		fprintf(stderr, "Error writing new DCM image file\n");
//		COND_DumpConditions();
//		THR_Shutdown();
//		exit(1);
//	}
/////////////////////

	retval = DCM_NORMAL;

abort:
	if(compress_started) jpegx_finish_compress(&cinfo);
	if(compress_created) jpeg_destroy_compress(&cinfo);
	if(tmprow) free(tmprow);
	if(pixels) free(pixels);
	if(mbs.memblk) free(mbs.memblk);

	return retval;
}

#ifdef DEBUGFILE
static void dumpmem(const unsigned char *mem, int len)
{
	int i;
	for(i=0;i<len;i++) {
		printf("%02x ",(int)mem[i]);
	}
	printf("\n");
}
#endif

// Read the next 'len' bytes of pixel data.
// Returns nonzero if successful.
static int read_pixel_data(DCM_OBJECT *object, void **pctx, void *buf, int len)
{
	CONDITION cond;
	DCM_ELEMENT ele;
	unsigned long amtread = 0;

	memset(&ele,0,sizeof(DCM_ELEMENT));

	ele.tag = DCM_PXLPIXELDATA;
	ele.length = len;
	ele.d.ot = buf;

	cond = DCM_GetElementValue(&object,&ele,&amtread,pctx);
#ifdef DEBUGFILE
	fprintf(stderr,"Read %d bytes\n",(int)amtread);
	dumpmem((unsigned char*)ele.d.ot,len>50?50:len);
#endif
	
	// (cond & 0xf) is the severity. 2==error, 4==fatal.
	// We can't just compare to DCM_NORMAL, because it will return a warning
	// code if not all the data was read, and by design we don't read all the
	// data at once.
	if((cond&0xf)==2 || (cond&0xf)==4) {
		fprintf(stderr,"Error calling GetElementValue (%d)\n",(int)cond);
		COND_DumpConditions();
		return 0;
	}

	return 1;
}

static void my_init_source_fn(j_decompress_ptr cinfo)
{
	struct my_srcmgr_struct *src;
	src = (struct my_srcmgr_struct*)cinfo->src;

	// This memory will (we assume) be automatically freed by libjpeg at some point,
	// when it frees the memory pool.
	src->buffer = (JOCTET*) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
            MY_JPEG_INPUT_BUF_SIZE * sizeof(JOCTET));
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = 0;
}

static boolean my_fill_input_buffer_fn(j_decompress_ptr cinfo)
{
	struct my_srcmgr_struct *src;
	int i,j;
	int bytes_to_copy;

	src = (struct my_srcmgr_struct*)cinfo->src;

	bytes_to_copy = src->uc->jpeg_image_data_len - src->uc->source_data_pos;
	if(bytes_to_copy > MY_JPEG_INPUT_BUF_SIZE) bytes_to_copy = MY_JPEG_INPUT_BUF_SIZE;
#ifdef DEBUGFILE
	fprintf(stderr,"[read: %d bytes]\n",bytes_to_copy);
#endif
	if(bytes_to_copy>0) {
		memcpy(src->buffer, &src->uc->jpeg_image_data[src->uc->source_data_pos], bytes_to_copy);
	}

	src->pub.next_input_byte = src->buffer;

	if(bytes_to_copy<MY_JPEG_INPUT_BUF_SIZE) {
		// If we didn't fill the whole buffer (presumably because end-of-file
		// was reached), fill remaining bytes with EOI markers. Libjpeg
		// doesn't care about the exact file size.
		j=0;
		for(i=bytes_to_copy;i<MY_JPEG_INPUT_BUF_SIZE;i++) {
			if(j==0) src->buffer[i]=0xff;
			else src->buffer[i]=0xd9;
			j = !j;
		}
	}
	src->pub.bytes_in_buffer = MY_JPEG_INPUT_BUF_SIZE;
	src->uc->source_data_pos += bytes_to_copy;
	return TRUE;
}

static void my_skip_input_data_fn(j_decompress_ptr cinfo, long num_bytes)
{
	struct my_srcmgr_struct *src;
	src = (struct my_srcmgr_struct*)cinfo->src;

	if(num_bytes<=0) return;
	// If the skip doesn't advance beyond the end of data already
	// buffered, just advance the buffer pointer, and decrease
	// the bytes-left count.
	if(num_bytes<(long)src->pub.bytes_in_buffer) {
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer -= num_bytes;
		return;
	}

	src->uc->source_data_pos += num_bytes;
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = 0;
}

static void my_term_source_fn(j_decompress_ptr cinfo)
{
}

// Set up custom read-JPEG functions
static int init_src_mgr(j_decompress_ptr cinfo, struct ucontext *uc)
{
	struct my_srcmgr_struct *src;
	cinfo->src = (struct jpeg_source_mgr *)
	       (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                  sizeof(struct my_srcmgr_struct));

	src = (struct my_srcmgr_struct*)cinfo->src;
	src->uc = uc;
	src->buffer = NULL;
	src->pub.init_source = my_init_source_fn;
	src->pub.fill_input_buffer = my_fill_input_buffer_fn;
	src->pub.skip_input_data = my_skip_input_data_fn;
	src->pub.resync_to_restart = jpegx_resync_to_restart; // use default
	src->pub.term_source = my_term_source_fn;
	return 1;
}


// Reads data representing a (presumably) jpeg image, and returns it in
// the ucontext struct.
// Returns nonzero if successful.
// If successful and uc->jpeg_image_data_len>0, uc->jpeg_image_data will be
// a malloc'd block of memory.
static int read_next_image_item(struct ucontext *uc)
{
	unsigned char buf[4];
	int ret;

#ifdef DEBUGFILE
	fprintf(stderr,"Reading next image item...\n");
#endif

	if(uc->jpeg_image_data) {
		free(uc->jpeg_image_data);
		uc->jpeg_image_data = NULL;
	}

	uc->jpeg_image_data_len=0;

	// Read the group/element, which should be either (fffe,e000) for "Item",
	// or (fffe,e0dd) for "Sequence Delimitation item".
	ret = read_pixel_data(uc->object,&uc->ctx,(void*)buf,4);
	if(!ret) return 0;

	if(buf[0]==0xfe && buf[1]==0xff && buf[2]==0x00 && buf[3]==0xe0) {
		;
	}
	else {
		return 0;
	}

	// Read the item length.
	ret = read_pixel_data(uc->object,&uc->ctx,(void*)buf,4);
	if(!ret) return 0;

	uc->jpeg_image_data_len = buf[0] | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24);
	if(uc->jpeg_image_data_len<0 || uc->jpeg_image_data_len>200000000) return 0; // image too big, or invalid length

	if(uc->jpeg_image_data_len==0) {
		// Data is zero-length. That's okay.
		return 1;
	}

	uc->jpeg_image_data = malloc(uc->jpeg_image_data_len);
	if(!uc->jpeg_image_data) {
		uc->jpeg_image_data_len=0;
		return 0;
	}


	ret = read_pixel_data(uc->object,&uc->ctx,uc->jpeg_image_data,uc->jpeg_image_data_len);
	if(!ret) {
		free(uc->jpeg_image_data);
		uc->jpeg_image_data=NULL;
		return 0;
	}

	return 1;
}

// Uncompress the memory-mapped jpeg file in uc->jpeg_image_data.
// Put uncompressed image in uc->u_image_data.
static int uncompress_routine(struct ucontext *uc)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	int cinfo_valid;
	int retval;
	char errmsg[JMSG_LENGTH_MAX];
	int j_width;
	int j_height;
	int j_linesize;
	int j_samplesperrow;
	int j;
	JSAMPROW rowptr[1];

	retval=0;
	cinfo_valid=0;

	memset(&cinfo,0,sizeof(struct jpeg_decompress_struct));
	memset(&jerr,0,sizeof(struct my_error_mgr));

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit_decode;
	if (setjmp(jerr.setjmp_buffer)) {
		(*cinfo.err->format_message) ((j_common_ptr)&cinfo, errmsg);
		fprintf(stderr,"JPEG decompression error: %s\n",errmsg);
		goto done;
	}

	jpegx_create_decompress(&cinfo);
	cinfo_valid = 1;

	uc->source_data_pos = 0;

	init_src_mgr(&cinfo,uc);
	jpegx_read_header(&cinfo,TRUE);


	jpegx_start_decompress(&cinfo);

	uc->nsamples = cinfo.output_components;
	if(uc->nsamples!=1 && uc->nsamples!=3) {
		fprintf(stderr,"Unsupported JPEG number of components\n");
		goto done;
	}

	j_width = cinfo.output_width;
	j_height = cinfo.output_height;
	j_samplesperrow = j_width * uc->nsamples;
	j_linesize =j_samplesperrow * sizeof(JSAMPLE);

	uc->unc_data_len = j_linesize * j_height;
	uc->unc_data = malloc(uc->unc_data_len);
	if(!uc->unc_data) {
		fprintf(stderr,"Failed to allocate uncompressed image data\n");
		goto done;
	}

	while (cinfo.output_scanline < cinfo.output_height) {
		j=cinfo.output_scanline;
		rowptr[0] = &uc->unc_data[j*j_samplesperrow];
		jpegx_read_scanlines(&cinfo, rowptr, 1);
	}

	jpegx_finish_decompress(&cinfo);
	retval = 1;

done:

	if(cinfo_valid) jpegx_destroy_decompress(&cinfo);

	return retval;
}

#if (JPEGBITDEPTH == 12)
CONDITION DCM_jpeg_uncompress_12(DCM_OBJECT *object)
#else
CONDITION DCM_jpeg_uncompress_8(DCM_OBJECT *object)
#endif
{
	CONDITION retval = DCM_ELEMENTCREATEFAILED;
	CONDITION cond;
	DCM_ELEMENT ele;
	DCM_ELEMENT ele_pix;
	U32 pixelLength;
	int ret;
	struct ucontext uc;
	char buf[200];

	memset(&uc,0,sizeof(struct ucontext));
	uc.object = object;

	// find size of old pixels
	cond = DCM_GetElement(&object, DCM_PXLPIXELDATA, &ele);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error getting info about PixelData element\n");
		COND_DumpConditions();
		retval = cond;
		goto done;
	}

#ifdef DEBUGFILE
	fprintf(stderr,"tag = %d\n",(int)ele.tag);
	fprintf(stderr,"rep = %d\n",(int)ele.representation);
	ele.description[47] = '\0';
	fprintf(stderr,"descr = %s\n", ele.description);
    fprintf(stderr,"mult = %d\n",(int)ele.multiplicity);
    fprintf(stderr,"len = %u\n",(unsigned int)ele.length);
	fprintf(stderr,"val = %p\n",ele.d.ot);
#endif

	// find size of old pixels
	cond = DCM_GetElementSize(&object, DCM_PXLPIXELDATA, &pixelLength);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error finding length of pixel data\n");
		COND_DumpConditions();
		retval = cond;
		goto done;
	}
#ifdef DEBUGFILE
	fprintf(stderr,"pixel length: %d\n",(int)pixelLength);
#endif

	if(pixelLength == 0xffffffff) {

		uc.ctx = NULL;

		// Read the first item (an index that we don't need)
		ret = read_next_image_item(&uc);
		if(!ret) goto done;

		// Read the second item (the first image)
		ret = read_next_image_item(&uc);
		if(!ret) goto done;
		if(uc.jpeg_image_data_len<1) goto done;

		ret = uncompress_routine(&uc);
		if(!ret) goto done;

	}
	else {
		fprintf(stderr,"This type of DICOM image is not supported.\n");
	}

	// Removed the old (compressed) image
	DCM_RemoveElement(&object, DCM_PXLPIXELDATA);

	// Add the new (uncompressed) image
	memset(&ele_pix,0,sizeof(DCM_ELEMENT));
	ele_pix.tag = DCM_PXLPIXELDATA;
#if (JPEGBITDEPTH == 12)
	ele_pix.representation = DCM_OW;
#else
	ele_pix.representation = DCM_OB;
#endif
	ele_pix.multiplicity = 1;
	ele_pix.length = (U32)uc.unc_data_len;
	ele_pix.d.ot = uc.unc_data;

	cond = DCM_AddElement(&object,&ele_pix);
	if (cond != DCM_NORMAL) {
		fprintf(stderr, "Error adding pixel data\n");
		COND_DumpConditions();
		retval = cond;
		goto done;
	}

	if(uc.nsamples==3) {
		cond = set_cs_value(object,DCM_IMGPHOTOMETRICINTERP,"RGB");
		if(cond!=DCM_NORMAL) {
			retval=cond;
			goto done;
		}
	}

	cond = set_cs_value(object,DCM_IMGLOSSYIMAGECOMPRESSION,"01");
	if(cond!=DCM_NORMAL) {
		retval=cond;
		goto done;
	}

	cond = get_ui_value(object,DCM_IDSOPINSTANCEUID,buf,sizeof(buf));
	if(cond==DCM_NORMAL) {
		// FIXME: figure out the best way to modify the SOP instance ID.
		strlcat(buf,".123",sizeof(buf));

		cond = set_ui_value(object,DCM_IDSOPINSTANCEUID,buf);
		if(cond!=DCM_NORMAL) {
			retval=cond;
			goto done;
		}
	}

	cond = get_ui_value(object,DCM_RELSERIESINSTANCEUID,buf,sizeof(buf));
	if(cond==DCM_NORMAL) {
		// FIXME: figure out the best way to modify the series ID.
		strlcat(buf,".123",sizeof(buf));

		cond = set_ui_value(object,DCM_RELSERIESINSTANCEUID,buf);
		if(cond!=DCM_NORMAL) {
			retval=cond;
			goto done;
		}
	}


	retval=DCM_NORMAL;

done:
	if(uc.jpeg_image_data) free(uc.jpeg_image_data);
	if(uc.unc_data) free(uc.unc_data);
	return retval;
}


#ifdef DEBUGMAIN

// Higher-level function than DCM_jpeg_compress_12. Sets the transfer syntax, etc.
#if (JPEGBITDEPTH == 12)
CONDITION DCM_jpeg_compress_12_highlevel(DCM_OBJECT *object, int options, int quality)
#else
CONDITION DCM_jpeg_compress_8_highlevel(DCM_OBJECT *object, int options, int quality)
#endif
{
	DCM_FILE_META *fileMeta;
	CONDITION cond = DCM_ELEMENTCREATEFAILED;
 

	if ((options & DCM_PART10FILE) != 0)
		(void) DCM_RemoveGroup(&object, 0x0002);

	cond = DCM_DefaultFileMeta(&object, &fileMeta);
	if (cond != DCM_NORMAL)
		goto done;

	if ((options & DCM_ORDERMASK) == DCM_EXPLICITBIGENDIAN)
		strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERBIGENDIANEXPLICIT);
	if ((options & DCM_ORDERMASK) == DCM_EXPLICITLITTLEENDIAN)
		strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERLITTLEENDIANEXPLICIT);

#if (JPEGBITDEPTH == 12)
	strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERJPEGEXTENDEDPROC2AND4);
#else
	strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERJPEGBASELINEPROCESS1);
#endif

	cond = DCM_SetFileMeta(&object, fileMeta);
	if (cond != DCM_NORMAL) {
		fflush(stderr);
		fprintf(stderr,"SetFileMeta failed, quitting\n");
		goto done;
	}

	cond = DCM_FreeFileMeta(&fileMeta);
	if (cond != DCM_NORMAL) {
		goto done;
	}

#if (JPEGBITDEPTH == 12)
	cond = DCM_jpeg_compress_12(object, -1);
#else
	cond = DCM_jpeg_compress_8(object, -1);
#endif

done:
	return cond;
}


// Higher-level function than DCM_jpeg_uncompress_x. Sets the transfer syntax, etc.
#if (JPEGBITDEPTH == 12)
CONDITION DCM_jpeg_uncompress_12_highlevel(DCM_OBJECT *object, int options)
#else
CONDITION DCM_jpeg_uncompress_8_highlevel(DCM_OBJECT *object, int options)
#endif
{
	DCM_FILE_META *fileMeta;
	CONDITION cond = DCM_ELEMENTCREATEFAILED;
	unsigned int order_and_vr;

	order_and_vr = (options & DCM_ORDERMASK);
	switch(order_and_vr) {
	case DCM_ORDERNATIVE:
	case DCM_ORDERLITTLEENDIAN:
	case DCM_EXPLICITLITTLEENDIAN:
		break;
	case DCM_ORDERBIGENDIAN:
	case DCM_EXPLICITBIGENDIAN:
		fprintf(stderr,"Big-endian order not supported\n");
		goto done;
	default:
		fprintf(stderr,"'Order' setting not support\n");
		goto done;
	}

 
	if (options & DCM_PART10FILE) {
		cond = DCM_DefaultFileMeta(&object, &fileMeta);
		if (cond != DCM_NORMAL)
			goto done;
	
		switch(order_and_vr) {
		case DCM_EXPLICITBIGENDIAN:
			strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERBIGENDIANEXPLICIT);
			break;
		case DCM_EXPLICITLITTLEENDIAN:
			strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERLITTLEENDIANEXPLICIT);
			break;
		case DCM_ORDERLITTLEENDIAN:
		case DCM_ORDERNATIVE:
			strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERLITTLEENDIAN);
			break;
		}

		cond = DCM_SetFileMeta(&object, fileMeta);
		if (cond != DCM_NORMAL) {
			fflush(stderr);
			fprintf(stderr,"SetFileMeta failed, quitting\n");
			goto done;
		}
	
		cond = DCM_FreeFileMeta(&fileMeta);
		if (cond != DCM_NORMAL) {
			goto done;
		}
	}
	else {
		(void) DCM_RemoveGroup(&object, 0x0002);
	}



#if (JPEGBITDEPTH == 12)
	cond = DCM_jpeg_uncompress_12(object);
#else
	cond = DCM_jpeg_uncompress_8(object);
#endif

done:
	return cond;
}


static void
usageerror()
{
    static const char msg[] = "\
Usage: [-bBL] [-v] filein fileout\n\
    -b  Big endian output (non standard)\n\
    -B  Big endian explicit transfer syntax\n\
    -L  Little endian explicit transfer syntax\n";

    fprintf(stderr, msg);
    exit(1);
}

int
main(int argc, char **argv)
{
    DCM_OBJECT
    * object;
    CONDITION
	cond;
    CTNBOOLEAN
	verbose = FALSE;
    long
        options = DCM_ORDERLITTLEENDIAN | DCM_PART10FILE;
    long
        openOptions = DCM_ORDERLITTLEENDIAN;
	int uncompress = 0;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 'B':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_EXPLICITBIGENDIAN;
	    break;
	case 'L':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_EXPLICITLITTLEENDIAN;
	    break;
	case 't':
	    openOptions |= DCM_PART10FILE;
	    break;
	case 'u':
	    uncompress = 1;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    fprintf(stderr, "Unrecognized option: %c\n", *(argv[0] + 1));
	    break;
	}
    }
    if (argc < 2)
	usageerror();

    if (strcmp(argv[0], argv[1]) == 0) {
	fprintf(stderr,
		"This program must have different input and output files\n");
	return 1;
    }
    THR_Init();
    DCM_Debug(verbose);

	if(uncompress) {
		openOptions = (openOptions & ~DCM_ORDERMASK) | DCM_EXPLICITLITTLEENDIAN 
		   | DCM_PART10FILE;
	}

    cond = DCM_OpenFile(*argv, openOptions, &object);
    if (cond != DCM_NORMAL)
	goto abort;

	if(uncompress) {
#if (JPEGBITDEPTH == 12)
		cond = DCM_jpeg_uncompress_12_highlevel(object, options);
#else
		cond = DCM_jpeg_uncompress_8_highlevel(object, options);
#endif

		if (cond == DCM_NORMAL) {
			fprintf(stderr,"Decompression successful.\n");
		}
		else {
			fprintf(stderr,"Decompression failed (condition=%u).\n",(unsigned int)cond);
			goto abort;
		}
	}
	else {
#if (JPEGBITDEPTH == 12)
		cond = DCM_jpeg_compress_12_highlevel(object, options, -1);
#else
		cond = DCM_jpeg_compress_8_highlevel(object, options, -1);
#endif

		if (cond == DCM_NORMAL) {
			fprintf(stderr,"Compression successful.\n");
		}
		else {
			fprintf(stderr,"Compression failed (condition=%u).\n",(unsigned int)cond);
			goto abort;
		}
	}

	//fflush(stderr); fflush(stdout);
	//COND_DumpConditions();
	//fflush(stderr); fflush(stdout);
	//printf("runing WriteFile\n");

    cond = DCM_WriteFile(&object, options, *++argv);
    (void) DCM_CloseObject(&object);
    if (cond != DCM_NORMAL) {
		fflush(stderr);
		fprintf(stderr,"WriteFile failed (%d / %d), aborting\n",(int)cond, (int)(DCM_NORMAL));
		goto abort;
	}

    THR_Shutdown();
    return 0;

abort:
    COND_DumpConditions();
    THR_Shutdown();
    return 1;
}
#endif
