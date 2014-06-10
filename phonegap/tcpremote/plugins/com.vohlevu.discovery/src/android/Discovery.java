package com.vohlevu.discovery;

import java.util.List;
import java.util.ArrayList;
import com.vohlevu.discovery.utils.NetInfo;
import com.vohlevu.discovery.utils.HostBean;
import com.vohlevu.discovery.AbstractDiscovery;

final public class Discovery {

	private final String TAG = "Discovery";
	private long network_ip = 0;
	private long network_start = 0;
	private long network_end = 0;
	private NetInfo net = null;
	private List<HostBean> hosts = null;
	private AbstractDiscovery mDiscoveryTask = null;

	public Discovery(NetInfo net) {
		this.net = net;
	}
	
	protected void setInfo() {
		// Get ip information
		network_ip = NetInfo.getUnsignedLongFromIp(net.ip);
		// Detected IP
		int shift = 8;
		network_start = (network_ip >> shift << shift);
		network_end = (network_start | ((1 << shift) - 1));
	}

	protected void cancelTasks() {
		if (mDiscoveryTask != null) {
			mDiscoveryTask.setCancelled(true);
			mDiscoveryTask = null;
		}
	}

	/**
	 * Discover hosts
	 */
	private void startDiscovering() {
		mDiscoveryTask = new AbstractDiscovery(this);//new DefaultDiscovery(this);
		mDiscoveryTask.setNetwork(network_ip, network_start, network_end);
		mDiscoveryTask.execute();

		//Utils.makeToast(this, getString(R.string.discover_start));
		initList();
	}

	public void stopDiscovering() {
		//Log.e(TAG, "stopDiscovering()");
		mDiscoveryTask = null;
	}

	private void initList() {
		hosts = new ArrayList<HostBean>();
	}

	public void addHost(HostBean host) {
		hosts.add(host);
	}
}