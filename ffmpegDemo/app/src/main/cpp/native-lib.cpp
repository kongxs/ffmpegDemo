#include <jni.h>
#include <string>

#include <syslog.h>
//#include "Utils.c"

extern "C"
{
    #include "libavformat/avformat.h"
    #include "libavutil/log.h"
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ffmpegdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    
//    av_dump_format()

    jclass stuClass = env -> FindClass("com/example/ffmpegdemo/Student");

    syslog(LOG_ERR,"jclass success ");

//    av_log()

    //构造函数
    jmethodID stuInit = env -> GetMethodID(stuClass,"<init>" , "()V");

    syslog(LOG_ERR,"init method success ");

    jmethodID getAge = env -> GetMethodID(stuClass,"getAge" , "()I");

    syslog(LOG_ERR,"get age method  success ");
    
    //构造对象

    jobject stuObj = env -> NewObject(stuClass, stuInit);

    syslog(LOG_ERR,"object success ");
    
    //调用 getage方法
    int age = env -> CallIntMethod(stuObj, getAge);


    char result[50] = "";

    sprintf(result," %s : age is %d " , "hello" , age);


    return env->NewStringUTF(result);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_ffmpegdemo_MainActivity_stringFromJNI2(JNIEnv *env, jobject instance) {

    jint  val = 10;


    return val;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_ffmpegdemo_MainActivity_getDuration(JNIEnv *env, jobject instance, jstring path_) {


    const char *path = env->GetStringUTFChars(path_, JNI_FALSE);




    int times ;

    av_register_all();

    avformat_network_init();


    AVFormatContext *avFormatContext = avformat_alloc_context();


    char re[] = "init - ";

    if (avFormatContext != NULL) {

        syslog(LOG_ERR , " 申请到内存context  path = %s " , path) ;

        
//        path = "http://vfx.mtime.cn/Video/2019/03/21/mp4/190321153853126488.mp4";

        if (avformat_open_input(&avFormatContext ,path, NULL, NULL) == 0 ) {
            
            //success
            syslog(LOG_ERR , " 文件打开成功  ") ;

            if (avformat_find_stream_info(avFormatContext,NULL) >= 0) {


                for (int i = 0; i < avFormatContext->nb_streams; i++) {

                    if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {

                        int64_t duration = avFormatContext->streams[i]->duration;

                        

                        syslog(LOG_ERR , "视频长度为：%d " , duration);

                        char result[50] = "";

                        sprintf(result, "视频长度为： %d   " , duration);

                        return env->NewStringUTF(result);
                    }
                }

                return env->NewStringUTF("没找到视频流");
            } else {

                return env->NewStringUTF("获取视频流失败");
            }

        } else {

            return env->NewStringUTF("文件打开失败 。。。 ");
        }

    } else {
        return env->NewStringUTF("上下文失败 。。。 ");
    }

}