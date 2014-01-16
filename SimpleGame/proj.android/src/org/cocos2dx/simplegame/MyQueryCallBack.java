/*
 * Copyright 2013 Kii
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.cocos2dx.simplegame;


import java.util.List;

import android.util.Log;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.callback.KiiObjectCallBack;
import com.kii.cloud.storage.callback.KiiQueryCallBack;
import com.kii.cloud.storage.query.KiiQueryResult;

/**
 * 
 * @author guest
 *
 */
public class MyQueryCallBack extends KiiQueryCallBack<KiiObject> {
	private final static String TAG ="MyQueryCallBack";
	private KiiListenerInterface m_listener = null;
	
    public MyQueryCallBack() {
        super();
        Log.v(TAG, "MyQueryCallBack");
    }    
    
    public void setListener(KiiListenerInterface l){
    	m_listener = l;	//リスナーを登録
    }
    
    @Override
    public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e) {
        super.onQueryCompleted(arg0, result, e);
        
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

    
/***
    public void onQueryCompletedxx(int token, KiiQueryResult<T> result, Exception e) {
        super.onQueryCompleted(token, object, e);
        
    	Log.v(TAG, "onSaveCompleted " + token + " " + object +" " + e);
    	m_listener.onSaveCompleted(token, object, e);	//リスナーを実行
    }
***/
}
