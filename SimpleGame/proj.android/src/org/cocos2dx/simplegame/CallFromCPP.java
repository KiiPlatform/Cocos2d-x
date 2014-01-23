package org.cocos2dx.simplegame;

import android.util.Log;

public class CallFromCPP {
	static final String TAG ="CallFromCPP";
	
	public static void test(){
		Log.v(TAG, "test");
	}
	
	public static void ranking_query_all(){
		Log.v(TAG, "ranking_query_all");
		
		Global.activity.ranking_query_all();
	}
	
	public static void ranking_post(String name, int score){
		Log.v(TAG, "ranking_post " + name +" "+ score);
		
		//xx Global.activity.ranking_post(name, score);
	}
}


/***
Guests-MacBook-Air:tmp guest$ javap -s CallFromCPP
Compiled from "CallFromCPP.java"
public class org.cocos2dx.simplegame.CallFromCPP extends java.lang.Object{
static final java.lang.String TAG;
  Signature: Ljava/lang/String;
public org.cocos2dx.simplegame.CallFromCPP();
  Signature: ()V
public static void test();
  Signature: ()V
public static void ranking_query_all();
  Signature: ()V
public static void ranking_post(java.lang.String, int);
  Signature: (Ljava/lang/String;I)V
}
***/