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

import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.callback.KiiUserCallBack;

/**
 * 
 * @author guest
 *
 */
class MyUserCallback extends KiiUserCallBack {
	private final static String TAG ="MyUserCallback";
	private KiiListenerInterface m_listener = null;
	
    public MyUserCallback() {
    	Log.v(TAG, "MyUserCallback");
    }

    public void setListener(KiiListenerInterface l){
    	m_listener = l;	//リスナーを登録
    }
    
    @Override
    public void onLoginCompleted(int token, KiiUser user, Exception e) {
        super.onLoginCompleted(token, user, e);
        
    	Log.v(TAG, "onLoginCompleted " + token + " " + user +" " + e);
    	m_listener.onLoginCompleted(token, user, e);	//リスナーを実行

    }

}
