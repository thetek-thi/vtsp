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

	public int getNext() throws IOException {
        int oldId, newId;

        rwlock.readLock().lock();
        try {
            DataInputStream in = new DataInputStream(new FileInputStream(file));
            oldId = in.readInt();
            in.close();
        } finally {
            rwlock.readLock().unlock();
        }
		
        rwlock.writeLock().lock();
        try {
            DataOutputStream out = new DataOutputStream(new FileOutputStream(file));
            newId = ++oldId;
            out.writeInt(newId);
            out.close();
        } finally {
            rwlock.writeLock().unlock();
        }

		return newId;
	}
}
