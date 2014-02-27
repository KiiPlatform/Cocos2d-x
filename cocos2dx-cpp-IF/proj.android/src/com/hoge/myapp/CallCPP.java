package com.hoge.myapp;

public class CallCPP {
	// �l�C�e�B�u���\�b�h��錾 Java����CPP���Ă�
	public static native String concat(String str1, String str2);
    public static native void init(int width, int height);
    public static native void step();
    public static native void nativeEnd();
	public static native void rankingResponse(String str1);	//�����L���O�f�[�^��c++�֓n���AJSON
	public static native void setDisplayame(String str1);	//Displayame��c++�֓n���AJSON
	public static native void setDisplayame2(String str1, int serviceID);	//Displayame��c++�֓n���AJSON

	
	//kii���X�|���X��Ԃ�
	public static native void kiires(String json, int serviceID);
	public static native void kiires2(String json);
	public static native void kiires3(String json);

}
