import java.awt.Color;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;

public class test {
	public static void main(String[] args) {
		new test();
	}

	public test() {
		EventQueue.invokeLater(new Runnable() {
			@Override
			public void run() {
				JFrame frame = new JFrame("Nbody simulation");
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.add(new TestPane());
				frame.pack();
				frame.setLocationRelativeTo(null);
				frame.setVisible(true);
			}
		});
	}

	public class TestPane extends JPanel {

		int m[];
		int x[];
		int y[];
		double vx[];
		double vy[];
		double fx[];
		double fy[];
		double dx, dy, d;
		double G = 0.002;
		double F, dt = 1;

		// 350 350 1000000 0 0
		// 350 500 1000 -3 0
		// 500 350 1000 0 -4
		public TestPane() {
			x = new int[3];
			y = new int[3];
			m = new int[3];
			fx = new double[3];
			fy = new double[3];
			vx = new double[3];
			vy = new double[3];
			x[0] = 350; 	x[1] = 350; 	x[2] = 500;
			y[0] = 350; 	y[1] = 500; 	y[2] = 350;
			m[0] = 1000000; m[1] = 600; 	m[2] = 600;
			vx[0] = 0; 		vx[1] = -3; 	vx[2] = 0; 
			vy[0] = 0; 		vy[1] = 0; 		vy[2] = -4;
			Timer timer = new Timer(40, new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					compute();
					repaint();
				}
			});
			timer.start();
			
			System.gc();
		}

		// wylczanie położenia stąd możemy pobierać dane
		protected void compute() {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (i != j) {
						dx = x[j] - x[i];
						dy = y[j] - y[i];
						d = Math.sqrt(dx * dx + dy * dy);
						F = (G * m[i] * m[j]) / (d * d);
						fx[i] += F * dx / d;
						fy[i] += F * dy / d;
					}
				}
				vx[i] += dt * fx[i] / m[i];
				vy[i] += dt * fy[i] / m[i];
				x[i] += dt * vx[i];
				y[i] += dt * vy[i];
				
				fx[i]=fy[i]=0;
				
				
			}
			
		}

		@Override
		public Dimension getPreferredSize() {
			return new Dimension(700, 700);
		}

		@Override
		protected void paintComponent(Graphics g) {
			super.paintComponent(g);
			Graphics2D g2d = (Graphics2D) g.create();
			g2d.setColor(Color.RED);
			for (int i = 0; i < 3; i++) {
				g2d.fillOval(x[i], y[i], 30, 30);
			}
			g2d.dispose();
		}

	}

}
