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

import java.util.List;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiBucket;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.Kii.Site;
import com.kii.cloud.storage.callback.KiiObjectCallBack;
import com.kii.cloud.storage.callback.KiiQueryCallBack;
import com.kii.cloud.storage.callback.KiiUserCallBack;
import com.kii.cloud.storage.query.KiiClause;
import com.kii.cloud.storage.query.KiiQuery;
import com.kii.cloud.storage.query.KiiQueryResult;

import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

class RankingData{
	String userName;
	int score;
}

public class SimpleGame extends Cocos2dxActivity{
	private final static String TAG ="SimpleGame";
	
	KiiBucket m_appRankingBucket = null;
	String m_username = null;
	KRanking m_kRanking = null;
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);

		Log.v(TAG, "onCreate");

		Global.activity = this;
		
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
	
	/**
	 * login
	 */
	private void login(){
		final String username ="muku";
		String password = "1234";		
		
		Log.v(TAG, "login");
		
        // call KiiCloud API
        KiiUser.logIn( new KiiUserCallBack() {
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	Log.v(TAG, "login onLoginCompleted " + token + " " + user +" " + e);
		    	//save();
		    	m_username = username;
		    	m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
		    	//ranking_post(123477, m_username);	//スコアを保存
		    	//ranking_post(m_appRankingBucket, m_username, 123456);
		    	//ranking_post(m_appRankingBucket, "hoge_name", 123456+2);
		    	//ranking_query_all(m_appRankingBucket);
		    	m_kRanking = new KRanking();
			}
        }, username, password);
	}
	
	public void ranking_query_all(){
		Log.v(TAG, "ranking_query_all");
		if(m_kRanking!=null){
			m_kRanking.ranking_query_all();
		}
		//ranking_query_all(m_appRankingBucket);
	}
	
	public void ranking_post(String name, int score){
		Log.v(TAG, "ranking_post " + name +" "+ score);
		
		if(m_kRanking!=null){
			m_kRanking.ranking_post(name, score);
		}
		//ranking_query(bucket, name, score);
	}
		
	/***
	 * ranking_query_all
	 * @param bucket
	 * @param name
	 */
	private void ranking_query_all(KiiBucket bucket){
		Log.v(TAG, "ranking_query_all");
		KiiQuery query = new KiiQuery();
		query.sortByDesc(Field.SCORE);
        // call KiiCloud API
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				Log.v(TAG, "ranking_query_all onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int size = objLists.size();
				Log.v(TAG, "size " + size);
				JSONArray jArray = new JSONArray();
				for (KiiObject obj : objLists) {
					//Log.v(TAG, "obj " + obj);
					String name2 = obj.getString(Field.NAME);
					String score2 = obj.getString(Field.SCORE);
					int score3 = obj.getInt(Field.SCORE);
					//Log.v(TAG, "ranking_query " + name2 +" "+ score2+ " "+ score3 );
					
					JSONObject nJArray = new JSONObject();
					try {
						nJArray.put(Field.NAME, name2);
					} catch (JSONException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					try {
						nJArray.put(Field.SCORE, score2);
					} catch (JSONException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					jArray.put(nJArray);

			  	}
				Log.v(TAG, "jArray " + jArray);
				String s = jArray.toString();
				CallCPP.rankingResponse(s);
			}
        }, query);	
	}
	
	
	/***
	 * ranking_query
	 * @param bucket
	 * @param name
	 */
	private void ranking_query(KiiBucket bucket, final String name, final int score){
		Log.v(TAG, "ranking_query");
		KiiQuery query = new KiiQuery();
		//KiiQuery query = new KiiQuery( KiiClause.equals(Field.NAME, name) );
		//query.sortByAsc(Field.SCORE);
		query.sortByDesc(Field.SCORE);
		
        // call KiiCloud API
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				Log.v(TAG, "ranking_query onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int myScore = 0;
				Uri uri = null;
				int size = objLists.size();
				Log.v(TAG, "size " + size);
				for (KiiObject obj : objLists) {
					Log.v(TAG, "obj " + obj);
					String name2 = obj.getString(Field.NAME);
					String score2 = obj.getString(Field.SCORE);
					int score3 = obj.getInt(Field.SCORE);
					Log.v(TAG, "ranking_query " + name2 +" "+ score2+ " "+ score3 );
					if(score3 > myScore){
						myScore = score3;
						uri = obj.toUri();	//ハイスコアのuriを保存
						Log.v(TAG, "myScore " + myScore);
					}
			  	}
				Log.v(TAG, "myScore " + myScore);
				Log.v(TAG, "uri " + uri);
				if(score>myScore){
				//if(true){
					Log.v(TAG, "hiscore");					
					if(uri!=null){
						ranking_save(uri, name, score);	//uriを使って更新
					} else {
						Log.v(TAG, "uri null");
						ranking_save(null, name, score);	//uriを使って更新
					}
				} else{
					Log.v(TAG, "not hiscore");					
				}
				CallCPP.rankingResponse("rankingResponse test test test");
			}
        }, query);	
	}
	
	/***
	 * ranking_save
	 * 自分のハイスコアのobjectを更新、uriを使用
	 * uriがnullなら新規作成
	 * @param name
	 * @param score
	 */
	private void ranking_save(Uri uri, String name, int score){
		Log.v(TAG, "ranking_save " + uri +" "+ name +" "+ score);
		
		KiiObject object;
		if(uri==null){
			Log.v(TAG, "新規作成");
			object = m_appRankingBucket.object();	//新規作成はこっちかも
		} else {
			Log.v(TAG, "更新");
			object = KiiObject.createByUri(uri);	//ハイスコアのobjectを更新するuriを使用して特定
		}
        object.set(Field.NAME, name);
        object.set(Field.SCORE, score);

        // call KiiCloud API
        object.save( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "ranking_save onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				ranking_refresh(uri);	//読み出して確認
			}
        });
	}
	
	/**
	 * ranking_refresh
	 * @param uri
	 */
	private void ranking_refresh(Uri uri){
		Log.v(TAG, "refresh_ranking " + uri);
        KiiObject object2 = KiiObject.createByUri(uri);
        
        // call KiiCloud API
        object2.refresh( new KiiObjectCallBack() {
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "refresh_ranking onRefreshCompleted " + token + " " + object +" " + e);
				String name = object.getString(Field.NAME);
				String score = object.getString(Field.SCORE);
				Log.v(TAG, "refresh_ranking " + name +" "+ score );
			}
        });         
	}

	/**
	 * save
	 */
	private void save(){
		Log.v(TAG, "save");
		
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
        object.save( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "save onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				refresh(uri);
			}
        });        
	}
	
	/**
	 * refresh
	 * @param uri
	 */
	private void refresh(Uri uri){
		Log.v(TAG, "refresh " + uri);
        KiiObject object2 = KiiObject.createByUri(uri);
        
        // call KiiCloud API
        object2.refresh( new KiiObjectCallBack() {
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "refresh onRefreshCompleted " + token + " " + object +" " + e);
				String name2 = object.getString(Field.NAME);
				String type2 = object.getString(Field.TYPE);
				String amount2 = object.getString(Field.AMOUNT);
				Log.v(TAG, "refresh " + name2 +" "+ type2 +" "+amount2 );
			}
        });  
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




