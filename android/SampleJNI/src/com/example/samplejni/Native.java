package com.example.samplejni;
import android.util.Log;
class Native {
	 static{
		System.loadLibrary("SampleJNI");
	}
	static int si;
	int number = 88;
	String message = "Hello from Java";
	native double sum(int a, double b);
	native boolean passBooleanReturnBoolean(boolean p);
	native char passCharReturnChar(char p);
	native String passStrReturnStr(String msg);
	native void staticFieldAccess();
	//native void staticMethodCall();
	native int [] reverse2(int [] arr);
	native void modifyInstanceVariable();
	
	private static String callback() {
		Log.d("-===========","=======================In Java");
		return "Static Method";
	}

}
