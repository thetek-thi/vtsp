import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class AndereFigur extends Figur {
    private ReadWriteLock rwlock = new ReentrantReadWriteLock();

    public synchronized void setPosition(char x, int y) {
        rwlock.writeLock().lock();
        try {
            this.x = x;
            MachMal.eineZehntelSekundeLangGarNichts();
            this.y = y;
        } finally {
            rwlock.writeLock().unlock();
        }
    }

    public synchronized String getPosition() {
        rwlock.readLock().lock();
        try {
            MachMal.eineSekundeLangGarNichts();
            return "(" + x + ", " + y + ")";
        } finally {
            rwlock.readLock().unlock();
        }
    }
}
