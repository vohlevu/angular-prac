package com.vohlevu.discovery.utils;

import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.telephony.TelephonyManager;
import android.util.Log;

public class NetInfo {
    private final String TAG = "NetInfo";
    public static final String NOIP = "0.0.0.0";
    public static final String NOMASK = "255.255.255.255";
    public static final String NOMAC = "00:00:00:00:00:00";
    private Context ctxt;
    private WifiInfo info;

    public String intf = "eth0";
    public String ip = NOIP;
    public int cidr = 24;

    public int speed = 0;
    public String ssid = null;
    public String bssid = null;
    public String carrier = null;
    public String macAddress = NOMAC;
    public String netmaskIp = NOMASK;
    public String gatewayIp = NOIP;

    public NetInfo(final Context ctxt) {
        this.ctxt = ctxt;
        getIp();
        getWifiInfo();
    }
    
    public NetInfo() {
    	
    }
    
    @Override
    public int hashCode() {
        return 42 + intf.hashCode() + ip.hashCode() + cidr;
    }
    
    public void getIp() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en
                    .hasMoreElements();) {
                NetworkInterface ni = en.nextElement();
                intf = ni.getName();
                ip = getInterfaceFirstIp(ni);
                if (ip != NOIP) {
                    break;
                }
            }
        } catch (SocketException e) {
            Log.e(TAG, e.getMessage());
        }
    }

    private String getInterfaceFirstIp(NetworkInterface ni) {
        if (ni != null) {
            for (Enumeration<InetAddress> nis = ni.getInetAddresses(); nis.hasMoreElements();) {
                InetAddress ia = nis.nextElement();
                if (!ia.isLoopbackAddress()) {
                    if (ia instanceof Inet6Address) {
                        Log.i(TAG, "IPv6 detected and not supported yet!");
                        continue;
                    }
                    return ia.getHostAddress();
                }
            }
        }
        return NOIP;
    }
    
    public boolean getMobileInfo() {
        TelephonyManager tm = (TelephonyManager) ctxt.getSystemService(Context.TELEPHONY_SERVICE);
        if (tm != null) {
            carrier = tm.getNetworkOperatorName();
        }
        return false;
    }

    public boolean getWifiInfo() {
        WifiManager wifi = (WifiManager) ctxt.getSystemService(Context.WIFI_SERVICE);
        if (wifi != null) {
            info = wifi.getConnectionInfo();
            // Set wifi variables
            speed = info.getLinkSpeed();
            ssid = info.getSSID();
            bssid = info.getBSSID();
            macAddress = info.getMacAddress();
            gatewayIp = getIpFromIntSigned(wifi.getDhcpInfo().gateway);
            netmaskIp = getIpFromIntSigned(wifi.getDhcpInfo().netmask);
            return true;
        }
        return false;
    }

    public static String getNetIp(int cidr, String ip) {
        int shift = (32 - cidr);
        int start = ((int) getUnsignedLongFromIp(ip) >> shift << shift);
        return getIpFromLongUnsigned((long) start);
    }

    public SupplicantState getSupplicantState() {
        return info.getSupplicantState();
    }

    public static boolean isConnected(Context ctxt) {
        NetworkInfo nfo = ((ConnectivityManager) ctxt
                .getSystemService(Context.CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
        if (nfo != null) {
            return nfo.isConnected();
        }
        return false;
    }

    public static long getUnsignedLongFromIp(String ip_addr) {
        String[] a = ip_addr.split("\\.");
        return (Integer.parseInt(a[0]) * 16777216 + Integer.parseInt(a[1]) * 65536
                + Integer.parseInt(a[2]) * 256 + Integer.parseInt(a[3]));
    }

    public static String getIpFromIntSigned(int ip_int) {
        String ip = "";
        for (int k = 0; k < 4; k++) {
            ip = ip + ((ip_int >> k * 8) & 0xFF) + ".";
        }
        return ip.substring(0, ip.length() - 1);
    }

    public static String getIpFromLongUnsigned(long ip_long) {
        String ip = "";
        for (int k = 3; k > -1; k--) {
            ip = ip + ((ip_long >> k * 8) & 0xFF) + ".";
        }
        return ip.substring(0, ip.length() - 1);
    }
}
