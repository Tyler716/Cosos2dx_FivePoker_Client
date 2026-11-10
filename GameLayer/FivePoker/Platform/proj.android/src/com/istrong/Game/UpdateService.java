package com.istrong.Game;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class UpdateService extends Service{
    private final static int DOWNLOAD_COMPLETE = 0;
    private final static int DOWNLOAD_FAIL = 1;
    private final static int HTTP_CONNECT_TIMEOUT = 10000;   //millisecond
    private final static int HTTP_READ_TIMEOUT = 20000;   //millisecond
    private final static String strConnectPath = "http://192.168.1.4:9999/update/";
    private File updateDir = null;
    private File updateFile = null;
    private Intent updateIntent = null;
    private UpdateBinder mBinder = new UpdateBinder();

    @Override  
    public void onCreate(){
        super.onCreate();
    }

    @Override  
    public void onStart(Intent intent, int startId){
        super.onStart(intent, startId);   
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        //handleCommand(intent);
        Log.i("UpdateService", "onStartCommand");
        if(android.os.Environment.MEDIA_MOUNTED.equals(android.os.Environment.getExternalStorageState())){
            updateDir = new File(Environment.getExternalStorageDirectory(), "download/");
            Log.i("UpdateService", "updateDir:"+updateDir.getPath());
            updateFile = new File(updateDir.getPath(), getResources().getString(R.string.app_name)+".apk");
            Log.i("UpdateService", "updateFile:"+updateFile.getPath());
        }

        new Thread(new updateRunnable()).start();

        // We want this service to continue running until it is explicitly
        // stopped, so return sticky.
        //return START_STICKY;
        return super.onStartCommand(intent, flags, startId);
    }

    @Override  
    public void onDestroy(){
        Log.i("UpdateService", "onDestroy");
        super.onDestroy();
    }

    @Override  
    public boolean onUnbind(Intent intent){
        Log.i("UpdateService", "onUnbind");
        return super.onUnbind(intent);
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO Auto-generated method stub
        Log.i("UpdateService", "onBind");
        return mBinder;
    }
    
    public class UpdateBinder extends Binder{
        UpdateService getService()
        {
            return UpdateService.this;
        }
        
        File getFilePath()
        {
            return updateFile;
        }
    }
    
    private Handler updateHandler = new  Handler(){
        @Override
        public void handleMessage(Message msg) {
            switch(msg.what){
                case DOWNLOAD_COMPLETE:
                    Log.i("UpdateService", "DOWNLOAD_COMPLETE");

                    Intent intent = new Intent("DOWNLOAD_COMPLETE");
                    sendBroadcast(intent);
                    break;
                    
                case DOWNLOAD_FAIL:
                    Log.i("UpdateService", "DOWNLOAD_FAIL");
                    Intent enterGameIntent = new Intent();
                    enterGameIntent.setClass(UpdateService.this, com.istrong.FivePoker.FivePoker.class);

                    stopService(updateIntent);
                    break;
                    
                default:
                    Log.i("UpdateService", "handleMessage error!");
            }
        }
    };
    
    class updateRunnable implements Runnable {
        Message message = updateHandler.obtainMessage();
        public void run() {
            Log.i("UpdateService", "updateRunnable");
            message.what = DOWNLOAD_COMPLETE;
            try{
                if(!updateDir.exists()){
                    updateDir.mkdirs();
                }
                if(!updateFile.exists()){
                    updateFile.createNewFile();
                }

                long downloadSize = downloadUpdateFile(strConnectPath+getResources().getString(R.string.app_name)+".apk",updateFile);
                if(downloadSize>0){
                    updateHandler.sendMessage(message);
                }
            }catch(Exception ex){
                ex.printStackTrace();
                message.what = DOWNLOAD_FAIL;
                updateHandler.sendMessage(message);
            }
        }
    }
    
    public long downloadUpdateFile(String downloadUrl, File saveFile) throws Exception {
        int downloadCount = 0;
        //int currentSize = 0;
        long totalSize = 0;
        int updateTotalSize = 0;
         
        HttpURLConnection httpConnection = null;
        InputStream is = null;
        FileOutputStream fos = null;
        
        Log.i("UpdateService", "downloadUpdateFile");
        
        try {
            URL url = new URL(downloadUrl);
            httpConnection = (HttpURLConnection)url.openConnection();
            //httpConnection.setRequestProperty("User-Agent", "PacificHttpClient");
            
            //Â_ÂIÄò¶Ç
            /*
            if(currentSize > 0) {
                httpConnection.setRequestProperty("RANGE", "bytes=" + currentSize + "-");
            }*/
            httpConnection.setConnectTimeout(HTTP_CONNECT_TIMEOUT);
            httpConnection.setReadTimeout(HTTP_READ_TIMEOUT);
            updateTotalSize = httpConnection.getContentLength();
            if (httpConnection.getResponseCode() == 404) {
                throw new Exception("fail!");
            }
            is = httpConnection.getInputStream();
            fos = new FileOutputStream(saveFile, false);
            byte buffer[] = new byte[4096];
            int readsize = 0;
            while((readsize = is.read(buffer)) > 0){
                fos.write(buffer, 0, readsize);
                totalSize += readsize;
            }
        } finally {
            if(httpConnection != null) {
                httpConnection.disconnect();
            }
            if(is != null) {
                is.close();
            }
            if(fos != null) {
                fos.close();
            }
        }
        return totalSize;
    }
}