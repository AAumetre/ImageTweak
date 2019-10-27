#include <iostream>

// Personal code
#include "image.hpp"

/* =========== PROGRAMING NOTES =============
 *
 * Important: images should only be manipulated under the form of
 *            an Image object.
============================================= */

int main( int argc, char* argv[] ){
	std::cout << "Hello World!" << std::endl;
	std::cout << "You have entered " << argc-1 << " arguments." << std::endl;
	// TODO: manage arguments
	for ( int i=1 ; i < argc ; ++i ){
		std::cout << *argv[i] << std::endl;
	}
	// Read an image file
	std::string file_name = "lena.ppm"; // 512x512
	ImagePPM* img = new ImagePPM( file_name );
	
        // Load reference images
        std::vector< ImagePPM > ref_img;
        ref_img.push_back( ImagePPM( "1.ppm" ) );
        ref_img.push_back( ImagePPM( "2.ppm" ) );
        ref_img.push_back( ImagePPM( "3.ppm" ) );
        ref_img.push_back( ImagePPM( "4.ppm" ) );
	
        // Modify it
        // /!!!!\ does not return, deletes memory
        std::vector< Image > blocks = img->splitImage( 64 );

        // Compare each bit with the references
        for( auto& block : blocks ){
            long distance = 1e12, new_distance = 9;
            int best_index = 0;
            for( int i=0 ; i<4 ; ++i ){ // Needs to be set to the size of ref_img (4)
                new_distance = ref_img[i].compareAbs( *block.getPixelValues() );
                // std::cout << new_distance << " ";
                if( new_distance < distance ){
                    best_index = i;
                    distance = new_distance;
                }
            }
            std::cout << best_index << " ";
            block = ref_img[ best_index ];
        }
        

        // Need a new ImagePPM constructor from sub-images
        // ImagePPM* new_img = new ImagePPM( image_ra, 512, 512, 255 );
        // finir au-dessus


	// Write an image file
	// new_img->writePPM( "new_test.ppm" );
	//img->writePPM( "new_test.ppm" );
	delete img;
        //delete new_img;
        //delete sub_img;
}
