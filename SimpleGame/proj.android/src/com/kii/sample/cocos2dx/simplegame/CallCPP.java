package com.kii.sample.cocos2dx.simplegame;

public class CallCPP {
	// �l�C�e�B�u���\�b�h��錾 Java����CPP���Ă�
	public static native String concat(String str1, String str2);
    public static native void init(int width, int height);
    public static native void step();
    public static native void nativeEnd();
	public static native void rankingResponse(String str1);	//�����L���O�f�[�^��c++�֓n���AJSON
	public static native void setDisplayame(String str1);	//Displayame��c++�֓n���AJSON

	//kii���X�|���X��Ԃ�
	public static native void  resKii(int serviceID, String json);
}
