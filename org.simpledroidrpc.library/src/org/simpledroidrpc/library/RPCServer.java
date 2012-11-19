package org.simpledroidrpc.library;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

import org.json.JSONArray;
import org.json.JSONObject;

import android.util.Log;


public class RPCServer implements Runnable {
	private int port = 12345;
	
	private RPCServerRegistry registry = null;
	private ServerSocket server = null;
	
	public RPCServer () {
		start_thread ();
	}
	
	public RPCServer (int port) {
		this.port = port;
		start_thread ();
	}
	
	private void start_thread () {
		Thread t = new Thread (this);
		t.start ();
	}

	public void run() {
		registry = new RPCServerRegistry ();
		try {
			server = new ServerSocket (port);
			
			while (true) {
				Socket socket = server.accept ();

				new ConnexionHandler (socket);
			}
		} catch (Exception e) {}
	}
	
	private class ConnexionHandler implements Runnable {
		private final String TAG = "SimpleDroidRPC.RpcServer";
		private Socket socket = null;
		
		public ConnexionHandler (Socket socket) {
			this.socket = socket;
			
			Thread t = new Thread (this);
			t.start ();
		}

		public void run() {
			BufferedReader input = null;
			JSONArray args = null;
			String iface = null;
			String method = null;

			try {
				input = new BufferedReader (new InputStreamReader (
						socket.getInputStream ()));
				
				JSONObject obj = new JSONObject (input.readLine ());
				
				iface = obj.getString ("interface");
				method = obj.getString ("method");
				args = obj.getJSONArray ("args");
			} catch (Exception e) {
				Log.e (TAG, e.getMessage ());
				return;
			}

			try {
				DataOutputStream output = new DataOutputStream (socket.getOutputStream ());

				JSONObject rep = new JSONObject ();
				JSONArray arr = null;
				
				rep.put ("interface", iface);
				rep.put ("method", method);
				
				RPCServerInterface clazz = registry.lookup (iface);
				if (clazz != null) {
					arr = clazz.call (method, args);
					rep.put ("values", arr);
				} else {
					rep.put ("error", "interface_NOTFOUND");
				}

				output.writeBytes (rep.toString () + "\n");

				socket.close ();
			} catch (Exception e) {
				Log.e (TAG, e.getMessage ());
			}
		}
		
	}

}
