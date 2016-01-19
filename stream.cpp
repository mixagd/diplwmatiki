 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <ao/ao.h>
 #include <mpg123.h>
 #include "pthread.h"
 #include <signal.h>
 #include <spawn.h>
 #include <sys/wait.h>
 #define BITS 8

 #include <iostream>
 #include <stdio.h>
 
 #include "fmod.hpp"
 #include "common.h"
 #include "/home/mixa/fmod_errors.h"
 #include "/home/mixa/common.cpp"
 #include "/home/mixa/common_platform.cpp"

 using namespace std;
 using namespace cv;

 /*1=happy
   2=sad
   3=open*/


 /** Function Headers */
 void detectAndDisplay( Mat frame );
void *start_posix_spawn(void* threadid);

 /** Global variables */
  pid_t pid, pid_happy, pid_sad, pid_open;
 const char* trackid;
 extern char **environ;
 pthread_t thread;
 int rc;
 int emotion, c_happy, c_sad, c_open=0;
 String face_cascade_name = "haarcascade_frontalface_alt.xml";
 String open_cascade_name = "open_1265.xml";
 String sad_cascade_name = "sad_1880.xml";
 String happy_cascade_name = "happy_1593.xml";
 String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

 CascadeClassifier face_cascade;
 CascadeClassifier open_cascade;
 CascadeClassifier sad_cascade;
 CascadeClassifier happy_cascade;
 CascadeClassifier eyes_cascade;

 string window_name = "Capture - Face detection";
 RNG rng(12345);
 
 int thread_arr[3]={0,0,0};
 int t=0;
 
 
 /** @function main */
 int main( int argc, const char** argv )
 {
   
   VideoCapture capture;
   Mat frame;

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !open_cascade.load( open_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !sad_cascade.load( sad_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !happy_cascade.load( happy_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. Read the video stream
   if (!capture.open("http://192.168.1.80:8081/?x.mjpg"))
      { printf(" --(!) No open -- Break!"); return 0; }
   //if( capture )
   //{
     while( true )
     {
   if(capture.read(frame)) 
     

   //-- 3. Apply the classifier to the frame
       
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
  // }
   return 0;
 }

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{

  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    
    Rect mouthROI;
    mouthROI.x = (faces[i].x);
    mouthROI.y = (faces[i].y+faces[i].height/2);
    mouthROI.width = (faces[i].width);
    mouthROI.height = (faces[i].height/2);
    Mat mouthroi=frame(mouthROI);
    
    std::vector<Rect> open;
    std::vector<Rect> sad;
    std::vector<Rect> happy;
    std::vector<Rect> eyes
;
//-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t k = 0; k < eyes.size(); k++ )
     {
       Point center( faces[i].x + eyes[k].x + eyes[k].width*0.5, faces[i].y + eyes[k].y + eyes[k].height*0.5 );
       int radius = cvRound( (eyes[k].width + eyes[k].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
	//cout<<"open"<<endl;
     

    //-- In each face, detect open
    open_cascade.detectMultiScale( mouthroi, open, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t j = 0; j < open.size(); j++ )
     {
       Point pt1 (open[j].x + mouthROI.x, open[j].y + mouthROI.y - 5);
       Point pt2 (open[j].x + mouthROI.x + open[j].width, open[j].y + (mouthROI.y - 5) + open[j].height);
       rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 1, 8, 0);
       /*Point center( faces[i].x + open[j].x + open[j].width*0.5, faces[i].y + open[j].y + open[j].height*0.5 );
       int radius = cvRound( (open[j].width + open[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );*/
	emotion=3;
	cout<<"open"<<endl;
     }
  

   //-- In each face, detect sad
    sad_cascade.detectMultiScale( mouthroi, sad, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t j = 0; j < sad.size(); j++ )
     {
     
                          Point pt1 (sad[j].x + mouthROI.x, sad[j].y + mouthROI.y - 5);
                          Point pt2 (sad[j].x + mouthROI.x + sad[j].width, sad[j].y + (mouthROI.y - 5) + sad[j].height);
                          rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 1, 8, 0);
       /*Point center( faces[i].x + sad[j].x + sad[j].width*0.5, faces[i].y + sad[j].y + sad[j].height*0.5 );
       int radius = cvRound( (sad[j].width + sad[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );*/
	emotion=2;
	cout<<"sad"<<endl;
     }

   //-- In each face, detect happy
    happy_cascade.detectMultiScale( mouthroi, happy, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t j = 0; j < happy.size(); j++ )
     {
       Point pt1 (happy[j].x + mouthROI.x, happy[j].y + mouthROI.y - 5);
                          Point pt2 (happy[j].x + mouthROI.x + happy[j].width, happy[j].y + (mouthROI.y - 5) + happy[j].height);
                          rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 1, 8, 0);
       /*Point center( faces[i].x + happy[j].x + happy[j].width*0.5, faces[i].y + happy[j].y + happy[j].height*0.5 );
       int radius = cvRound( (happy[j].width + happy[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );*/
	emotion=1;
	cout<<"happy"<<endl;
     }
 
if (emotion==1){
c_happy=c_happy+1;
cout<<c_happy<<endl;
}
else{
cout<<"pro midenismo count_h="<<c_happy<<endl;
c_happy=0;
}

if (c_happy==5){
trackid="1";
  if (pid_sad!=0){
     int s=kill(pid_sad,9);} 
  else if (pid_open!=0){
     int s=kill(pid_open,9);}
rc=pthread_create(&thread,NULL,start_posix_spawn,(void*) trackid);


} 


if (emotion==2){
c_sad=c_sad+1;
cout<<c_sad<<endl;
}
else{
cout<<"pro midenismo count_s="<<c_sad<<endl;
c_sad=0;
}

if (c_sad==5){
trackid="2";
if (pid_happy!=0){
     int s=kill(pid_happy,9);}
else if (pid_open!=0){
     int s=kill(pid_open,9);}
rc=pthread_create(&thread,NULL,start_posix_spawn,(void*) trackid);


//playaudio(trackid);
} 


if (emotion==3){
c_open=c_open+1;
cout<<c_open<<endl;
}
else{
cout<<"pro midenismo count_h="<<c_open<<endl;
c_open=0;
}

if (c_open==3){
trackid="3";
  if (pid_sad!=0){
     int s=kill(pid_sad,9);}
  else if (pid_happy!=0){
     int s=kill(pid_happy,9);}  
rc=pthread_create(&thread,NULL,start_posix_spawn,(void*) trackid);


} 
   }

 }

  //-- Show what you got
 imshow( window_name, frame );
 }


void *start_posix_spawn(void* threadid) {
  std::cout<<"Process starts"<<std::endl;
  
 char * exe=(char *)"fmod_main";
 char * orisma=(char *)threadid;
 long tid=(long)threadid;
  char *argv[] = {exe, orisma, (char *) 0};
  int status;
  puts("Testing start_posix_spawn");
  fflush(NULL);
  status = posix_spawn(&pid, "/home/mixa/fmod_main", NULL, NULL, argv, environ);
  
   if (status == 0) {
      if (orisma=="1"){
         pid_happy=pid;}
      else if (orisma=="2"){
         pid_sad=pid;}
      else if (orisma=="3"){
         pid_open=pid;}
    printf("Child id: %i\n", pid);
    fflush(NULL);
    if (waitpid(pid, &status, 0) != -1) {
      printf("Child exited with status %i\n", status);
    } else {
      perror("waitpid");
    }
  } else {
    printf("posix_spawn: %s\n", strerror(status));
  }
}
