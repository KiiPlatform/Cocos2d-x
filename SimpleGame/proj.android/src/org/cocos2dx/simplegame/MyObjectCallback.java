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


import android.util.Log;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.callback.KiiObjectCallBack;

/**
 * 
 * @author guest
 *
 */
public class MyObjectCallback extends KiiObjectCallBack {
	private final static String TAG ="MyObjectCallback";
	private KiiListenerInterface m_listener = null;
	
    public MyObjectCallback() {
        super();
        Log.v(TAG, "MyObjectCallback");
    }    
    
    public void setListener(KiiListenerInterface l){
    	m_listener = l;	//リスナーを登録
    }
    
    /*
     * (non-Javadoc)
     * @see com.kii.cloud.storage.callback.KiiObjectCallBack#onSaveCompleted(int, com.kii.cloud.storage.KiiObject, java.lang.Exception)
     */
    @Override
    public void onSaveCompleted(int token, KiiObject object, Exception e) {
        super.onSaveCompleted(token, object, e);
        
    	Log.v(TAG, "onSaveCompleted " + token + " " + object +" " + e);
    	m_listener.onSaveCompleted(token, object, e);	//リスナーを実行
    }
    
    @Override
    public void onRefreshCompleted(int token, KiiObject object, Exception e) {
        super.onSaveCompleted(token, object, e);
        
    	Log.v(TAG, "onRefreshCompleted " + token + " " + object +" " + e);
    	m_listener.onRefreshCompleted(token, object, e);	//リスナーを実行
    }  
    
}
