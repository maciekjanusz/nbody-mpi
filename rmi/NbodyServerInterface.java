import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface NbodyServerInterface extends Remote {

    String NAME = "ServerInterface";

    void startSimulation(List<Particle> particles,
                         NbodyClientInterface callback) throws RemoteException;
}
