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

//import android.support.v4.app.Fragment;

import java.io.IOException;

import android.net.Uri;
import android.util.Log;

import com.kii.cloud.storage.KiiBucket;
import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.callback.KiiUserCallBack;
//import com.kii.util.dialog.ProgressDialogFragment;
import com.kii.cloud.storage.exception.app.BadRequestException;
import com.kii.cloud.storage.exception.app.ConflictException;
import com.kii.cloud.storage.exception.app.ForbiddenException;
import com.kii.cloud.storage.exception.app.NotFoundException;
import com.kii.cloud.storage.exception.app.UnauthorizedException;
import com.kii.cloud.storage.exception.app.UndefinedException;

class LoginCallback extends KiiUserCallBack {
	private final static String TAG ="LoginCallback";
//    private LoginDialogFragment dialog;

/***
    public LoginCallback(LoginDialogFragment dialog) {
        this.dialog = dialog;
    }
***/
	
    public LoginCallback() {
    	Log.v(TAG, "LoginCallback");
        //this.dialog = dialog;
    }

    
    @Override
    public void onLoginCompleted(int token, KiiUser user, Exception e) {
        super.onLoginCompleted(token, user, e);
        
    	Log.v(TAG, "onLoginCompleted " + token + " " + user +" " + e);
    	if(e==null){
    		save();
    	}
        
/***        
        ProgressDialogFragment.hide(dialog.getFragmentManager());
        if (e != null) {
            dialog.showErrorMessage("login failed");
            return;
        }
        dialog.dismiss();
        Fragment callback = dialog.getTargetFragment();
        if (callback != null && callback instanceof TitleFragment) {
            TitleFragment fragment = (TitleFragment) callback;
            fragment.onLoginFinished();
        }
***/
    }
    
	private void save(){
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
        //BalanceListFragment target = (BalanceListFragment)fragment.getTargetFragment();
        AddCallback callback = new AddCallback(null);
        //object.save(callback);	//saveここでクラウドへ保存される、callbackなしのメソッドもある？
        
        try {
			object.save();
		} catch (BadRequestException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (ConflictException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (ForbiddenException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (NotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (UnauthorizedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (UndefinedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
        
        
        //取得
        Uri uri = object.toUri();
        Log.v(TAG, "save uri " + uri);
        
        KiiObject object2 = KiiObject.createByUri(uri);
        try {
			object2.refresh();
			String name2 = object2.getString(Field.NAME);
			String type2 = object2.getString(Field.TYPE);
			String amount2 = object2.getString(Field.AMOUNT);
			Log.v(TAG, "save refresh " + name2 +" "+ type2 +" "+amount2 );
			
		} catch (BadRequestException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnauthorizedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ForbiddenException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ConflictException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UndefinedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
