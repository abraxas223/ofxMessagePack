//
//  ofxMsgPackUnPacker.h
//  example
//
//  Created by 堀 宏行 on 2013/01/29.
//
//

#ifndef example_ofxMsgPackUnPacker_h
#define example_ofxMsgPackUnPacker_h
#include "ofxMsgPacker.h"

template <class T>
class ofxMsgUnPacker
{
public:
    
    msgpack::unpacked unpacked;
    msgpack::unpacker unpakr;
    
    FILE * file = NULL;
    bool bLoopFile = true;
    bool bBufferEnd = false;
    
    void setupBuffer( ofxMsgPacker<T >& serializer )
    {
        unpakr.reserve_buffer( serializer.size() );
        memcpy( unpakr.buffer(), serializer.data(), serializer.size());
        unpakr.buffer_consumed( serializer.size() );
        bBufferEnd = false;
    }
    
    void updateBuffer()
    {
        if( !unpakr.next(&unpacked) && !bBufferEnd)
        {
            ofLogNotice() << "buffer end";
            bBufferEnd = true;
        }
    }
    
    void updateFile()
    {
        size_t size = 0;
        fread(&size, sizeof(size_t),1,file);
        if ( size > 0)
        {
            char *buf = new char[size];
            fread(buf, size,1, file);
            deserialize(buf, size);
            
        }
        if ( bLoopFile )
        {
            if (feof(file) != 0)
            {
                fseek(file, 0, SEEK_SET);
            }
        }
    }
    
    inline void get(T& result)
    {
        if ( !bBufferEnd )
            unpacked.get().convert( &result );
    }
    
    void openFile( string filename )
    {
        file = fopen(ofToDataPath(filename, true).c_str(), "rb");
        if ( file == NULL)
        {
            ofLogError() << "cannot open file:" + ofToDataPath(filename, true);
        }
    }
    
    void close()
    {
        if ( file != NULL) fclose(file);
        file = NULL;
    }
    
private:
    inline void deserialize( const char * data, size_t size )
    {
        msgpack::unpack( &unpacked, data, size );
    }
};


#endif
