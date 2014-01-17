package org.cocos2dx.simplegame;

public class CallCPP {
	// ネイティブメソッドを宣言 JavaからCPPを呼ぶ
	public static native String concat(String str1, String str2);
    public static native void init(int width, int height);
    public static native void step();
    public static native void nativeEnd();
	public static native void rankingResponse(String str1);	//ランキングデータをc++へ渡す、JSON
}
