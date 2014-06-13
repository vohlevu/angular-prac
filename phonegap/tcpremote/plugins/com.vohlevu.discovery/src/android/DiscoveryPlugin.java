package com.vohlevu.discovery;
 
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONException;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import com.vohlevu.discovery.utils.NetInfo;

public class DiscoveryPlugin extends CordovaPlugin {

	public static final String ACTION_START_ENTRY = "start";
	public static final String ACTION_GET_NETWORK_INFO_ENTRY = "getNetworkInfo";
	public static final String ACTION_SET_SHOW_TOAST_CALLBACK_ENTRY = "setShowToastCallback";
	
	private static CallbackContext showToastCallback = null;
	private static CallbackContext progressUpdateCallback = null;
	private NetInfo net = null;
	
	public DiscoveryPlugin() {
	}
	
	@Override
	public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
		try {
			if (ACTION_START_ENTRY.equals(action)) {
				callbackContext.success("Success...");
				if (net != null) {
					progressUpdateCallback = callbackContext;
					Discovery d = new Discovery(net);
					d.startDiscovering();
				}
				return true;
			} else if (ACTION_SET_SHOW_TOAST_CALLBACK_ENTRY.equals(action)) {
				showToastCallback = callbackContext;
				return true;
			} else if (ACTION_GET_NETWORK_INFO_ENTRY.equals(action)) {
				Context context = cordova.getActivity().getApplicationContext();
				PluginResult pluginResult = new PluginResult(PluginResult.Status.OK, this.loadData(context));
				pluginResult.setKeepCallback(true);
				callbackContext.sendPluginResult(pluginResult);
				return true;
			}
			callbackContext.error("Invalid action");
			return false;
		} catch(Exception e) {
			System.err.println("Exception: " + e.getMessage());
			callbackContext.error(e.getMessage());
			return false;
		}
	}
	private JSONObject loadData(Context context) {
		JSONObject obj = new JSONObject();
		try {
			net = new NetInfo(context);
			net.getWifiInfo();
			if (net.ssid != null) {
				net.getIp();
				String info_ip_str = String.format("IP: %s (%s)", net.ip, net.intf);
				String info_in_str = String.format("SSID: %s", net.ssid);
				obj.put("IP", info_ip_str);
				obj.put("SSID", info_in_str);
				DiscoveryPlugin.showToast(info_ip_str);
			}
		} catch (Exception e) {
			System.err.println("Exception: " + e.getMessage());
		}
		return obj;
	}
	
	public static void showToast(String message) {
		if (showToastCallback != null) {
			PluginResult result = new PluginResult(PluginResult.Status.OK, message);
			result.setKeepCallback(true);
			showToastCallback.sendPluginResult(result);
		}
	}
	
	public static void updateProgress(String message) {
		if (progressUpdateCallback != null) {
			PluginResult result = new PluginResult(PluginResult.Status.OK, message);
			result.setKeepCallback(true);
			progressUpdateCallback.sendPluginResult(result);
		}
	}
}