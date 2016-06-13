import java.util.List;

public interface ParticlePlotter {

    void init();

    void plot(List<Particle> particles);

    void deinit();
}
