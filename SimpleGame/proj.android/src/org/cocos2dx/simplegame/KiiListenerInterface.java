package org.cocos2dx.simplegame;

import com.kii.cloud.storage.KiiObject;
import com.kii.cloud.storage.KiiUser;
import com.kii.cloud.storage.callback.KiiQueryCallBack;
import com.kii.cloud.storage.query.KiiQueryResult;

public interface KiiListenerInterface {
	public void onLoginCompleted(int token, KiiUser user, Exception e);		//KiiUserCallBack
    //public void onRegisterCompleted(int token, KiiUser user, Exception e);	//KiiUserCallBack
    public void onSaveCompleted(int token, KiiObject object, Exception e);	//KiiObjectCallBack
    public void onRefreshCompleted(int token, KiiObject object, Exception e);	//KiiObjectCallBack
    public void onQueryCompleted(int arg0, KiiQueryResult<KiiObject> result, Exception e);//KiiQueryCallBack<KiiObject>

}
