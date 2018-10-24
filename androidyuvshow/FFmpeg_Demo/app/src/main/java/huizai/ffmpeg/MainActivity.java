package huizai.ffmpeg;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.Button;



import static huizai.ffmpeg.BtnAction.ACCELE;
import static huizai.ffmpeg.BtnAction.ATIMESLOW;
import static huizai.ffmpeg.BtnAction.PLAY;
import static huizai.ffmpeg.BtnAction.SLOW;
import static huizai.ffmpeg.BtnAction.STOP;
import static huizai.ffmpeg.BtnAction.VTIMESLOW;

enum BtnAction{
        PLAY, STOP, ACCELE, SLOW, VTIMESLOW, ATIMESLOW;
};

public class MainActivity extends Activity {
    //真是他妹的烦android这种布局，明明可以循环解决的问题非的要一个一个的做
    AVPlayer          avPlayer;
    String            input;
    GlView            glView;
    MainViewAdapter   adapter;
    Button            btnPlay;
    Button            btnStop;
    Button            btnAccele;
    Button            btnSlow;
    Button            btnVTimeSlow;
    Button            btnATimeSlow;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        glView = findViewById(R.id.glView);
        btnPlay=findViewById(R.id.btn_play);
        btnStop=findViewById(R.id.btn_stop);
        btnSlow=findViewById(R.id.btn_slow);
        btnAccele=findViewById(R.id.btn_accelerate);
        btnATimeSlow=findViewById(R.id.btn_acce_video);
        btnVTimeSlow=findViewById(R.id.btn_acce_audio);
        AddBtnAction();
        avPlayer = new AVPlayer();
        glView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }
    @Override
    protected void onResume() {
        super.onResume();
        int REQUEST_EXTERNAL_STORAGE = 1;
        String[] PERMISSIONS_STORAGE = {
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
        };
        int permission = ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(
                    MainActivity.this,
                    PERMISSIONS_STORAGE,
                    REQUEST_EXTERNAL_STORAGE
            );
        }
    }

    void BtnAction(BtnAction action){
        switch (action){
            case PLAY:
                {
                    //String liveInput = "http://221.228.226.23/11/t/j/v/b/tjvbwspwhqdmgouolposcsfafpedmb/sh.yinyuetai.com/691201536EE4912BF7E4F1E2C67B8119.mp4";
                   // String liveInput = "http://flv2.bn.netease.com/videolib3/1604/28/fVobI0704/SD/fVobI0704-mobile.mp4";
                   // String liveInput = "http://221.228.226.5/15/t/s/h/v/tshvhsxwkbjlipfohhamjkraxuknsc/sh.yinyuetai.com/88DC015DB03C829C2126EEBBB5A887CB.mp4";
                   // String liveInput = new File(Environment.getExternalStorageDirectory(),"video.mp4").getAbsolutePath();
                    String liveInput = "/data/peplocass.mp4";
                    //  Toast.makeText(this,input,Toast.LENGTH_SHORT).show();
                    int ret =  avPlayer.OpenAVWithUrlAndView(liveInput,null,glView);
                }
                break;
            case STOP:
                {

                }
                break;
            case ACCELE:
                {
                    avPlayer.setPlayRate(1);
                }
                break;
            case SLOW:
                {
                    avPlayer.setPlayRate(-1);
                }
                break;
            case ATIMESLOW:
                {//单位毫秒
                    avPlayer.setPlayAudioOrVideoRate(100);
                }
                break;
            case VTIMESLOW:
                {
                    avPlayer.setPlayAudioOrVideoRate(-100);
                }
                break;
                default:
                break;
        }
    }

    void AddBtnAction(){
        btnPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BtnAction(PLAY);
            }
        });
        btnStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BtnAction(STOP);
            }
        });
        btnSlow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BtnAction(SLOW);
            }
        });
        btnAccele.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BtnAction(ACCELE);
            }
        });
        btnVTimeSlow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BtnAction(VTIMESLOW);
            }
        });
        btnATimeSlow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                BtnAction(ATIMESLOW);
            }
        });
    }
}
