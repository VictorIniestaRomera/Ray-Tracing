#ifndef RTC_STB_IMAGE_H
#define RTC_STB_IMAGE_H

//Deactivate warnings from Microsoft C++ compiler
#ifndef _MSC_VER
#pragma warning (push, 0)
#endif

#define STB_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION

#include "external/stb_image.h"

#include <iostream>
#include <cstdlib>

class RTW_image {
	const int bytesPerPixel = 3;		//Number of bytes per pixel
	float* f_data = nullptr;			//Linear floating point pixel data
	unsigned char* b_data = nullptr;	//Linear 8bit pixel data
	int imageWidth = 0;					//Loaded image width
	int imageHeight = 0;				//Loaded image height
	int bytesPerScanline = 0;			//Number of bytes per scanline

	static int clamp(int x, int low, int high) {
		if (x < low) return low;

		if (x < high) return x;

		return high - 1;
	}

	static unsigned char float_to_byte(float value) {
		if (value <= 0.0) return 0;

		if (1.0 <= value) return 255;

		return static_cast<unsigned char>(256.0 * value);
	}

	void convert_to_bytes() {
		int totalBytes = imageWidth * imageHeight * bytesPerPixel;
		b_data = new unsigned char[totalBytes];

		unsigned char* b_ptr = b_data;
		float* f_ptr = f_data;

		for (auto i = 0; i < totalBytes; i++, b_ptr++, f_ptr++) {
			*b_ptr = float_to_byte(*f_ptr);
		}
	}

public:
	RTW_image() {}

	RTW_image(const char* imageFilename) {
		std::string filename = std::string(imageFilename);
		auto imageDir = getenv("RTW_IMAGES");

		//Hunt for the image location
		if (imageDir && load(std::string(imageDir) + "/" + imageFilename)) return;

		if (load(filename)) return;

		if (load("images/" + filename)) return;

		if (load("../images/" + filename)) return;

		if (load("../../images/" + filename)) return;

		if (load("../../../images/" + filename)) return;

		if (load("../../../../images/" + filename)) return;

		if (load("../../../../../images/" + filename)) return;

		if (load("../../../../../../images/" + filename)) return;

		std::cerr << "ERROR; Could not load	image file: '" << imageFilename << "'!\n";
	}

	~RTW_image() {
		delete[] b_data;

		stbi_image_free(f_data);
	}

	bool load(const std::string& filename) {
		int n = bytesPerPixel;

		f_data = stbi_loadf(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);

		if (f_data == nullptr) return false;

		bytesPerScanline = imageWidth * bytesPerPixel;
		convert_to_bytes();

		return true;
	}

	int width() const {
		return (f_data == nullptr) ? 0 : imageWidth;
	}

	int height() const {
		return (f_data == nullptr) ? 0 : imageHeight;
	}

	const unsigned char* pixel_data(int x, int y) const {
		static unsigned char magenta[] = {255, 0, 255};

		if (b_data == nullptr) return magenta;

		x = clamp(x, 0, imageWidth);
		y = clamp(y, 0, imageHeight);

		return b_data + y * bytesPerScanline + x * bytesPerPixel;
	}
};

#ifdef _MSC_VER
#pragma warning(pop);
#endif

#endif