package org.cocos2dx.cpp;

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
		
		Global.activity.ranking_post(name, score);
	}
	
	public static void save_display_name(String name){
		Log.v(TAG, "save_display_name " + name);
		
		Global.activity.displayNameUpdate(name);
	}
	
	public static void kiiReq(String json, int serviceID){
		Log.v(TAG, "reqKii " + json + "  " + serviceID);	
		KiiIF.kiiReq(serviceID, json);
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