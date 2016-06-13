import javax.swing.*;
import java.awt.*;
import java.util.List;

public class SwingParticlePlotter implements ParticlePlotter {

    private NbodyPanel panel = new NbodyPanel();

    @Override
    public void init() {
        EventQueue.invokeLater(() -> {
            JFrame frame = new JFrame("Nbody simulation");
            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.add(panel);
            frame.pack();
            frame.setLocationRelativeTo(null);
            frame.setVisible(true);
        });
    }

    @Override
    public void plot(List<Particle> particles) {
        panel.setParticles(particles);
    }

    @Override
    public void deinit() {

    }

    private static final class NbodyPanel extends JPanel {

        private List<Particle> particles;

        @Override
        public Dimension getPreferredSize() {
            return new Dimension(700, 700);
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2d = (Graphics2D) g.create();
            g2d.setColor(Color.RED);
            for (Particle particle : particles) {
                g2d.fillOval(
                        ((int) particle.x),
                        ((int) particle.y),
                        30,
                        30);
            }
            g2d.dispose();
        }

        public void setParticles(List<Particle> particles) {
            this.particles = particles;
            repaint();
        }
    }
}
