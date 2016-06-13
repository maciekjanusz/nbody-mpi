package server;

import java.awt.Graphics;
import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;

public class NbodyServer extends UnicastRemoteObject implements
  NbodyServerInterface {

	public NbodyServer() throws RemoteException {

	}

	public static void main(String[] args) {
		try {
			NbodyServer hw = new NbodyServer();
			Naming.bind("server", hw);

		} catch (Exception e) {
			System.err.println(e);
		}
	}

}
