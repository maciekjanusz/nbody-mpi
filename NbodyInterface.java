import java.rmi.*;

public interface NbodyInterface extends Remote {
	void print() throws RemoteException;
}
