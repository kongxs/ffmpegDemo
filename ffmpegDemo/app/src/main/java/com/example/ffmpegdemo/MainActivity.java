package com.example.ffmpegdemo;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");

        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);


        File externalStorageDirectory = Environment.getExternalStorageDirectory();

        File file = new File(externalStorageDirectory,"1111.mp4");
//        File file = new File(externalStorageDirectory,"wwise_cfg.txt");

        if (file.exists()) {
            tv.setText(getDuration(file.getAbsolutePath()));
        } else {
            tv.setText("file not exits ... ");
        }

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native int stringFromJNI2();


    public native String getDuration(String path);


}
