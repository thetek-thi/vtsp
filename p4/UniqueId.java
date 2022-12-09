// Datei: UniqueId.java
import java.io.*;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class UniqueId {
	private String file;
    private ReadWriteLock rwlock = new ReentrantReadWriteLock();

	public UniqueId(String file) {
		this.file = file;
	}

	public void init(int id) throws IOException {
		DataOutputStream out = new DataOutputStream(new FileOutputStream(file));
		out.writeInt(id);
		out.close();
	}		

	public synchronized int getNext() throws IOException {
            DataInputStream in = new DataInputStream(new FileInputStream(file));
            int oldId = in.readInt();
            in.close();
		
            DataOutputStream out = new DataOutputStream(new FileOutputStream(file));
            int newId = ++oldId;
            out.writeInt(newId);
            out.close();

            return newId;
	}
}
