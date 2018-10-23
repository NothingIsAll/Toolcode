package huizai.ffmpeg;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.support.v7.widget.RecyclerView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;

import huizai.ffmpeg.Base.ABRecyclerViewHolder;
import huizai.ffmpeg.Base.RecyclerBaseAdapter;


/**
 * Created by huizai on 2017/12/7.
 */

public class MainViewAdapter extends RecyclerBaseAdapter{

    List      nameList;
    Activity  mainActivity;
    GlView    glView;

    public MainViewAdapter(Activity activity,List list){
        super(activity,list);
        mainActivity = activity;
        nameList = list;
    }

    public GlView getGlView() {
        return glView;
    }

    @Override
    public boolean hasHeader() {
        return true;
    }

    @Override
    public boolean hasFooter() {
        return false;
    }

    @Override
    public ABRecyclerViewHolder onCreateHolder(ViewGroup parent, int viewType)
    {
        if(viewType == Item.TYPE_HEADER){
           return new VideoViewHolder(LayoutInflater.from(getActivity()).inflate(R.layout.video_view,parent,false));
        }else {
           return new ContentHolder(LayoutInflater.from(getActivity()).inflate(R.layout.cell_btn,parent,false));
        }
    }


    class ContentHolder extends ABRecyclerViewHolder {
        TextView cellScore;
        Button btnLeft;
        Button btnRight;
        ImageView cellIcon;
        Button stestBtn;
        public ContentHolder(View view){
            super(view);
            btnLeft = view.findViewById(R.id.cell_btn_left);
            btnRight = view.findViewById(R.id.cell_btn_right);
        }
        @Override
        public void onBindViewHolder(int position) {
            final int index = position;
            btnLeft.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (view.isSelected()){
                        return;
                    }
                    if (mainActivity != null){
                        MainActivity activity = (MainActivity)mainActivity;
                       // activity.adapterBtnClickAction("",view,index);
                    }
                }
            });
            btnRight.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (view.isSelected()){
                        return;
                    }
                    if (mainActivity != null){
                        MainActivity activity = (MainActivity)mainActivity;
                       // activity.adapterBtnClickAction("",view,index);
                    }
                }
            });
        }
    }

    class VideoViewHolder extends ABRecyclerViewHolder {
        public VideoViewHolder(View view){
            super(view);
           // glView = view.findViewById(R.id.glView);
            // ButterKnife.inject(this,view);
        }

        @Override
        public void onBindViewHolder(int position) {
            final int index = position;

        }
    }
}
