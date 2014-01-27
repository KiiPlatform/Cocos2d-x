package org.cocos2dx.simplegame;

import java.util.List;
import java.util.UUID;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.net.Uri;
import android.util.Log;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiBucket;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.callback.KiiObjectCallBack;
import com.kii.cloud.storage.callback.KiiQueryCallBack;
import com.kii.cloud.storage.callback.KiiUserCallBack;
import com.kii.cloud.storage.query.KiiClause;
import com.kii.cloud.storage.query.KiiQuery;
import com.kii.cloud.storage.query.KiiQueryResult;

public class KRanking {
	private final static String TAG ="KRanking";
	KiiBucket m_appRankingBucket;
	String m_username, m_displayName;
	private SimpleGame m_simpleGame;
	
	public KRanking(){
		m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
	}
	
	public KRanking(SimpleGame a) {
		m_simpleGame = a;
		m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
	}

	/***
	 * ranking_query_all
	 * ランキンデータを取得する
	 */
	public void ranking_query_all(){
		Log.v(TAG, "ranking_query_all");
		ranking_query_all(m_appRankingBucket);
		//ranking_post("muku1111",1234+1);
	}
	
	/***
	 * スコアをポストする、ハイスコアなら更新される
	 * @param name
	 * @param score
	 */
	public void ranking_post(String name, int score){
		Log.v(TAG, "ranking_post " + name +" "+ score);
		ranking_query(m_appRankingBucket, name, score);
	}
	
	/***
	 * ranking_query_all
	 * ランキンデータを取得してc++へ渡す、c++はランキングを表示する
	 * @param bucket
	 */
	private void ranking_query_all(KiiBucket bucket){
		Log.v(TAG, "ranking_query_all " + bucket);
		KiiQuery query = new KiiQuery();
		query.sortByDesc(Field.SCORE);
        // call KiiCloud API コールバックでレスポンスをもらう
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				Log.v(TAG, "ranking_query_all onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int size = objLists.size();
				Log.v(TAG, "size " + size);
				JSONArray jArray = new JSONArray();
				int i = 0;
				for (KiiObject obj : objLists) {
					i++;
					//Log.v(TAG, "obj "+ i +" " + obj);
					String name2 = obj.getString(Field.NAME, "null_name");
					String score2 = obj.getString(Field.SCORE,"0");
					int score3 = obj.getInt(Field.SCORE, 0);

					Log.v(TAG, "ranking_query " + name2 +" "+ score2+ " "+ score3 );
					
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
				CallCPP.rankingResponse(s);	//C++を呼び出す
			}
        }, query);	
	}
	
	/***
	 * ranking_query
	 * プレイ結果をポストするためにまず、ランキングを取得してハイスコアならsaveする
	 * @param bucket
	 * @param name
	 * @param score
	 */
	private void ranking_query(KiiBucket bucket, final String name, final int score){
		Log.v(TAG, "ranking_query");
		KiiQuery query = new KiiQuery( KiiClause.equals(Field.NAME, name) );
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
				if(score>myScore){	//sizeが0の場合myScoreが0のままなのでscoreが0より大きいと真になる
				//if(true){
					Log.v(TAG, "hiscore");					
					if(uri!=null){
						ranking_save(uri, name, score);	//uriを使って更新 save
					} else {
						Log.v(TAG, "uri null");
						ranking_save(null, name, score);//uriはnullなので新規作成 save
					}
				} else{
					Log.v(TAG, "not hiscore");					
				}
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
	public void save(){
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
	
	public void login(String token){
		Log.v(TAG, "login "+ token);
		KiiUser.loginWithToken( new KiiUserCallBack() {
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	Log.v(TAG, "loginWithToken onLoginCompleted " + token + " " + user +" " + e);
		    	m_displayName = user.getDisplayname();
		    	Log.v(TAG, "m_displayName " + m_displayName );
		    	if(m_displayName==null){
		    		m_displayName = "PlayerName";
		    	}
		    	CallCPP.setDisplayame(m_displayName);	//C++を呼び出す
			}
		}, token);
	}
	
	/**
	 * login
	 */
	public void login(String userName, String passWord){
		Log.v(TAG, "login");
		
		final String username = userName;
		String password = passWord;		
		
        // call KiiCloud API
        KiiUser.logIn( new KiiUserCallBack() {
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	Log.v(TAG, "login onLoginCompleted " + token + " " + user +" " + e);
		    	//save();
		    	m_username = username;
		    	m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
			}
        }, username, password);
	}
	
	public void regist(String userName, String passWord){
		Log.v(TAG, "regist " + userName +" "+ passWord);

		String username = userName;
		String password = passWord;
        KiiUser user = KiiUser.createWithUsername(username);
        user.register( new KiiUserCallBack() {
        	@Override
        	public void onRegisterCompleted(int token, KiiUser user, Exception e){
        		Log.v(TAG, "onRegisterCompleted " + token + " " + user +" " + e);
        		
                // store access token
                KiiUser user2 = KiiUser.getCurrentUser();
                String token2 = user2.getAccessToken();
                Log.v(TAG, "user2 " + user2);
                Log.v(TAG, "token2 " + token2);
                Pref.setStoredAccessToken(m_simpleGame.getApplicationContext(), token2);
        	}
        }, password);
	}
	
	/**
	 * regist
	 */
	public void regist(){
		Log.v(TAG, "regist");

		String username;
		String password;
		String uuid = UUID.randomUUID().toString();
		//Pref.setUUID( m_simpleGame.getApplicationContext(), uuid);
		username = uuid;
		password = "1234";
		boolean flag;
		flag = KiiUser.isValidUserName(username);
		if(!flag){
			Log.v(TAG, "isValidUserName error");
			return;
		}

		flag = KiiUser.isValidPassword(password);
		if(!flag){
			Log.v(TAG, "isValidPassword error");
			return;
		}
		
		Pref.setUUID( m_simpleGame.getApplicationContext(), uuid);
		regist(username, password);

		String uuid2 = Pref.getUUID( m_simpleGame.getApplicationContext() );
		Log.v(TAG, "uuid2 " + uuid2);
	}
	
}
