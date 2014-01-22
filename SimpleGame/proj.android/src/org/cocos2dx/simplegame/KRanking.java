package org.cocos2dx.simplegame;

import java.util.List;

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
import com.kii.cloud.storage.query.KiiClause;
import com.kii.cloud.storage.query.KiiQuery;
import com.kii.cloud.storage.query.KiiQueryResult;

public class KRanking {
	private final static String TAG ="KRanking";
	KiiBucket m_appRankingBucket;
	String m_username;
	
	public KRanking(){
		m_appRankingBucket = Kii.bucket(Field.B_RANKING);	//B_RANKING
	}
	
	/***
	 * ranking_query_all
	 * �����L���f�[�^���擾����
	 */
	public void ranking_query_all(){
		Log.v(TAG, "ranking_query_all");
		ranking_query_all(m_appRankingBucket);
		//ranking_post("muku1111",1234+1);
	}
	
	/***
	 * �X�R�A���|�X�g����A�n�C�X�R�A�Ȃ�X�V�����
	 * @param name
	 * @param score
	 */
	public void ranking_post(String name, int score){
		Log.v(TAG, "ranking_post " + name +" "+ score);
		ranking_query(m_appRankingBucket, name, score);
	}
	
	/***
	 * ranking_query_all
	 * �����L���f�[�^���擾����c++�֓n���Ac++�̓����L���O��\������
	 * @param bucket
	 */
	private void ranking_query_all(KiiBucket bucket){
		Log.v(TAG, "ranking_query_all " + bucket);
		KiiQuery query = new KiiQuery();
		query.sortByDesc(Field.SCORE);
        // call KiiCloud API �R�[���o�b�N�Ń��X�|���X�����炤
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
				CallCPP.rankingResponse(s);	//rankingResponse��JSON��n��
			}
        }, query);	
	}
	
	/***
	 * ranking_query
	 * �v���C���ʂ��|�X�g���邽�߂ɂ܂��A�����L���O���擾���ăn�C�X�R�A�Ȃ�save����
	 * @param bucket
	 * @param name
	 * @param score
	 */
	private void ranking_query(KiiBucket bucket, final String name, final int score){
		Log.v(TAG, "ranking_query");
		//KiiQuery query = new KiiQuery();
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
						uri = obj.toUri();	//�n�C�X�R�A��uri��ۑ�
						Log.v(TAG, "myScore " + myScore);
					}
			  	}
				Log.v(TAG, "myScore " + myScore);
				Log.v(TAG, "uri " + uri);
				if(score>myScore){	//size��0�̏ꍇmyScore��0�̂܂܂Ȃ̂�score��0���傫���Ɛ^�ɂȂ�
				//if(true){
					Log.v(TAG, "hiscore");					
					if(uri!=null){
						ranking_save(uri, name, score);	//uri���g���čX�V save
					} else {
						Log.v(TAG, "uri null");
						ranking_save(null, name, score);//uri��null�Ȃ̂ŐV�K�쐬 save
					}
				} else{
					Log.v(TAG, "not hiscore");					
				}
				//CallCPP.rankingResponse("rankingResponse test test test");
			}
        }, query);	
	}
	
	/***
	 * ranking_save
	 * �����̃n�C�X�R�A��object���X�V�Auri���g�p
	 * uri��null�Ȃ�V�K�쐬
	 * @param name
	 * @param score
	 */
	private void ranking_save(Uri uri, String name, int score){
		Log.v(TAG, "ranking_save " + uri +" "+ name +" "+ score);
		
		KiiObject object;
		if(uri==null){
			Log.v(TAG, "�V�K�쐬");
			object = m_appRankingBucket.object();	//�V�K�쐬�͂���������
		} else {
			Log.v(TAG, "�X�V");
			object = KiiObject.createByUri(uri);	//�n�C�X�R�A��object���X�V����uri���g�p���ē���
		}
        object.set(Field.NAME, name);
        object.set(Field.SCORE, score);

        // call KiiCloud API
        object.save( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "ranking_save onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				ranking_refresh(uri);	//�ǂݏo���Ċm�F
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
	
	/**
	 * regist
	 */
	public void regist(){
		String username = "muku";
		String password = "1234";
		
		Log.v(TAG, "regist");
		
        // call user registration API
        RegisterCallback callback = new RegisterCallback();	//callback�̍쐬 RegisterCallback��KiiUserCallBack���p�������N���X
        KiiUser user = KiiUser.createWithUsername(username);
        user.register(callback, password);	//KiiUserCallBack��n�� callback��KiiUserCallBack���p�������N���X�̃C���X�^���X�i���́j
	}
	
}
