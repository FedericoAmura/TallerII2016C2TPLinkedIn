#ifndef APPSERVER_INCLUDE_DATABASE_JPEGWRAPPER_H_
#define APPSERVER_INCLUDE_DATABASE_JPEGWRAPPER_H_

#include "DBExceptions.h"
#include "../jpeg/jpeglib.h"
#include "../jpeg/jerror.h"
#include <vector>

class FotoData
{
 public:
	FotoData() : width(0),height(0) {}
	size_t width;
	size_t height;
	vector<char> data;
};

using std::vector;

/*void onErrorExit (j_common_ptr cinfo)
{
  jpeg_destroy_decompress(&cinfo);
  throw BadInputException("Error de archivo jpeg");
}*/

FotoData descomprimir (std::vector<char> binaryData)
{
	// TODO: Terminar
	FotoData result;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);


	JSAMPARRAY buffer;	// Output row buffer
	int row_stride;		// physical row width in output buffer
	jpeg_mem_src(&cinfo, (unsigned char*) binaryData.data(), binaryData.size());
	int headerReturn = jpeg_read_header(&cinfo, TRUE);
	if (headerReturn != JPEG_HEADER_OK) throw BadInputException("No es un jpeg");
	(void) jpeg_start_decompress(&cinfo);
	result.width = cinfo.output_width;
	result.height = cinfo.output_height;
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, TRUE);
		result.data.insert(result.data.end(),
				(char*)&buffer, (char*)&buffer+row_stride);
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}



#endif /* APPSERVER_INCLUDE_DATABASE_JPEGWRAPPER_H_ */
