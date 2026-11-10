package com.istrong.Game;

import com.istrong.FivePoker.FivePoker;
import com.istrong.Game.R;
import org.apache.http.util.ByteArrayBuffer;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.URL;

public class UpdateActivity extends Activity{
    /** Called when the activity is first created. */
    private Handler mHandler;
    private UpdateService mUpdateBinder;
    private File mUpdateFile = null;
    ProgressDialog mBar =null;
    private Activity otherActivity = this;
    private boolean bCheckVersion = false;
    
    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        
        boolean b = false;
        if (savedInstanceState != null) {
            b = savedInstanceState.getBoolean("KEY_START_FROM_OTHER_ACTIVITY", false);
            if (b){
                Log.i("UpdateActivity", "savedInstanceState");
            }
        }
        if (!b)
        {
            mHandler = new Handler();
            registerReceiver(onDownloadCompleteReceiver, new IntentFilter("DOWNLOAD_COMPLETE")); 
    
            /* Get Last Update Time from Preferences */
            //SharedPreferences prefs = getPreferences(0);
            //long lastUpdateTime =  prefs.getLong("lastUpdateTime", System.currentTimeMillis());
    
            int curVersion = 0;
            try {
                    curVersion = getPackageManager().getPackageInfo(getApplication().getPackageName(), 0).versionCode;
            } catch (NameNotFoundException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            Log.i("UpdateActivity",String.valueOf(curVersion));
            /* Should Activity Check for Updates Now? */
    
            /*if ((lastUpdateTime + (24 * 60 * 60 * 1000)) < System.currentTimeMillis()) {
    
                // Save current timestamp for next Check
                lastUpdateTime = System.currentTimeMillis();
                SharedPreferences.Editor editor = getPreferences(0).edit();
                editor.putLong("lastUpdateTime", lastUpdateTime);
                editor.commit();
                */
    
                mHandler.post(checkNetwork);
            //}
        }
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
    }
    
    @Override
    public void onStop() {
        super.onStop();

        unregisterReceiver(onDownloadCompleteReceiver);
    }
    
    public void setActivity(Activity paramActivity) {  
        Log.d("UpdateActivity", "setActivity..." + paramActivity);  
        this.otherActivity = paramActivity;  
    } 
    
    /* This Thread checks for Updates in the Background */
    private Thread checkUpdate = new Thread() 
    {
        public void run() {
            try {
                URL updateURL = new URL("http://192.168.1.4:9999/update/Version.txt");
                HttpURLConnection conn = (HttpURLConnection)updateURL.openConnection();
                conn.setConnectTimeout(5000);
                conn.setReadTimeout(10000);

                Log.i("UpdateActivity", "connect()");
                conn.connect();
                
                int status = conn.getResponseCode();
                Log.i("UpdateActivity", "getResponseCode:"+String.valueOf(status));
                
                if(status == HttpURLConnection.HTTP_OK)
                {
                    InputStream is = conn.getInputStream();
                    BufferedInputStream bis = new BufferedInputStream(is);
                    ByteArrayBuffer baf = new ByteArrayBuffer(50);
    
                    int current = 0;
                    while((current = bis.read()) != -1){
                         baf.append((byte)current);
                    }
    
                    // Convert the Bytes read to a String.
                    final String s = new String(baf.toByteArray());
    
                    // Get current Version Number
                    int curVersion = getPackageManager().getPackageInfo(getApplication().getPackageName(), 0).versionCode;
    
                    int newVersion = Integer.valueOf(s);
                    Log.i("UpdateActivity",String.valueOf(newVersion));
    
                    // Is a higher version than the current already out? 
                    if (newVersion > curVersion) {
                        // Post a Handler for the UI to pick up and open the Dialog
                        mHandler.post(showUpdate);
                    }
                    else
                    {
                        mHandler.post(stratGame);
                    }
                }
                else
                {
                    mHandler.post(stratGame);
                }
            } catch (Exception e) {
            }
        }
    };
    
