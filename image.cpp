#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>

#include "image.hpp"
#include "file_utils.hpp"

void Image::addBrightness( int more ){
        for( auto& pxl:_pixel_array ){
                if( pxl+more > _levels ) pxl = _levels;
                else pxl = pxl+more;
        }
}

void Image::setFileName( std::string file ){
	_file = file;
}

void Image::setFileType( std::string type ){
	_type = type;
}

// Takes a vector of vectors and fills it with square sub-images
// TODO: manage not square confs
void Image::splitImage( int cuts, std::vector< std::vector<unsigned char> >* storage_ra ){
	if( _width%cuts!=0 || _height%cuts!=0 ){
		std::cout << "Uneven cuts of the image, aborting." << std::endl;
		exit( -1 );
	}
    int cut_size_i = _width/cuts;
    int cut_size_j = _height/cuts;
    int curr_block = 0;
    int curr_index = 0;
    unsigned int n_block = cuts*cuts;
    // Initialize vector
    while( storage_ra->size() < n_block ){
        storage_ra->push_back( {} );
    }

	for( int j=0 ; j<_height ; ++j ){
        for( int i=0 ; i<3*_width ; ++i ){
            curr_block = (j/cut_size_j)*cuts + ( i/3/cut_size_i);
            curr_index = j*_width*3+i;

            /* std::cout << "Block " << curr_block <<
                   " index " << j*_width+i <<
                   " (i,j): " << i << " : " << j << std::endl;*/
            storage_ra->at( curr_block ).push_back(
                        _pixel_array.at( curr_index ));
        } 
    }
}

// Displays pixel values, range in number of pixels,
// not in number of values
void Image::dumpValues( int start_index, int count ){
    if( 3*(start_index+count) > int(_pixel_array.size()) ){
        std::cout << "Query out of range, aborting." << std::endl;
        exit( -1 );
    }
    for( int i=3*start_index ; i<3*(start_index+count) ; i+=3 ){
        std::cout << "["
            << int(_pixel_array.at( i+0 )) << ", "
            << int(_pixel_array.at( i+1 )) << ", "
            << int(_pixel_array.at( i+2 )) << "]  ";
    }
    std::cout << std::endl;
}

// Compares a vector of pixels values with the image and
// returns a distance
int Image::compareAbs( std::vector<unsigned char>& img ){
    // TODO: check size !!!
    int distance = 0;

    for( int i=0 ; i<3*_width*_height ; ++i ){
        distance += abs( _pixel_array[i]-img[i] );
    }
    return distance;
}

/* ======================================================================
 * 		ImagePPM Implementation
 ======================================================================*/
ImagePPM::ImagePPM( std::string file ){
        setFileName( file );
        setFileType( "ppm" );
        readPPM();
}

ImagePPM::ImagePPM( std::vector<unsigned char>& raw_data, int width, int height, int levels ){
    setFileName( "Raw data" );
    setFileType( "ppm" );
    _pixel_array = raw_data;
    _width = width;
    _height = height;
    _levels = levels;
}

void ImagePPM::readPPM(){
        std::ifstream file_obj( _file, std::ios::binary  );
        std::vector< unsigned char > buffer( std::istreambuf_iterator< char >( file_obj ), {} );
        std::cout << "File " << _file
                        <<      " of size " << buffer.size()
                        << " was successfully opened." << std::endl;
        // Extract metadata
        std::cout << std::endl;
        int cursor_position = 0;
        if ( getBlock( buffer, cursor_position ) != "P6" ){
                        std::cout << "Wrong file type." << std::endl;
                        exit( -1 );
        }
        _width  = std::stoi( getBlock( buffer, cursor_position ) );
        _height = std::stoi( getBlock( buffer, cursor_position ) );
        _levels = std::stoi( getBlock( buffer, cursor_position ) );

        std::cout << "PPM image of " << _width << "x" << _height <<
                " with " << _levels << " levels." << std::endl;

        // Copy content into pixel_array
        for ( unsigned int i=cursor_position ; i<buffer.size() ; ++i ){
                _pixel_array.push_back( buffer[ i ] );
        }
}

void ImagePPM::writePPM( std::string new_file  ){
        std::ofstream output_file;
        output_file.open( new_file );

        // Write metadata
        output_file << "P6" <<std::endl;
        output_file << _width << " ";
        output_file << _height << std::endl;
        output_file << _levels << "\n\0";

        // Write binary content
        for( auto& pxl:_pixel_array ){
                output_file << pxl;
        }
        output_file.close();
}

