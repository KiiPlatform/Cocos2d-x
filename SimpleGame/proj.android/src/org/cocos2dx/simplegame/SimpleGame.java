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

		SimpleGame a = this;
		
		Log.v(TAG, "onCreate");

		Global.activity = this;
		
	    //initialize
        initKiiSDK();
		m_kRanking = new KRanking(a);
		
        // check access token
        String token = Pref.getStoredAccessToken(getApplicationContext());
        if (token == null || token.length() == 0) {
        	Log.v(TAG, "token null");
        	m_kRanking.regist();
        } else {
        	Log.v(TAG, "login proc start");  
            m_kRanking.login(token);
        }
       
		Log.v(TAG, "onCreate end");
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




