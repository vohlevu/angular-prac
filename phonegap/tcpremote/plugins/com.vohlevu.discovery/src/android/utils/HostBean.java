package com.vohlevu.discovery.utils;

import android.os.Parcel;
import android.os.Parcelable;

public class HostBean implements Parcelable {

    public String ipAddress = null;
    public String hostname = null;
    public String pinCode = null;

    public HostBean() {
        // New object
    }

    public HostBean(Parcel in) {
        // Object from parcel
        readFromParcel(in);
    }

    public int describeContents() {
        return 0;
    }

    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(ipAddress);
        dest.writeString(hostname);
        dest.writeString(pinCode);
    }

    private void readFromParcel(Parcel in) {
        ipAddress = in.readString();
        hostname = in.readString();
        pinCode = in.readString();
    }

    @SuppressWarnings("rawtypes")
	public static final Parcelable.Creator CREATOR = new Parcelable.Creator() {
        public HostBean createFromParcel(Parcel in) {
            return new HostBean(in);
        }

        public HostBean[] newArray(int size) {
            return new HostBean[size];
        }
    };
}
