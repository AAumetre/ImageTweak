#pragma once

#include <vector>
#include <array>

// Corresponds to the RGB layers of the image
using ImgRA = std::array< std::vector<unsigned char>, 3 >;

// TODO: have three separate layers to work with 
class Image{
	public:
		Image( void ){}
		~Image( void ){}

	public:
		void setFileName( std::string file );
		void setFileType( std::string type );
                ImgRA* getPixelValues( void );
                void addBrightness( int more );
                std::vector< Image > splitImage( int cuts );
                void dumpValues( int start_index, int count );
                int compareAbs( ImgRA& img );

	protected:
		std::string _file;
		std::string _type;
		int _width;
		int _height;
		int _levels;
		// std::vector<unsigned char> _pixel_array;
                ImgRA _pixel_layers;
};

class ImagePPM : public Image {
	public:
		ImagePPM( void );
		ImagePPM( std::string file );
                ImagePPM( std::vector<unsigned char>& raw_data,
                        int width, int height, int levels );
                ImagePPM( std::vector< std::vector<unsigned char> >& raw_data,
                        int width, int height, int levels );
                ~ImagePPM( void ){}

	public: 
		void readPPM( void );
		void writePPM( std::string new_file );
};
