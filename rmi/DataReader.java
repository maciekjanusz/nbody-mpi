import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.List;

public class DataReader {

    public List<Particle> readParticlesFromFile(String filename) throws FileNotFoundException {
        Scanner fileScanner = new Scanner(new File(filename));
        List<Particle> particles = new ArrayList<>();
        while (fileScanner.hasNext()) {
            double x = fileScanner.nextDouble();
            double y = fileScanner.nextDouble();
            double m = fileScanner.nextDouble();
            double vx = fileScanner.nextDouble();
            double vy = fileScanner.nextDouble();
            particles.add(new Particle(x, y, m, vx, vy));
        }
        return particles;
    }
}
