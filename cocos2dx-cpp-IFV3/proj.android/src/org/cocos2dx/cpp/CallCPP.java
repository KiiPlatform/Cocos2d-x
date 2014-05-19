package org.cocos2dx.cpp;

public class CallCPP {
    public static native String concat(String str1, String str2);

    public static native void init(int width, int height);

    public static native void step();

    public static native void nativeEnd();

    public static native void rankingResponse(String str1);

    public static native void setDisplayame(String str1);

    public static native void setName(String str1);

    public static native void setDisplayame2(String str1, int serviceID);

    public static native void kiires(String json, int serviceID);

    public static native void kiires2(String json);

    public static native void kiires3(String json);

}
