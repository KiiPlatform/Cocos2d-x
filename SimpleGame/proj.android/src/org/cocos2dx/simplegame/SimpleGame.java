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

import org.cocos2dx.lib.Cocos2dxActivity;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.Kii.Site;

import android.os.Bundle;
import android.util.Log;

public class SimpleGame extends Cocos2dxActivity{
	private final static String TAG ="SimpleGame";
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);

		Log.v(TAG, "onCreate");

/***
		CallCPP.nativeEnd();
		String s = CallCPP.concat("hoge", "fuga");
		Log.v(TAG, "s = " +s);
***/		

	    //initialize
        initKiiSDK();
        
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
       
        // login with token
        AutoLoginCallback callback = new AutoLoginCallback(this);
        KiiUser.loginWithToken(callback, token);
        
		Log.v(TAG, "onCreate end");
	}
	
	//regist
	
	private void login(){
		String username ="muku";
		String password = "1234";		
		
		Log.v(TAG, "login");
		
        // call user registration API
        LoginCallback callback = new LoginCallback();
        KiiUser.logIn(callback, username, password);
	}
	
	private void regist(){
		String username ="muku";
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




