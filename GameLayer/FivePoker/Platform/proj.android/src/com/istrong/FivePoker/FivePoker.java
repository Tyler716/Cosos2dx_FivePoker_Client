package com.istrong.FivePoker;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.lib.Cocos2dxRenderer;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.util.Log;
import android.widget.FrameLayout;
import android.view.ViewGroup;
import android.os.Build;

public class FivePoker extends Cocos2dxActivity{
	private Cocos2dxGLSurfaceView mGLView;
	private static String versionName;
	
    @Override
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		EnterGame();
	}
	
	@Override
	protected void onPause() {
	    super.onPause();
	    if(mGLView != null)
	    {
	        mGLView.onPause();
	    }
	}

	@Override
	protected void onResume() {
	    super.onResume();
	    if(mGLView != null)
	    {
	        mGLView.onResume();
	    }
	}
	
	private boolean detectOpenGLES20() 
	{
	    ActivityManager am =
	           (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
	    ConfigurationInfo info = am.getDeviceConfigurationInfo();
	    return (info.reqGlEsVersion >= 0x20000 || Build.FINGERPRINT.startsWith("generic"));
	}

	public static String getGameVersion()
	{
	    return "v"+versionName;
	}

	private void EnterGame()
	{
	    if (detectOpenGLES20()) {
	            // get the packageName,it's used to set the resource path
	           String packageName = getApplication().getPackageName();
	           Log.d("JNIMsg", packageName);
	           super.setPackageName(packageName);

	           // FrameLayout
	           ViewGroup.LayoutParams framelayout_params =
	               new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
	                                          ViewGroup.LayoutParams.FILL_PARENT);

	           FrameLayout framelayout = new FrameLayout(this);
	           framelayout.setLayoutParams(framelayout_params);

	           // Cocos2dxEditText layout
	           ViewGroup.LayoutParams edittext_layout_params =
	               new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
	                                          ViewGroup.LayoutParams.WRAP_CONTENT);

	           Cocos2dxEditText edittext = new Cocos2dxEditText(this);
	           edittext.setLayoutParams(edittext_layout_params);

	           // ...add to FrameLayout
	           framelayout.addView(edittext);

	           // Cocos2dxGLSurfaceView
	           mGLView = new Cocos2dxGLSurfaceView(this);

	           // ...add to FrameLayout
	           framelayout.addView(mGLView);

	           mGLView.setEGLContextClientVersion(2);
	           mGLView.setCocos2dxRenderer(new Cocos2dxRenderer());
	           mGLView.setTextField(edittext);

	           // Set framelayout as the content view
	           setContentView(framelayout);
	           
	           try
	           {
	               versionName = getPackageManager().getPackageInfo(packageName, 0).versionName;
	           }
	           catch (NameNotFoundException e) 
	           {
	               // TODO Auto-generated catch block
	               e.printStackTrace();
	           }
	       }
	       else {
	           Log.d("activity", "don't support gles2.0");
	           finish();
	       }   
	}
	
    static {
        System.loadLibrary("game");
    }
}