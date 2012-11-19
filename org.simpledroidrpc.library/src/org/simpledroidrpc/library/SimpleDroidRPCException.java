package org.simpledroidrpc.library;

public class SimpleDroidRPCException extends Exception {

	private static final long serialVersionUID = 1L;
	
	public SimpleDroidRPCException () {
		super ("RPC error");
	}

	public SimpleDroidRPCException (String message) {
		super (message);
	}

	public SimpleDroidRPCException (Throwable cause) {
		super (cause);
	}
	
	public SimpleDroidRPCException (String message, Throwable cause) {
		super (message, cause);
	}

}
