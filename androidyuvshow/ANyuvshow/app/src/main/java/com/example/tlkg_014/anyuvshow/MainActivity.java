package com.example.tlkg_014.anyuvshow;

import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    Button bt1;
    // Used to load the 'native-lib' library on application startup.
    yuvglsurfaceView glview;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(new yuvglsurfaceView(this));
        setContentView(R.layout.activity_main);
        ViewGroup mains = findViewById(R.id.activity_s);

        glview = new yuvglsurfaceView(this);;//findViewById(R.id.glView);//new yuvglsurfaceView(this,set);
        glview.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        //glview.setBackgroundColor(Color.rgb(255,0,255));
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(720,1280);
        mains.addView(glview,lp);
        bt1 = (Button)findViewById(R.id.yuvshow);
        bt1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               new myThread(glview).start();
            }
        });
        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);

        tv.setText(yuvNatives.getInstance().stringFromJNI());
        //new myThread(glview).start();
    }


}


class myThread extends Thread{
    private String path;
    private yuvglsurfaceView view;
    myThread(String path){
        this.path = path;
    }

    public myThread(yuvglsurfaceView view) {
        this.view = view;
    }

    @Override
    public void run() {
        super.run();
        //SystemClock.sleep(5000);
        String path = "/data/camera.yuv";
        int width = 1280;
        int height = 720;
        int readsize = 1280*720*3/2;
        int readres = 1;
        int putres = -1;
        try {
            FileInputStream inputStream = new FileInputStream(path);
            byte[] buffer = new byte[readsize];
            while(readres > 0){
                readres = inputStream.read(buffer,0,readsize);
               // do {
                    putres = yuvNatives.getInstance().PutYuvData(buffer, readsize, width, height);
                view.requestRender();
                    //SystemClock.sleep(5);
                //}while (putres < 0);
                SystemClock.sleep(20);
            }
            inputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
