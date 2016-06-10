import java.awt.Graphics;
import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class NbodyServer extends UnicastRemoteObject implements NbodyInterface {

	public NbodyServer() throws RemoteException {
	}

	public void print() throws RemoteException {
		JFrame frame = new JFrame();
		DrawNbody panel = new DrawNbody();
		frame.setSize(900, 600);
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setTitle("Nbody simulation");
		frame.add(panel);
	}

	public static void main(String[] args) {
		try {
			NbodyServer hw = new NbodyServer();
			Naming.bind("hello", hw);
		} catch (Exception e) {
			System.err.println(e);
		}
	}

	class DrawNbody extends JPanel {
		public void paintComponent(Graphics g) {
			g.fillRect(20, 20, 100, 200);
		}
	}
}
