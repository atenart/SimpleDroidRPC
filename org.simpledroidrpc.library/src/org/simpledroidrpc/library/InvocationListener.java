package org.simpledroidrpc.library;

public abstract class InvocationListener<RetType> {
	
	public abstract void onComplete (RetType result);
	public abstract void onException (Exception e);

}
