
#include <jni.h>
#include <string>
#include <android/log.h>



#include <syslog.h>

//#include "Utils.c"


#define TAG "MediaPlayer"

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR, TAG, FORMAT, ##__VA_ARGS__);

#define Null NULL;


#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096


extern "C"
{
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
    #include "libavformat/avformat.h"
    #include "libavutil/log.h"
    #include "libavutil/avutil.h"
    #include <libavcodec/avcodec.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "ffmpeg/ffmpeg.h"
#include <libavcodec/jni.h>

#include <libavutil/frame.h>
#include <libavutil/mem.h>
}



extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ffmpegdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    LOGE("hello ");
    
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


jstring error(JNIEnv *env);


jstring error(JNIEnv *env) {
    return env->NewStringUTF("-1");
}


#define ERROR_STR_SIZE 1024


extern "C"
JNIEXPORT void JNICALL
Java_com_example_ffmpegdemo_MainActivity_decodeAudio(JNIEnv *env, jobject instance,
                                                     jstring filename_, jstring outfilename_,
                                                     jstring type_) {


    const char *src_filename = env->GetStringUTFChars(filename_, 0);

    const char *dst_filename = env->GetStringUTFChars(outfilename_, 0);


    LOGE("decode audio file : %s , outfile : %s " , src_filename,dst_filename);


    int err_code;
    char errors[1024];


    int audio_stream_index;

    //上下文
    AVFormatContext *fmt_ctx = NULL;
    AVFormatContext *ofmt_ctx = NULL;

    //支持各种各样的输出文件格式，MP4，FLV，3GP等等
    AVOutputFormat *output_fmt = NULL;

    AVStream *in_stream = NULL;
    AVStream *out_stream = NULL;

    AVPacket pkt;

    av_log_set_level(AV_LOG_DEBUG);

    av_register_all();
    avcodec_register_all();

    fmt_ctx = avformat_alloc_context();
    ofmt_ctx = avformat_alloc_context();


    if ((err_code = avformat_open_input(&fmt_ctx, src_filename, NULL, NULL)) < 0) {
        av_strerror(err_code, errors, 1024);
        LOGE("打开输入文件失败: %s, %d(%s)\n",
               src_filename,
               err_code,
               errors);
    }

    if ((err_code = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
        av_strerror(err_code, errors, 1024);
        LOGE( "failed to find stream info: %s, %d(%s)\n",
               src_filename,
               err_code,
               errors);
    }

    av_dump_format(fmt_ctx, 0, src_filename, 0);

    if (fmt_ctx->nb_streams < 2) {
        //流数小于2，说明这个文件音频、视频流这两条都不能保证，输入文件有错误
        LOGE("输入文件错误，流不足2条\n");
        exit(1);
    }

    //拿到文件中音频流
    /**只需要修改这里AVMEDIA_TYPE_VIDEO参数**/

    AVMediaType t = AVMEDIA_TYPE_VIDEO;

    if(type_ == (jstring) "a") {
        AVMediaType t = AVMEDIA_TYPE_AUDIO;
    } else {
        AVMediaType t = AVMEDIA_TYPE_VIDEO;
    }


    audio_stream_index = av_find_best_stream(fmt_ctx, t /*AVMEDIA_TYPE_VIDEO*/, -1, -1, NULL, 0);
    if (audio_stream_index < 0) {
        LOGE(" 获取音频流失败%s,%s\n",
               av_get_media_type_string(AVMEDIA_TYPE_VIDEO),
               src_filename);
    }

    in_stream = fmt_ctx->streams[audio_stream_index];
    //参数信息
    AVCodecParameters *in_codecpar = in_stream->codecpar;


    // 输出上下文
    ofmt_ctx = avformat_alloc_context();

    //根据目标文件名生成最适合的输出容器
    output_fmt = av_guess_format(NULL, dst_filename, NULL);
    if (!output_fmt) {
        LOGE("根据目标生成输出容器失败！\n");
        exit(1);
    }

    ofmt_ctx->oformat = output_fmt;

    //新建输出流
    out_stream = avformat_new_stream(ofmt_ctx, NULL);
    if (!out_stream) {
        LOGE( "创建输出流失败！\n");
        exit(1);
    }

    // 将参数信息拷贝到输出流中，我们只是抽取音频流，并不做音频处理，所以这里只是Copy
    if ((err_code = avcodec_parameters_copy(out_stream->codecpar, in_codecpar)) < 0) {
        av_strerror(err_code, errors, ERROR_STR_SIZE);
        LOGE(
               "拷贝编码参数失败！, %d(%s)\n",
               err_code, errors);
    }

    out_stream->codecpar->codec_tag = 0;

    //初始化AVIOContext,文件操作由它完成
    if ((err_code = avio_open(&ofmt_ctx->pb, dst_filename, AVIO_FLAG_WRITE)) < 0) {
        av_strerror(err_code, errors, 1024);
        LOGE("文件打开失败 %s, %d(%s)\n",
               dst_filename,
               err_code,
               errors);
        exit(1);
    }



    av_dump_format(ofmt_ctx, 0, dst_filename, 1);


    //初始化 AVPacket， 我们从文件中读出的数据会暂存在其中
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;


    // 写头部信息
    if (avformat_write_header(ofmt_ctx, NULL) < 0) {
        LOGE( "写入头部信息失败！");
        exit(1);
    }

    //每读出一帧数据
    while (av_read_frame(fmt_ctx, &pkt) >= 0) {
        if (pkt.stream_index == audio_stream_index) {
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                       AV_ROUND_NEAR_INF);
            pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                       AV_ROUND_NEAR_INF );

            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
            pkt.pos = -1;
            pkt.stream_index = 0;
            //将包写到输出媒体文件
            av_interleaved_write_frame(ofmt_ctx, &pkt);
            //减少引用计数，避免内存泄漏
            av_packet_unref(&pkt);
        }
    }

    //写尾部信息
    av_write_trailer(ofmt_ctx);

    //最后别忘了释放内存
    avformat_close_input(&fmt_ctx);
    avio_close(ofmt_ctx->pb);


}extern "C"
JNIEXPORT jstring JNICALL
Java_com_coder_ffmpeg_jni_FFMpeg_run(JNIEnv *env, jclass type, jint cmdLen, jobjectArray cmd) {


    //set java vm
    JavaVM *jvm = NULL;
    env->GetJavaVM(&jvm);
    av_jni_set_java_vm(jvm, NULL);
//
    char *argCmd[cmdLen] ;
    jstring buf[cmdLen];

    for (int i = 0; i < cmdLen; ++i) {
        buf[i] = static_cast<jstring>(env->GetObjectArrayElement(cmd, i));
        char *string = const_cast<char *>(env->GetStringUTFChars(buf[i], JNI_FALSE));
        argCmd[i] = string;
        LOGE("argCmd=%s",argCmd[i]);
    }

    int retCode = ffmpegmain(cmdLen, argCmd);
    LOGE("ffmpeg-invoke: retCode=%s",av_err2str(retCode));

    return env->NewStringUTF(av_err2str(retCode));

}