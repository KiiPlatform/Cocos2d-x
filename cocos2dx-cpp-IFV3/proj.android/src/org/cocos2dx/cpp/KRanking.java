package org.cocos2dx.cpp;

import java.io.IOException;
import java.util.List;
import java.util.UUID;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.net.Uri;
//import android.util.Log;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiBucket;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.callback.KiiObjectCallBack;
import com.kii.cloud.storage.callback.KiiQueryCallBack;
import com.kii.cloud.storage.callback.KiiUserCallBack;
import com.kii.cloud.storage.exception.app.AppException;
import com.kii.cloud.storage.query.KiiClause;
import com.kii.cloud.storage.query.KiiQuery;
import com.kii.cloud.storage.query.KiiQueryResult;

public class KRanking {
	private final static String TAG ="KRanking";
	KiiBucket m_appRankingBucket;
	String m_username, m_displayName;
	//private KiiUser m_user;
	private Cocos2dxActivity m_simpleGame;
	
	public KRanking(){
		m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
	}
	
	public KRanking(Cocos2dxActivity a) {
		m_simpleGame = a;
		m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
	}

	public void displayNameUpdate(String name){
		MYLog.v(TAG, "displayNameUpdate " + name);
		
		if( KiiUser.isValidDisplayName(name)==true ){
			MYLog.v(TAG, "isValidDisplayName OK");
		} else {
			MYLog.v(TAG, "isValidDisplayName NG");
			return;
		}
		
		KiiUser user2 = KiiUser.getCurrentUser();		
		user2.setDisplayname(name);
		user2.update( new KiiUserCallBack() {
			@Override
			public void onUpdateCompleted(int token,   KiiUser user, Exception e) {
				MYLog.v(TAG, "displayNameUpdate onUpdateCompleted " + token + " " + user +" " + e);
				m_displayName = user.getDisplayname();
				CallCPP.setDisplayame(m_displayName);	//C++���Ăяo��
			}
        });
	}
	
	/***
	 * ranking_query_all
	 */
	public void ranking_query_all(){
		MYLog.v(TAG, "ranking_query_all");
		m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
		if(m_appRankingBucket!=null){
			ranking_query_all(m_appRankingBucket);
		} else {
			MYLog.v(TAG, "ranking_query_all m_appRankingBucket null");
		}
	}
	
	/***
	 * @param name
	 * @param score
	 */
	public void ranking_post(String name, int score){
		MYLog.v(TAG, "ranking_post " + name +" "+ score);
		ranking_query(m_appRankingBucket, name, score);
	}
	
	/***
	 * ranking_query_all
	 * @param bucket
	 */
	private void ranking_query_all(KiiBucket bucket){
		MYLog.v(TAG, "ranking_query_all " + bucket);
		KiiQuery query = new KiiQuery();
		query.sortByDesc(Field.SCORE);
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				MYLog.v(TAG, "ranking_query_all onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int size = objLists.size();
				MYLog.v(TAG, "size " + size);
				JSONArray jArray = new JSONArray();
				for (KiiObject obj : objLists) {
					//Log.v(TAG, "obj "+ i +" " + obj);
					String name2 = obj.getString(Field.NAME, "null_name");
					String dname = obj.getString(Field.DISPLAYNAME, "null_name");
					String score2 = obj.getString(Field.SCORE,"0");

					MYLog.v(TAG, "ranking_query_all " + name2 +" "+ dname + " "+ score2 );
					
					JSONObject nJArray = new JSONObject();
					try {
						nJArray.put(Field.NAME, dname);
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
				MYLog.v(TAG, "jArray " + jArray);
				String s = jArray.toString();
				CallCPP.rankingResponse(s);
			}
        }, query);	
	}
	
