#pragma once

#include <vector>

// TODO: have three separate layers to work with 
class Image{
	public:
		Image( void ){}
		~Image( void ){}

	public:
		void setFileName( std::string file );
		void setFileType( std::string type );
                std::vector<unsigned char> getPixelValues( void );
                void addBrightness( int more );
		void splitImage( int cuts,
                        std::vector< std::vector<unsigned char> >* storage_ra );
                void dumpValues( int start_index, int count );
                int compareAbs( std::vector<unsigned char>& img );

	protected:
		std::string _file;
		std::string _type;
		int _width;
		int _height;
		int _levels;
		std::vector<unsigned char> _pixel_array;
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
