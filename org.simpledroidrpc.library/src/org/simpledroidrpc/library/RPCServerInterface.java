package org.simpledroidrpc.library;

import org.json.JSONArray;

public abstract class RPCServerInterface {
	public abstract void register ();
	public abstract JSONArray call (String method, JSONArray args);
}
