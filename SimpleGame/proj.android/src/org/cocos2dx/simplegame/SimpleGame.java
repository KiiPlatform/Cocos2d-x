/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.simplegame;

import java.io.IOException;
import java.util.List;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiBucket;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.Kii.Site;
import com.kii.cloud.storage.exception.app.AppException;
import com.kii.cloud.storage.query.KiiClause;
import com.kii.cloud.storage.query.KiiQuery;
import com.kii.cloud.storage.query.KiiQueryResult;

import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

public class SimpleGame extends Cocos2dxActivity{
	private final static String TAG ="SimpleGame";
	
	KiiBucket m_appRankingBucket;
	String m_username;
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);

		Log.v(TAG, "onCreate");

/***
		CallCPP.nativeEnd();
		String s = CallCPP.concat("hoge", "fuga");
		Log.v(TAG, "s = " +s);
***/		

	    //initialize
        initKiiSDK();
        login();
/***        
        // check access token
        String token = Pref.getStoredAccessToken(getApplicationContext());
        if (token == null || token.length() == 0) {
    		Log.v(TAG, "onCreate regist");
            //toTitleFragment();
        	//regist
        	//regist();
        	login();

            return;
        } 
***/        
        // login with token
        //AutoLoginCallback callback = new AutoLoginCallback(this);
        //KiiUser.loginWithToken(callback, token);
/***        
        MyUserCallback callback = new MyUserCallback();
        callback.setOnClickListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	Log.v(TAG, "onLoginCompleted " + token + " " + user +" " + e);
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
			}
		});
        
        KiiUser.loginWithToken(callback, token);
***/        
		Log.v(TAG, "onCreate end");
	}
	

	private void ranking_query(KiiBucket bucket, String name){
		Log.v(TAG, "ranking_query");
		
		// Prepare the target bucket to be queried
		//KiiBucket bucket = Kii.bucket("myBucket");

		// Define query conditions 
		KiiQuery query = new KiiQuery( KiiClause.equals(Field.NAME, name) );

        // call KiiCloud API
		MyQueryCallBack callback = new MyQueryCallBack();
        callback.setListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				Log.v(TAG, "login onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int myScore = 0;
				for (KiiObject obj : objLists) {
					Log.v(TAG, "obj " + obj);
					String name2 = obj.getString(Field.NAME);
					String score2 = obj.getString(Field.SCORE);
					int score3 = obj.getInt(Field.SCORE);
					Log.v(TAG, "ranking_query " + name2 +" "+ score2+ " "+ score3 );
					if(score3>myScore){
						myScore = score3;
						Log.v(TAG, "myScore " + myScore);
					}
			  	}
				Log.v(TAG, "myScore " + myScore);
			}
		});   
        bucket.query(callback,query);
