import java.rmi.Naming;
import java.rmi.RemoteException;
import java.io.FileNotFoundException;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;

public class NbodyClient extends UnicastRemoteObject implements
    NbodyClientInterface {

    private final NbodyServerInterface server;
    private final ParticlePlotter plotter;
    private final DataReader reader;

    public NbodyClient(NbodyServerInterface server) throws RemoteException {
        this.server = server;
        this.plotter = new SwingParticlePlotter();
        this.reader = new DataReader();
    }

    public static void main(String[] args) {
        try {
            String filename = args[0];
            NbodyServerInterface server =
                  (NbodyServerInterface) Naming.lookup("//localhost:3000/server");
            NbodyClient client = new NbodyClient(server);
            client.start(filename);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    void start(String dataFileName) {
        plotter.init();
      try {
        List<Particle> particles = reader.readParticlesFromFile(dataFileName);
        server.startSimulation(particles, this);
      } catch (FileNotFoundException | RemoteException e) {
        e.printStackTrace();
      }
    }

    @Override
    public void onNewResult(List<Particle> result) throws RemoteException {
        plotter.plot(result);
        System.out.print(".");
    }

    @Override
    public void onFinish() throws RemoteException {
        System.out.println("Client: Finished.");
        System.exit(0);
    }
}
