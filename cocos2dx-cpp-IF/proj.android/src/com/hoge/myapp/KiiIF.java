package com.hoge.myapp;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiBucket;

public class KiiIF {
	static final String TAG ="KiiIF";
	
	static HashMap<String,KiiBucket> bucket_map  = new HashMap<String,KiiBucket>();
	static HashMap<Integer,String>   service_map = new HashMap<Integer,String>();	
	
	static int buket_key_counter = 0;
	public static void kiiReq(int serviceID, String json){
		service_map.put(serviceID, json);
		decodeJson(serviceID, json);
	}

	private static void decodeJson(int serviceID, String json) {
		Log.v(TAG, "decodeJson " + json );	
		
		// TODO Auto-generated method stub
		try {
			JSONObject rootObject = new JSONObject(json);
			//JSONObject queryObject  = rootObject.getJSONObject("query");
			//Log.v(TAG, "queryObject =" + queryObject);
			 Iterator<String> it = rootObject.keys();
			 int i = 0;
			 HashMap<String,String>   json_map = new HashMap<String,String>();
			 while (it.hasNext()) {
				 i++;
				 String key =it.next();
				 String val = rootObject.getString(key);
				 Log.v(TAG, i + " key " + key+ ","+val);
				 json_map.put(key, val);
			 } 
			String cmd = rootObject.getString("cmd");
	        Class c = null;
			try {
				c = Class.forName("com.hoge.myapp.KiiIF");
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	        Method method = null;
	        try {
				//method = c.getMethod("staticMethod", null);
				//method = c.getMethod(cmd, null);	//引数なし
				//method = c.getMethod(cmd, new Class[]{ int.class, });	//引数１つ
				method = c.getMethod(cmd, new Class[]{ int.class, HashMap.class });	//メソッドを作成、引数２つ
				Log.v(TAG, "method " + method); 
			} catch (SecurityException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	        //me.invoke(sample, new Object[]{a,b,c,map});
	        //Object[] args   = new Object[]{ new Integer( 2 ), new Integer( 3 ) };
	        try {
				//method.invoke(null, serviceID, json_map);
				//method.invoke(null, null);	//引数なし
	        	//method.invoke(null, serviceID);	//引数１つ
	        	Object[] args   = new Object[]{  Integer.valueOf(serviceID), json_map };
	        	method.invoke(null, args);	//メソッドを実行
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				Log.v(TAG, "e " + e); 
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				Log.v(TAG, "e " + e); 
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				Log.v(TAG, "e " + e); 
				e.printStackTrace();
			}
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			Log.v(TAG, "e " + e); 
			e.printStackTrace();
		}
	}

	public static void createApplicationScopeBucket() {
		Log.v(TAG, "createApplicationScopeBucket1");
	}

	public static void createApplicationScopeBucket(int serviceID) {
		Log.v(TAG, "createApplicationScopeBucket2 " + serviceID);
	}
			
    public static void createApplicationScopeBucket( int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "createApplicationScopeBucket3 " + serviceID +" "+ json_map);
		String backet_key = json_map.get("backet_key");
		KiiBucket b = Kii.bucket(backet_key);	//bucketを作成する
		bucket_map.put(backet_key, b);
		Log.v(TAG, "backet_key " + backet_key);
		
		//jsonを作成する
		JSONObject json_obj = new JSONObject();
		try {
			json_obj.put("backet_key", backet_key);
			String s = json_obj.toString();
			//CallCPP.kiires(serviceID, s);	
			CallCPP.setDisplayame2(s, serviceID);	//C++へ　-----
			//CallCPP.nativeEnd();
			//CallCPP.kiires3("hoge");
			//CallCPP.setDisplayame("hogehoge");
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			Log.v(TAG, "createApplicationScopeBucket e " + e1);
			e1.printStackTrace();
		}
    }
	
    //save
    public static void object_save(final int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "object_save");
		KiiListenerInterface l = new KiiListenerInterface(){
			@Override
			public void onCompleted(String json) {
				Log.v(TAG, "object_save onCompleted " + serviceID + " " + json);
				CallCPP.setDisplayame2(json, serviceID);//C++へ -----
			}
		};
		KiiAPICall kapi = new KiiAPICall(json_map, l);
		kapi.run_object_save();	//object
    }

    //refresh
    public static void object_refresh(final int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "object_refresh");
		KiiListenerInterface l = new KiiListenerInterface(){
			@Override
			public void onCompleted(String json) {
				Log.v(TAG, "object_refresh onCompleted " + serviceID + " " + json);
				CallCPP.setDisplayame2(json, serviceID);//C++へ -----
			}
		};
		KiiAPICall kapi = new KiiAPICall(json_map, l);
		kapi.run_object_refresh();	//object
    }

    //update
    public static void object_update(final int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "object_update");
		KiiListenerInterface l = new KiiListenerInterface(){
			@Override
			public void onCompleted(String json) {
				Log.v(TAG, "object_update onCompleted " + serviceID + " " + json);
				CallCPP.setDisplayame2(json, serviceID);//C++へ -----
			}
		};
		KiiAPICall kapi = new KiiAPICall(json_map, l);
		kapi.run_object_update();	//object
    }
    
    public static void object_saveAllFields(final int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "object_saveAllFields");
		KiiListenerInterface l = new KiiListenerInterface(){
			@Override
			public void onCompleted(String json) {
				Log.v(TAG, "object_saveAllFields onCompleted " + serviceID + " " + json);
				CallCPP.setDisplayame2(json, serviceID);//C++へ -----
			}
		};
		KiiAPICall kapi = new KiiAPICall(json_map, l);
		kapi.run_object_saveAllFields();	//object
    }    
        
    public static void bucket_query(final int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "bucket_query");
		KiiListenerInterface l = new KiiListenerInterface(){
			@Override
			public void onCompleted(String json) {
				Log.v(TAG, "bucket_query onCompleted " + serviceID + " " + json);
				CallCPP.setDisplayame2(json, serviceID);//C++へ -----
			}
		};
		KiiAPICall kapi = new KiiAPICall(json_map, l);
		kapi.run_query2();	//2
    }

    public static void display_name_update(int serviceID, HashMap<String,String> json_map) {
		Log.v(TAG, "display_name_update");
    }
    
	/***
	private class KiiServie{
		public KiiServie(){
			
		}
	}
	***/
}