    private Runnable checkNetwork = new Runnable(){
        public void run(){
            if(!isNetworkAvailable(otherActivity)){
                Log.i("UpdateActivity", "checkNetwork: fail");
                new AlertDialog.Builder(otherActivity)
                .setIcon(R.drawable.icon)
                .setTitle("Warning!")
                .setMessage("Network is not available, please check it!")
                .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        /* User clicked OK so do some stuff */
                        mHandler.post(checkNetwork);
                    }
                }).show();
            }
            else
            {
                if(bCheckVersion)
                {
                    Log.i("UpdateActivity", "checkNetwork: OK");
                    
                    mBar = new ProgressDialog(otherActivity);
                    mBar.setTitle("Check version");
                    mBar.setMessage("Please wait...");
                    mBar.setProgressStyle(ProgressDialog.STYLE_SPINNER);
                    mBar.show();
                    /* Start Update */
                    checkUpdate.start();
                }
                else
                {
                    Intent intent = new Intent();
                    intent.setClass(otherActivity, com.istrong.FivePoker.FivePoker.class);
                    startActivity(intent);
                    finish();
                }
            }
        }
    };

    /* This Runnable creates a Dialog and asks the user to open the Market */
    private Runnable showUpdate = new Runnable(){
           public void run(){
            mBar.cancel();
            new AlertDialog.Builder(otherActivity)
            .setIcon(R.drawable.icon)
            .setTitle("Update Available")
            .setMessage ("An update is available!\n\nDo you want to update?")
            .setPositiveButton("Update", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                            /* User clicked OK so do some stuff */
                            Log.i("UpdateActivity", "startService: UpdateService");
                            Intent updateIntent =new Intent(otherActivity, UpdateService.class);
                            //updateIntent.putExtra("titleId",R.string.app_name);
                            startService(updateIntent);
                    }
            })
            .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                            /* User clicked Cancel */
                        Intent intent = new Intent();
                        intent.setClass(otherActivity, com.istrong.FivePoker.FivePoker.class);
                        startActivity(intent);
                        finish();
                    }
            })
            .show();
           }
    };
    
    private Runnable stratGame = new Runnable(){
        public void run(){
            mBar.cancel();
            Intent intent = new Intent();
            intent.setClass(otherActivity, FivePoker.class);
            startActivity(intent);
            finish();
        }
    };

    public static boolean isNetworkAvailable(Context context) {
        try {
                ConnectivityManager cm = (ConnectivityManager) context
                                .getSystemService(Context.CONNECTIVITY_SERVICE);

                NetworkInfo info = cm.getActiveNetworkInfo();

                boolean bResult = false;
                if(info == null || !info.isConnected())
                {
                    bResult = false;
                }
                else
                {
                    if(!info.isAvailable())
                    {
                        bResult = false;
                    }
                    else
                    {
                        bResult = true;
                    }
                }
                    
                return bResult;
        } catch (Exception e) {
                e.printStackTrace();
                return false;
        }
    }
    
    private ServiceConnection mServiceConnection = new ServiceConnection() {    
        public void onServiceConnected(ComponentName name, IBinder service) {  
            // TODO Auto-generated method stub  
            mUpdateFile = ((UpdateService.UpdateBinder)service).getFilePath();
            
            Log.i("UpdateActivity", "onServiceConnected path:"+mUpdateFile.getPath());
            
            if(mUpdateFile != null)
            {
                Intent updateIntent =new Intent(otherActivity, UpdateService.class);
                unbindService(mServiceConnection);
                stopService(updateIntent);
                
                Uri uri = Uri.fromFile(mUpdateFile);
                Intent installIntent = new Intent(Intent.ACTION_VIEW);
                installIntent.setDataAndType(uri, "application/vnd.android.package-archive");
                startActivity(installIntent);
                finish();
            }
        }  
          
        public void onServiceDisconnected(ComponentName name) {  
            // TODO Auto-generated method stub  
              
        }  
    };
    
    private BroadcastReceiver onDownloadCompleteReceiver = new BroadcastReceiver() {  
        
        public void onReceive(Context context, Intent intent) {
            Log.i("UpdateActivity", "onDownloadCompleteReceiver");

            Intent updateIntent =new Intent(otherActivity, UpdateService.class);
            bindService(updateIntent, mServiceConnection, BIND_AUTO_CREATE);
        }  
    };
}