import java.rmi.RemoteException;
import java.rmi.Remote;
import java.util.List;

public interface NbodyClientInterface extends Remote {

  String NAME = "ClientInterface";

  void onNewResult(List<Particle> particles) throws RemoteException;

  void onFinish() throws RemoteException;
}
