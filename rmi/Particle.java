import java.io.Serializable;

public class Particle implements Serializable {

    public double x;
    public double y;
    public double mass;
    public double vx;
    public double vy;

    public Particle(double x, double y, double mass, double vx, double vy) {
        this.x = x;
        this.y = y;
        this.mass = mass;
        this.vx = vx;
        this.vy = vy;
    }
}
