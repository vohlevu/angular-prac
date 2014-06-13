package com.vohlevu.discovery;

import java.lang.ref.WeakReference;
import android.os.AsyncTask;
import com.vohlevu.discovery.utils.HostBean;

public abstract class AbstractDiscovery extends AsyncTask<Void, HostBean, Void> {

    protected int hosts_done = 0;
    final protected WeakReference<Discovery> mDiscover;

    protected long ip;
    protected long start = 0;
    protected long end = 0;
    protected int size = 0;
    protected Boolean flagCancelled;

    public AbstractDiscovery(Discovery discover) {
        mDiscover = new WeakReference<Discovery>(discover);
    }

    public void setNetwork(long ip, long start, long end) {
        this.ip = ip;
        this.start = start;
        this.end = end;
    }

    abstract protected Void doInBackground(Void... params);

    @Override
    protected void onPreExecute() {
        size = (int) (end - start + 1);
    }

    @Override
    protected void onProgressUpdate(HostBean... host) {
        if (mDiscover != null) {
            final Discovery discover = mDiscover.get();
            if (discover != null) {
                if (!isCancelled()) {
                    if (host[0] != null) {
                        discover.addHost(host[0]);
                    }
                    if (size > 0) {
						discover.updateProgress("Percent : " + Integer.toString((int) (hosts_done * 100 / size)));
						// Update progress on GUI
                    	/*mProgressDialog.setProgress(hosts_done);
                    	mProgressDialog.setSecondaryProgress((int) (hosts_done * 100 / size));*/
                    }
                }

            }
        }
    }

    @Override
    protected void onPostExecute(Void unused) {
        if (mDiscover != null) {
            final Discovery discover = mDiscover.get();
            if (discover != null) {
            	if (flagCancelled) {
					discover.showToast("Discovery canceled !");
            	} else {
					discover.showToast("Discovery finished !");
            	}
            	//mProgressDialog.dismiss();
                discover.stopDiscovering();
            }
        }
    }

    protected void setCancelled(Boolean flag) {
    	flagCancelled = flag;
    }
}
