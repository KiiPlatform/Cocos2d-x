package org.cocos2dx.cpp;

import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
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
import com.kii.cloud.storage.exception.app.AppException;
import com.kii.cloud.storage.exception.app.BadRequestException;
import com.kii.cloud.storage.exception.app.ConflictException;
import com.kii.cloud.storage.exception.app.ForbiddenException;
import com.kii.cloud.storage.exception.app.NotFoundException;
import com.kii.cloud.storage.exception.app.UnauthorizedException;
import com.kii.cloud.storage.exception.app.UndefinedException;
import com.kii.cloud.storage.query.KiiClause;
import com.kii.cloud.storage.query.KiiQuery;
import com.kii.cloud.storage.query.KiiQueryResult;

public class KiiAPICall {
	private final static String TAG ="KiiAPICall";
	private KiiBucket mBucket;
	private KiiListenerInterface m_listener;
	private HashMap<String,Object> m_json_map;
	//private int m_serviceID;
	
	public KiiAPICall(HashMap<String,Object> json_map, final KiiListenerInterface listener){
		Log.v(TAG, "KiiAPICall " + json_map);
		//m_serviceID = serviceID;
		m_listener = listener;
		m_json_map = json_map;
	}
	
	//query
	public void run_query(){
		Log.v(TAG, "run_query");
		String backet_key = (String) m_json_map.get("backet_key");
		String error;
		//bucket
		if(backet_key==null){
			error = "backet_key";
			Log.v(TAG, "error " + error);
			return;
		}
		KiiBucket bucket = Kii.bucket(backet_key);	//B_RANKING
		//KiiClause_equals
		KiiQuery query;

		String eq1 = (String) m_json_map.get("KiiClause_equals1");
		String eq2 = (String) m_json_map.get("KiiClause_equals2");
		if( (eq1 != null) && (eq2 != null) ){
			 KiiClause cl = KiiClause.equals(eq1, eq2);
			 query = new KiiQuery(cl);
		} else {
			query = new KiiQuery();
		}
		//sortByDesc
		String field = (String) m_json_map.get("sortByDesc");
		if(field != null){
			query.sortByDesc(field);
		}
		
		//queryを実行
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				Log.v(TAG, "run_query onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int size = objLists.size();
				Log.v(TAG, "size " + size);
				JSONArray jArray = new JSONArray();
				int i = 0;
				for (KiiObject obj : objLists) {
					i++;
					String name2 = obj.getString(Field.NAME, "null_name");
					String dname = obj.getString(Field.DISPLAYNAME, "null_name");
					String score2 = obj.getString(Field.SCORE,"0");
					Log.v(TAG, "onQueryCompleted " + i+ " " +name2 +" "+ dname + " "+ score2 );
					JSONObject nJArray = new JSONObject();
					//keyのloop
					
					//
					jArray.put(nJArray);
			  	}
				Log.v(TAG, "jArray " + jArray);
				String s = jArray.toString();
				//リスナーを実行する
				m_listener.onCompleted(s);
			}
        }, query);			
		//
	}

	//query
	public void run_query2(){
		Log.v(TAG, "run_query2");
		
		String error;
		//bucket
		String backet_key = (String) m_json_map.get("backet_key");
		Log.v(TAG, "run_query2 backet_key="+backet_key);

		if(backet_key==null){
			error = "backet_key";
			Log.v(TAG, "error " + error);
			return;
		}
		
		//query
		String s_query = (String) m_json_map.get("query");
		Log.v(TAG, "run_query2 s_query="+s_query);

		if(s_query==null){
			error = "query";
			Log.v(TAG, "error " + error);
			return;
		}
		Log.v(TAG, "s_query " + s_query);
		KiiBucket bucket = Kii.bucket(backet_key);	//B_RANKING
		
		KiiQuery query = new KiiQuery(s_query);
		
		//JavaでつくったQuery、for test
		//KiiQuery query = new KiiQuery( KiiClause.equals(Field.NAME, "63533edf-d796-4dec-b341-feb61d07ced3") );
		//query.sortByDesc(Field.SCORE);
		//
		//KiiQuery query = new KiiQuery();
		
		//queryを実行
		bucket.query( new KiiQueryCallBack<KiiObject>() {
			@Override
			public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
				Log.v(TAG, "run_query2 onQueryCompleted " + e);
				List<KiiObject> objLists = result.getResult();
				int size = objLists.size();
				Log.v(TAG, "size " + size);
				Log.v(TAG, "objLists " + objLists);
				JSONArray jArray = new JSONArray();	//レスポンスするすべてのjsonを入れる
				for (KiiObject obj : objLists) {
					Uri uri = obj.toUri();
					Log.v(TAG, "uri=" + uri );
					
					//nJArrayの作成
					HashSet<String> keyset = obj.keySet();//objectのkeyを取得
			        Iterator<String> it = keyset.iterator();
					JSONObject nJArray = new JSONObject();	//１行分のjson
					//keyのvalを集めて１行分のjsonを作成する
					while (it.hasNext()) {
			        	String key = it.next();
			        	Log.v(TAG, "key " + key );
			        	//String val = obj.getString(key);	Stringはやめ
			        	Object val = obj.getObject(key);	//getObjectを新設した
			        	Log.v(TAG, "val " + val );
			        	try {
			        		nJArray.put(key, val);		//すべてkeyについてputする
						} catch (JSONException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
			        }
					try {
						nJArray.put("_uri_", uri.toString());
					} catch (JSONException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					jArray.put(nJArray);	//作成した１行をputする
			  	}
				Log.v(TAG, "jArray " + jArray);
				String s = jArray.toString();
				//リスナーを実行する
				m_listener.onCompleted(s);
			}
        }, query);			
		//
	}
	

	//save
	public void run_object_save() {
		Log.v(TAG, "run_object_save");
		
		String backet_key = (String) m_json_map.get("backet_key");
		KiiObject object = Kii.bucket(backet_key).object();
		
		//setを取り出して実行する
		for (String key : m_json_map.keySet()) {
			if (key.startsWith("set")) {
				Log.v(TAG, "key " + key);
				//String val = m_json_map.get(key);
				Object val = m_json_map.get(key);

				Log.v(TAG, "val " + val);
				//String[] strAry = key.split("_");
				String[] strAry = key.split("set_");
				Log.v(TAG, "strAry.length " + strAry.length );
				Log.v(TAG, "strAry " + strAry[0] + " "+ strAry[1]);
				Log.v(TAG, "set " + strAry[1] +","+ val);
				
				//型判定してsetする
				if (val instanceof String){
					Log.v(TAG, "instanceof String");
					String val2 = (String)val;
					object.set(strAry[1], val2);	//setを実行
				} else if(val instanceof Integer){
					Log.v(TAG, "instanceof Integer");
					Integer val2 = (Integer)val;
					object.set(strAry[1], val2);	//setを実行
										
				} else if(val instanceof Double){
					Log.v(TAG, "instanceof Double");
					Double val2 = (Double)val;
					object.set(strAry[1], val2);	//setを実行
				} else {
					//エラー
				}
			}
		}

        // call KiiCloud API
        object.save( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "run_object onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				Log.v(TAG, "uri " + uri );
				//jsonを作成する
				JSONObject json_obj = new JSONObject();
				String json = null;
				try {
					json_obj.put("uri", uri);
					json = json_obj.toString();
				} catch (JSONException e1) {
					// TODO Auto-generated catch block
					Log.v(TAG, "onSaveCompleted e " + e1);
					e1.printStackTrace();
				}
				m_listener.onCompleted(json);
			}
        });
		
	}
	
	//refresh
	public void run_object_refresh() {
		Log.v(TAG, "run_object_refresh");
		
		String s_uri = (String) m_json_map.get("uri");
		Uri uri = Uri.parse(s_uri);
		Log.v(TAG, "uri = " + uri);
        KiiObject object2 = KiiObject.createByUri(uri);
        HashSet<String> keyset = object2.keySet();
        Log.v(TAG, "keyset = " + keyset);
        
        // call KiiCloud API
        object2.refresh( new KiiObjectCallBack() {
			@Override
			public void onRefreshCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "refresh_ranking onRefreshCompleted " + token + " " + object +" " + e);
				HashSet<String> keyset = object.keySet();//objectのkeyを取得
		        Iterator<String> it = keyset.iterator();
				JSONObject json_obj = new JSONObject();
		        while (it.hasNext()) {
		        	String key = it.next();
		        	Log.v(TAG, "key " + key );
		        	//String val = object.getString(key);	//String
		        	//Object val = object.get(key);	//Object
		        	Object val = object.getObject(key);	//getObjectを新設した
		        	Log.v(TAG, "val " + val );
		        	try {
						json_obj.put(key, val);	//valをObjectにした
					} catch (JSONException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
		        }
		        String json = json_obj.toString();
		        m_listener.onCompleted(json);
			}
        });
	}

	//update
	public void run_object_update() {
		// TODO Auto-generated method stub
		Log.v(TAG, "run_object_update");
		
		//String backet_key = m_json_map.get("backet_key");
		//KiiObject object = Kii.bucket(backet_key).object();
		String s_uri = (String) m_json_map.get("uri");
		Uri uri = Uri.parse(s_uri);
		Log.v(TAG, "uri = " + uri);
        KiiObject object = KiiObject.createByUri(uri);
        
        //refresh
        Log.v(TAG, "run_object_update refresh1");
        try {
			object.refresh();	//refresh
		} catch (BadRequestException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		} catch (UnauthorizedException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		} catch (ForbiddenException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		} catch (ConflictException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		} catch (NotFoundException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		} catch (UndefinedException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		} catch (IOException e2) {
			// TODO Auto-generated catch block
			e2.printStackTrace();
		}
        Log.v(TAG, "run_object_update refresh2");
        //
		
		//setを取り出して実行する
		for (String key : m_json_map.keySet()) {
			if (key.startsWith("set")) {
				Log.v(TAG, "key " + key);
				Object val = m_json_map.get(key);	//Objectにする
				Log.v(TAG, "val " + val);
				//String[] strAry = key.split("_");
				String[] strAry = key.split("set_");
				Log.v(TAG, "strAry.length " + strAry.length );
				Log.v(TAG, "strAry[0] " + strAry[0] );
				Log.v(TAG, "strAry[1] " + strAry[1] );
				Log.v(TAG, "set " + strAry[1] +","+ val);
				//object.set(strAry[1], val);	//setを実行
				
				//型判定してsetする
				if (val instanceof String){
					Log.v(TAG, "instanceof String");
					String val2 = (String)val;
					object.set(strAry[1], val2);	//setを実行
				} else if(val instanceof Integer){
					Log.v(TAG, "instanceof Integer");
					Integer val2 = (Integer)val;
					object.set(strAry[1], val2);	//setを実行
										
				} else if(val instanceof Double){
					Log.v(TAG, "instanceof Double");
					Double val2 = (Double)val;
					object.set(strAry[1], val2);	//setを実行
				} else {
					//エラー
				}
				
			}
		}
		Log.v(TAG, "object="+object);
        // call KiiCloud API
		Log.v(TAG, "run_object_update object.save");
        object.save( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "run_object_update onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				Log.v(TAG, "uri " + uri );
				//jsonを作成する
				JSONObject json_obj = new JSONObject();
				String json = null;
				try {
					json_obj.put("uri", uri);
					json = json_obj.toString();
				} catch (JSONException e1) {
					// TODO Auto-generated catch block
					Log.v(TAG, "onSaveCompleted e " + e1);
					e1.printStackTrace();
				}
				m_listener.onCompleted(json);
			}
        });		
	}	

