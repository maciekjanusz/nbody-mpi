import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;

public class NbodyServer extends UnicastRemoteObject implements
        NbodyServerInterface {

    public NbodyServer() throws RemoteException {

    }

    public static void main(String[] args) {
        try {
            NbodyServer server = new NbodyServer();
            Naming.bind("//localhost:3000/server", server);
            System.out.println("Server: Started.");

        } catch (Exception e) {
            System.err.println(e);
            System.exit(1);
        }
    }

    @Override
    public void startSimulation(List<Particle> particles,
                                NbodyClientInterface callback) throws RemoteException {
        int size = particles.size();
        double dx, dy, d, F;
        for (int t = 0; t < Physics.TMAX; t++) {
            for (int i = 0; i < size; i++) {
                double fx = 0.0;
                double fy = 0.0;
                for (int j = 0; j < size; j++) {
                    if (i == j) continue;
                    dx = particles.get(j).x - particles.get(i).x;
                    dy = particles.get(j).y - particles.get(i).y;
                    d = Math.sqrt(dx * dx + dy * dy);
                    F = (Physics.G * particles.get(j).mass * particles.get(i).mass) / (d * d);
                    fx += F * dx / d;
                    fy += F * dy / d;
                }

                particles.get(i).vx += Physics.DT * fx / particles.get(i).mass;
                particles.get(i).vy += Physics.DT * fy / particles.get(i).mass;
                particles.get(i).x += Physics.DT * particles.get(i).vx;
                particles.get(i).y += Physics.DT * particles.get(i).vy;

                callback.onNewResult(particles);
            }
        }
        // todo: run simulation, update with callback
        callback.onFinish();
    }
}
