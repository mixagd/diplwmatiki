#include <ao/ao.h>
#include <mpg123.h>
#include "pthread.h"
#define BITS 8

#include <iostream>
using namespace std;


int main (int argc, char* argv[])
{

//works when called here including path




const char * traklink="";
int tid= atoi (argv[1]);
//char *tid=argv[1];

if (tid==1){
	traklink= "/home/mixa/timon.mp3";

	}
else if (tid==2){
	traklink= "/home/mixa/karavi.mp3";

	}
else if (tid==3){
	traklink= "/home/mixa/timon.mp3";
	}
else if (tid==4){
	traklink= "/home/mixa/hippo.mp3";
	}
else if (tid==5){
	traklink= "/home/mixa/nanourisma.mp3";
	}
   mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;


    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    //mpg123_open(mh,traklink);
    mpg123_open(mh,traklink);
     mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
   format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
        //ao_play(dev, buffer, done);
        ao_play(dev, (char*)buffer, done);
    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

 // return 0;
}
