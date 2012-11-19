package org.simpledroidrpc.library;

import java.util.HashMap;


public class RPCServerRegistry {
	static HashMap<String, RPCServerInterface> directory = null;
	
	public RPCServerRegistry () {
		if (directory == null)
			directory = new HashMap<String, RPCServerInterface> ();
	}
	
	public void register (String name, RPCServerInterface clazz) {
		if (directory.containsValue (name)) directory.remove (name);
		directory.put (name, clazz);
	}
	
	public RPCServerInterface lookup (String name) {
		return directory.get (name);
	}
}
