import java.rmi.Naming;

public class NbodyClient {
	public static void main(String[] args) {
		try {
			NbodyInterface h = (NbodyInterface) Naming.lookup("hello");
			h.print();
		} catch (Exception e) {
			System.err.println(e);
		}
	}
}
