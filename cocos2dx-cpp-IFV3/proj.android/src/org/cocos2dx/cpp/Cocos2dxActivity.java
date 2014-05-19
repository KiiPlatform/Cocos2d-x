package org.cocos2dx.cpp;

import com.kii.cloud.storage.Kii;
import com.kii.cloud.storage.KiiBucket;

import android.app.NativeActivity;
import android.content.SharedPreferences;
import android.os.Bundle;

public class Cocos2dxActivity extends NativeActivity {

    private static final String TAG = "onCreate";
    KiiBucket m_appRankingBucket = null;
    String m_username = null;
    KRanking m_kRanking = null;

    static {
        System.loadLibrary("cocos2dcpp");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        MYLog.v(TAG, "onCreate1");
        Global.activity = this;

        MYLog.v(TAG, "onCreate2");

        // initialize
        initKiiSDK();
        m_kRanking = new KRanking(this);

        // check access token
        String token = Pref.getStoredAccessToken(getApplicationContext());
        if (token == null || token.length() == 0) {
            MYLog.v(TAG, "token null");
            m_kRanking.regist();
        } else {
            MYLog.v(TAG, "login proc start");
            m_kRanking.login(token); // ASYNC
            // m_kRanking.loginWithTokenSYNC(token); //SYNC
        }

        // For supports translucency

        // 1.change "attribs" in cocos\2d\platform\android\nativeactivity.cpp
        /*
         * const EGLint attribs[] = { EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
         * EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //EGL_BLUE_SIZE, 5,
         * -->delete //EGL_GREEN_SIZE, 6, -->delete //EGL_RED_SIZE, 5, -->delete
         * EGL_BUFFER_SIZE, 32, //-->new field EGL_DEPTH_SIZE, 16,
         * EGL_STENCIL_SIZE, 8, EGL_NONE };
         */

        // 2.Set the format of window
        // getWindow().setFormat(PixelFormat.TRANSLUCENT);

    }

    // CallFromCPP
    // static なし
    public void kiiReq(String json, int serviceID) {
        MYLog.v(TAG, "reqKii " + json + "  " + serviceID);
        KiiIF.kiiReq(serviceID, json);
    }

    //
    /***
     * 
     * @param name
     */
    public void displayNameUpdate(String name) {
        MYLog.v(TAG, "displayNameUpdate");
        if (m_kRanking != null) {
            m_kRanking.displayNameUpdate(name);
        }
    }

    public void ranking_query_all() {
        MYLog.v(TAG, "ranking_query_all");
        if (m_kRanking != null) {
            m_kRanking.ranking_query_all();
        }
    }

    public void ranking_post(String name, int score) {
        MYLog.v(TAG, "ranking_post " + name + " " + score);

        if (m_kRanking != null) {
            m_kRanking.ranking_post(name, score);
        }
    }

    /**
     * Initialize KiiSDK Please change APP_ID/APP_KEY to your application
     */
    private void initKiiSDK() {
        MYLog.v(TAG, "initKiiSDK");

        Kii.initialize(Constants.APP_ID, // Put your App ID
                Constants.APP_KEY, // Put your App Key
                Constants.APP_SITE // Put your site as you've specified upon creating the
                        // app on the dev portal
        );
    }

    private static final String FILE_NAME = "PreferenceSampleFIle";

    public void TestPreference() {
        MYLog.v(TAG, "TestPreference");
        SharedPreferences preference = getSharedPreferences(FILE_NAME,
                MODE_PRIVATE);
        SharedPreferences.Editor editor = preference.edit();
        editor.putString("DAT", "abc");
        editor.commit();
    }

    public void TestPreference2() {
        MYLog.v(TAG, "TestPreference2");
    }
}