/***
		// Query the bucket
		try {
			KiiQueryResult<KiiObject> result = bucket.query(query);
		  
			List<KiiObject> objLists = result.getResult();
			int myScore = 0;
			for (KiiObject obj : objLists) {
				Log.v(TAG, "obj " + obj);
				String name2 = obj.getString(Field.NAME);
				String score2 = obj.getString(Field.SCORE);
				int score3 = obj.getInt(Field.SCORE);
				Log.v(TAG, "ranking_query " + name2 +" "+ score2+ " "+ score3 );
				if(score3>myScore){
					myScore = score3;
					Log.v(TAG, "myScore " + myScore);
				}
		  	}
			Log.v(TAG, "myScore " + myScore);
		} catch (IOException e) {
		  // handle error
		} catch (AppException e) {
		  // handle error
		}
***/		
	}

	/**
	 * login
	 */
	private void login(){
		final String username ="muku";
		String password = "1234";		
		
		Log.v(TAG, "login");
		
        // call user registration API
        MyUserCallback callback = new MyUserCallback();
        callback.setListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	Log.v(TAG, "login onLoginCompleted " + token + " " + user +" " + e);
		    	//save();

		    	m_username = username;
		    	m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
		    	//ranking_post(123499, m_username);	//スコアを保存
		    	
		    	ranking_query(m_appRankingBucket,m_username);
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
			}
		});
        
        KiiUser.logIn(callback, username, password);
	}
	
	/**
	 * ranking_post
	 */
	private void ranking_post(int score, String name){
		Log.v(TAG, "ranking_post");
		
        KiiObject object = m_appRankingBucket.object();

        object.set(Field.NAME, name);
        object.set(Field.SCORE, score);

        // call KiiCloud API
        MyObjectCallback callback = new MyObjectCallback();
        callback.setListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	//Log.v(TAG, "login onLoginCompleted " + token + " " + user +" " + e);
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "ranking_post onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				refresh_ranking(uri);	//読み出して確認
			}
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
			}
		});       
        object.save(callback);	//saveここでクラウドへ保存される、callbackなしのメソッドもある？		
	}

	/**
	 * save
	 */
	private void save(){
		String name = "name1";
		String type = "type1";
		String amount = "1234";
		
        // Create an Object instance
        KiiUser user = KiiUser.getCurrentUser();
        KiiBucket bucket = user.bucket(Constants.BUCKET_NAME);
        KiiObject object = bucket.object();
        
        object.set(Field.NAME, name);
        object.set(Field.TYPE, type);
        object.set(Field.AMOUNT, amount);
     
        // call KiiCloud API
        MyObjectCallback callback = new MyObjectCallback();
        callback.setListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	//Log.v(TAG, "login onLoginCompleted " + token + " " + user +" " + e);
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "save onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				refresh(uri);
			}
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
			}
		});       
        object.save(callback);	//saveここでクラウドへ保存される、callbackなしのメソッドもある？
	}
	
	/**
	 * refresh
	 * @param uri
	 */
	private void refresh_ranking(Uri uri){
		Log.v(TAG, "refresh_ranking " + uri);
        KiiObject object2 = KiiObject.createByUri(uri);
        
        // call KiiCloud API
        MyObjectCallback callback = new MyObjectCallback();
        callback.setListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "refresh_ranking onRefreshCompleted " + token + " " + object +" " + e);
				String name = object.getString(Field.NAME);
				String score = object.getString(Field.SCORE);
				Log.v(TAG, "refresh_ranking " + name +" "+ score );
			}
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
			}
		});       
        
		object2.refresh(callback);
	}
	
	/**
	 * refresh
	 * @param uri
	 */
	private void refresh(Uri uri){
        KiiObject object2 = KiiObject.createByUri(uri);
        
        // call KiiCloud API
        MyObjectCallback callback = new MyObjectCallback();
        callback.setListener(new KiiListenerInterface(){
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
			}
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
			}
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "refresh onRefreshCompleted " + token + " " + object +" " + e);
				String name2 = object.getString(Field.NAME);
				String type2 = object.getString(Field.TYPE);
				String amount2 = object.getString(Field.AMOUNT);
				Log.v(TAG, "refresh " + name2 +" "+ type2 +" "+amount2 );
			}
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
			}
		});       
        
		object2.refresh(callback);
	}
	
	/**
	 * regist
	 */
	private void regist(){
		String username = "muku";
		String password = "1234";
		
		Log.v(TAG, "regist");
		
        // call user registration API
        RegisterCallback callback = new RegisterCallback();	//callbackの作成 RegisterCallbackはKiiUserCallBackを継承したクラス
        KiiUser user = KiiUser.createWithUsername(username);
        user.register(callback, password);	//KiiUserCallBackを渡す callbackはKiiUserCallBackを継承したクラスのインスタンス（実体）
	}
	
	 /**
     * Initialize KiiSDK
     * Please change APP_ID/APP_KEY to your application
     */
    private void initKiiSDK() {
		Log.v(TAG, "initKiiSDK");
		
        Kii.initialize(
                Constants.APP_ID,  // Put your App ID
                Constants.APP_KEY, // Put your App Key
                Site.US            // Put your site as you've specified upon creating the app on the dev portal
                );
    }
    
	
    //public static native void nativeEnd();
    //public static native String concat(String str1, String str2);
    
    static {
        System.loadLibrary("game");
    }
}