	/***
	 * ranking_query
	 * @param bucket
	 * @param name
	 * @param score
	 */
	private void ranking_query(KiiBucket bucket, final String name, final int score){
		MYLog.v(TAG, "ranking_query " + name);
		KiiQuery query = new KiiQuery( KiiClause.equals(Field.NAME, m_username) );
		query.sortByDesc(Field.SCORE);
		
        // call KiiCloud API
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				MYLog.v(TAG, "ranking_query onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int myScore = 0;
				Uri uri = null;
				int size = objLists.size();
				MYLog.v(TAG, "size " + size);
				for (KiiObject obj : objLists) {
					MYLog.v(TAG, "obj " + obj);
					String name2 = obj.getString(Field.NAME);
					String score2 = obj.getString(Field.SCORE);
					int score3 = obj.getInt(Field.SCORE);
					MYLog.v(TAG, "ranking_query " + name2 +" "+ score2+ " "+ score3 );
					if(score3 > myScore){
						myScore = score3;
						uri = obj.toUri();
						MYLog.v(TAG, "myScore " + myScore);
					}
			  	}
				MYLog.v(TAG, "myScore " + myScore);
				MYLog.v(TAG, "uri " + uri);
				if(score>myScore){
				//if(true){
					MYLog.v(TAG, "hiscore");					
					if(uri!=null){
						ranking_save(uri, name, score);
					} else {
						MYLog.v(TAG, "uri null");
						ranking_save(null, name, score);
					}
				} else{
					MYLog.v(TAG, "not hiscore");					
				}
			}
        }, query);	
	}
	
	/***
	 * ranking_save
	 * @param name
	 * @param score
	 */
	private void ranking_save(Uri uri, String name, int score){
		MYLog.v(TAG, "ranking_save " + uri +" "+ name +" "+ score);
		
		KiiObject object;
		if(uri==null){
			MYLog.v(TAG, "新規");
			object = m_appRankingBucket.object();
		} else {
			MYLog.v(TAG, "更新");
			object = KiiObject.createByUri(uri);
		}
        object.set(Field.NAME, m_username);
        object.set(Field.DISPLAYNAME, m_displayName);
        object.set(Field.SCORE, score);

        // call KiiCloud API
        object.save( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				MYLog.v(TAG, "ranking_save onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				ranking_refresh(uri);
			}
        });
	}
	
	/**
	 * ranking_refresh
	 * @param uri
	 */
	private void ranking_refresh(Uri uri){
		MYLog.v(TAG, "refresh_ranking " + uri);
        KiiObject object2 = KiiObject.createByUri(uri);
        
        // call KiiCloud API
        object2.refresh( new KiiObjectCallBack() {
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				MYLog.v(TAG, "refresh_ranking onRefreshCompleted " + token + " " + object +" " + e);
				String name = object.getString(Field.NAME,"null");
				String score = object.getString(Field.SCORE,"0");
				String display_name = object.getString(Field.DISPLAYNAME,"null");
				MYLog.v(TAG, "refresh_ranking " + name +" "+display_name+" "+ score );
			}
        });         
	}

	/**
	 * save
	 */
	public void save(){
		MYLog.v(TAG, "save");
		
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
				MYLog.v(TAG, "save onSaveCompleted " + token + " " + object +" " + e);
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
		MYLog.v(TAG, "refresh " + uri);
        KiiObject object2 = KiiObject.createByUri(uri);
        
        // call KiiCloud API
        object2.refresh( new KiiObjectCallBack() {
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				MYLog.v(TAG, "refresh onRefreshCompleted " + token + " " + object +" " + e);
				String name2 = object.getString(Field.NAME);
				String dname = object.getString(Field.DISPLAYNAME, "null_name");
				MYLog.v(TAG, "refresh " + name2 +" "+dname );
			}
        });  
	}
	
	public void login(String token){
		MYLog.v(TAG, "login "+ token);
		KiiUser.loginWithToken( new KiiUserCallBack() {
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	MYLog.v(TAG, "loginWithToken onLoginCompleted " + token + " " + user +" " + e);
                m_username = user.getUsername();
		    	m_displayName = user.getDisplayname();
		    	MYLog.v(TAG, "m_username " + m_username );
		    	MYLog.v(TAG, "m_displayName " + m_displayName );
		    	if(m_displayName==null){
		    		m_displayName = "PlayerName";
		    	}
		    	CallCPP.setName(m_username);
		    	CallCPP.setDisplayame(m_displayName);
			}
		}, token);
	}
	
	public void loginWithTokenSYNC(String accessToken){
    	MYLog.v(TAG, "loginWithTokenSYNC start"); 
		try {
		  KiiUser.loginWithToken(accessToken);
		  KiiUser user = KiiUser.getCurrentUser();
	    	MYLog.v(TAG, "user " + user);
	    	//save();
	    	m_username = user.getUsername();
	    	m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
		} catch (IOException e) {
			MYLog.v(TAG, "IOException e " + e); 
		} catch (AppException e) {
			MYLog.v(TAG, "AppException e " + e); 
		}
    	MYLog.v(TAG, "loginWithTokenSYNC end"); 
	}
	
	/**
	 * login
	 */
	public void login(String userName, String passWord){
		MYLog.v(TAG, "login " + userName +" "+passWord);
		
		final String username = userName;
		String password = passWord;		
		
        // call KiiCloud API
        KiiUser.logIn( new KiiUserCallBack() {
			@Override
			public void onLoginCompleted(int token, KiiUser user, Exception e) {
		    	MYLog.v(TAG, "login onLoginCompleted " + token + " " + user +" " + e);
		    	//save();
		    	m_username = username;
		    	m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
			}
        }, username, password);
	}
	
	public void regist(String userName, String passWord){
		MYLog.v(TAG, "regist " + userName +" "+ passWord);

		String username = userName;
		String password = passWord;
        KiiUser user = KiiUser.createWithUsername(username);
        user.register( new KiiUserCallBack() {
        	@Override
        	public void onRegisterCompleted(int token, KiiUser user, Exception e){
        		MYLog.v(TAG, "onRegisterCompleted " + token + " " + user +" " + e);
        		
                // store access token
                KiiUser user2 = KiiUser.getCurrentUser();
                String token2 = user2.getAccessToken();
                m_username = user2.getUsername();
                MYLog.v(TAG, "user2 " + user2);
                MYLog.v(TAG, "token2 " + token2);
                MYLog.v(TAG, "m_username " + m_username);
                Pref.setStoredAccessToken(m_simpleGame.getApplicationContext(), token2);
                Pref.setUSERNAME(m_simpleGame.getApplicationContext(), m_username);
                Pref.setPASSWORD(m_simpleGame.getApplicationContext(), "1234");
                
                displayNameUpdate("PlayerName");
		    	CallCPP.setName(m_username);
        	}
        }, password);
	}
	
	/**
	 * regist
	 */
	public void regist(){
		MYLog.v(TAG, "regist");

		String username;
		String password;
		String uuid = UUID.randomUUID().toString();
		//Pref.setUUID( m_simpleGame.getApplicationContext(), uuid);
		username = uuid;
		password = "1234";
		boolean flag;
		flag = KiiUser.isValidUserName(username);
		if(!flag){
			MYLog.v(TAG, "isValidUserName error");
			return;
		}

		flag = KiiUser.isValidPassword(password);
		if(!flag){
			MYLog.v(TAG, "isValidPassword error");
			return;
		}
		
		Pref.setUUID( m_simpleGame.getApplicationContext(), uuid);
		regist(username, password);

		String uuid2 = Pref.getUUID( m_simpleGame.getApplicationContext() );
		MYLog.v(TAG, "uuid2 " + uuid2);
	}
	
}
