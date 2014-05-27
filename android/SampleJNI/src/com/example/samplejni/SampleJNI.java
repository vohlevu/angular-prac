package com.example.samplejni;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.view.View.OnClickListener;
import android.widget.CheckBox;
import android.util.Log;
public class SampleJNI extends Activity {
	
	Native Native = new Native();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_sample_jni);
		Button callNativeCode = (Button) findViewById(R.id.callNativeCode);
		callNativeCode.setOnClickListener(new OnClickListener() {
			//sum
			TextView sumValue = (TextView) findViewById(R.id.sumValue);
			EditText intValue = (EditText) findViewById(R.id.intValue);
			EditText doubleValue = (EditText) findViewById(R.id.doubleValue);
			//boolean
			TextView booleanReturn = (TextView) findViewById(R.id.booleanReturn);
			CheckBox chk = (CheckBox) findViewById(R.id.booleanValue);
			//char 
			EditText charValue = (EditText) findViewById(R.id.charValue);
			TextView charReturn = (TextView) findViewById(R.id.charReturn);
			//string
			EditText stringValue = (EditText) findViewById(R.id.stringValue);
			TextView stringReturn = (TextView) findViewById(R.id.stringReturn);
			//static field
			EditText staticFieldValue = (EditText) findViewById(R.id.staticFieldValue);
			TextView staticFieldReturn = (TextView) findViewById(R.id.staticFieldReturn);
			//array
			EditText intNumberArray = (EditText) findViewById(R.id.txtArray);
			TextView arrayReturn = (TextView)findViewById(R.id.arrayReturn);
			// instance access
			EditText instanceIntValue = (EditText) findViewById(R.id.instanceIntValue);
			EditText instanceStrValue = (EditText) findViewById(R.id.instanceStrValue);
			TextView instanceIntReturn = (TextView) findViewById(R.id.instanceIntReturn);
			TextView instanceStrReturn =(TextView) findViewById(R.id.instanceStrReturn);
			
			public void onClick(View v) {
				
				// sum
				int v1=0;
				double v2=0, sum;
				if(!intValue.getText().toString().equals("") && !doubleValue.getText().toString().equals("")){
					v1 = Integer.parseInt(intValue.getText().toString());
					v2 = Double.parseDouble(doubleValue.getText().toString());
					sum = Native.sum(v1, v2);
					sumValue.setText(new Double(sum).toString());
				}
				
				//boolean
				boolean result = true;
				if(chk.isChecked())result = true;
				else result = false;
				booleanReturn.setText(new Boolean(Native.passBooleanReturnBoolean(result)).toString()); 
				
				//char 
				char ch;
				if(!charValue.getText().toString().equals("")){
					ch = charValue.getText().charAt(0);
					charReturn.setText(Character.toString(Native.passCharReturnChar(ch)));
				}
				
				//string
				String str = stringValue.getText().toString();
				if(!str.equals("")){ 
					str = Native.passStrReturnStr(str);
					stringReturn.setText(str);
				}
				
				//static Field Access
				if(!staticFieldValue.getText().toString().equals("") ){
					Native.si = Integer.parseInt(staticFieldValue.getText().toString());
					Native.staticFieldAccess();
					staticFieldReturn.setText(new Integer(Native.si).toString());
				}	
				
				// static method call
				/*try{
					Native.staticMethodCall();
				}catch(Exception e){
					Log.d("==================","==========  "+ e);
				}*/
				// array
				int numberArray;
				if(!intNumberArray.getText().toString().equals("")){
					numberArray = Integer.parseInt(intNumberArray.getText().toString());
					int[] intArray = new int[numberArray];
					for(int k = 0; k < intArray.length; k++)
					intArray[k] = k*k;
					//int [] b = Native.reverse2(intArray);
					intArray = Native.reverse2(intArray);
					arrayReturn.setText("Array result   : ");
					for(int k = 0; k < intArray.length; k++){
						arrayReturn.append(intArray[k] + " ");
					}
				}
				
				//modify instance variable
				if((!instanceIntValue.getText().toString().equals("")) && (!instanceStrValue.getText().toString().equals(""))){
					Native.number= Integer.parseInt(instanceIntValue.getText().toString());
					Native.message = instanceStrValue.getText().toString();
					Native.modifyInstanceVariable();
					instanceIntReturn.setText(new Integer(Native.number).toString());
					instanceStrReturn.setText(Native.message);
				}
			}
		});

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.sample_jni, menu);
		return true;
	}

}
