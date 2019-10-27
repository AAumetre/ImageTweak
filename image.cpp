#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator>
#include <cmath>

#include "image.hpp"
#include "file_utils.hpp"

// Image constructor
Image::Image( int width, int height ){
    _width = width;
    _height = height;
    // Fill pixel array
    /*for( int i=0 ; i<3 ; ++i){
        _pixel_layers[ i ].resize( _width*_height );           
    }*/
}

void Image::addBrightness( int more ){
    for( int i=0 ; i<3 ; ++i ){
        for( auto& pxl : _pixel_layers[i] ){
            if( pxl+more > _levels ) pxl = _levels;
            else pxl = pxl+more;
        }
    }
}

void Image::setFileName( std::string file ){
    _file = file;
}

void Image::setFileType( std::string type ){
    _type = type;
}

ImgRA* Image::getPixelValues( void ){
    return &_pixel_layers;
}

// Takes a vector of vectors and fills it with square sub-images
// TODO: manage not square confs
// TODO: to be updated for RGB layers
std::vector< Image > Image::splitImage( int cuts ){
    // Validity check
    if( _width%cuts!=0 || _height%cuts!=0 ){
	std::cout << "Uneven cuts of the image, aborting." << std::endl;
	exit( -1 );
    }
    // Internal variables
    int cut_size_i = _width/cuts;
    int cut_size_j = _height/cuts;
    int curr_block = 0;
    int curr_index = 0;
    unsigned int n_block = cuts*cuts;
    
    // Initialize Image vector
    std::vector< Image > result;

    for( unsigned int i=0 ; i<n_block ; ++i){
        result.push_back( Image( cut_size_i, cut_size_j ) );
    }
    
    // Blocks are numbered
    for( int j=0 ; j<_height ; ++j ){
         for( int i=0 ; i<_width ; ++i ){
            curr_block = j/cut_size_j*cuts + i/cut_size_i;
            curr_index = j*_width + i;

            /*if( curr_block == 0){
                std::cout << "Block " << curr_block << " index " << curr_index << "  "; 
                std::cout << "RGB: " << int(_pixel_layers[0][curr_index]) << " " << int(_pixel_layers[1][curr_index]) << " " << int(_pixel_layers[2][curr_index]) << "  ";
            }*/

            // Need to correctly index new values
            // int() does not solve it
            result[ curr_block ].getPixelValues()->at(0).push_back(
                                            _pixel_layers[0][curr_index] ); // Red
            result[ curr_block ].getPixelValues()->at(1).push_back( 
                                            _pixel_layers[1][curr_index] ); // Green
            result[ curr_block ].getPixelValues()->at(2).push_back(
                                            _pixel_layers[2][curr_index] ); // Blue
          } 
    }
    // std::cout << std::endl;
    // result[0].dumpValues(0, 6);
    return result;
}

// Displays pixel values, range in number of pixels,
// not in number of values [R, G, B]
void Image::dumpValues( int start_index, int count ){
    if( start_index+count > int(_pixel_layers[0].size()) ){
        std::cout << "Query out of range, aborting." << std::endl;
        exit( -1 );
    }
    for( int i=start_index ; i<(start_index+count) ; ++i ){
        std::cout << "["
            << int(_pixel_layers[0].at(i)) << ", "
            << int(_pixel_layers[1].at(i)) << ", "
            << int(_pixel_layers[2].at(i)) << "]  ";
    }
    std::cout << std::endl;
}

// Compares a vector of pixels values with the image and
// returns a distance
int Image::compareAbs( ImgRA& img ){
    // TODO: check size !!!
    int distance = 0;

    for( int c=0 ; c<3 ; ++c ){
        for( int i=0 ; i<_width*_height ; ++i ){
            distance += abs( _pixel_layers[c].at(i) - 
                    img[c].at(i) );
        }
    }
    return distance;
}

// Sets part of an image, from an image
/*void Image::setImagePatch( Image& src_img, int x, int y ){
        
}*/

/* ======================================================================
 * 		ImagePPM Implementation
 ======================================================================*/
ImagePPM::ImagePPM( std::string file ){
        setFileName( file );
        setFileType( "ppm" );
        readPPM();
}

// Creates an ImagePPM object, based on the raw data, which
// is formated as : R G B R G B ...
ImagePPM::ImagePPM( std::vector<unsigned char>& raw_data,
        int width, int height, int levels ){
    setFileName( "Raw data" );
    setFileType( "ppm" );
    _width = width;
    _height = height;
    _levels = levels;

    for( int i=0 ; i<3*(width*height) ; i+=3){
        _pixel_layers[0].push_back( raw_data[ i+0 ] );
        _pixel_layers[1].push_back( raw_data[ i+1 ] );
        _pixel_layers[2].push_back( raw_data[ i+2 ] );
    }
}

// TODO: to be updated for RGB layers
// Creates an ImagePPM object from pieces
/*ImagePPM::ImagePPM( std::vector< std::vector<unsigned char> >& raw_data,
        int width, int height, int levels ){
    setFileName( "" );
    setFileType( "ppm" );
    _width = width;
    _height = height;
    _levels = levels;

    int cuts = int( std::sqrt( raw_data.size() ) );
    int cut_size = _width/cuts;
    int curr_index;
    int curr_block;
    int local_index;
    for( int i=0 ; i<3*_width*_height ; ++i ){
        _pixel_layers[0].push_back( 0 );
        _pixel_layers[1].push_back( 0 );
        _pixel_layers[2].push_back( 0 );
    }
    
    for( int j=0 ; j<_height ; ++j ){
        for( int i=0 ; i<3*_width ; ++i ){
            curr_index = j*_width*3 + i;
            curr_block = (j/cut_size)*cuts + ( i/3/cut_size);  
            
            int l_i = i - 3*(curr_block%cuts)*cut_size;
            int l_j = j - (curr_block/cuts)*cut_size;
            
            local_index = l_j*3*cut_size + l_i;
            // std::cout << "[" << l_i << "," << l_j << "] => " << local_index << " ";
            _pixel_array[ curr_index ] =
                raw_data[ curr_block ][ local_index ];
        }
    }
}*/

void ImagePPM::readPPM(){
    std::ifstream file_obj( _file, std::ios::binary  );
    std::vector< unsigned char > buffer(
            std::istreambuf_iterator< char >( file_obj ), {} );
    std::cout << "File " << _file
                    <<      " of size " << buffer.size()
                    << " was successfully opened." << std::endl;
    // Extract metadata
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
    std::cout << std::endl;
    // Copy content into pixel_array
    for ( unsigned int i=cursor_position ; i<buffer.size() ; i+=3 ){
        _pixel_layers[0].push_back( buffer[ i+0 ] );
        _pixel_layers[1].push_back( buffer[ i+1 ] );
        _pixel_layers[2].push_back( buffer[ i+2 ] );
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
        for ( int i=0 ; i<_width*_height ; ++i ){
            output_file << _pixel_layers[0][ i ];
            output_file << _pixel_layers[1][ i ];
            output_file << _pixel_layers[2][ i ];
        }
        output_file.close();
}
