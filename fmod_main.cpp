 #include "pthread.h"
 #include <signal.h>
 #define BITS 8

 #include <iostream>
 #include <stdio.h>
 
 #include "fmod.hpp"
 #include "common.h"
 #include "/home/mixa/fmod_errors.h"
 #include "/home/mixa/common.cpp"
 #include "/home/mixa/common_platform.cpp"

 using namespace std;



FMOD_RESULT       result;
int sound=0;
int main (int argc, char* argv[])
{
int trakid= atoi (argv[1]);
cout<<"eimai mesa"<<endl;
   FMOD::System     *system;
	FMOD::Sound      *sound1, *sound2, *sound3, *sound4;
	FMOD::Channel    *channel = 0;
	unsigned int    version;
	void             *extradriverdata = 0;

	bool         playing = 0;

	Common_Init(&extradriverdata);

	    /*
	        Create a System object and initialize
	    */
	    result = System_Create(&system);
	    ERRCHECK(result);
	    result = system->getVersion(&version);
	    ERRCHECK(result);


	if (version < FMOD_VERSION)
	{
	    printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
	}

	result = system->init(2, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);

	result = system->createSound("/home/mixa/timon.mp3", FMOD_LOOP_OFF, 0, &sound1);
	ERRCHECK(result);
    result = system->createSound("/home/mixa/karavi.mp3",FMOD_LOOP_OFF, 0, &sound2 );
    ERRCHECK(result);
    result = system->createSound("/home/mixa/timon.mp3",FMOD_LOOP_OFF, 0, &sound3);
    ERRCHECK(result);
    result = system->createSound("/home/mixa/hippo.mp3", FMOD_LOOP_OFF, 0, &sound4);
    ERRCHECK(result);

    switch (trakid)
                    {
                        case 1 :
                        {
                        	result = system->playSound(sound1, 0, false, &channel);
                        	ERRCHECK(result);

                        	break;
                        }
                        case 2 :
                        {
                        	result = system->playSound(sound2, 0, false, &channel);
                        	ERRCHECK(result);

                        	break;
                        }
                        case 3 :
                        {
                        	result = system->playSound(sound3, 0, false, &channel);
                        	ERRCHECK(result);

                        	break;
                        }
                        case 4 :
                        {
                        	result = system->playSound(sound4, 0, false, &channel);
                        	ERRCHECK(result);

                        	break;
                         }
                    }




    do
        {
    	 Common_Update();

            {
            	 if (channel)
            	  {

            		 result = channel->isPlaying(&playing);
            		 if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            		 {
            			 ERRCHECK(result);
            		 }

            	   }
            }
            Common_Sleep(10);
        }while(playing>0);




result = system->close();
result = system->release();
Common_Close();

return sound;


}