/***
	//saveAllFields
	public void run_object_saveAllFields() {
		Log.v(TAG, "run_object_saveAllFields");
		
		String s_uri = (String) m_json_map.get("uri");
		Uri objUri = Uri.parse(s_uri);
		KiiObject object = KiiObject.createByUri(objUri);

		//setを取り出して実行する
		for (String key : m_json_map.keySet()) {
			if (key.startsWith("set")) {
				Log.v(TAG, "key " + key);
				String val = m_json_map.get(key);
				Log.v(TAG, "val " + val);
				String[] strAry = key.split("_");
				Log.v(TAG, "strAry.length " + strAry.length );
				Log.v(TAG, "strAry " + strAry[0] + " "+ strAry[1]);
				Log.v(TAG, "set " + strAry[1] +","+ val);
				object.set(strAry[1], val);	//setを実行
			}
		}

        // call KiiCloud API
        object.saveAllFields( new KiiObjectCallBack() {
			@Override
			public void onSaveCompleted(int token, KiiObject object, Exception e) {
				Log.v(TAG, "run_object_saveAllFields onSaveCompleted " + token + " " + object +" " + e);
				Uri uri = object.toUri();
				Log.v(TAG, "uri " + uri );
				//jsonを作成する
				JSONObject json_obj = new JSONObject();
				String json = null;
				try {
					json_obj.put("uri", uri);
					json = json_obj.toString();
				} catch (JSONException e1) {
					// TODO Auto-generated catch block
					Log.v(TAG, "createApplicationScopeBucket e " + e1);
					e1.printStackTrace();
				}
				m_listener.onCompleted(json);
			}
        },true);	
	}
***/

}
