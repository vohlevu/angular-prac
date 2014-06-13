package com.vohlevu.discovery;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.IllegalArgumentException;
import java.net.InetSocketAddress;
import java.net.InetAddress;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

import com.vohlevu.discovery.utils.HostBean;
import com.vohlevu.discovery.utils.NetInfo;

public class DefaultDiscovery extends AbstractDiscovery {

    private final String TAG = "DefaultDiscovery";
    //private final int[] DPORTS = { Utils.SERVER_PORT };
    private final int TIMEOUT_SCAN = 3600; // seconds
    private final int TIMEOUT_SHUTDOWN = 10; // seconds
    private final int THREADS = 10;
    private int pt_move = 2; // 1=backward 2=forward
    private ExecutorService mPool;

    public DefaultDiscovery(Discovery discover) {
        super(discover);
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
    	flagCancelled = false;
    }

    @Override
    protected Void doInBackground(Void... params) {
        if (mDiscover != null) {
            final Discovery discover = mDiscover.get();
            if (discover != null) {
                /*Log.v(TAG, "start=" + NetInfo.getIpFromLongUnsigned(start) + " (" + start
                        + "), end=" + NetInfo.getIpFromLongUnsigned(end) + " (" + end
                        + "), length=" + size);*/
                mPool = Executors.newFixedThreadPool(THREADS);
                if (ip <= end && ip >= start) {
                    //Log.i(TAG, "Back and forth scanning");
                    // gateway
                    launch(start);

                    // hosts
                    long pt_backward = ip;
                    long pt_forward = ip + 1;
                    long size_hosts = size - 1;

                    for (int i = 0; i < size_hosts; i++) {
                        // Set pointer if of limits
                        if (pt_backward <= start) {
                            pt_move = 2;
                        } else if (pt_forward > end) {
                            pt_move = 1;
                        }
                        // Move back and forth
                        if (pt_move == 1) {
                            launch(pt_backward);
                            pt_backward--;
                            pt_move = 2;
                        } else if (pt_move == 2) {
                            launch(pt_forward);
                            pt_forward++;
                            pt_move = 1;
                        }
                    }
                } else {
                    //Log.i(TAG, "Sequencial scanning");
                    for (long i = start; i <= end; i++) {
                        launch(i);
                    }
                }
                mPool.shutdown();
                try {
                    if(!mPool.awaitTermination(TIMEOUT_SCAN, TimeUnit.SECONDS)){
                        mPool.shutdownNow();
                        //Log.e(TAG, "Shutting down pool");
                        if(!mPool.awaitTermination(TIMEOUT_SHUTDOWN, TimeUnit.SECONDS)){
                            //Log.e(TAG, "Pool did not terminate");
                        }
                    }
                } catch (InterruptedException e){
                    //Log.e(TAG, e.getMessage());
                    mPool.shutdownNow();
                    Thread.currentThread().interrupt();
                }
            }
        }
        return null;
    }

	private void launch(long i) {
        if(!mPool.isShutdown()) {
            mPool.execute(new CheckRunnable(NetInfo.getIpFromLongUnsigned(i)));
        }
    }
	/*
	public void sendDataWithString(BufferedOutputStream out) {
    	try {
    		out.write(Utils.getByteArrayNameCode());
    		out.flush();
    	} catch (Exception e) {
			//Log.e(TAG, "Error sending:  " + e.getMessage());
		}
    }
	
    public HostBean receiveDataFromServer(HostBean host, BufferedReader in) {
        try {
        	String message = in.readLine();
            if (message != null && !"".equals(message)) {
            	if ("Off".equals(message))
            		return null;
            	if (message.contains("|")) {
            		String[] parts = message.split("\\|");
            		if (parts.length > 1) {
    	            	host.pinCode = parts[0];
    	            	host.hostname = parts[1];
            		}
            	}
            }
        } catch (IOException e) {
        	//Log.e(TAG, "Error receiving response:  " + e.getMessage());
        }
        return host;
    }
    */
    private class CheckRunnable implements Runnable {
        private String addr;

        CheckRunnable(String addr) {
            this.addr = addr;
        }

        public void run() {
            if(flagCancelled) {
                publish(null);
                return;
            }
            //Log.e(TAG, "run="+addr);
            String progressMessage = "Discovering... " + addr;
            mDiscover.get().updateProgress(progressMessage);
            // Create host object
            HostBean host = new HostBean();
            host.ipAddress = addr;
            try {
                InetAddress h = InetAddress.getByName(addr);
                // Native InetAddress check
                if (!h.isReachable(2000/*Utils.TIMEOUT_SOCKET_CONNECT*/)) {
                    publish(null);
                    return;
                }
				publish(host);
                //Log.e(TAG, "found using InetAddress ping "+addr);
                // Custom check
                /*Socket s = new Socket();
                BufferedOutputStream out = null;
                BufferedReader in = null;
                for (int i = 0; i < DPORTS.length; i++) {
                    try {
                        s.bind(null);
                        s.connect(new InetSocketAddress(addr, DPORTS[i]), Utils.TIMEOUT_SOCKET_CONNECT);
                        if (s.isConnected()) {
                        	s.setSoTimeout(Utils.TIMEOUT_SOCKET_CONNECT);
                        	out = new BufferedOutputStream(s.getOutputStream());
                        	in = new BufferedReader(new InputStreamReader(s.getInputStream()));
                            sendDataWithString(out);
                            host = receiveDataFromServer(host, in);
                        	publish(host);
                        	//Log.v(TAG, "found using TCP connect "+addr);
                        	//Log.v(TAG, "found using TCP connect "+addr+" on port=" + DPORTS[i]+" [name=" + host.hostname+"]");
                        	return;
                        }
					} catch (IOException e) {
					} catch (IllegalArgumentException e) {
					} finally {
						Utils.closeSocket(s, out, in);
					}
                }
                publish(null);*/
            } catch (IOException e) {
                publish(null);
                //Log.e(TAG, e.getMessage());
            } 
        }
    }

    private void publish(final HostBean host) {
        hosts_done++;
        if(host == null){
            publishProgress((HostBean) null);
            return;
        }
        publishProgress(host);
    }
}
