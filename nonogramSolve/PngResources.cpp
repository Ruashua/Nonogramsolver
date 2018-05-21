#include "PngResources.h"



/* Given "bitmap", this returns the pixel of bitmap at the point
("x", "y"). */

pixel_t * pixel_at(bitmap_t * bitmap, int x, int y)
{
	return bitmap->pixels + bitmap->width * y + x;
}

/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
success, non-zero on error. */

int save_png_to_file(bitmap_t *bitmap, const char *path)
{
	FILE * fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	size_t x, y;
	png_byte ** row_pointers = NULL;
	/* "status" contains the return value of this function. At first
	it is set to a value which means 'failure'. When the routine
	has finished its work, it is set to a value which means
	'success'. */
	int status = -1;
	/* The following number is set by trial and error only. I cannot
	see where it it is documented in the libpng manual.
	*/
	int pixel_size = 4;
	int depth = 8;

	fp = fopen(path, "wb");
	if (!fp) {
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	/* Set up error handling. */

	if (setjmp(png_jmpbuf(png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR(png_ptr,
		info_ptr,
		bitmap->width,
		bitmap->height,
		depth,
		PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */

	row_pointers = (png_byte**)png_malloc(png_ptr, bitmap->height * sizeof(png_byte *));
	for (y = 0; y < bitmap->height; ++y) {
		png_byte* row =
			(png_byte*)png_malloc(png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size);
		row_pointers[y] = row;
		for (x = 0; x < bitmap->width; ++x) {
			pixel_t * pixel = pixel_at(bitmap, x, y);
			*row++ = pixel->red;
			*row++ = pixel->green;
			*row++ = pixel->blue;
			//*row++ = pixel->alpha;
		}
	}

	/* Write the image data to "fp". */

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	"status" to a value which indicates success. */

	status = 0;

	for (y = 0; y < bitmap->height; y++) {
		png_free(png_ptr, row_pointers[y]);
	}
	png_free(png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
	png_destroy_write_struct(&png_ptr, &info_ptr);
png_create_write_struct_failed:
	fclose(fp);
fopen_failed:
	return status;
}

#ifdef referencestuffignore
void write_png(char *file_name /* , ... other image information ... */)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_colorp palette;

	/* Open the file */
	fp = fopen(file_name, "wb");
	if (fp == NULL)
		return (ERROR);

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also check that
	* the library version is compatible with the one used at compile time,
	* in case we are using dynamically linked libraries.  REQUIRED.
	*/
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
		png_voidp user_error_ptr, user_error_fn, user_warning_fn);

	if (png_ptr == NULL)
	{
		fclose(fp);
		return (ERROR);
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_write_struct(&png_ptr, NULL);
		return (ERROR);
	}

	/* Set error handling.  REQUIRED if you aren't supplying your own
	* error handling functions in the png_create_write_struct() call.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* If we get here, we had a problem writing the file */
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return (ERROR);
	}

	/* One of the following I/O initialization functions is REQUIRED */

#ifdef streams /* I/O initialization method 1 */
	/* Set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp);

#else no_streams /* I/O initialization method 2 */
	/* If you are using replacement write functions, instead of calling
	* png_init_io() here you would call
	*/
	png_set_write_fn(png_ptr, (void *)user_io_ptr, user_write_fn,
		user_IO_flush_function);
	/* where user_io_ptr is a structure you want available to the callbacks */
#endif no_streams /* Only use one initialization method */

#ifdef hilevel
	/* This is the easy way.  Use it if you already have all the
	* image info living in the structure.  You could "|" many
	* PNG_TRANSFORM flags into the png_transforms integer here.
	*/
	png_write_png(png_ptr, info_ptr, png_transforms, NULL);

#else
	/* This is the hard way */

	/* Set the image information here.  Width and height are up to 2^31,
	* bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	* the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	* PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	* or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	* PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	* currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	*/
	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_ ? ? ? ,
		PNG_INTERLACE_ ? ? ? ? , PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* Set the palette if there is one.  REQUIRED for indexed-color images */
	palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH
		* png_sizeof(png_color));
	/* ... Set palette colors ... */
	png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
	/* You must not free palette here, because png_set_PLTE only makes a link to
	* the palette that you malloced.  Wait until you are about to destroy
	* the png structure.
	*/

	/* Optional significant bit (sBIT) chunk */
	png_color_8 sig_bit;

	/* Otherwise, if we are dealing with a color image then */
	sig_bit.red = true_red_bit_depth;
	sig_bit.green = true_green_bit_depth;
	sig_bit.blue = true_blue_bit_depth;




	/* Optionally write comments into the image */
	{
		png_text text_ptr[3];

		char key0[] = "Title";
		char text0[] = "Mona Lisa";
		text_ptr[0].key = key0;
		text_ptr[0].text = text0;
		text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[0].itxt_length = 0;
		text_ptr[0].lang = NULL;
		text_ptr[0].lang_key = NULL;

		char key1[] = "Author";
		char text1[] = "Leonardo DaVinci";
		text_ptr[1].key = key1;
		text_ptr[1].text = text1;
		text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
		text_ptr[1].itxt_length = 0;
		text_ptr[1].lang = NULL;
		text_ptr[1].lang_key = NULL;

		char key2[] = "Description";
		char text2[] = "<long text>";
		text_ptr[2].key = key2;
		text_ptr[2].text = text2;
		text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
		text_ptr[2].itxt_length = 0;
		text_ptr[2].lang = NULL;
		text_ptr[2].lang_key = NULL;

		png_set_text(write_ptr, write_info_ptr, text_ptr, 3);
	}

	/* Other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs */

	/* Note that if sRGB is present the gAMA and cHRM chunks must be ignored
	* on read and, if your application chooses to write them, they must
	* be written in accordance with the sRGB profile
	*/

	/* Write the file header information.  REQUIRED */
	png_write_info(png_ptr, info_ptr);

	/* If you want, you can write the info in two steps, in case you need to
	* write your private chunk ahead of PLTE:
	*
	*   png_write_info_before_PLTE(write_ptr, write_info_ptr);
	*   write_my_chunk();
	*   png_write_info(png_ptr, info_ptr);
	*
	* However, given the level of known- and unknown-chunk support in 1.2.0
	* and up, this should no longer be necessary.
	*/

	/* Once we write out the header, the compression type on the text
	* chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
	* PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
	* at the end.
	*/

	/* Set up the transformations you want.  Note that these are
	* all optional.  Only call them if you want them.
	*/



	/* Shift the pixels up to a legal bit depth and fill in
	* as appropriate to correctly scale the image.
	*/
	png_set_shift(png_ptr, &sig_bit);

	/* Pack pixels into bytes */
	png_set_packing(png_ptr);

	/* Swap location of alpha bytes from ARGB to RGBA */
	png_set_swap_alpha(png_ptr);

	/* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
	* RGB (4 channels -> 3 channels). The second parameter is not used.
	*/
	png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

	/* Flip BGR pixels to RGB */
	png_set_bgr(png_ptr);

	/* Swap bytes of 16-bit files to most significant byte first */
	png_set_swap(png_ptr);

	/* Swap bits of 1, 2, 4 bit packed pixel formats */
	png_set_packswap(png_ptr);

	/* The easiest way to write the image (you may have a different memory
	* layout, however, so choose what fits your needs best).  You need to
	* use the first method if you aren't handling interlacing yourself.
	*/
	png_uint_32 k, height, width;
	png_byte image[height][width*bytes_per_pixel];
	png_bytep row_pointers[height];

	if (height > PNG_UINT_32_MAX / png_sizeof(png_bytep))
		png_error(png_ptr, "Image is too tall to process in memory");

	for (k = 0; k < height; k++)
		row_pointers[k] = image + k*width*bytes_per_pixel;

	/* One of the following output methods is REQUIRED */

#ifdef entire /* Write out the entire image data in one call */
	png_write_image(png_ptr, row_pointers);

	/* The other way to write the image - deal with interlacing */

#else no_entire /* Write out the image data by one or more scanlines */

	/* The number of passes is either 1 for non-interlaced images,
	* or 7 for interlaced images.
	*/
	for (pass = 0; pass < number_passes; pass++)
	{
		/* Write a few rows at a time. */
		png_write_rows(png_ptr, &row_pointers[first_row], number_of_rows);

		/* If you are only writing one row at a time, this works */
		for (y = 0; y < height; y++)
			png_write_rows(png_ptr, &row_pointers[y], 1);
	}
#endif no_entire /* Use only one output method */

	/* You can write optional chunks like tEXt, zTXt, and tIME at the end
	* as well.  Shouldn't be necessary in 1.2.0 and up as all the public
	* chunks are supported and you can use png_set_unknown_chunks() to
	* register unknown chunks into the info structure to be written out.
	*/

	/* It is REQUIRED to call this to finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);
#endif hilevel

	/* If you png_malloced a palette, free it here (don't free info_ptr->palette,
	* as recommended in versions 1.0.5m and earlier of this example; if
	* libpng mallocs info_ptr->palette, libpng will free it).  If you
	* allocated it with malloc() instead of png_malloc(), use free() instead
	* of png_free().
	*/
	png_free(png_ptr, palette);
	palette = NULL;

	/* Similarly, if you png_malloced any data that you passed in with
	* png_set_something(), such as a hist or trans array, free it here,
	* when you can be sure that libpng is through with it.
	*/
	png_free(png_ptr, trans);
	trans = NULL;
	/* Whenever you use png_free() it is a good idea to set the pointer to
	* NULL in case your application inadvertently tries to png_free() it
	* again.  When png_free() sees a NULL it returns without action, thus
	* avoiding the double-free security problem.
	*/

	/* Clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* Close the file */
	fclose(fp);

	/* That's it */
	return (OK);
}
#endif