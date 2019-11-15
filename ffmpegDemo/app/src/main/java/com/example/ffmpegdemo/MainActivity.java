package com.example.ffmpegdemo;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;
import android.widget.Toast;

import com.coder.ffmpeg.jni.FFMpeg;

import java.io.File;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {



    private File file;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        File externalStorageDirectory = Environment.getExternalStorageDirectory();

        file = new File(externalStorageDirectory,"1111.mp4");


        File outfile = new File(externalStorageDirectory, "1111_cmd.mp4");

        if (outfile.exists()) {
            outfile.delete();
        }


//        StringBuilder builder = new StringBuilder();
//        builder.append("ffmpeg -i ");
//        builder.append(file.getAbsoluteFile());
//        builder.append(" -vn ");
//        builder.append(outfile.getAbsoluteFile());

//
        String command = "ffmpeg -d -ss 00:00:05 -t 00:00:5 -i %s -vcodec copy -acodec copy %s";


        String result = String.format(command, file.getAbsoluteFile(), outfile.getAbsoluteFile());

        Toast.makeText(this , result, Toast.LENGTH_LONG).show();

        String s = FFMpeg.runCmd(result.split(" "));

        Toast.makeText(this , "s = " + s, Toast.LENGTH_LONG).show();


//        decodeAudio(file.getAbsolutePath() , outfile.getAbsolutePath() , "v");

    }

    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        new Thread(new Runnable() {
            @Override
            public void run() {
            }
        }).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native String getDuration(String path);


    //提取音视频

    /**
     *
     * @param filename
     * @param outfilename
     * @param type  v: video . a: audio
     */
    public native void decodeAudio(String filename,String outfilename , String type);

}
