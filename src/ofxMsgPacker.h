//
//  ofxMessagePack.h
//  nikeLIT
//
//  Created by 堀 宏行 on 2013/01/25.
//
//

#ifndef __nikeLIT__ofxMessagePack__
#define __nikeLIT__ofxMessagePack__

#include <iostream>
#include <msgpack.hpp>
#include "ofMain.h"

template <class T>
class ofxMsgPacker
{
public:
    msgpack::sbuffer sbuff;
    msgpack::packer<msgpack::sbuffer> * pkr;
    
    FILE * file = NULL;
    
    void setup()
    {
        pkr = new msgpack::packer<msgpack::sbuffer>(sbuff);
    }
    
    
    void openFile( string filename )
    {
        file = fopen(ofToDataPath(filename).c_str(),"wb");
    }
    
    void closeFile()
    {
        if ( file != NULL )
        {
            fclose(file);
            file = NULL;
        }
    }
    
    void add( const T data )
    {
        if ( file != NULL )clear();
        if ( pkr != NULL) pkr->pack( data );
        else
        {
            ofLogError() << "please call ofxMsgPacker::setup";
        }
        size_t dSize = size();
        if ( file != NULL ) //write to file
        {
            fwrite( &dSize, sizeof(size_t), 1, file);
            fwrite( sbuff.data(), size(), 1, file);
        }
    }
    
    void clear()
    {
        sbuff.clear();
    }
    
    inline const char* data()
    {
        return sbuff.data();
    }
    
    inline const size_t size()
    {
        return sbuff.size();
    }
};

#endif /* defined(__nikeLIT__ofxMessagePack__) */
