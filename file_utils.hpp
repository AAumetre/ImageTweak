#pragma once

std::string getLine( std::vector< unsigned char >& buffer, int& start );
std::string getBlock( std::vector< unsigned char >& buffer, int& start );

// Takes in a buffer and a start position,
// finds a line, returns it and sets the last visited position
std::string getLine( std::vector< unsigned char >& buffer, int& start ){
        std::string line;
        int cursor = start;
        while( buffer[ cursor ]!='\0' && buffer[ cursor ]!='\r' && buffer[ cursor ]!='\n' ){
                line.push_back( buffer[ cursor ] );
                ++cursor;
        }
        start = cursor+1;
        return line;
}

// Takes in a buffer and a start position,
// finds a block of data, returns it and sets the last visited position
std::string getBlock( std::vector< unsigned char >& buffer, int& start ){
        std::string block;
        int cursor = start;
        while( buffer[ cursor ]!='\0' &&
                        buffer[ cursor ]!='\r' && 
                        buffer[ cursor ]!='\n' &&
                        buffer[ cursor ]!=' ' ){
                block.push_back( buffer[ cursor ] );
                ++cursor;
        }
        start = cursor+1;
        return block;
}   
