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
            // std::cout << best_index << " ";
            block = ref_img[ best_index ];
        }
        
        // What's next?
        // => create some dummy basis in the form of 0-1 bitmaps
        // => allow to compare individual layers and not a full image
        // => new block = sum_of( RGB values * best shape )
        const int BASIS_SIZE = 4;
        std::vector< std::vector<unsigned char> > basis = 
                                              {{0, 1, 0, 0,
                                                0, 1, 0, 0,
                                                0, 1, 0, 0,
                                                0, 1, 0, 0 },
                                               {0, 0, 0, 0,
                                                0, 0, 0, 0,
                                                1, 1, 1, 1,
                                                0, 0, 0, 0 },
                                               {0, 0, 0, 1,
                                                0, 0, 1, 0,
                                                0, 1, 0, 0,
                                                1, 0, 0, 0 },
                                               {1, 0, 0, 0,
                                                0, 1, 0, 0,
                                                0, 0, 1, 0,
                                                0, 0, 0, 1 }};
        /*std::vector< std::vector<unsigned char> > basis = 
                                              {{0, 1, 0, 0,
                                                1, 1, 1, 1,
                                                0, 1, 0, 0,
                                                0, 1, 0, 0 },
                                              
                                               {0, 0, 1, 0,
                                                0, 0, 1, 0,
                                                1, 1, 1, 1,
                                                0, 0, 1, 0 },

                                               {1, 0, 0, 1,
                                                1, 0, 0, 1,
                                                1, 0, 0, 1,
                                                1, 0, 0, 1 },
                                              
                                               {1, 1, 1, 1,
                                                0, 0, 0, 0,
                                                0, 0, 0, 0,
                                                1, 1, 1, 1 }};*/


        std::vector< Image > new_blocks = img->splitImage( 128 ); // Careful here
        for( auto& block : new_blocks ){
            for( int i=0 ; i<3 ; ++i){
                std::array< int, BASIS_SIZE+1 > scores; // Stores the index of the best last
                scores.fill( 0 );
                // Fill the scores array
                for( int j=0 ; j<BASIS_SIZE ; ++j ){
                    // Compare layer with basis' elements and compute score
                    for( int k=0 ; k<BASIS_SIZE*BASIS_SIZE ; ++k ){
                        scores[ j ] += basis[j][k] * block.getPixelValues()->at(i)[k];
                    }
                    if ( scores[j] > scores[ scores[BASIS_SIZE] ] )
                        scores[ BASIS_SIZE ] = j;
                }
                //std::cout << "Final scores: " << scores[0] << "-" << scores[1] << "-" << scores[2] << "-" << scores[3] << std::endl;
                
                // Compute mean value for current layer
                int mean_value = 0;
                for( int j=0 ; j<BASIS_SIZE*BASIS_SIZE ; ++j ){
                    mean_value += block.getPixelValues()->at(i)[j];
                }
                mean_value = mean_value/BASIS_SIZE/BASIS_SIZE; 

                // Assign best basis' element, with mean value to block's layer
                for( int j=0 ; j<BASIS_SIZE*BASIS_SIZE ; ++j ){
                    block.getPixelValues()->at(i)[j] =
                        mean_value * basis[ scores[BASIS_SIZE] ][j];
                }
            }
        }



        // Need a new ImagePPM constructor from sub-images
        ImagePPM* new_img = new ImagePPM( blocks, 512, 512, 255 );
        ImagePPM* b_img = new ImagePPM( new_blocks, 512, 512, 255 );
        b_img->addBrightness( 40 );

	// Write an image file
	new_img->writePPM( "new_test.ppm" );
	b_img->writePPM( "b_test.ppm" );
	delete img;
        delete new_img;
        delete b_img;
}
