package org.cocos2dx.cpp;

import android.util.Log;

public class MYLog {
    public static boolean DEBUG = false;

    public static void v(String tag, String string) {
        // TODO Auto-generated method stub
        if (DEBUG) {
            Log.v(tag, string);
        }
    }

}
