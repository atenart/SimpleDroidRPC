package org.simpledroidrpc.library;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;
import java.net.Socket;

import org.json.JSONArray;
import org.json.JSONObject;

import android.util.Log;


public class RPCClientInterface {
	private final String TAG = "SimpleDroidRPC.RpcClientInterface";
	
	private int port = 12345;
	private int timeout = 5000; // in ms
	String address = null;

	public RPCClientInterface (String address) {
		this.address = address;
	}
	
	public RPCClientInterface (String address, int port) {
		this (address);
		this.port = port;
	}
	
	public RPCClientInterface (String address, int port, int timeout) {
		this (address, port);
		this.timeout = timeout;
	}

	protected JSONArray invoque (String iface, String method, JSONArray args) {
		JSONObject rep = null;
		try {
			Socket socket = new Socket ();
			socket.connect (new InetSocketAddress (address, port), timeout);
			
			DataOutputStream output = new DataOutputStream (socket.getOutputStream ());
			
			JSONObject req = new JSONObject ();
			
			req.put ("interface", iface);
			req.put ("method", method);
			req.put ("args", args);
			
			output.writeBytes (req.toString () + "\n");
			
			BufferedReader input = new BufferedReader (
					new InputStreamReader (socket.getInputStream()));
			
			rep = new JSONObject (input.readLine ());
			if (!rep.getString ("method").equals (method) || !rep.getString ("interface").equals (iface))
				return null;
			
			socket.close ();
			
			return rep.getJSONArray ("values");
		} catch (Exception e) {
			Log.e (TAG, e.getMessage ());
			
			if (rep != null) {
				try {
					String error = rep.getString ("errors");
					Log.e (TAG, error);
				} catch (Exception ex) {}
			}
			
			return null;
		}
	}

}
